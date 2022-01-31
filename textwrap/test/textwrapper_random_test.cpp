//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "textwrap/textwrap.h"

#include <common/compilers.h>
#include <contract/contract.h>
#include <logging/logging.h>

#include "gmock/gmock.h"
#include <gtest/gtest.h>

#include <array>
#include <random>

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

using ::testing::IsTrue;

namespace asap::wrap {

namespace {

std::mt19937 rng; // NOLINT
using rng_type = std::uniform_int_distribution<std::mt19937::result_type>;
rng_type word_char_generator(33, 126); // NOLINT
rng_type ws_generator(0, 11);          // NOLINT
rng_type word_size_generator(1, 10);   // NOLINT
rng_type ws_size_generator(1, 2);      // NOLINT
rng_type punct_generator(0, 40);       // NOLINT

auto RandomWordChar() -> char {
  return static_cast<char>(word_char_generator(rng));
}
auto RandomWhiteSpace() -> char {
  static const auto white_space = std::array<const char, 14>{' ', ' ', ' ', ' ',
      ' ', ' ', ' ', ' ', ' ', '\t', '\v', '\f', '\r', '\n'};
  return (white_space.at(ws_generator(rng)));
}
auto GenerateWord(size_t length) -> std::string {
  std::string result;
  for (; length > 0; --length) {
    result += RandomWordChar();
  }
  return result;
}
auto GenerateWord() -> std::string {
  return GenerateWord(word_size_generator(rng));
}
auto GenerateWhiteSpace(size_t length) -> std::string {
  std::string result;
  for (; length > 0; --length) {
    result += RandomWhiteSpace();
  }
  return result;
}
auto GenerateText(size_t words) -> std::string {
  std::string text;
  for (; words > 0; --words) {
    text += GenerateWord();
    switch (punct_generator(rng)) {
    case 1:
    case 2:
      text += ",";
      break;
    case 3:
    case 4:
      text += ".";
      break;
    }
    text += GenerateWhiteSpace(ws_size_generator(rng));
  }
  return text;
}

template <class DT = std::chrono::milliseconds,
    class ClockT = std::chrono::steady_clock>
class Timer {
  using timep_t = typename ClockT::time_point;
  timep_t _start = ClockT::now(), _end = {};

public:
  void tick() {
    _end = timep_t{};
    _start = ClockT::now();
  }

  void tock() {
    _end = ClockT::now();
  }

  template <class T = DT> [[nodiscard]] auto duration() const {
    ASAP_EXPECT(_end != timep_t{} && "toc before reporting");
    return std::chrono::duration_cast<T>(_end - _start);
  }
};

// NOLINTNEXTLINE
TEST(TextWrapper, RandomTests) {
  asap::logging::Registry::instance().SetLogLevel(
      asap::logging::Logger::Level::off);

  constexpr size_t maximum_text_length = 80;

  Timer<std::chrono::milliseconds, std::chrono::steady_clock> clock;

  clock.tick();
  for (size_t i = 1; i < maximum_text_length; ++i) {
    auto text = GenerateText(i);
    auto size = text.size();
    for (size_t column_width = 3; column_width < size; ++column_width) {
      TextWrapper text_wrapper = TextWrapper::Create()
                                     .Width(column_width)
                                     .TrimLines()
                                     .CollapseWhiteSpace()
                                     .BreakOnHyphens();
      EXPECT_THAT(text_wrapper.Fill(text).has_value(), IsTrue());
    }
  }
  clock.tock();

  std::cout << "Run time = " << clock.duration().count() << " ms\n";
}

} // namespace

} // namespace asap::wrap

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
