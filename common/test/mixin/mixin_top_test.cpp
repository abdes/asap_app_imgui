//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License.See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "common/mixin.h"

#include <common/compilers.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>

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

namespace asap::mixin {

namespace {

//! [Mixin calls other mixin]
template <typename Base> struct WithLogging : Base {
  void Log(const std::string &message) const {
    out << message;
  }
  auto LoggerOutput() -> std::string {
    return out.str();
  }

private:
  mutable std::stringstream out;
};

template <typename Base> struct Persistence : Base {
  void Store() const {
    // self() is provided by Base, so we need to mark it dependent.
    // this->self() is short and obvious.
    this->self().Log("storing...");
  }
};
//! [Mixin calls other mixin]

// NOLINTNEXTLINE
TEST(MixinSelf, UseSelfToAccessOtherMixinInterfaces) {
  struct Concrete : asap::mixin::Mixin<Concrete, Persistence, WithLogging> {};

  Concrete concrete;
  concrete.Store();
  EXPECT_THAT(concrete.LoggerOutput(), Eq("storing..."));
}

// NOLINTNEXTLINE
TEST(MixinSelf, CompositionOrderDoesNotMatter) {
  //! [Composition order does not matter]
  struct Concrete1 : Mixin<Concrete1, Persistence, WithLogging> {};
  struct Concrete2 : Mixin<Concrete2, WithLogging, Persistence> {};
  //! [Composition order does not matter]

  [[maybe_unused]] Concrete1 concreate_1;
  [[maybe_unused]] Concrete2 concrete_2;
}

} // namespace

} // namespace asap::mixin

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
