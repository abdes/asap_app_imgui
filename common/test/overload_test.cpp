//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "common/overload.h"

#include <common/compilers.h>

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <variant>

// Disable compiler and linter warnings originating from the unit test framework
// and for which we cannot do anything. Additionally every TEST or TEST_X macro
// usage must be preceded by a '// NOLINTNEXTLINE'.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__) && ASAP_HAS_WARNING("-Wused-but-marked-unused")
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif
// NOLINTBEGIN(used-but-marked-unused)

using ::testing::Eq;
using ::testing::IsTrue;
using ::testing::Ne;

namespace asap {

namespace {

// NOLINTNEXTLINE
TEST(Overload, ExampleWithDefault) {
  //! [Example Overload with default]
  const char char_value = 5;
  const int int_value = 2017;
  const unsigned int uint_value = 2017;
  const float float_value = 3.5F;
  const double double_value = 10.4;

  std::vector<std::variant<char, float, int, unsigned int, double>> variants = {
      char_value, int_value, uint_value, float_value, double_value};

  auto TypeOfIntegral = Overload{
      [&](char value) { EXPECT_THAT(value, Eq(char_value)); },
      [&](float value) { EXPECT_THAT(value, Eq(float_value)); },
      [&](double value) { EXPECT_THAT(value, Eq(double_value)); },
      [&](auto value) { EXPECT_THAT(static_cast<int>(value), Eq(int_value)); },
  };

  for (auto value : variants) {
    std::visit(TypeOfIntegral, value);
  }
  //! [Example Overload with default]
}

} // namespace

} // namespace asap

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
