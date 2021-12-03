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
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif
// NOLINTBEGIN(used-but-marked-unused)

namespace asap {
namespace contract {
namespace {

// NOLINTNEXTLINE
TEST(AuditModeContractViolations, ExpectDefaultWillAbort) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestExpectDefault(nullptr), exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}

// NOLINTNEXTLINE
TEST(AuditModeContractViolations, EnsureDefaultWillAbort) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestEnsureDefault(nullptr), exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}

// NOLINTNEXTLINE
TEST(AuditModeContractViolations, AssertDefaultWillAbort) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestAssertDefault(nullptr), exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}

// NOLINTNEXTLINE
TEST(AuditModeContractViolations, ExpectAuditWillAbort) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestExpectAudit(nullptr), exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}

// NOLINTNEXTLINE
TEST(AuditModeContractViolations, EnsureAuditWillAbort) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestEnsureAudit(nullptr), exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}

// NOLINTNEXTLINE
TEST(AuditModeContractViolations, AssertAuditWillAbort) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestAssertAudit(nullptr), exit(0)), ::testing::KilledBySignal(SIGABRT), ".*");
}

// NOLINTNEXTLINE
TEST(AuditModeContractViolations, ExpectAxiomWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestExpectAxiom(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(AuditModeContractViolations, EnsureAxiomWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestEnsureAxiom(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

// NOLINTNEXTLINE
TEST(AuditModeContractViolations, AssertAxiomWillSegFault) {
  // NOLINTNEXTLINE
  ASSERT_EXIT(
      (testing::TestAssertAxiom(nullptr), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

} // namespace
} // namespace contract
} // namespace asap

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
