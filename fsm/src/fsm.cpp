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
#include <gsl/gsl>

#include <utility>
#include <stdexcept>

namespace asap::fsm {

class StateMachineError::Impl {
public:
  explicit Impl() = default;
  explicit Impl(std::string description) : what_{std::move(description)} {
  }
  Impl(const Impl &) = default;
  Impl(Impl &&) noexcept = default;
  auto operator=(const Impl &) -> Impl & = default;
  auto operator=(Impl &&) noexcept -> Impl & = default;
  virtual ~Impl() = default;

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

StateMachineError::StateMachineError(const StateMachineError &other)
    : pimpl(new Impl(*other.pimpl)) {
}

StateMachineError::StateMachineError(StateMachineError &&other) noexcept
    : pimpl(other.pimpl) {
  other.pimpl = nullptr;
}

auto StateMachineError::operator=(const StateMachineError &rhs)
    -> StateMachineError & {
  if (&rhs == this) {
    return *this;
  }
  delete pimpl;
  pimpl = nullptr;
  pimpl = gsl::owner<Impl*>(new Impl(*rhs.pimpl));
  return *this;
}

auto StateMachineError::operator=(StateMachineError &&rhs) noexcept
    -> StateMachineError & {
  if (&rhs == this) {
    return *this;
  }
  delete pimpl;
  pimpl = rhs.pimpl;
  rhs.pimpl = nullptr;
  return *this;
}

StateMachineError::~StateMachineError() noexcept {
  delete pimpl;
  pimpl = nullptr;
}

auto StateMachineError::What() const -> const char * {
  if (pimpl != nullptr) {
    return pimpl->What();
  }
  return nullptr;
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
