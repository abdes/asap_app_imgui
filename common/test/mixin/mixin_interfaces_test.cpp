//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License.See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "common/mixin.h"

#include <common/compilers.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Disable compiler and linter warnings originating from the unit test framework
// and for which we cannot do anything. Additionally, every TEST or TEST_X macro
// usage must be preceded by a '// NOLINTNEXTLINE'.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif
// NOLINTBEGIN(used-but-marked-unused)

using testing::NotNull;

namespace asap::mixin {

namespace {
// NOLINTBEGIN(cppcoreguidelines-special-member-functions,
// hicpp-special-member-functions, cppcoreguidelines-virtual-class-destructor)
//! [Virtual interface]
struct Interface {
  virtual ~Interface() = default;
  virtual void foo() = 0;
};
//! [Virtual interface]

//! [Mixin implements interface]
template <typename Base> struct ImplementsInterface : Base, Interface {
  template <typename... Args>
  constexpr ImplementsInterface(Args &&...args)
      : Base(static_cast<decltype(args)>(args)...) {
  }

  void foo() override {
    // Implement foo interface in the mixin
  }
};
//! [Mixin implements interface]

// NOLINTEND(cppcoreguidelines-special-member-functions,
// hicpp-special-member-functions, cppcoreguidelines-virtual-class-destructor)

// NOLINTNEXTLINE
TEST(MixinInterfaces, MixinCanImplementInterface) {
  struct Composite : Mixin<Composite, ImplementsInterface> {};
  // This code compiles and defines the `foo()` method
  Composite composite{};
  EXPECT_THAT(dynamic_cast<Interface *>(&composite), NotNull());
  composite.foo();
}

// NOLINTNEXTLINE
TEST(MixinInterfaces, MixinCanProvideInterface) {
  //! [Mixin provides interface]
  using asap::mixins::Provides;
  struct Composite final : Mixin<Composite, Provides<Interface>::mixin> {
    void foo() override {
      // Implement foo interface in the composite
    }
  };
  //! [Mixin provides interface]
  // This code compiles and defines the `foo()` method
  Composite composite{};
  EXPECT_THAT(dynamic_cast<Interface *>(&composite), NotNull());
  composite.foo();
}

} // namespace

} // namespace asap::mixin

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
