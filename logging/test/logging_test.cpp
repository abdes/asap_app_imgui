//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "logging/logging.h"

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
#endif

#include <logging/logging.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include <catch2/catch.hpp>

ASAP_DIAGNOSTIC_POP

#include <iostream>
#include <mutex>
#include <sstream>

template <typename Mutex> class TestSink : public spdlog::sinks::base_sink<Mutex> {
public:
  void Reset() {
    out_.clear();
    called_ = 0;
  }

  std::ostringstream out_;
  int called_{0};

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {
    ++called_;
    out_.write(msg.payload.data(), static_cast<std::streamsize>(msg.payload.size()));
    out_.write("\n", 1);
  }

  void flush_() override { out_.flush(); }
};

using TestSink_mt = TestSink<std::mutex>;
using TestSink_st = TestSink<spdlog::details::null_mutex>;

namespace asap {
namespace logging {

class Foo : Loggable<Foo> {
public:
  Foo() { ASLOG(trace, "Foo constructor"); }

  static const char *const LOGGER_NAME;
};
const char *const Foo::LOGGER_NAME = "foo";

TEST_CASE("TestLoggable", "[common][logging]") {
  auto *test_sink = new TestSink_mt();
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(test_sink);
  Registry::PushSink(test_sink_ptr);

  Foo foo;
  REQUIRE(test_sink->called_);
  auto msg = test_sink->out_.str();
  REQUIRE(!msg.empty());
  REQUIRE(msg.find("Foo constructor") != std::string::npos);

  Registry::PopSink();
}

TEST_CASE("TestMultipleThreads", "[common][logging]") {
  auto *test_sink = new TestSink_mt();
  spdlog::set_pattern("%v");
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(test_sink);
  Registry::PushSink(test_sink_ptr);

  std::thread th1([]() {
    for (auto ii = 0; ii < 5; ++ii) {
      ASLOG_MISC(debug, "THREAD_1: {}", ii);
    }
  });
  std::thread th2([]() {
    auto &test_logger = Registry::GetLogger("testing");
    for (auto ii = 0; ii < 5; ++ii) {
      ASLOG_TO_LOGGER(test_logger, trace, "THREAD_2: {}", ii);
    }
  });
  th1.join();
  th2.join();

  REQUIRE(test_sink->called_ == 10);
  std::istringstream msg_reader(test_sink->out_.str());
  std::string line;
  auto expected_seq_th1 = 0;
  auto expected_seq_th2 = 0;
  while (std::getline(msg_reader, line)) {
    if (line.find("THREAD_1") != std::string::npos) {
      REQUIRE(line.find(std::string("THREAD_1: ") + std::to_string(expected_seq_th1)) !=
              std::string::npos);
      ++expected_seq_th1;
    }
    if (line.find("THREAD_2") != std::string::npos) {
      REQUIRE(line.find(std::string("THREAD_2: ") + std::to_string(expected_seq_th2)) !=
              std::string::npos);
      ++expected_seq_th2;
    }
  }
  REQUIRE(expected_seq_th1 == 5);
  REQUIRE(expected_seq_th2 == 5);

  Registry::PopSink();
}

TEST_CASE("TestLogWithPrefix", "[common][logging]") {
  auto *test_sink = new TestSink_mt();
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(test_sink);
  Registry::PushSink(test_sink_ptr);

  auto &test_logger = Registry::GetLogger("testing");

  AS_DO_LOG(test_logger, debug, "message");
  REQUIRE(test_sink->called_ == 1);
  REQUIRE(test_sink->out_.str().find("message") != std::string::npos);
  test_sink->Reset();

  AS_DO_LOG(test_logger, debug, "message {}", 1);
  REQUIRE(test_sink->called_ == 1);
  REQUIRE(test_sink->out_.str().find("message 1") != std::string::npos);
  test_sink->Reset();

  AS_DO_LOG(test_logger, debug, "message {} {}", 1, 2);
  REQUIRE(test_sink->called_ == 1);
  REQUIRE(test_sink->out_.str().find("message 1 2") != std::string::npos);
  test_sink->Reset();

  AS_DO_LOG(test_logger, debug, "message {} {} {}", 1, 2, 3);
  REQUIRE(test_sink->called_ == 1);
  REQUIRE(test_sink->out_.str().find("message 1 2 3") != std::string::npos);
  test_sink->Reset();

  AS_DO_LOG(test_logger, debug, "message {} {} {} {}", 1, 2, 3, 4);
  REQUIRE(test_sink->called_ == 1);
  REQUIRE(test_sink->out_.str().find("message 1 2 3 4") != std::string::npos);
  test_sink->Reset();

  Registry::PopSink();
}

namespace {
class MockSink : public spdlog::sinks::sink {
public:
  void log(const spdlog::details::log_msg & /*msg*/) override { ++called_; }
  void flush() override {}
  void set_pattern(const std::string & /*pattern*/) override {}
  void set_formatter(std::unique_ptr<spdlog::formatter> /*sink_formatter*/) override {}

  void Reset() { called_ = 0; }

  int called_{0};
};
} // namespace

TEST_CASE("TestLogPushSink", "[common][logging]") {
  auto *first_mock = new MockSink();
  auto *second_mock = new MockSink();
  auto first_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(first_mock);
  auto second_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(second_mock);

  auto &test_logger = Registry::GetLogger("testing");
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
  // mute the logger output
  auto *test_sink = new TestSink_mt();
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(test_sink);
  Registry::PushSink(test_sink_ptr);
  ASLOG_TO_LOGGER(test_logger, debug, "message");

  REQUIRE(first_mock->called_ == 0);
  REQUIRE(second_mock->called_ == 0);

  Registry::PopSink();
}

} // namespace logging
} // namespace asap
