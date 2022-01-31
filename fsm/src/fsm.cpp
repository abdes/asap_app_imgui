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

auto asap::fsm::StateMachineError::What() const -> const char * {
  return what_ ? what_.value().c_str() : "unspecified state machine error";
}

auto asap::fsm::DoNothing::data() noexcept -> const std::any & {
  ASAP_DIAGNOSTIC_PUSH
#if defined(ASAP_CLANG_VERSION)
  ASAP_PRAGMA(clang diagnostic ignored "-Wexit-time-destructors")
#endif
  // Nothing to worry about here for the exit time destructor.
  static const std::any data_{};
  ASAP_DIAGNOSTIC_POP
  return data_;
}
