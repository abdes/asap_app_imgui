/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <gtest/gtest.h>

#include "test_helper.h"

#include <common/compilers.h>

// Disable compiler and linter warnings originating from the unit test framework and for which we
// cannot do anything.
// Additionally every TEST or TEST_X macro usage must be preceded by a '// NOLINTNEXTLINE'.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__) && ASAP_HAS_WARNING("-Wused-but-marked-unused")
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif
// NOLINTBEGIN(used-but-marked-unused)

namespace asap {
namespace contract {
namespace {

// NOLINTNEXTLINE
TEST(OffModeContractViolations, ExpectDefaultWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestExpectDefault(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(OffModeContractViolations, EnsureDefaultWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestEnsureDefault(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(OffModeContractViolations, AssertDefaultWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestAssertDefault(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(OffModeContractViolations, ExpectAuditWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestExpectAudit(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(OffModeContractViolations, EnsureAuditWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestEnsureAudit(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(OffModeContractViolations, AssertAuditWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestAssertAudit(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(OffModeContractViolations, ExpectAxiomWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestExpectAxiom(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(OffModeContractViolations, EnsureAxiomWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestEnsureAxiom(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(OffModeContractViolations, AssertAxiomWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestAssertAxiom(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

} // namespace
} // namespace contract
} // namespace asap

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
