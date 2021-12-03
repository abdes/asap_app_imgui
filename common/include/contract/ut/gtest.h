/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include "framework.h"
#include <gtest/gtest.h>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

#define CHECK_VIOLATES_CONTRACT(call) ASSERT_VIOLATES_CONTRACT(call)

#define ASSERT_VIOLATES_CONTRACT(call)                                                             \
  ASAP_CONTRACT_UT_CHECK_VIOLATION_(call, ASAP_INTERNAL_FATAL_CHECK)

#define EXPECT_VIOLATES_CONTRACT(call)                                                             \
  ASAP_CONTRACT_UT_CHECK_VIOLATION_(call, ASAP_INTERNAL_NON_FATAL_CHECK)

#define ASAP_INTERNAL_FATAL_CHECK(cond, msg) ASSERT_TRUE(cond) << msg;

#define ASAP_INTERNAL_NON_FATAL_CHECK(cond, msg) EXPECT_TRUE(cond) << msg;

// NOLINTEND(cppcoreguidelines-macro-usage)
