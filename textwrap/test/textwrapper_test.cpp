//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "textwrap/textwrap.h"

#include <common/compilers.h>
#include <logging/logging.h>

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

using ::testing::Eq;

namespace asap::wrap {

namespace {

class TextWrapperTest : public ::testing::Test {
public:
  static void SetUpTestSuite() {
    asap::logging::Registry::instance().SetLogLevel(
        asap::logging::Logger::Level::off);
  }
};

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, ShortString) {
  constexpr size_t column_width = 30;
  const auto *text = "short string";
  TextWrapper wrapper = TextWrapper::Create().Width(column_width);
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("short string"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, ExactString) {
  const auto *text = "short string";
  const size_t column_width = std::strlen(text);
  TextWrapper wrapper = TextWrapper::Create().Width(column_width);
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("short string"));
}

using LongStringTestParamType = std::tuple<
    // width
    size_t,
    // replace white space
    bool,
    // Initial indent
    std::string,
    // Subsequent indents
    std::string,
    // expected result
    std::string>;

class LongStringTest
    : public ::testing::TestWithParam<LongStringTestParamType> {
public:
  inline static constexpr const char *text =
      "The quick brown fox jumped over the lazy dog";

  static void SetUpTestSuite() {
    asap::logging::Registry::instance().SetLogLevel(
        asap::logging::Logger::Level::off);
  }
};

// NOLINTNEXTLINE
TEST_P(LongStringTest, Fill) {
  const auto &[width, trim_lines, initial_indent, subsequent_indents,
      expected] = GetParam();
  auto builder = TextWrapper::Create();
  builder.Width(width);
  if (trim_lines) {
    builder.TrimLines();
  }
  builder.IndentWith().Initially(initial_indent).Then(subsequent_indents);
  TextWrapper wrapper = builder;
  EXPECT_THAT(wrapper.Fill(text).value(), Eq(expected));
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(TrimLines, LongStringTest,
    // clang-format off
    testing::Values(
      LongStringTestParamType{7, true, "", "",
        "The\n"
        "quick\n"
        "brown\n"
        "fox\n"
        "jumped\n"
        "over\n"
        "the\n"
        "lazy\n"
        "dog"
      },
      LongStringTestParamType{8, true, "", "",
        "The\n"
        "quick\n"
        "brown\n"
        "fox\n"
        "jumped\n"
        "over the\n"
        "lazy dog"
      },
      LongStringTestParamType{9, true, "", "",
        "The quick\n"
        "brown fox\n"
        "jumped\n"
        "over the\n"
        "lazy dog"
      },
      LongStringTestParamType{10, true, "", "",
        "The quick\n"
        "brown fox\n"
        "jumped\n"
        "over the\n"
        "lazy dog"
      },
      LongStringTestParamType{11, true, "", "",
        "The quick\n"
        "brown fox\n"
        "jumped over\n"
        "the lazy\n"
        "dog"
      },
      LongStringTestParamType{12, true, "", "",
        "The quick\n"
        "brown fox\n"
        "jumped over\n"
        "the lazy dog"
      },
      LongStringTestParamType{20, true, "", "",
        "The quick brown fox\n"
        "jumped over the lazy\n"
        "dog"
      }
    ) //  clang-format on
);

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(NoTrimLines, LongStringTest,
    // clang-format off
    testing::Values(
      LongStringTestParamType{7, false, "", "",
        "The \n"
        "quick\n"
        " brown\n"
        " fox \n"
        "jumped\n"
        " over\n"
        " the \n"
        "lazy \n"
        "dog"
      },
      LongStringTestParamType{10, false, "", "",
        "The quick\n"
        " brown fox\n"
        " jumped \n"
        "over the \n"
        "lazy dog"
      },
      LongStringTestParamType{20, false, "", "",
        "The quick brown fox \n"
        "jumped over the lazy\n"
        " dog"
      }
    ) // clang-format on
);

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, EmptyString) {
  constexpr size_t column_width = 30;
  const auto *text = "";
  TextWrapper wrapper = TextWrapper::Create().Width(column_width);
  EXPECT_THAT(wrapper.Fill(text).value(), Eq(""));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, Space) {
  constexpr size_t column_width = 30;
  const auto *text = " ";
  TextWrapper wrapper = TextWrapper::Create().Width(column_width);
  EXPECT_THAT(wrapper.Fill(text).value(), Eq(" "));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, EmptyLine) {
  constexpr size_t column_width = 30;
  const auto *text = "\n";
  TextWrapper wrapper = TextWrapper::Create().Width(column_width);
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("\n"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, OneShortWord) {
  constexpr size_t column_width = 30;
  const auto *text = "hello";
  TextWrapper wrapper = TextWrapper::Create().Width(column_width);
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("hello"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, OneLongWord) {
  constexpr size_t column_width = 5;
  const auto *text = "unequivocally";
  TextWrapper wrapper = TextWrapper::Create().Width(column_width);
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("unequivocally"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, IndentInitialOnly) {
  constexpr size_t column_width = 6;
  const auto *text = "hello world!";
  TextWrapper wrapper = TextWrapper::Create()
                            .Width(column_width)
                            .TrimLines()
                            .IndentWith()
                            .Initially("--- ");
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("--- hello\nworld!"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, IndentNoInitial) {
  constexpr size_t column_width = 6;
  const auto *text = "hello world!";
  TextWrapper wrapper = TextWrapper::Create()
                            .Width(column_width)
                            .TrimLines()
                            .IndentWith()
                            .Initially("")
                            .Then("--- ");
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("hello\n--- world!"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, IndentAll) {
  constexpr size_t column_width = 6;
  const auto *text = "hello world!";
  TextWrapper wrapper = TextWrapper::Create()
                            .Width(column_width)
                            .TrimLines()
                            .IndentWith()
                            .Initially("== ")
                            .Then("---- ");
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("== hello\n---- world!"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, IndentOneWord) {
  constexpr size_t column_width = 3;
  const auto *text = "hello";
  TextWrapper wrapper = TextWrapper::Create()
                            .Width(column_width)
                            .TrimLines()
                            .IndentWith()
                            .Initially("== ")
                            .Then("---- ");
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("== hello"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, IndentExactWidth) {
  constexpr size_t column_width = 17;
  const auto *text = "hello world!";
  TextWrapper wrapper = TextWrapper::Create()
                            .Width(column_width)
                            .TrimLines()
                            .IndentWith()
                            .Initially("==== ")
                            .Then("---- ");
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("==== hello world!"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, IndentMultipleLines) {
  constexpr size_t column_width = 10;
  const auto *text = "bye world - welcome universe!";
  TextWrapper wrapper = TextWrapper::Create()
                            .Width(column_width)
                            .TrimLines()
                            .IndentWith()
                            .Initially("==== ")
                            .Then("---- ");
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("==== bye\n"
                                             "---- world\n"
                                             "---- -\n"
                                             "---- welcome\n"
                                             "---- universe!"));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, IndentEmptyStringNotIndented) {
  constexpr size_t column_width = 17;
  const auto *text = "";
  TextWrapper wrapper = TextWrapper::Create()
                            .Width(column_width)
                            .TrimLines()
                            .IndentWith()
                            .Initially("==== ")
                            .Then("---- ");
  EXPECT_THAT(wrapper.Fill(text).value(), Eq(""));
}

// NOLINTNEXTLINE
TEST_F(TextWrapperTest, IndentEmptyLineNotIndented) {
  constexpr size_t column_width = 17;
  const auto *text = "hello\n\n\nworld!";
  TextWrapper wrapper = TextWrapper::Create()
                            .Width(column_width)
                            .IndentWith()
                            .Initially("==== ")
                            .Then("---- ");
  EXPECT_THAT(wrapper.Fill(text).value(), Eq("==== hello\n\n==== world!"));
}

} // namespace

} // namespace asap::wrap

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
