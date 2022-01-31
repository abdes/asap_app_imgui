//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "tokenizer.h"

#include <common/compilers.h>
#include <logging/logging.h>

#include <gmock/gmock-more-matchers.h>
#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

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
using ::testing::IsTrue;

namespace asap::wrap::detail {

namespace {

class TokenizerTest : public ::testing::Test {
public:
  static void SetUpTestSuite() {
    asap::logging::Registry::instance().SetLogLevel(
        asap::logging::Logger::Level::off);
  }
};

// NOLINTNEXTLINE
TEST_F(TokenizerTest, Example) {
  //! [Tokenizer example]
  constexpr const char *tab = " ";
  constexpr bool replace_ws = true;
  constexpr bool collapse_ws = true;
  constexpr bool break_on_hyphens = true;

  Tokenizer tokenizer{tab, replace_ws, collapse_ws, break_on_hyphens};

  constexpr const char *text = "Why? \nJust plain \tfinger-licking good!";
  std::vector<Token> tokens;
  auto status = tokenizer.Tokenize(
      text, [&tokens](TokenType token_type, std::string token) {
        if (token_type != detail::TokenType::EndOfInput) {
          tokens.emplace_back(token_type, std::move(token));
        }
      });
  // All white spaces replaced and collapsed, hyphenated words
  // broken, to produce the following tokens:
  //     "Why?", " ", "Just", " ", "plain", " ",
  //     "finger-", "licking", " ", "good!"
  //! [Tokenizer example]

  auto expected = std::vector<Token>{Token{TokenType::Chunk, "Why?"},
      Token{TokenType::WhiteSpace, " "}, Token{TokenType::Chunk, "Just"},
      Token{TokenType::WhiteSpace, " "}, Token{TokenType::Chunk, "plain"},
      Token{TokenType::WhiteSpace, " "}, Token{TokenType::Chunk, "finger-"},
      Token{TokenType::Chunk, "licking"}, Token{TokenType::WhiteSpace, " "},
      Token{TokenType::Chunk, "good!"}};

  EXPECT_THAT(status, IsTrue());
  EXPECT_THAT(tokens.size(), Eq(10));
  auto expected_token = expected.cbegin();
  auto token = tokens.cbegin();
  auto end = tokens.cend();
  while (token != end) {
    EXPECT_THAT(*token, Eq(*expected_token));
    token++;
    expected_token++;
  }
}

// NOLINTNEXTLINE
TEST_F(TokenizerTest, CallsTokenConsumerWhenTokenIsReady) {
  Tokenizer tokenizer{"\t", false, false, false};

  //! [Example token consumer]
  std::vector<Token> tokens;
  auto status = tokenizer.Tokenize(
      "Hello", [&tokens](TokenType token_type, std::string token) {
        if (token_type != detail::TokenType::EndOfInput) {
          tokens.emplace_back(token_type, std::move(token));
        }
      });
  //! [Example token consumer]

  EXPECT_THAT(status, IsTrue());
  EXPECT_THAT(tokens.size(), Eq(1));
  EXPECT_THAT(tokens, Eq(std::vector<Token>{Token{TokenType::Chunk, "Hello"}}));
}

using ParamType = std::tuple<
    // Input text
    std::string,
    // `tab` expansion
    std::string,
    // replace white space
    bool,
    // collapse white space
    bool,
    // break on hyphens
    bool,
    // expected tokens
    std::vector<Token>>;

class TokenizerScenariosTest : public ::testing::TestWithParam<ParamType> {
public:
  static void SetUpTestSuite() {
    asap::logging::Registry::instance().SetLogLevel(
        asap::logging::Logger::Level::off);
  }
};

// NOLINTNEXTLINE
TEST_P(TokenizerScenariosTest, Tokenize) {
  const auto &[text, tab, replace_ws, collapse_ws, break_on_hyphens, expected] =
      GetParam();
  Tokenizer tokenizer{tab, replace_ws, collapse_ws, break_on_hyphens};

  std::vector<Token> tokens;
  auto status = tokenizer.Tokenize(
      text, [&tokens](TokenType token_type, std::string token) {
        if (token_type != detail::TokenType::EndOfInput) {
          tokens.emplace_back(token_type, std::move(token));
        }
      });

  EXPECT_THAT(status, IsTrue());
  EXPECT_THAT(tokens.size(), Eq(expected.size()));
  auto expected_token = expected.cbegin();
  auto token = tokens.cbegin();
  auto end = tokens.cend();
  while (token != end) {
    EXPECT_THAT(*token, Eq(*expected_token));
    token++;
    expected_token++;
  }
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(AllOff, TokenizerScenariosTest,
    // clang-format off
    testing::Values(
      ParamType{"",
        "\t", false, false, false,
        {
        }},
      ParamType{"\n",
        "\t", false, false, false,
        {
          {TokenType::WhiteSpace, "\n"}
        }},
      ParamType{"\n\n",
        "\t", false, false, false,
        {
          {TokenType::ParagraphMark, ""}
        }},
      ParamType{"very-very-long-word",
        "\t", false, false, false,
        {
          {TokenType::Chunk, "very-very-long-word"}
        }},
      ParamType{"Items\n\n1.\ta-a-a\n\n\n2.\tbbb or ccc",
        "\t", false, false, false,
        {
          {TokenType::Chunk, "Items"},
          {TokenType::ParagraphMark, ""},
          {TokenType::Chunk, "1."},
          {TokenType::WhiteSpace, "\t"},
          {TokenType::Chunk, "a-a-a"},
          {TokenType::ParagraphMark, ""},
          {TokenType::ParagraphMark, ""},
          {TokenType::Chunk, "2."},
          {TokenType::WhiteSpace, "\t"},
          {TokenType::Chunk, "bbb"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "or"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "ccc"}
        }}
    )
);

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(TabExpansionOn, TokenizerScenariosTest,
    // clang-format off
    testing::Values(
      ParamType{"\t", "    ", false, false, false,
        {{TokenType::WhiteSpace, "    "}}},
      ParamType{"\t\taaa \t \tbbb", "__", false, false, false,
        {
          {TokenType::WhiteSpace, "____"},
          {TokenType::Chunk, "aaa"},
          {TokenType::WhiteSpace, " __ __"},
          {TokenType::Chunk, "bbb"}
        }}
    )
);

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(ReplaceWhiteSpaceOn, TokenizerScenariosTest,
    // clang-format off
    testing::Values(
      ParamType{"\t",
        "\t", true, false, false,
        {{TokenType::WhiteSpace, " "}}},
      ParamType{"\t",
        "  ", true, false, false,
        {{TokenType::WhiteSpace, "  "}}},
      ParamType{"\t",
        "....", true, false, false,
        {{TokenType::WhiteSpace, "...."}}},
      ParamType{"\t",
        "-\n-", true, false, false,
        {{TokenType::WhiteSpace, "- -"}}},
      ParamType{"hello\fworld!\n\nbye\rbye\ncruel\vworld! \r\n ",
        "..", true, false, false,
        {
          {TokenType::Chunk, "hello"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "world!"},
          {TokenType::ParagraphMark, ""},
          {TokenType::Chunk, "bye"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "bye"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "cruel"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "world!"},
          {TokenType::WhiteSpace, "    "}
        }}
    )
);

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(CollapseWhiteSpaceOn, TokenizerScenariosTest,
    // clang-format off
    testing::Values(
      ParamType{"\t",
        "\t", false, true, false,
        {{TokenType::WhiteSpace, " "}}},
      ParamType{"\t",
        "  ", false, true, false,
        {{TokenType::WhiteSpace, " "}}},
      ParamType{"\t",
        "....", false, true, false,
        {{TokenType::WhiteSpace, " "}}},
      ParamType{"\t",
        "-\n-", true, true, false,
        {{TokenType::WhiteSpace, " "}}},
      ParamType{"hello\f   world!\n\nbye\t\rbye \ncruel\v \t world! \r\n ",
        "..", false, true, false,
        {
          {TokenType::Chunk, "hello"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "world!"},
          {TokenType::ParagraphMark, ""},
          {TokenType::Chunk, "bye"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "bye"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "cruel"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "world!"},
          {TokenType::WhiteSpace, " "}
        }}
    )
);

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(BreakOnHyphensOn, TokenizerScenariosTest,
    // clang-format off
    testing::Values(
      ParamType{"a-b",
        "  ", false, false, true,
        {
          {TokenType::Chunk, "a-"},
          {TokenType::Chunk, "b"}
        }},
      ParamType{"universally-true",
        "  ", false, false, true,
        {
          {TokenType::Chunk, "universally-"},
          {TokenType::Chunk, "true"}
        }},
      ParamType{"some things-never-change",
        "  ", false, false, true,
        {
          {TokenType::Chunk, "some"},
          {TokenType::WhiteSpace, " "},
          {TokenType::Chunk, "things-"},
          {TokenType::Chunk, "never-"},
          {TokenType::Chunk, "change"}
        }},
      ParamType{"a-",
        "  ", false, false, true,
        {
          {TokenType::Chunk, "a-"}
        }},
      ParamType{"a--",
        "  ", false, false, true,
        {
          {TokenType::Chunk, "a-"},
          {TokenType::Chunk, "-"}
        }},
      ParamType{"--",
        "  ", false, false, true,
        {
          {TokenType::Chunk, "--"}
        }},
      ParamType{"---",
        "  ", false, false, true,
        {
          {TokenType::Chunk, "---"}
        }},
      ParamType{"-a-b-c---d-ef",
        "  ", false, false, true,
        {
          {TokenType::Chunk, "-a-"},
          {TokenType::Chunk, "b-"},
          {TokenType::Chunk, "c-"},
          {TokenType::Chunk, "--d-"},
          {TokenType::Chunk, "ef"}
        }}
    )
);

} // namespace

} // namespace asap::wrap::detail

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
