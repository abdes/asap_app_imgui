//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

/*!
 * \file
 *
 * \brief Macros used with Google Test unit testing framework to test contract
 * checking assertions without the need for death tests.
 *
 * To use, include the following call in your test main:
 * ```
 * auto main(int argc, char **argv) -> int {
 *   asap::contract::PrepareForTesting();
 *   ::testing::InitGoogleTest(&argc, argv);
 *   return RUN_ALL_TESTS();
 * }
 * ```
 */

#pragma once

#include "framework.h"
#include <gtest/gtest.h>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

// -------------------------------------------------------------------------------------------------
// Public test macros
// -------------------------------------------------------------------------------------------------

/*!
 * \brief An alias for ASSERT_VIOLATES_CONTRACT(), checks that the statement
 * produces a contract violation and if not, the test is immediately terminated.
 *
 * If the statement does not produce a violation contract, execution is aborted.
 *
 * \note The implementation of this macro is not thread safe.
 *
 * \param call the statement to test.
 *
 * \see ASSERT_VIOLATES_CONTRACT
 * \see EXPECT_VIOLATES_CONTRACT
 */
#define CHECK_VIOLATES_CONTRACT(call) ASSERT_VIOLATES_CONTRACT(call)

/*!
 * \brief Assert that the statement produces a contract violation. If the
 * statement does not produce a contract violation contract, the test is
 * immediately terminated.
 *
 * Use ASSERT_VIOLATES_CONTRACT when the condition must hold - if it doesn't the
 * test stops right there. Use this when the remainder of the test doesn't have
 * semantic meaning without this condition holding.
 *
 * \note The implementation of this macro is not thread safe.
 *
 * \param call the statement to test.
 *
 * \see EXPECT_VIOLATES_CONTRACT
 */
#define ASSERT_VIOLATES_CONTRACT(call)                                         \
  ASAP_CONTRACT_UT_CHECK_VIOLATION_(call, ASAP_INTERNAL_FATAL_CHECK)

/*!
 * \brief Expect the statement to produce a contract violation. If the statement
 * does not produce a contract, execution still continues.
 *
 * Use EXPECT when the condition should hold, but in cases where it doesn't we
 * can still get value out of continuing the test. The test will still
 * ultimately fail at the end, though.
 *
 * \note The implementation of this macro is not thread safe.
 *
 * \param call the statement to test.
 *
 * \see ASSERT_VIOLATES_CONTRACT
 */
#define EXPECT_VIOLATES_CONTRACT(call)                                         \
  ASAP_CONTRACT_UT_CHECK_VIOLATION_(call, ASAP_INTERNAL_NON_FATAL_CHECK)

// -------------------------------------------------------------------------------------------------
// Internal macros
// -------------------------------------------------------------------------------------------------

// Exclude internal macros from doxygen documentation
#if !defined(DOXYGEN_DOCUMENTATION_BUILD)
#define ASAP_INTERNAL_FATAL_CHECK(cond, msg) ASSERT_TRUE(cond) << (msg);
#define ASAP_INTERNAL_NON_FATAL_CHECK(cond, msg) EXPECT_TRUE(cond) << (msg);
#endif // DOXYGEN_DOCUMENTATION_BUILD

// NOLINTEND(cppcoreguidelines-macro-usage)
