//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include <fsm/fsm.h>

#include <any>
#include <common/compilers.h>

#include <exception>

#include <gmock/gmock-spec-builders.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>

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

using testing::ByRef;
using testing::Eq;
using testing::IsFalse;
using testing::IsTrue;
using testing::Ref;
using testing::Return;

namespace asap::fsm {

namespace {

// NOLINTNEXTLINE
TEST(StateMachine, MachineHandleEventRelaysToStateAndExecutesReturnedAction) {
  struct TestEvent {};
  struct DefaultedEvent {};
  struct FirstState;
  using Machine = StateMachine<FirstState>;

  // This abstraction is only here to manage the circular dependencies created
  // by mocking the TestAction class.
  struct Action { // NOLINT
    virtual auto Execute(Machine &machine, FirstState &state,
        const TestEvent &event) -> Status = 0;

  protected:
    ~Action() = default;
  };

  struct TestAction {
    auto Execute(Machine &machine, FirstState &state, const TestEvent &event)
        -> Status {
      return mock_->Execute(machine, state, event);
    }
    const std::shared_ptr<Action> mock_;
  };

  struct MockState {
    // NOLINTNEXTLINE
    MOCK_METHOD(TestAction, Handle, (const TestEvent &), ());
  };

  struct FirstState : ByDefault<DoNothing> {
    using ByDefault::Handle;
    explicit FirstState(std::shared_ptr<MockState> mock)
        : mock_{std::move(mock)} {
    }
    auto Handle(const TestEvent &event) -> TestAction {
      return mock_->Handle(event);
    }
    const std::shared_ptr<MockState> mock_;
  };

  // NOLINTNEXTLINE
  struct MockAction final : public Action {
    // NOLINTNEXTLINE
    MOCK_METHOD(
        Status, Execute, (Machine &, FirstState &, const TestEvent &), ());
  };

  auto mock_state = std::make_shared<MockState>();
  auto mock_action = std::make_shared<MockAction>();

  Machine machine{FirstState{mock_state}};

  TestEvent test_event{};
  TestAction test_action{mock_action};
  auto &state = machine.TransitionTo<FirstState>();
  EXPECT_CALL(*mock_state, Handle)
      .Times(1)
      .WillOnce(Return(ByRef(test_action)));
  EXPECT_CALL(*mock_action, Execute(Ref(machine), Ref(state), Ref(test_event)))
      .Times(1);
  machine.Handle(test_event);

  // An event falling under a ByDefault rule will not trigger a call to the
  // state Handle method
  DefaultedEvent event{};
  EXPECT_CALL(*mock_state, Handle).Times(0);
  machine.Handle(event);
}

// NOLINTNEXTLINE
TEST(StateMachine, DoNothingExample) {
  //! [DoNothing action example]
  struct DoNothingEvent {};
  struct TestState {
    static auto Handle(const DoNothingEvent & /*event*/) -> DoNothing {
      // Returning the `DoNothing` action will result in no side effects
      // from the state machine calling the `Execute` method of the action.
      return DoNothing{};
    }
  };

  StateMachine<TestState> machine{TestState{}};
  machine.Handle(DoNothingEvent{});
  //! [DoNothing action example]

  ASSERT_THAT(machine.IsIn<TestState>(), IsTrue());
}

// NOLINTNEXTLINE
TEST(StateMachine, ByDefaultExample) {
  //! [ByDefault example]
  struct NotForMeEvent {};
  struct SpecialEvent {};
  struct SecondState : Will<ByDefault<DoNothing>> {};

  // If an event does not have a dedicated `Handle` overload, use the
  // `ByDefault` action (i.e. `DoNothing` in this case).
  struct FirstState : ByDefault<DoNothing> {
    // Bring in the `Handle` overloads from the `ByDefault` base type.
    using ByDefault::Handle;

    // A dedicated overload of `Handle` for the special event ensures
    // that it is not using the `ByDefault` action.
    static auto Handle(const SpecialEvent & /*event*/)
        -> TransitionTo<SecondState> {
      return TransitionTo<SecondState>{};
    }
  };

  StateMachine<FirstState, SecondState> machine{FirstState{}, SecondState{}};

  machine.Handle(NotForMeEvent{}); // DoNothing
  ASSERT_THAT(machine.IsIn<FirstState>(), IsTrue());

  machine.Handle(SpecialEvent{}); // TransitionTo AnotherState
  ASSERT_THAT(machine.IsIn<SecondState>(), IsTrue());
  //! [ByDefault example]
}

// NOLINTNEXTLINE
TEST(StateMachine, OnExample) {
  //! [On example]
  struct SpecialEvent {};
  struct SecondState : Will<ByDefault<DoNothing>> {};

  struct FirstState : Will<On<SpecialEvent, TransitionTo<SecondState>>> {};

  StateMachine<FirstState, SecondState> machine{FirstState{}, SecondState{}};

  machine.Handle(SpecialEvent{}); // TransitionTo AnotherState
  ASSERT_THAT(machine.IsIn<SecondState>(), IsTrue());
  //! [On example]
}

// NOLINTNEXTLINE
TEST(StateMachine, WillExample) {
  //! [Will example]
  struct EventOne {};
  struct EventTwo {};
  struct EventThree {};

  struct FirstState;
  struct SecondState;
  using Machine = StateMachine<FirstState, SecondState>;

  struct FirstState
      : Will<ByDefault<DoNothing>, On<EventOne, TransitionTo<SecondState>>> {};

  struct SpecialAction {
    static auto Execute(Machine & /*machine*/, SecondState & /*state*/,
        const EventTwo & /*event*/) -> Status {
      return Continue{};
    }
  };

  struct SecondState
      : Will<ByDefault<DoNothing>, On<EventThree, TransitionTo<FirstState>>> {
    using Will::Handle;

    // A dedicated overload of `Handle` for the special event ensures
    // that it is not using the `ByDefault` action.
    static auto Handle(const EventTwo & /*event*/) -> SpecialAction {
      return SpecialAction{};
    }
  };

  Machine machine{FirstState{}, SecondState{}};

  EXPECT_THAT(machine.IsIn<FirstState>(), IsTrue());
  machine.Handle(EventOne{});
  EXPECT_THAT(machine.IsIn<SecondState>(), IsTrue());
  machine.Handle(EventTwo{}); // SpecialAction `Execute` called
  EXPECT_THAT(machine.IsIn<SecondState>(), IsTrue());
  machine.Handle(EventThree{});
  EXPECT_THAT(machine.IsIn<FirstState>(), IsTrue());
  //! [Will example]
}

// NOLINTNEXTLINE
TEST(StateMachine, OneOfExample) {
  //! [OneOf example]
  struct SpecialEvent {};

  struct FirstState;
  struct SecondState;
  using Machine = StateMachine<FirstState, SecondState>;

  struct SecondState : Will<ByDefault<DoNothing>> {};

  struct SpecialAction {
    static auto Execute(Machine & /*machine*/, FirstState & /*state*/,
        const SpecialEvent & /*event*/) -> Status {
      return Continue{};
    }
  };

  struct FirstState {
    explicit FirstState(bool transition) : transition_{transition} {
    }

    // This handler has two alternate paths. We use the `OneOf`
    // helper to still be able to return a single action type
    [[nodiscard]] auto Handle(const SpecialEvent & /*event*/) const
        -> OneOf<TransitionTo<SecondState>, SpecialAction> {
      if (transition_) {
        return TransitionTo<SecondState>{};
      }
      return SpecialAction{};
    }
    const bool transition_;
  };

  Machine machine1{FirstState{true}, SecondState{}};
  try {
    machine1.Handle(SpecialEvent{});
  } catch (...) {
  }
  ASSERT_THAT(machine1.IsIn<SecondState>(), IsTrue());

  Machine machine2{FirstState{false}, SecondState{}};
  machine2.Handle(SpecialEvent{});
  ASSERT_THAT(machine2.IsIn<FirstState>(), IsTrue());
  //! [OneOf example]
}

// NOLINTNEXTLINE
TEST(StateMachine, MaybeExample) {
  //! [Maybe example]
  struct SpecialEvent {
    bool transition{false};
  };

  struct FirstState;
  struct SecondState;
  using Machine = StateMachine<FirstState, SecondState>;

  struct SecondState : Will<ByDefault<DoNothing>> {};

  struct FirstState {
    // This handler either transitions to `AnotherState` or does
    // nothing at all
    [[nodiscard]] static auto Handle(const SpecialEvent &event)
        -> Maybe<TransitionTo<SecondState>> {
      if (event.transition) {
        return TransitionTo<SecondState>{};
      }
      return DoNothing{};
    }
  };

  Machine machine1{FirstState{}, SecondState{}};
  machine1.Handle(SpecialEvent{true});
  ASSERT_THAT(machine1.IsIn<SecondState>(), IsTrue());

  Machine machine2{FirstState{}, SecondState{}};
  machine2.Handle(SpecialEvent{false});
  ASSERT_THAT(machine2.IsIn<FirstState>(), IsTrue());
  //! [Maybe example]
}

// NOLINTNEXTLINE
TEST(StateMachine, TransitionToExample) {
  //! [TransitionTo example]
  struct TransitionEvent {};

  struct ExampleMockState {
    // NOLINTNEXTLINE
    MOCK_METHOD(Status, OnEnter, (const TransitionEvent &), ());
    // NOLINTNEXTLINE
    MOCK_METHOD(Status, OnLeave, (const TransitionEvent &), ());
  };

  struct FirstState;
  struct SecondState;
  using Machine = StateMachine<FirstState, SecondState>;

  struct FirstState : Will<On<TransitionEvent, TransitionTo<SecondState>>> {
    explicit FirstState(std::shared_ptr<ExampleMockState> mock)
        : mock_{std::move(mock)} {
    }
    // We only implement the OnLeave method, but that's ok.
    // The action will only call whatever is implemented.
    auto OnLeave(const TransitionEvent &event) -> Status {
      return mock_->OnLeave(event);
    }

  private:
    const std::shared_ptr<ExampleMockState> mock_;
  };

  struct SecondState : Will<ByDefault<DoNothing>> {
    explicit SecondState(std::shared_ptr<ExampleMockState> mock)
        : mock_{std::move(mock)} {
    }
    // We only implement the OnEnter method, but that's ok.
    // The action will only call whatever is implemented.
    auto OnEnter(const TransitionEvent &event) -> Status {
      return mock_->OnEnter(event);
    }

  private:
    const std::shared_ptr<ExampleMockState> mock_;
  };

  auto mock_initial_state = std::make_shared<ExampleMockState>();
  auto mock_another_state = std::make_shared<ExampleMockState>();
  Machine machine{
      FirstState{mock_initial_state}, SecondState{mock_another_state}};

  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(Return(Continue{}));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event)))
      .Times(1)
      .WillOnce(Return(Status{Continue{}}));
  machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsTrue());
  //! [TransitionTo example]
}

