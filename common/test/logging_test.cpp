//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <catch2/catch.hpp>

#include <common/logging.h>


namespace asap {
namespace logging {

TEST_CASE("TestLoggable", "[common][logging]") {
  class Foo : Loggable<Id::TESTING> {
   public:
    Foo() { ASLOG(trace, "Foo construcor"); }
  };

  ASLOG_MISC(info, "Hello World!");
  Foo foo;
}

TEST_CASE("TestMultipleThreads", "[common][logging]") {
  std::thread th1([]() {
    for (auto ii = 0; ii < 5; ++ii)
      ASLOG_MISC(debug, "Logging from thread 1: {}", ii);
  });
  std::thread th2([]() {
    auto &test_logger = Registry::GetLogger(Id::TESTING);
    for (auto ii = 0; ii < 5; ++ii)
      ASLOG_TO_LOGGER(test_logger, trace, "Logging from thread 2: {}", ii);
  });
  th1.join();
  th2.join();
}

TEST_CASE("TestLogWithPrefix", "[common][logging]") {
  auto &test_logger = Registry::GetLogger(Id::TESTING);

  AS_DO_LOG(test_logger, debug, "message");
  AS_DO_LOG(test_logger, debug, "message {}", 1);
  AS_DO_LOG(test_logger, debug, "message {} {}", 1, 2);
  AS_DO_LOG(test_logger, debug, "message {} {} {}", 1, 2, 3);
  AS_DO_LOG(test_logger, debug, "message {} {} {} {}", 1, 3, 3, 4);
}

class MockSink : public spdlog::sinks::sink {
public:
  void log(const spdlog::details::log_msg &) override {
    ++called_;
  }
  void flush() override {}
  void Reset() { called_ = 0; }

  int called_{0};
};

TEST_CASE("TestLogPushSink", "[common][logging]") {
  auto *first_mock = new MockSink();
  auto *second_mock = new MockSink();
	auto first_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(first_mock);
	auto second_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(second_mock);

	auto &test_logger = Registry::GetLogger(Id::TESTING);
	Registry::PushSink(first_sink_ptr);
	ASLOG_TO_LOGGER(test_logger, debug, "message");

  REQUIRE(first_mock->called_ == 1);
  first_mock->Reset();
  second_mock->Reset();

	Registry::PushSink(second_sink_ptr);
	ASLOG_TO_LOGGER(test_logger, debug, "message");

  REQUIRE(first_mock->called_ == 0);
  REQUIRE(second_mock->called_ == 1);
  first_mock->Reset();
  second_mock->Reset();

  Registry::PopSink();
	ASLOG_TO_LOGGER(test_logger, debug, "message");

  REQUIRE(first_mock->called_ == 1);
  REQUIRE(second_mock->called_ == 0);
  first_mock->Reset();
  second_mock->Reset();

	Registry::PopSink();
	ASLOG_TO_LOGGER(test_logger, debug, "message");

  REQUIRE(first_mock->called_ == 0);
  REQUIRE(second_mock->called_ == 0);
  first_mock->Reset();
  second_mock->Reset();
}

}  // namespace logging
}  // namespace asap
