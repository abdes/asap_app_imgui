//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include <fsm/fsm.h>

#include <common/compilers.h>

#include <gmock/gmock-more-matchers.h>
#include <gtest/gtest.h>

// Disable compiler and linter warnings originating from the unit test framework
// and for which we cannot do anything. Additionally, every TEST or TEST_X macro
// usage must be preceded by a '// NOLINTNEXTLINE'.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__) && ASAP_HAS_WARNING("-Wused-but-marked-unused")
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif
// NOLINTBEGIN(used-but-marked-unused)

using testing::Eq;
using testing::IsNull;
using testing::Ne;
using testing::StrEq;

namespace asap::fsm {

namespace {

// NOLINTNEXTLINE
TEST(StateMachineError, DefaultConstructor) {
  StateMachineError err;
  EXPECT_THAT(err.What(), Ne(""));
}

// NOLINTNEXTLINE
TEST(StateMachineError, ConstructWithMessage) {
  StateMachineError err("__error__");
  EXPECT_THAT(err.What(), StrEq("__error__"));
}

// NOLINTNEXTLINE
TEST(StateMachineError, CopyConstructor) {
  StateMachineError err("__error__");
  StateMachineError copy(err); // NOLINT
  EXPECT_THAT(std::string(copy.What()), Eq(std::string(err.What())));
}

// NOLINTNEXTLINE
TEST(StateMachineError, MoveConstructor) {
  StateMachineError err("__error__");
  StateMachineError copy(std::move(err));
  EXPECT_THAT(copy.What(), StrEq("__error__"));
}

// NOLINTNEXTLINE
TEST(StateMachineError, Assign) {
  StateMachineError err("__error__");
  StateMachineError copy = err; // NOLINT
  EXPECT_THAT(copy.What(), StrEq("__error__"));
  EXPECT_THAT(err.What(), StrEq("__error__"));
}

// NOLINTNEXTLINE
TEST(StateMachineError, MoveAssign) {
  StateMachineError err("__error__");
  StateMachineError copy = std::move(err);
  EXPECT_THAT(copy.What(), StrEq("__error__"));
}

} // namespace

} // namespace asap::fsm

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
