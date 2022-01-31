//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include <fsm/fsm.h>

#include <common/compilers.h>
#include <exception>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Disable compiler and linter warnings originating from the unit test framework
// and for which we cannot do anything. Additionally, every TEST or TEST_X macro
// usage must be preceded by a '// NOLINTNEXTLINE'.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__) && ASAP_HAS_WARNING("-Wused-but-marked-unused")
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif
// NOLINTBEGIN(used-but-marked-unused)

using testing::Eq;
using testing::IsTrue;

namespace asap::fsm {

namespace {

//! [Full State Machine Example]
/*
 * This example simulates a door with an electronic lock. When the door
 * is locked, the user choses a lock code that needs to be re-entered
 * to unlock it.
 */

struct OpenEvent {};
struct CloseEvent {};

struct LockEvent {
  uint32_t newKey; // the lock code chosen by the user
};

struct UnlockEvent {
  uint32_t key; // the lock key entered when unlocking
};

struct ClosedState;
struct OpenState;
struct LockedState;

struct ClosedState
    : Will<ByDefault<DoNothing>, On<LockEvent, TransitionTo<LockedState>>,
          On<OpenEvent, TransitionTo<OpenState>>> {};

struct OpenState
    : Will<ByDefault<DoNothing>, On<CloseEvent, TransitionTo<ClosedState>>> {};

struct LockedState : ByDefault<DoNothing> {
  using ByDefault::Handle;

  explicit LockedState(uint32_t key) : key_(key) {
  }

  auto OnEnter(const LockEvent &event) -> Status {
    key_ = event.newKey;
    return Continue{};
  }

  //! [State Handle method]
  [[nodiscard]] auto Handle(const UnlockEvent &event) const
      -> Maybe<TransitionTo<ClosedState>> {
    if (event.key == key_) {
      return TransitionTo<ClosedState>{};
    }
    return DoNothing{};
  }
  //! [State Handle method]

private:
  uint32_t key_;
};

using Door = StateMachine<ClosedState, OpenState, LockedState>;

// NOLINTNEXTLINE
TEST(StateMachine, ExampleTest) {
  Door door{ClosedState{}, OpenState{}, LockedState{0}};

  constexpr int lock_code = 1234;
  constexpr int bad_code = 2;

  door.Handle(LockEvent{lock_code});
  door.Handle(UnlockEvent{bad_code});
  door.Handle(UnlockEvent{lock_code});
}
//! [Full State Machine Example]

} // namespace

} // namespace asap::fsm

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