// NOLINTNEXTLINE
TEST(StateMachine, TransitionToWithDataExample) {
  //! [TransitionTo with data example]
  struct TransitionEvent {};

  struct ExampleMockState {
    // NOLINTNEXTLINE
    MOCK_METHOD(Status, OnEnter, (const TransitionEvent &, int), ());
    // NOLINTNEXTLINE
    MOCK_METHOD(Status, OnLeave, (const TransitionEvent &), ());
  };

  struct FirstState;
  struct SecondState;
  using Machine = StateMachine<FirstState, SecondState>;

  struct FirstState {
    explicit FirstState(std::shared_ptr<ExampleMockState> mock)
        : mock_{std::move(mock)} {
    }
    // We only implement the OnLeave method, but that's ok.
    // The action will only call whatever is implemented.
    auto OnLeave(const TransitionEvent &event) -> Status {
      return mock_->OnLeave(event);
    }

    // This special handler allows for passing data via the
    // transition object to the next state which can be consumed
    // if that state implements `onEnter(event, data)`
    [[nodiscard]] static auto Handle(const TransitionEvent & /*event*/)
        -> TransitionTo<SecondState> {
      return TransitionTo<SecondState>{1};
    }

  private:
    const std::shared_ptr<ExampleMockState> mock_;
  };

  struct SecondState : Will<ByDefault<DoNothing>> {
    explicit SecondState(std::shared_ptr<ExampleMockState> mock)
        : mock_{std::move(mock)} {
    }
    // This implementation expects data to be passed from the
    // previous state.
    auto OnEnter(const TransitionEvent &event, std::any data) -> Status {
      EXPECT_THAT(std::any_cast<int>(data), Eq(1));
      // Mocking methods with std::any arguments will fail to compile with clang
      // and there is no fix for it from gmock. Just specifically cast the
      // std::any before forwarding to the mock.
      return mock_->OnEnter(event, std::any_cast<int>(data));
    }

  private:
    const std::shared_ptr<ExampleMockState> mock_;
  };

  auto mock_initial_state = std::make_shared<ExampleMockState>();
  auto mock_another_state = std::make_shared<ExampleMockState>();
  Machine machine{
      FirstState{mock_initial_state}, SecondState{mock_another_state}};

  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(Return(Continue{}));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event), ::testing::_))
      .Times(1)
      .WillOnce(Return(Status{Continue{}}));
  machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsTrue());
  //! [TransitionTo with data example]
}

