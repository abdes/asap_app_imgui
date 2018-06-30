//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <common/platform.h>

#ifndef ASAP_USE_ASSERTS
#define ASAP_USE_ASSERTS 1
#endif  // ASAP_USE_ASSERTS


#ifndef ASAP_USE_EXECINFO
#define ASAP_USE_EXECINFO 0
#endif

#if ASAP_COMPILER_IS_GNU
#define ASAP_FUNCTION __PRETTY_FUNCTION__
#else
#define ASAP_FUNCTION __FUNCTION__
#endif

#if defined __GNUC__ || defined __clang__
#define ASAP_FORMAT(fmt, ellipsis) \
  __attribute__((__format__(__printf__, fmt, ellipsis)))
#else
#define ASAP_FORMAT(fmt, ellipsis)
#endif


#if ASAP_USE_ASSERTS


#include <string>
namespace asap {
void print_backtrace(char* out, int len, int max_depth = 0,
                     void* ctx = nullptr);
}  // namespace asap
#endif

// This is to disable the warning of conditional expressions
// being constant in msvc
// clang-format off
#ifdef _MSC_VER
#define ASAP_WHILE_0  \
	__pragma( warning(push) ) \
	__pragma( warning(disable:4127) ) \
	while (false) \
	__pragma( warning(pop) )
#else
#define ASAP_WHILE_0 while (false)
#endif
// clang-format on

namespace asap {
// declarations of the two functions

// internal
void assert_print(char const* fmt, ...) ASAP_FORMAT(1, 2);

// internal
void assert_fail(const char* expr, int line, char const* file,
                 char const* function, char const* val, int kind = 0);

}  // namespace asap

#if ASAP_USE_ASSERTS

#ifdef ASAP_PRODUCTION_ASSERTS
extern char const* asap_assert_log;
#endif

#if ASAP_USE_IOSTREAM
#include <sstream>
#endif

#ifndef ASAP_USE_SYSTEM_ASSERTS

#define ASAP_ASSERT_PRECOND(x)                                             \
  do {                                                                        \
    if (x) {                                                                  \
    } else                                                                    \
      asap::assert_fail(#x, __LINE__, __FILE__, ASAP_FUNCTION, nullptr, \
                           1);                                                \
  }                                                                           \
  ASAP_WHILE_0

#define ASAP_ASSERT(x)                                                     \
  do {                                                                        \
    if (x) {                                                                  \
    } else                                                                    \
      asap::assert_fail(#x, __LINE__, __FILE__, ASAP_FUNCTION, nullptr, \
                           0);                                                \
  }                                                                           \
  ASAP_WHILE_0

#define ASAP_ASSERT_VAL(x, y)                                     \
  do {                                                               \
    if (x) {                                                         \
    } else {                                                         \
      std::stringstream __s__;                                       \
      __s__ << #y ": " << y;                                         \
      asap::assert_fail(#x, __LINE__, __FILE__, ASAP_FUNCTION, \
                           __s__.str().c_str(), 0);                  \
    }                                                                \
  }                                                                  \
  ASAP_WHILE_0

#define ASAP_ASSERT_FAIL_VAL(y)                                  \
  do {                                                              \
    std::stringstream __s__;                                        \
    __s__ << #y ": " << y;                                          \
    asap::assert_fail("<unconditional>", __LINE__, __FILE__,     \
                         ASAP_FUNCTION, __s__.str().c_str(), 0); \
  }                                                                 \
  ASAP_WHILE_0


#define ASAP_ASSERT_FAIL()                                 \
  asap::assert_fail("<unconditional>", __LINE__, __FILE__, \
                       ASAP_FUNCTION, nullptr, 0)

#else
#include <cassert>
#define ASAP_ASSERT_PRECOND(x) assert(x)
#define ASAP_ASSERT(x) assert(x)
#define ASAP_ASSERT_VAL(x, y) assert(x)
#define ASAP_ASSERT_FAIL_VAL(x) assert(false)
#define ASAP_ASSERT_FAIL() assert(false)
#endif

#else  // ASAP_USE_ASSERTS

#define ASAP_ASSERT_PRECOND(a) \
  do {                            \
  }                               \
  ASAP_WHILE_0
#define ASAP_ASSERT(a) \
  do {                    \
  }                       \
  ASAP_WHILE_0
#define ASAP_ASSERT_VAL(a, b) \
  do {                           \
  }                              \
  ASAP_WHILE_0
#define ASAP_ASSERT_FAIL_VAL(a) \
  do {                             \
  }                                \
  ASAP_WHILE_0
#define ASAP_ASSERT_FAIL() \
  do {                        \
  }                           \
  ASAP_WHILE_0

#endif  // ASAP_USE_ASSERTS
