//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#if defined(__clang__)
#pragma clang diagnostic push
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

#include <common/flag_ops.h>

#include <catch2/catch.hpp>
#include <cstdint>

namespace asap {

TEST_CASE("Flag / Set", "[common][flag]") {
  std::uint64_t mask = 0x100010;
  std::uint64_t flag = 0x1000;

  FlagSet(mask, flag);
  // bit corresponding to flag is set
  REQUIRE((mask & flag) != 0);
  // other bits not touched
  REQUIRE(mask == 0x101010);

  // multi-bit flag
  mask = 0x100010;
  flag = 0x1001;
  FlagSet(mask, flag);
  // bit corresponding to flag is set
  REQUIRE(mask == 0x101011);
}

TEST_CASE("Flag / Clear", "[common][flag]") {
  std::uint64_t mask = 0x100010;
  std::uint64_t flag = 0x10;

  FlagClear(mask, flag);
  // bit corresponding to flag is cleared
  REQUIRE((mask & flag) == 0);
  // other bits not touched
  REQUIRE(mask == 0x100000);

  // multi-bit flag
  mask = 0x10101010;
  flag = 0x101000;
  FlagClear(mask, flag);
  REQUIRE((mask & flag) == 0);
  REQUIRE(mask == 0x10000010);
}

TEST_CASE("Flag / Flip", "[common][flag]") {
  std::uint64_t mask = 0x100010;
  std::uint64_t flag = 0x10;

  FlagFlip(mask, flag);
  // bit corresponding to flag is cleared
  REQUIRE((mask & flag) == 0);
  // other bits not touched
  REQUIRE(mask == 0x100000);
  FlagFlip(mask, flag);
  // bit corresponding to flag is cleared
  REQUIRE((mask & flag) != 0);
  // other bits not touched
  REQUIRE(mask == 0x100010);

  // multi-bit flag
  mask = 0x10101010;
  flag = 0x11111111;
  FlagFlip(mask, flag);
  REQUIRE(mask == 0x01010101);
  FlagFlip(mask, flag);
  REQUIRE(mask == 0x10101010);
}

TEST_CASE("Flag / Test", "[common][flag]") {
  std::uint64_t mask = 0x100010;
  std::uint64_t flag = 0x10;

  REQUIRE(FlagTest(mask, flag));
  REQUIRE(FlagTest(mask, mask));
}

} // namespace asap

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // __clang__
