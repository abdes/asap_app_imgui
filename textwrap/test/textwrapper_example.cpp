//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "textwrap/textwrap.h"

#include <common/compilers.h>
#include <logging/logging.h>

#include <gtest/gtest.h>

#include <type_traits>

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

namespace asap::wrap {

namespace {

// NOLINTNEXTLINE
TEST(TextWrapper, WrapExample) {
  asap::logging::Registry::instance().SetLogLevel(
      asap::logging::Logger::Level::off);

  //! [Example usage]
  const auto *book = "Pride and Prejudice:";
  const auto *passage = "It is a truth universally acknowledged, that a single "
                        "man in possession of a good fortune, must be in want "
                        "of a wife.";

  constexpr size_t column_width = 28;
  TextWrapper title_wrapper = TextWrapper::Create().Width(column_width);
  TextWrapper text_wrapper = TextWrapper::Create()
                                 .Width(column_width)
                                 .TrimLines()
                                 .ReplaceWhiteSpace()
                                 .CollapseWhiteSpace()
                                 .IndentWith()
                                 .Initially("   ")
                                 .Then("   ");

  std::cout << title_wrapper.Fill(book).value_or("error") << std::endl;
  std::cout << text_wrapper.Fill(passage).value_or("error") << std::endl;

  // Pride and Prejudice:
  //    It is a truth universally
  //    acknowledged, that a single
  //    man in possession of a good
  //    fortune, must be in want of
  //    a wife.

  //! [Example usage]
}

} // namespace

} // namespace asap::wrap

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