struct StateMachineTransitionToErrors : public ::testing::Test {
  struct TransitionEvent {};

  struct ExampleMockState {
    // NOLINTNEXTLINE
    MOCK_METHOD(Status, OnEnter, (const TransitionEvent &), ());
    // NOLINTNEXTLINE
    MOCK_METHOD(Status, OnLeave, (const TransitionEvent &), ());
  };

  struct FirstState;
  struct SecondState;
  using Machine = StateMachine<FirstState, SecondState>;

  struct FirstState : Will<On<TransitionEvent, TransitionTo<SecondState>>> {
    explicit FirstState(std::shared_ptr<ExampleMockState> mock)
        : mock_{std::move(mock)} {
    }
    // We only implement the OnLeave method, but that's ok.
    // The action will only call whatever is implemented.
    auto OnLeave(const TransitionEvent &event) -> Status {
      return mock_->OnLeave(event);
    }

  private:
    const std::shared_ptr<ExampleMockState> mock_;
  };

  struct SecondState : Will<ByDefault<DoNothing>> {
    explicit SecondState(std::shared_ptr<ExampleMockState> mock)
        : mock_{std::move(mock)} {
    }
    // We only implement the OnEnter method, but that's ok.
    // The action will only call whatever is implemented.
    auto OnEnter(const TransitionEvent &event) -> Status {
      return mock_->OnEnter(event);
    }

