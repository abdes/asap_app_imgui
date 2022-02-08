//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

/*!
 * \file
 *
 * \brief Implementation details of the StateMachine and related types.
 */

#include <common/compilers.h>
#include <fsm/fsm.h>

#include <utility>

namespace asap::fsm {

class StateMachineError::Impl {
public:
  explicit Impl() = default;
  explicit Impl(std::string description) : what_{std::move(description)} {
  }

  [[nodiscard]] auto What() const -> const char * {
    return what_ ? what_.value().c_str() : "unspecified state machine error";
  }

private:
  std::optional<std::string> what_;
};

StateMachineError::StateMachineError() : pimpl(new Impl()) {
}
StateMachineError::StateMachineError(std::string description)
    : pimpl(new Impl(std::move(description))) {
}
StateMachineError::StateMachineError(const StateMachineError &) = default;
StateMachineError::StateMachineError(StateMachineError &&) noexcept = default;
auto StateMachineError::operator=(const StateMachineError &)
    -> StateMachineError & = default;
auto StateMachineError::operator=(StateMachineError &&) noexcept
    -> StateMachineError & = default;
StateMachineError::~StateMachineError() noexcept {
  delete pimpl;
  pimpl = nullptr;
}

auto StateMachineError::What() const -> const char * {
  return pimpl->What();
}

auto DoNothing::data() noexcept -> const std::any & {
  ASAP_DIAGNOSTIC_PUSH
#if defined(ASAP_CLANG_VERSION)
  ASAP_PRAGMA(clang diagnostic ignored "-Wexit-time-destructors")
#endif
  // Nothing to worry about here for the exit time destructor.
  static const std::any data_{};
  ASAP_DIAGNOSTIC_POP
  return data_;
}

} // namespace asap::fsm
