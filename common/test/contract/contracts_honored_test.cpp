//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "test_helper.h"

#include <common/compilers.h>

#include <gtest/gtest.h>

// Disable compiler and linter warnings originating from the unit test framework
// and for which we cannot do anything. Additionally every TEST or TEST_X macro
// usage must be preceded by a '// NOLINTNEXTLINE'.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__) && ASAP_HAS_WARNING("-Wused-but-marked-unused")
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif
// NOLINTBEGIN(used-but-marked-unused)

namespace asap::contract {
namespace {

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#if defined(ASAP_CONTRACT_DEFAULT)
#define INTERNAL_ACT_TESTSUITE_NAME DefaultModeContrcatsHonored
#elif defined(ASAP_CONTRACT_OFF)
#define INTERNAL_ACT_TESTSUITE_NAME OffModeContrcatsHonored
#elif defined(ASAP_CONTRACT_AUDIT)
#define INTERNAL_ACT_TESTSUITE_NAME AuditModeContrcatsHonored
#else
#define INTERNAL_ACT_TESTSUITE_NAME ContrcatsHonored
#endif
// NOLINTEND(cppcoreguidelines-macro-usage)

// NOLINTNEXTLINE
TEST(INTERNAL_ACT_TESTSUITE_NAME, Expect) {
  static int value = 1;
  testing::TestExpectDefault(&value);
}

// NOLINTNEXTLINE
TEST(INTERNAL_ACT_TESTSUITE_NAME, Ensure) {
  static int value = 1;
  testing::TestEnsureDefault(&value);
}

// NOLINTNEXTLINE
TEST(INTERNAL_ACT_TESTSUITE_NAME, Assert) {
  static int value = 1;
  testing::TestAssertDefault(&value);
}

// NOLINTNEXTLINE
TEST(INTERNAL_ACT_TESTSUITE_NAME, ExpectAudit) {
  static int value = 1;
  testing::TestExpectAudit(&value);
}

// NOLINTNEXTLINE
TEST(INTERNAL_ACT_TESTSUITE_NAME, EnsureAudit) {
  static int value = 1;
  testing::TestEnsureAudit(&value);
}

// NOLINTNEXTLINE
TEST(INTERNAL_ACT_TESTSUITE_NAME, AssertAudit) {
  static int value = 1;
  testing::TestAssertAudit(&value);
}

} // namespace
} // namespace asap::contract

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
