//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include <contract/ut/framework.h>
#include <contract/ut/gtest.h>

#include <common/compilers.h>
#include <contract/contract.h>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest-matchers.h>
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

namespace asap::contract {
namespace {

auto TestExpectDefault(const int *ptr) -> int {
  ASAP_EXPECT(ptr);
  return *ptr;
}

auto TestEnsureDefault(const int *ptr) -> int {
  ASAP_ENSURE(ptr);
  return *ptr;
}

auto TestAssertDefault(const int *ptr) -> int {
  ASAP_ASSERT(ptr);
  return *ptr;
}

// NOLINTNEXTLINE
TEST(GoogleTestDeathMacros, DefaultModeExpectDeath) {
  CHECK_VIOLATES_CONTRACT(TestExpectDefault(nullptr));
}

// NOLINTNEXTLINE
TEST(GoogleTestDeathMacros, DefaultModeEnsureDeath) {
  CHECK_VIOLATES_CONTRACT(TestEnsureDefault(nullptr));
}

// NOLINTNEXTLINE
TEST(GoogleTestDeathMacros, DefaultModeAssertDeath) {
  CHECK_VIOLATES_CONTRACT(TestAssertDefault(nullptr));
}

void NestedViolator(int *ptr) {
  CHECK_VIOLATES_CONTRACT(TestAssertDefault(ptr));
  TestEnsureDefault(nullptr);
}
void Violator(int *ptr) {
  CHECK_VIOLATES_CONTRACT(TestAssertDefault(ptr));
  CHECK_VIOLATES_CONTRACT(NestedViolator(ptr));
  TestExpectDefault(nullptr);
}

// NOLINTNEXTLINE
TEST(GoogleTestDeathMacros, NestedChecks) {
  CHECK_VIOLATES_CONTRACT(Violator(nullptr));
}

// NOLINTNEXTLINE
TEST(GoogleTestDeathMacros, VerboseTestPrintsViolationInfo) {
  class ErrorOutputRedirect {
  public:
    explicit ErrorOutputRedirect(std::streambuf *new_buffer)
        : old(std::cerr.rdbuf(new_buffer)) {
    }

    ErrorOutputRedirect(const ErrorOutputRedirect &) = delete;
    ErrorOutputRedirect(const ErrorOutputRedirect &&) = delete;
    auto operator=(const ErrorOutputRedirect &)
        -> ErrorOutputRedirect & = delete;
    auto operator=(const ErrorOutputRedirect &&)
        -> ErrorOutputRedirect & = delete;

    ~ErrorOutputRedirect() {
      std::cerr.rdbuf(old);
    }

  private:
    std::streambuf *old;
  };

  std::stringstream buffer;
  ErrorOutputRedirect output(buffer.rdbuf());
  SetVerbosity(Verbosity::VERBOSE);
  CHECK_VIOLATES_CONTRACT(TestExpectDefault(nullptr));
  auto text = buffer.str();
  EXPECT_THAT(text, ::testing::ContainsRegex("violated"));
}

} // namespace
} // namespace asap::contract

auto main(int argc, char **argv) -> int {
  asap::contract::PrepareForTesting();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// NOLINTEND(used-but-marked-unused)
ASAP_DIAGNOSTIC_POP
