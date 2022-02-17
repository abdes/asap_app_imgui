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
#if defined(__clang__) && ASAP_HAS_WARNING("-Wused-but-marked-unused")
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif
// NOLINTBEGIN(used-but-marked-unused)

namespace asap::mixin {

namespace {

//! [Curry mixin with multiple parameters]
template <typename Arg1, typename Arg2, typename Base> struct MyMixin : Base {
  /// Forwarding constructor
  template <typename... Args>
  constexpr explicit MyMixin(Arg1 field_1, Arg2 field_2, Args &&...args)
      : Base(static_cast<decltype(args)>(args)...), field_1_{field_1},
        field_2_{field_2} {
  }

private:
  Arg1 field_1_;
  Arg2 field_2_;
};

struct MyClass : Mixin<MyClass, mixin::Curry<MyMixin, int, float>::mixin> {
  template <typename... Args>
  constexpr explicit MyClass(Args &&...args)
      : Mixin(static_cast<decltype(args)>(args)...) {
  }
};
//! [Curry mixin with multiple parameters]

// NOLINTNEXTLINE
TEST(MixinCurry, MultipleTemplateParameters) {
  [[maybe_unused]] MyClass composite{0, 0.0F};
}

} // namespace

} // namespace asap::mixin

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
