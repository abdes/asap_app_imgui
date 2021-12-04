/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <common/asap_common_api.h>

#include <csetjmp>
#include <cstdlib>
#include <cstring>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#if __cplusplus >= 201103L
#define ASAP_CONTRACT_THREAD_LOCAL thread_local
#elif __STDC_VERSION__ >= 201112L
#define ASAP_CONTRACT_THREAD_LOCAL _Thread_local
#else
#define ASAP_CONTRACT_THREAD_LOCAL
#endif
// NOLINTEND(cppcoreguidelines-macro-usage)

namespace asap {
namespace contract {

enum class ASAP_COMMON_API Verbosity { QUIET = 0, VERBOSE };

void ASAP_COMMON_API SetVerbosity(enum Verbosity verbosity);

void ASAP_COMMON_API PrepareForTesting();

namespace details {

void ASAP_COMMON_API ContractCheckPush();
void ASAP_COMMON_API ContractCheckPop();

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
extern ASAP_CONTRACT_THREAD_LOCAL ASAP_COMMON_API jmp_buf jmp_env;

} // namespace details

} // namespace contract
} // namespace asap

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define ASAP_CONTRACT_UT_DO_CHECK_VIOLATION_(call, check, expected, msg)                           \
  do {                                                                                             \
    jmp_buf old_jmp_buf;                                                                           \
    int violation_detected = 0;                                                                    \
    asap::contract::details::ContractCheckPush();                                                  \
    /* preserve any existing setjmp environment before the macro was called */                     \
    memcpy(static_cast<void *>(old_jmp_buf),                                                       \
        static_cast<void *>(asap::contract::details::jmp_env), sizeof(old_jmp_buf));               \
    /* Save stack environment for restore using the longjmp call in the test handler.           */ \
    /* Returns 0 after saving the stack environment. If setjmp returns because of a longjmp,    */ \
    /* it returns the value argument of longjmp, or if the value argument of longjmp is 0,      */ \
    /* setjmp returns 1. There's no error return.                                               */ \
    if (!setjmp(&asap::contract::details::jmp_env[0])) {                                           \
      call;                                                                                        \
      /* This will never be reached if a violation occurs. The handler will call longjmp with a */ \
      /* non - zero return value, causing the other branch after setjmp to be executed.         */ \
      violation_detected = 0;                                                                      \
    } else {                                                                                       \
      /* This branch is executed when longjmp from the handler returns a non-zero value         */ \
      /* indicating a violation has occured.                                                    */ \
      violation_detected = 1;                                                                      \
    }                                                                                              \
    check(violation_detected == (expected), msg);                                                  \
    /* restore saved setjmp environment before the macro was called */                             \
    memcpy(static_cast<void *>(asap::contract::details::jmp_env),                                  \
        static_cast<void *>(old_jmp_buf), sizeof(old_jmp_buf));                                    \
    asap::contract::details::ContractCheckPop();                                                   \
  } while (0)

#if defined(_MSC_VER)
#define ASAP_CONTRACT_UT_CHECK_VIOLATION_(call, check)                                             \
  __pragma(warning(push)) __pragma(warning(disable : 4611)) ASAP_CONTRACT_UT_DO_CHECK_VIOLATION_(  \
      call, check, 1, "'" #call ";' does not violate any contract") __pragma(warning(pop))
#else
#define ASAP_CONTRACT_UT_CHECK_VIOLATION_(call, check)                                             \
  ASAP_CONTRACT_UT_DO_CHECK_VIOLATION_(call, check, 1, "'" #call ";' does not violate any contract")
#endif

// NOLINTEND(cppcoreguidelines-macro-usage)
