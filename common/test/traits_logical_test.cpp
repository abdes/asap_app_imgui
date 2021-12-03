//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <common/traits/logical.h>

#include <common/compilers.h>

ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__)
// Catch2 uses a lot of macro names that will make clang go crazy
#if (__clang_major__ >= 13) && !defined(__APPLE__)
#pragma clang diagnostic ignored "-Wreserved-identifier"
#endif
// Big mess created because of the way spdlog is organizing its source code
// based on header only builds vs library builds. The issue is that spdlog
// places the template definitions in a separate file and explicitly
// instantiates them, so we have no problem at link, but we do have a problem
// with clang (rightfully) complaining that the template definitions are not
// available when the template needs to be instantiated here.
#pragma clang diagnostic ignored "-Wundefined-func-template"
#endif // __clang__

#include <catch2/catch.hpp>


ASAP_DIAGNOSTIC_PUSH
#if ASAP_GNUC_VERSION
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif

namespace asap {

namespace {

TEST_CASE("Conjunction_typedef", "[common][traits][logical]") {
  // Check for required typedefs
  using test_type = conjunction<std::true_type, std::true_type>;
  using value_type = test_type::value_type;
  using type = test_type::type;
  using type_value_type = test_type::type::value_type;
  using type_type = test_type::type::type;
}

TEST_CASE("Disjunction_typedef", "[common][traits][logical]") {
  // Check for required typedefs
  using test_type = disjunction<std::false_type, std::true_type>;
  using value_type = test_type::value_type;
  using type = test_type::type;
  using type_value_type = test_type::type::value_type;
  using type_type = test_type::type::type;
}

TEST_CASE("Negation_typedef", "[common][traits][logical]") {
  // Check for required typedefs
  using test_type = negation<std::false_type>;
  using value_type = test_type::value_type;
  using type = test_type::type;
  using type_value_type = test_type::type::value_type;
  using type_type = test_type::type::type;
}

ASAP_DIAGNOSTIC_POP

TEST_CASE("Value", "[common][traits][logical]") {
  REQUIRE(negation<std::false_type>{});
  REQUIRE(!negation<std::true_type>{});
  REQUIRE(conjunction<>{});
  REQUIRE(!disjunction<>{});
  REQUIRE(conjunction<std::true_type>{});
  REQUIRE(!conjunction<std::false_type>{});
  REQUIRE(disjunction<std::true_type>{});
  REQUIRE(!disjunction<std::false_type>{});
  REQUIRE(conjunction<std::true_type, std::true_type>{});
  REQUIRE(!conjunction<std::true_type, std::false_type>{});
  REQUIRE(disjunction<std::false_type, std::true_type>{});
  REQUIRE(!disjunction<std::false_type, std::false_type>{});
  REQUIRE(conjunction<std::true_type, std::true_type, std::true_type>{});
  REQUIRE(!conjunction<std::true_type, std::true_type, std::false_type>{});
  REQUIRE(disjunction<std::false_type, std::false_type, std::true_type>{});
  REQUIRE(!disjunction<std::false_type, std::false_type, std::false_type>{});
}

ASAP_DIAGNOSTIC_POP

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // __clang__

} // namespace

} // namespace asap

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
