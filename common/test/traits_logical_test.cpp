/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <common/traits/logical.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <type_traits>

#include <common/compilers.h>

// Disable compiler and linter warnings originating from the unit test framework and for which we
// cannot do anything.
// Additionally every TEST or TEST_X macro usage must be preceded by a '// NOLINTNEXTLINE'.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__) && ASAP_HAS_WARNING("-Wused-but-marked-unused")
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif
// NOLINTBEGIN(used-but-marked-unused)

using ::testing::IsFalse;
using ::testing::IsTrue;

// -------------------------------------------------------------------------------------------------
// Check we can use the type aliases
// -------------------------------------------------------------------------------------------------

ASAP_DIAGNOSTIC_PUSH
#if ASAP_GNUC_VERSION
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif

namespace asap {

namespace {

// NOLINTNEXTLINE
TEST(TraitsLogical, Conjunctiontypedef) {
  using test_type = conjunction<std::true_type, std::true_type>;
  using value_type = test_type::value_type;
  using type = test_type::type;
  using type_value_type = test_type::type::value_type;
  using type_type = test_type::type::type;
}

// NOLINTNEXTLINE
TEST(TraitsLogical, Disjunctiontypedef) {
  using test_type = disjunction<std::false_type, std::true_type>;
  using value_type = test_type::value_type;
  using type = test_type::type;
  using type_value_type = test_type::type::value_type;
  using type_type = test_type::type::type;
}

// NOLINTNEXTLINE
TEST(TraitsLogical, Negationtypedef) {
  using test_type = negation<std::false_type>;
  using value_type = test_type::value_type;
  using type = test_type::type;
  using type_value_type = test_type::type::value_type;
  using type_type = test_type::type::type;
}

ASAP_DIAGNOSTIC_POP

// -------------------------------------------------------------------------------------------------
// Check that the logic in the implementation is correct
// -------------------------------------------------------------------------------------------------

// NOLINTNEXTLINE
TEST(TraitsLogical, NegationOfTrueIsFalse) {
  EXPECT_THAT(negation<std::true_type>{}, IsFalse());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, NegationOfFalseIsTrue) {
  EXPECT_THAT(negation<std::false_type>{}, IsTrue());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, ConjunctionDefaultIsTrue) {
  EXPECT_THAT(conjunction<>{}, IsTrue());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, DisjunctionDefaultIsFalse) {
  EXPECT_THAT(disjunction<>{}, IsFalse());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, ConjunctionOfOneTrueIsTrue) {
  EXPECT_THAT(conjunction<std::true_type>{}, IsTrue());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, ConjunctionOfOneFalseIsFalse) {
  EXPECT_THAT(conjunction<std::false_type>{}, IsFalse());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, DisjunctionOfOneTrueIsTrue) {
  EXPECT_THAT(disjunction<std::true_type>{}, IsTrue());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, DisjunctionOfOneFalseIsFalse) {
  EXPECT_THAT(disjunction<std::false_type>{}, IsFalse());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, ConjunctionOfAllFalseIsFalse) {
  auto value = conjunction<std::false_type, std::false_type, std::false_type>{};
  EXPECT_THAT(value, IsFalse());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, ConjunctionOfAllTrueIsTrue) {
  auto value = conjunction<std::true_type, std::true_type, std::true_type>{};
  EXPECT_THAT(value, IsTrue());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, ConjunctionOfMixedValuesIsFalse) {
  auto value1 = conjunction<std::true_type, std::false_type, std::true_type>{};
  EXPECT_THAT(value1, IsFalse());
  auto value2 = conjunction<std::true_type, std::true_type, std::false_type>{};
  EXPECT_THAT(value2, IsFalse());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, DisjunctionOfAllFalseIsFalse) {
  auto value = disjunction<std::false_type, std::false_type, std::false_type>{};
  EXPECT_THAT(value, IsFalse());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, DisjunctionOfAllTrueIsTrue) {
  auto value = disjunction<std::true_type, std::true_type, std::true_type>{};
  EXPECT_THAT(value, IsTrue());
}

// NOLINTNEXTLINE
TEST(TraitsLogical, DisjunctionOfMixedValuesIsTrue) {
  auto value1 = disjunction<std::true_type, std::false_type, std::true_type>{};
  EXPECT_THAT(value1, IsTrue());
  auto value2 = disjunction<std::false_type, std::false_type, std::true_type>{};
  EXPECT_THAT(value2, IsTrue());
}

} // namespace

} // namespace asap

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
