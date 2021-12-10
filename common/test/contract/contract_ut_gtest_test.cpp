/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "contract/ut/framework.h"
#include "contract/ut/gtest.h"
#include "test_helper.h"

#include <common/compilers.h>

#include <gtest/gtest.h>

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

namespace asap::contract {
namespace {

// NOLINTNEXTLINE
TEST(GoogleTestDeathMacros, DefaultModeExpectDeath) {
  CHECK_VIOLATES_CONTRACT(testing::TestExpectDefault(nullptr));
}

// NOLINTNEXTLINE
TEST(GoogleTestDeathMacros, DefaultModeEnsureDeath) {
  CHECK_VIOLATES_CONTRACT(testing::TestEnsureDefault(nullptr));
}

// NOLINTNEXTLINE
TEST(GoogleTestDeathMacros, DefaultModeAssertDeath) {
  CHECK_VIOLATES_CONTRACT(testing::TestAssertDefault(nullptr));
}

void NestedViolator(int *ptr) {
  CHECK_VIOLATES_CONTRACT(testing::TestAssertDefault(ptr));
  testing::TestEnsureDefault(nullptr);
}
void Violator(int *ptr) {
  CHECK_VIOLATES_CONTRACT(testing::TestAssertDefault(ptr));
  CHECK_VIOLATES_CONTRACT(NestedViolator(ptr));
  testing::TestExpectDefault(nullptr);
}

// NOLINTNEXTLINE
TEST(GoogleTestDeathMacros, NestedChecks) {
  CHECK_VIOLATES_CONTRACT(Violator(nullptr));
}

} // namespace
}  // namespace asap::contract

auto main(int argc, char **argv) -> int {
  asap::contract::PrepareForTesting();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
