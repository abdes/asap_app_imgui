//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

/*!
 * \file
 *
 * \brief Implementation details for the contract checking API.
 */

#include "contract/contract.h"

#include <common/compilers.h>

#include <cassert>
#include <csetjmp>
#include <iostream>

namespace asap::contract {

// -----------------------------------------------------------------------------
// Unit Testing stuff
// -----------------------------------------------------------------------------

namespace {

void PrintViolation(const Violation *violation) {
  std::cerr << violation->file << ":" << violation->line << ": in "
            << violation->function << ": " << violation->type << " '"
            << violation->condition << "' violated" << std::endl;
}

[[noreturn]] void DefaultViolationHandler(const Violation *violation) {
  PrintViolation(violation);
  abort();
}

} // namespace

// Internal implementation of the singleton violation handler.
class ViolationHandler_impl : public ViolationHandler {
public:
  static inline auto instance() -> ViolationHandler & {
    ASAP_DIAGNOSTIC_PUSH
#if defined(ASAP_CLANG_VERSION)
    ASAP_PRAGMA(clang diagnostic ignored "-Wexit-time-destructors")
#endif
    static ViolationHandler_impl instance_;
    ASAP_DIAGNOSTIC_POP
    return instance_;
  }

  void HandleViolation(const Violation *violation) override;
  void SwapHandler(WrapperType &other_handler) override;

private:
  ViolationHandler_impl() = default;

  WrapperType handler = WrapperType{DefaultViolationHandler};
};

void ViolationHandler_impl::HandleViolation(const Violation *violation) {
  assert(violation != nullptr); // NOLINT
  handler(violation);
}

void ViolationHandler_impl::SwapHandler(WrapperType &other_handler) {
  other_handler.swap(handler);
}

auto GetViolationHandler() -> ViolationHandler & {
  return ViolationHandler_impl::instance();
}

ViolationHandler::~ViolationHandler() = default;

} // namespace asap::contract
