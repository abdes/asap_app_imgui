//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "common/singleton.h"

#include <common/compilers.h>

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vector>

// Disable compiler and linter warnings originating from the unit test framework
// and for which we cannot do anything. Additionally every TEST or TEST_X macro
// usage must be preceded by a '// NOLINTNEXTLINE'.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__) && ASAP_HAS_WARNING("-Wused-but-marked-unused")
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif

namespace asap {

namespace {

class TheOne : public Singleton<TheOne> {
public:
  explicit TheOne(typename Singleton<TheOne>::token /*unused*/) {
  }
  [[nodiscard]] auto data() const
      -> const std::chrono::high_resolution_clock::time_point & {
    return data_;
  }

private:
  std::chrono::high_resolution_clock::time_point data_ =
      std::chrono::high_resolution_clock::now();
};

// NOLINTNEXTLINE
TEST(Singleton, OneSingleInstance) {
  auto first_data = TheOne::instance().data();
  // Any subsequent calls to instance() will return the same instance
  auto second_data = TheOne::instance().data();
  ASSERT_THAT(first_data, testing::Eq(second_data));
}

// NOLINTNEXTLINE
TEST(Singleton, OneSingleInstanceInMultiThreads) {
  const auto &the_data = TheOne::instance().data();

  constexpr auto NUM_THREADS = 5;
  std::vector<std::thread> workers;
  workers.reserve(NUM_THREADS);
  for (int ii = 0; ii < NUM_THREADS; ii++) {
    workers.emplace_back(std::thread([the_data]() {
      auto data = TheOne::instance().data();
      ASSERT_THAT(data, testing::Eq(the_data));
    }));
  }

  std::for_each(
      workers.begin(), workers.end(), [](std::thread &thr) { thr.join(); });
}

} // namespace

} // namespace asap

ASAP_DIAGNOSTIC_POP