  private:
    const std::shared_ptr<ExampleMockState> mock_;
  };

  std::shared_ptr<ExampleMockState> mock_initial_state =
      std::make_shared<ExampleMockState>();
  std::shared_ptr<ExampleMockState> mock_another_state =
      std::make_shared<ExampleMockState>();
  Machine machine{
      FirstState{mock_initial_state}, SecondState{mock_another_state}};
};

// NOLINTNEXTLINE
TEST_F(StateMachineTransitionToErrors, OnLeaveReturnsTerminate) {
  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(Return(Terminate{}));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event))).Times(0);
  auto status = machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsFalse());
  EXPECT_THAT(std::holds_alternative<Terminate>(status), IsTrue());
}

// NOLINTNEXTLINE
TEST_F(StateMachineTransitionToErrors, OnLeaveReturnsTerminateWithError) {
  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(Return(TerminateWithError{"error"}));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event))).Times(0);
  auto status = machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsFalse());
  EXPECT_THAT(std::holds_alternative<TerminateWithError>(status), IsTrue());
  EXPECT_THAT(std::get<TerminateWithError>(status).error_message, Eq("error"));
}

// NOLINTNEXTLINE
TEST_F(StateMachineTransitionToErrors, OnLeaveThrowsStateMachineError) {
  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(testing::Throw(StateMachineError("error")));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event))).Times(0);
  auto status = machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsFalse());
  EXPECT_THAT(std::holds_alternative<TerminateWithError>(status), IsTrue());
  EXPECT_THAT(std::get<TerminateWithError>(status).error_message, Eq("error"));
}

// NOLINTNEXTLINE
TEST_F(StateMachineTransitionToErrors, OnLeaveThrowsOtherError) {
  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(testing::Throw(std::runtime_error("error")));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event))).Times(0);
  auto status = machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsFalse());
  EXPECT_THAT(std::holds_alternative<TerminateWithError>(status), IsTrue());
  EXPECT_THAT(
      std::get<TerminateWithError>(status).error_message, Eq("another error"));
}

