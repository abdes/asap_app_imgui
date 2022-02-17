//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

/*!
 * \file
 *
 * \brief Declaration for the unit testing support for contract checking.
 */

#pragma once

#include <contract/asap_contract_api.h>

#include <csetjmp>
#include <cstdlib>
#include <cstring>

namespace asap::contract {

/// Verbosity level of the test violation handler.
enum class Verbosity {
  /// Do not print anything
  QUIET = 0,
  /// Print the same information as if the default handler were running.
  VERBOSE
};

/*!
 * \brief Set the verbosity level of the test violation handler.
 *
 * \param verbosity a value of Verbosity::QUIET will not produce any output,
 * while a value of Verbosity::VERBOSE will produce the same output than the
 * default handler.
 */
void ASAP_CONTRACT_API SetVerbosity(enum Verbosity verbosity);

/*!
 * \brief Prepare the violation handler for testing.
 *
 * This function replaces the default violation handler with a special purpose
 * handler that allows to test the contract checking assertions without the need
 * for sophisticated death test features from the unit testing framework.
 *
 * Here is an example of its usage with the Google Test framework:
 *
 * ```
 * auto main(int argc, char **argv) -> int {
 *   asap::contract::PrepareForTesting();
 *   ::testing::InitGoogleTest(&argc, argv);
 *   return RUN_ALL_TESTS();
 * }
 * ```
 *
 * \see CHECK_VIOLATES_CONTRACT
 * \see EXPECT_VIOLATES_CONTRACT
 * \see ASSERT_VIOLATES_CONTRACT
 */
void ASAP_CONTRACT_API PrepareForTesting();

namespace details {
/// Push a contract check for nested violation checks.
void ASAP_CONTRACT_API ContractCheckPush();
/// Pop a contract check for nested violation checks.
void ASAP_CONTRACT_API ContractCheckPop();

/// Stack environment saved/restored with the setjmp/longjmp used to handle
/// contract violations during testing.
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
extern ASAP_CONTRACT_API jmp_buf jmp_env;

} // namespace details

} // namespace asap::contract

#if !defined(DOXYGEN_DOCUMENTATION_BUILD)

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define ASAP_CONTRACT_UT_DO_CHECK_VIOLATION_(call, check, expected, msg)       \
  do {                                                                         \
    jmp_buf old_jmp_buf;                                                       \
    int violation_detected = 0;                                                \
    asap::contract::details::ContractCheckPush();                              \
    /* preserve any existing setjmp environment before the macro was called */ \
    memcpy(static_cast<void *>(old_jmp_buf),                                   \
        static_cast<void *>(asap::contract::details::jmp_env),                 \
        sizeof(old_jmp_buf));                                                  \
    /* Save stack environment for restore using the longjmp call in the test   \
     * handler.           */                                                   \
    /* Returns 0 after saving the stack environment. If setjmp returns because \
     * of a longjmp,    */                                                     \
    /* it returns the value argument of longjmp, or if the value argument of   \
     * longjmp is 0,      */                                                   \
    /* setjmp returns 1. There's no error return. */                           \
    if (!setjmp(&asap::contract::details::jmp_env[0])) {                       \
      call;                                                                    \
      /* This will never be reached if a violation occurs. The handler will    \
       * call longjmp with a non-zero return value, causing the other branch   \
       * after setjmp to be */                                                 \
      violation_detected = 0;                                                  \
    } else {                                                                   \
      /* This branch is executed when longjmp from the handler returns a       \
       * non-zero value indicating a violation has occurred. */                \
      violation_detected = 1;                                                  \
    }                                                                          \
    check(violation_detected == (expected), msg);                              \
    /* restore saved setjmp environment before the macro was called */         \
    memcpy(static_cast<void *>(asap::contract::details::jmp_env),              \
        static_cast<void *>(old_jmp_buf), sizeof(old_jmp_buf));                \
    asap::contract::details::ContractCheckPop();                               \
  } while (0)

#if defined(_MSC_VER)
#define ASAP_CONTRACT_UT_CHECK_VIOLATION_(call, check)                         \
  __pragma(warning(push)) __pragma(warning(disable : 4611))                    \
      ASAP_CONTRACT_UT_DO_CHECK_VIOLATION_(call, check, 1,                     \
          "'" #call ";' does not violate any contract") __pragma(warning(pop))
#else
#define ASAP_CONTRACT_UT_CHECK_VIOLATION_(call, check)                         \
  ASAP_CONTRACT_UT_DO_CHECK_VIOLATION_(                                        \
      call, check, 1, "'" #call ";' does not violate any contract")
#endif

#endif // DOXYGEN_DOCUMENTATION_BUILD

// NOLINTEND(cppcoreguidelines-macro-usage)
