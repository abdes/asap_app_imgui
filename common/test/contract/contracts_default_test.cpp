//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "test_helper.h"

#include <common/compilers.h>
#include <common/platform.h>

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

// NOLINTNEXTLINE
TEST(DefaultModeContractViolations, ExpectDefaultWillAbort) {
#if !defined(ASAP_WINDOWS)
  // NOLINTNEXTLINE
  ASSERT_EXIT(testing::TestExpectDefault(nullptr),
      ::testing::KilledBySignal(SIGABRT), "precondition.*violated");
#else
  ASSERT_DEATH(testing::TestExpectDefault(nullptr), "");
#endif
}

// NOLINTNEXTLINE
TEST(DefaultModeContractViolations, EnsureDefaultWillAbort) {
#if !defined(ASAP_WINDOWS)
  // NOLINTNEXTLINE
  ASSERT_EXIT(testing::TestEnsureDefault(nullptr),
      ::testing::KilledBySignal(SIGABRT), "postcondition.*violated");
#else
  ASSERT_DEATH(testing::TestEnsureDefault(nullptr), "");
#endif
}

// NOLINTNEXTLINE
TEST(DefaultModeContractViolations, AssertDefaultWillAbort) {
#if !defined(ASAP_WINDOWS)
  // NOLINTNEXTLINE
  ASSERT_EXIT(testing::TestAssertDefault(nullptr),
      ::testing::KilledBySignal(SIGABRT), "assertion.*violated");
#else
  ASSERT_DEATH(testing::TestAssertDefault(nullptr), "");
#endif
}

// NOLINTNEXTLINE
TEST(DefaultModeContractViolations, ExpectAuditWillSegFault) {
#if !defined(ASAP_WINDOWS)
  // NOLINTNEXTLINE
  ASSERT_EXIT(testing::TestExpectAudit(nullptr),
      ::testing::KilledBySignal(SIGSEGV), ".*");
#else
  ASSERT_DEATH(testing::TestExpectAudit(nullptr), "");
#endif
}

// NOLINTNEXTLINE
TEST(DefaultModeContractViolations, EnsureAuditWillSegFault) {
#if !defined(ASAP_WINDOWS)
  // NOLINTNEXTLINE
  ASSERT_EXIT(testing::TestEnsureAudit(nullptr),
      ::testing::KilledBySignal(SIGSEGV), ".*");
#else
  ASSERT_DEATH(testing::TestEnsureAudit(nullptr), "");
#endif
}

// NOLINTNEXTLINE
TEST(DefaultModeContractViolations, AssertAuditWillSegFault) {
#if !defined(ASAP_WINDOWS)
  // NOLINTNEXTLINE
  ASSERT_EXIT(testing::TestAssertAudit(nullptr),
      ::testing::KilledBySignal(SIGSEGV), ".*");
#else
  ASSERT_DEATH(testing::TestAssertAudit(nullptr), "");
#endif
}

} // namespace
} // namespace asap::contract

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
