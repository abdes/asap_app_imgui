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
struct Argument1 {
  int a1;
};
struct Argument2 {
  int a2;
};

template <typename Base> struct Fragment1 : Base {
  template <typename... Args>
  Fragment1(Argument1 arg1, int foo, Args &&...args)
      : Base(std::forward<Args>(args)...), x_{arg1.a1}, y_{foo} {
  }
  int x_;
  int y_;
};

template <typename Base> struct Fragment2 : Base {
  template <typename... Args>
  explicit Fragment2(Argument2 arg2, Args &&...args)
      : Base(std::forward<Args>(args)...), z_{arg2.a2} {
  }
  int z_;
};

// NOLINTNEXTLINE
TEST(MixinChainedInit, MixinConstructorForwardsUnusedArgumentsToBase) {
  struct Composite : Mixin<Composite, Fragment1, Fragment2> {
    constexpr Composite(Argument1 arg1, int foo, Argument2 arg2)
        : Mixin(arg1, foo, arg2) {
    }
  };

  Composite composite{Argument1{1}, 2, Argument2{3}};
  EXPECT_THAT(composite.x_ + composite.y_ + composite.z_, Eq(1 + 2 + 3));
}

} // namespace

} // namespace asap::mixin

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
