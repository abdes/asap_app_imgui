/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

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

enum class Verbosity { QUIET = 0, VERBOSE };

void SetVerbosity(enum Verbosity verbosity);

void PrepareForTesting();

namespace details {

void ContractCheckPush();
void ContractCheckPop();

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
extern ASAP_CONTRACT_THREAD_LOCAL jmp_buf jmp_env;

} // namespace details

} // namespace contract
} // namespace asap

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASAP_CONTRACT_UT_DO_CHECK_VIOLATION_(call, check, expected, msg)                           \
  do {                                                                                             \
    jmp_buf old_jmp_buf;                                                                           \
    int violation_detected = 0;                                                                    \
    asap::contract::details::ContractCheckPush();                                                  \
    memcpy(static_cast<void *>(old_jmp_buf),                                                       \
        static_cast<void *>(asap::contract::details::jmp_env), sizeof(old_jmp_buf));               \
    if (!setjmp(&asap::contract::details::jmp_env[0])) {                 \
      call;                                                                                        \
      violation_detected = 0;                                                                      \
    } else {                                                                                       \
      violation_detected = 1;                                                                      \
    }                                                                                              \
    check(violation_detected == (expected), msg);                                                  \
    memcpy(static_cast<void *>(asap::contract::details::jmp_env),                                  \
        static_cast<void *>(old_jmp_buf), sizeof(old_jmp_buf));                                    \
    asap::contract::details::ContractCheckPop();                                                   \
  } while (0)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASAP_CONTRACT_UT_CHECK_VIOLATION_(call, check)                                             \
  ASAP_CONTRACT_UT_DO_CHECK_VIOLATION_(call, check, 1, "'" #call ";' does not violate any contract")