// NOLINTNEXTLINE
TEST_F(StateMachineTransitionToErrors, OnEnterReturnsTerminate) {
  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(Return(Continue{}));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event)))
      .Times(1)
      .WillOnce(Return(Terminate{}));
  auto status = machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsTrue());
  EXPECT_THAT(std::holds_alternative<Terminate>(status), IsTrue());
}

// NOLINTNEXTLINE
TEST_F(StateMachineTransitionToErrors, OnEnterReturnsTerminateWithError) {
  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(Return(Continue{}));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event)))
      .Times(1)
      .WillOnce(Return(TerminateWithError{"error"}));
  auto status = machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsTrue());
  EXPECT_THAT(std::holds_alternative<TerminateWithError>(status), IsTrue());
  EXPECT_THAT(std::get<TerminateWithError>(status).error_message, Eq("error"));
}

// NOLINTNEXTLINE
TEST_F(StateMachineTransitionToErrors, OnEnterReturnsReissueEvent) {
  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(Return(Continue{}));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event)))
      .Times(1)
      .WillOnce(Return(ReissueEvent{}));
  auto status = machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsTrue());
  EXPECT_THAT(std::holds_alternative<ReissueEvent>(status), IsTrue());
}

// NOLINTNEXTLINE
TEST_F(StateMachineTransitionToErrors, OnEnterThrowsStateMachineError) {
  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(Return(Continue{}));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event)))
      .Times(1)
      .WillOnce(::testing::Throw(StateMachineError("error")));
  auto status = machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsTrue());
  EXPECT_THAT(std::holds_alternative<TerminateWithError>(status), IsTrue());
  EXPECT_THAT(std::get<TerminateWithError>(status).error_message, Eq("error"));
}

// NOLINTNEXTLINE
TEST_F(StateMachineTransitionToErrors, OnEnterThrowsOtherError) {
  TransitionEvent event{};

  EXPECT_CALL(*mock_initial_state, OnLeave(Ref(event)))
      .Times(1)
      .WillOnce(Return(Continue{}));
  EXPECT_CALL(*mock_another_state, OnEnter(Ref(event)))
      .Times(1)
      .WillOnce(::testing::Throw(std::runtime_error("error")));
  auto status = machine.Handle(event);
  ASSERT_THAT(machine.IsIn<SecondState>(), IsTrue());
  EXPECT_THAT(std::holds_alternative<TerminateWithError>(status), IsTrue());
  EXPECT_THAT(
      std::get<TerminateWithError>(status).error_message, Eq("another error"));
}

// NOLINTNEXTLINE
TEST(StateMachine, TransitionToIsATest) {
  struct State {};
  constexpr int data = 1;
  auto action = TransitionTo<State>{data};
  ASSERT_THAT(action.IsA<TransitionTo<State>>(), IsTrue());
  ASSERT_THAT(action.IsA<DoNothing>(), IsFalse());

  auto specific_ok = action.GetAs<TransitionTo<State>>();
  EXPECT_THAT(std::any_cast<int>(specific_ok.data()), Eq(data));
  const auto &const_specific_ok =
      // NOLINTNEXTLINE
      const_cast<const TransitionTo<State> &>(action)
          .GetAs<TransitionTo<State>>();
  EXPECT_THAT(std::any_cast<int>(const_specific_ok.data()), Eq(data));
}

// NOLINTNEXTLINE
TEST(StateMachine, DoNothingIsATest) {
  struct State {};
  auto action = DoNothing{};
  EXPECT_THAT(action.IsA<DoNothing>(), IsTrue());

  [[maybe_unused]] auto specific_ok = action.GetAs<DoNothing>();
  [[maybe_unused]] const auto &const_specific_ok =
      // NOLINTNEXTLINE
      const_cast<const DoNothing &>(action).GetAs<DoNothing>();
}

