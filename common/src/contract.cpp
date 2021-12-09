/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

/*!
 * \file contract.cpp
 *
 * \brief Implementation details for the contract checking API.
 */

#include <cassert>
#include <csetjmp>
#include <iostream>

#include "common/singleton.h"
#include "contract/contract.h"
#include "contract/ut/framework.h"

namespace asap::contract {

// ---------------------------------------------------------------------------------------------------------------------
// Unit Testing stuff
// ---------------------------------------------------------------------------------------------------------------------

namespace {

void PrintViolation(const Violation *violation) {
  std::cerr << violation->file << ":" << violation->line << ": in " << violation->function << ": "
            << violation->type << " '" << violation->condition << "' violated" << std::endl;
}

[[noreturn]] void DefaultViolationHandler(const Violation *violation) {
  PrintViolation(violation);
  abort();
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
int contract_check_active = 0;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
Verbosity verbosity_level = Verbosity::QUIET;

[[noreturn]] void TestViolationHandler(const Violation *violation) {
  if (contract_check_active == 0) {
    std::cerr << "Unexpected contract violation:" << std::endl;
    DefaultViolationHandler(violation);
  }

  if (verbosity_level == Verbosity::VERBOSE) {
    PrintViolation(violation);
  }

  // Restore calling environment and jump back to setjmp. Return
  // -1 so that setjmp will return false for conditional test.
  // NOLINTNEXTLINE
  longjmp(details::jmp_env, 1);
}

} // namespace

namespace details {

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
jmp_buf jmp_env;

void ContractCheckPush() {
  contract_check_active++;
}

void ContractCheckPop() {
  contract_check_active--;
}

} // namespace details

ViolationHandler::~ViolationHandler() = default;

void SetVerbosity(Verbosity verbosity) {
  verbosity_level = verbosity;
}

void PrepareForTesting() {
  auto test_handler = ViolationHandler::WrapperType{TestViolationHandler};
  GetViolationHandler().SwapHandler(test_handler);
}

// Internal implementation of the singleton violation handler.
class ViolationHandler_impl : public ViolationHandler, public Singleton<ViolationHandler_impl> {
public:
  explicit ViolationHandler_impl(typename Singleton<ViolationHandler_impl>::token /*unused*/) {
  }

  void HandleViolation(const Violation *violation) override;
  void SwapHandler(WrapperType &other_handler) override;

private:
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

} // namespace asap::contract
