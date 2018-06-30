//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <boost/core/demangle.hpp>

#include <common/assert.h>

#if ASAP_USE_ASSERTS

#include <array>
#include <cinttypes>  // for PRId64 et.al.
#include <csignal>
#include <cstdarg>  // for va_start, va_end
#include <cstdio>   // for snprintf
#include <cstdlib>
#include <cstring>  // for strncat
#include <string>   // for strstr, strchr

#if ASAP_USE_EXECINFO
#include <execinfo.h>

namespace asap {

void print_backtrace(char* out, int len, int max_depth, void*) {
  void* stack[50];
  int size = ::backtrace(stack, 50);
  char** symbols = ::backtrace_symbols(stack, size);

  for (int i = 1; i < size && len > 0; ++i) {
    int ret = std::snprintf(out, std::size_t(len), "%d: %s\n", i,
                            boost::core::demangle(symbols[i]).c_str());
    out += ret;
    len -= ret;
    if (i - 1 == max_depth && max_depth > 0) break;
  }

  ::free(symbols);
}
}  // namespace asap

#elif defined _WIN32

#include <mutex>
//#include "asap/utf8.hpp"
#include "windows.h"

#include "dbghelp.h"
#include "winbase.h"

namespace asap {

void print_backtrace(char* out, int len, int max_depth, void* ctx) {
  // all calls to DbgHlp.dll are thread-unsafe. i.e. they all need to be
  // synchronized and not called concurrently. This mutex serializes access
  static std::mutex dbghlp_mutex;
  std::lock_guard<std::mutex> l(dbghlp_mutex);

  CONTEXT context_record;
  if (ctx) {
    context_record = *static_cast<CONTEXT*>(ctx);
  } else {
    // use the current thread's context
    RtlCaptureContext(&context_record);
  }

  int size = 0;
  std::array<void*, 50> stack;

  STACKFRAME64 stack_frame = {};
#if defined(_WIN64)
  int const machine_type = IMAGE_FILE_MACHINE_AMD64;
  stack_frame.AddrPC.Offset = context_record.Rip;
  stack_frame.AddrFrame.Offset = context_record.Rbp;
  stack_frame.AddrStack.Offset = context_record.Rsp;
#else
  int const machine_type = IMAGE_FILE_MACHINE_I386;
  stack_frame.AddrPC.Offset = context_record.Eip;
  stack_frame.AddrFrame.Offset = context_record.Ebp;
  stack_frame.AddrStack.Offset = context_record.Esp;
#endif
  stack_frame.AddrPC.Mode = AddrModeFlat;
  stack_frame.AddrFrame.Mode = AddrModeFlat;
  stack_frame.AddrStack.Mode = AddrModeFlat;
  while (StackWalk64(machine_type, GetCurrentProcess(), GetCurrentThread(),
                     &stack_frame, &context_record, nullptr,
                     &SymFunctionTableAccess64, &SymGetModuleBase64, nullptr) &&
         size < int(stack.size())) {
    stack[size++] = reinterpret_cast<void*>(stack_frame.AddrPC.Offset);
  }

  struct symbol_bundle : SYMBOL_INFO {
    wchar_t name[MAX_SYM_NAME];
  };

  HANDLE p = GetCurrentProcess();
  static bool sym_initialized = false;
  if (!sym_initialized) {
    sym_initialized = true;
    SymInitialize(p, nullptr, true);
  }
  SymRefreshModuleList(p);
  for (int i = 0; i < size && len > 0; ++i) {
    DWORD_PTR frame_ptr = reinterpret_cast<DWORD_PTR>(stack[i]);

    DWORD64 displacement = 0;
    symbol_bundle symbol;
    symbol.MaxNameLen = MAX_SYM_NAME;
    symbol.SizeOfStruct = sizeof(SYMBOL_INFO);
    BOOL const has_symbol = SymFromAddr(p, frame_ptr, &displacement, &symbol);

    DWORD line_displacement = 0;
    IMAGEHLP_LINE64 line = {};
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    BOOL const has_line = SymGetLineFromAddr64(GetCurrentProcess(), frame_ptr,
                                               &line_displacement, &line);

    int ret = std::snprintf(out, len, "%2d: %p", i, stack[i]);
    out += ret;
    len -= ret;
    if (len <= 0) break;

    if (has_symbol) {
      ret = std::snprintf(out, len, " %s +%-4" PRId64,
                          boost::core::demangle(symbol.Name).c_str(),
                          displacement);
      out += ret;
      len -= ret;
      if (len <= 0) break;
    }

    if (has_line) {
      ret = std::snprintf(out, len, " %s:%d", line.FileName,
                          int(line.LineNumber));
      out += ret;
      len -= ret;
      if (len <= 0) break;
    }

    ret = std::snprintf(out, len, "\n");
    out += ret;
    len -= ret;

    if (i == max_depth && max_depth > 0) break;
  }
}
}  // namespace asap

#else

namespace asap {

void print_backtrace(char* out, int len, int /*max_depth*/, void* /* ctx */) {
  out[0] = 0;
  std::strncat(out, "<not supported>", std::size_t(len));
}

}  // namespace asap

#endif

#endif

namespace asap {

#if ASAP_USE_ASSERTS

ASAP_FORMAT(1, 2)
void assert_print(char const* fmt, ...) {
  FILE* out = stderr;
  va_list va;
  va_start(va, fmt);
  std::vfprintf(out, fmt, va);
  va_end(va);

#ifdef ASAP_PRODUCTION_ASSERTS
  if (out != stderr) fclose(out);
#endif
}

// we deliberately don't want asserts to be marked as no-return, since that
// would trigger warnings in debug builds of any code coming after the assert
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

void assert_fail(char const* expr, int line, char const* file,
                 char const* function, char const* value, int kind) {
#ifdef ASAP_PRODUCTION_ASSERTS
  // no need to flood the assert log with infinite number of asserts
  if (assert_counter.fetch_add(1) + 1 > 500) return;
#endif

  char stack[8192];
  stack[0] = '\0';
  print_backtrace(stack, sizeof(stack), 0);

  char const* message =
      "Assertion failed. Please file a bugreport at "
      "https://github.com/xxxxxxxxxxx/issues\n";

  switch (kind) {
    case 1:
      message =
          "A precondition of a asap function has been violated.\n"
          "This indicates a bug in the client application using asap\n";
  }

  assert_print(
      "%s\n"
#ifdef ASAP_PRODUCTION_ASSERTS
      "#: %d\n"
#endif
      "file: '%s'\n"
      "line: %d\n"
      "function: %s\n"
      "expression: %s\n"
      "%s%s\n"
      "stack:\n"
      "%s\n",
      message
#ifdef ASAP_PRODUCTION_ASSERTS
      ,
      assert_counter.load()
#endif
          ,
      file, line, function, expr, value ? value : "", value ? "\n" : "", stack);

  // if production asserts are defined, don't abort, just print the error
#ifndef ASAP_PRODUCTION_ASSERTS
#ifdef ASAP_WINDOWS
  // SIGINT doesn't trigger a break with msvc
  DebugBreak();
#else
  // send SIGINT to the current process
  // to break into the debugger
  ::raise(SIGABRT);
#endif
  ::abort();
#endif
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#elif !ASAP_USE_ASSERTS

// these are just here to make it possible for a client that built with debug
// enable to be able to link against a release build (just possible, not
// necessarily supported)
ASAP_FORMAT(1, 2)
void assert_print(char const*, ...) {}
void assert_fail(char const*, int, char const*, char const*, char const*, int) {
}

#endif

}  // namespace asap