// NOLINTNEXTLINE
TEST(StateMachine, ReportErrorIsATest) {
  struct State {};
  auto error = StateMachineError{};
  auto action = ReportError{error};
  EXPECT_THAT(action.IsA<ReportError>(), IsTrue());

  [[maybe_unused]] auto specific_ok = action.GetAs<ReportError>();
  [[maybe_unused]] const auto &const_specific_ok =
      // NOLINTNEXTLINE
      const_cast<const ReportError &>(action).GetAs<ReportError>();
  auto stored_error =
      std::any_cast<StateMachineError>(const_specific_ok.data());
  EXPECT_THAT(stored_error.What(), Eq(error.What()));
}

// NOLINTNEXTLINE
TEST(StateMachine, OneOfIsATest) {
  struct State {};
  auto action = OneOf<DoNothing, TransitionTo<State>>{DoNothing{}};

  // compile-time type check
  if constexpr (!is_one_of_v<OneOf<DoNothing, TransitionTo<State>>>) {
    FAIL();
  }

  EXPECT_THAT(action.IsA<DoNothing>(), IsTrue());
  EXPECT_THAT(action.IsA<TransitionTo<State>>(), IsFalse());

  [[maybe_unused]] auto specific_ok = action.GetAs<DoNothing>();
  [[maybe_unused]] const auto &const_specific_ok =
      // NOLINTNEXTLINE
      const_cast<const OneOf<DoNothing, TransitionTo<State>> &>(action)
          .GetAs<DoNothing>();
  // NOLINTNEXTLINE
  EXPECT_THROW(action.GetAs<TransitionTo<State>>(), std::bad_variant_access);
}

struct State {};
struct OtherState {};

template <typename Action> struct DynamicActionTestValue {
  Action action;
  bool is_transition_to_state;
};

// NOLINTNEXTLINE
template <typename Action> class DynamicActionTest : public ::testing::Test {
protected:
  // NOLINTNEXTLINE
  static std::vector<DynamicActionTestValue<Action>> test_actions;
};
// NOLINTNEXTLINE
TYPED_TEST_SUITE_P(DynamicActionTest);

// NOLINTNEXTLINE
TYPED_TEST_P(DynamicActionTest, CheckAction) {
  for (const auto &value : DynamicActionTest<TypeParam>::test_actions) {
    // Test the action
    auto check = value.action.template IsA<TransitionTo<State>>();
    EXPECT_THAT(check, Eq(value.is_transition_to_state));
  }
}

REGISTER_TYPED_TEST_SUITE_P(DynamicActionTest, CheckAction);

using OneOf_TT_State = OneOf<TransitionTo<State>, DoNothing>;
using OneOf_TT_OtherState = OneOf<TransitionTo<OtherState>, DoNothing>;
using ActionTypes = ::testing::Types<
    // clang-format off
    OneOf_TT_State,
    OneOf_TT_OtherState,
    TransitionTo<State>,
    DoNothing
>; // clang-format on

// NOLINTNEXTLINE
INSTANTIATE_TYPED_TEST_SUITE_P(My, DynamicActionTest, ActionTypes, );
// Silence the warning zero-variadic-macro-arguments by adding a comma and an
// empty argument for the macros
// https://github.com/google/googletest/issues/2271#issuecomment-665742471
TYPED_TEST_SUITE(DynamicActionTest, ActionTypes, );
} // namespace

template <>
std::vector<DynamicActionTestValue<OneOf_TT_State>>
    // NOLINTNEXTLINE
    DynamicActionTest<OneOf_TT_State>::test_actions{
        {TransitionTo<State>{}, true}, {DoNothing{}, false}};

template <>
std::vector<DynamicActionTestValue<OneOf_TT_OtherState>>
    // NOLINTNEXTLINE
    DynamicActionTest<OneOf_TT_OtherState>::test_actions{
        {TransitionTo<OtherState>{}, false}, {DoNothing{}, false}};

template <>
std::vector<DynamicActionTestValue<TransitionTo<State>>>
    // NOLINTNEXTLINE
    DynamicActionTest<TransitionTo<State>>::test_actions{
        {TransitionTo<State>{}, true}};

template <>
std::vector<DynamicActionTestValue<DoNothing>>
    // NOLINTNEXTLINE
    DynamicActionTest<DoNothing>::test_actions{{DoNothing{}, false}};

} // namespace asap::fsm

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
