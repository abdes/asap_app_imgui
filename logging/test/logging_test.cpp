/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "logging/logging.h"

#include <common/compilers.h>

#include <gmock/gmock-more-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <mutex>
#include <sstream>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

// Disable compiler and linter warnings originating from the unit test framework and for which we
// cannot do anything.
// Additionally every TEST or TEST_X macro usage must be preceded by a '// NOLINTNEXTLINE'.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__) && ASAP_HAS_WARNING("-Wused-but-marked-unused")
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wunused-member-function"
#endif
// NOLINTBEGIN(used-but-marked-unused)

using ::testing::ContainsRegex;
using ::testing::Eq;
using ::testing::IsFalse;
using ::testing::IsTrue;
using ::testing::Ne;

// spdlog puts template definitions in separate files from the .h files. We ned to declare the
// template explicit instantiation present in the implementation files to avoid compiler warnings.
extern template class spdlog::sinks::base_sink<std::mutex>;

namespace asap::logging {

namespace {

template <typename Mutex> class TestSink : public spdlog::sinks::base_sink<Mutex> {
public:
  void Reset() {
    out_.str("");
    out_.clear();
    called_ = 0;
    flushed_ = 0;
  }

  std::ostringstream out_;
  int called_{0};
  int flushed_{0};

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {
    ++called_;
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
    out_ << fmt::to_string(formatted);
  }

  void flush_() override {
    out_.flush();
    ++flushed_;
  }
};

using TestSink_mt = TestSink<std::mutex>;
using TestSink_st = TestSink<spdlog::details::null_mutex>;

class Foo : Loggable<Foo> {
public:
  static void LogSomethingTrace(const std::string &msg) {
    ASLOG(trace, "{}", msg);
  }

  static const char *const LOGGER_NAME;
};
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__) && ASAP_HAS_WARNING("-Wunused-const-variable")
#pragma clang diagnostic ignored "-Wunused-const-variable"
#endif
const char *const Foo::LOGGER_NAME = "foo";
ASAP_DIAGNOSTIC_POP

// NOLINTNEXTLINE
TEST(Logging, LoggingByExtendingLoggable) {
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(new TestSink_mt());
  Registry::instance().PushSink(test_sink_ptr);
  auto *test_sink = dynamic_cast<TestSink_mt *>(test_sink_ptr.get());

  Foo::LogSomethingTrace("Hello");
  EXPECT_THAT(test_sink->called_, IsTrue());
  auto msg = test_sink->out_.str();
  EXPECT_THAT(msg.empty(), IsFalse());
  EXPECT_THAT(msg.find("Hello"), Ne(std::string::npos));
  test_sink->Reset();

  Registry::instance().PopSink();
}

// NOLINTNEXTLINE
TEST(Logging, RegistrySetLogLevel) {
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(new TestSink_mt());
  Registry::instance().PushSink(test_sink_ptr);
  auto *test_sink = dynamic_cast<TestSink_mt *>(test_sink_ptr.get());

  // We can set the log level of all loggers through the registry and it will cascade down to each
  // existing logger.
  Registry::instance().SetLogLevel(Logger::Level::off);
  auto &test_logger = Registry::instance().GetLogger("testing");
  ASLOG_TO_LOGGER(test_logger, debug, "Hello");
  EXPECT_THAT(test_sink->called_, IsFalse());
  auto msg = test_sink->out_.str();
  EXPECT_THAT(msg.empty(), IsTrue());
  test_sink->Reset();

  // Any new logger created after the log level was set will also use the new log level.
  auto &new_logger = Registry::instance().GetLogger("new_logger");
  ASLOG_TO_LOGGER(new_logger, error, "Hello");
  EXPECT_THAT(test_sink->called_, IsFalse());
  msg = test_sink->out_.str();
  EXPECT_THAT(msg.empty(), IsTrue());
  test_sink->Reset();

  Registry::instance().SetLogLevel(Logger::Level::trace);
  Registry::instance().PopSink();
}

// NOLINTNEXTLINE
TEST(Logging, RegistrySetLogFormat) {
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(new TestSink_mt());
  Registry::instance().PushSink(test_sink_ptr);
  auto *test_sink = dynamic_cast<TestSink_mt *>(test_sink_ptr.get());

  // We can set the log format of all loggers through the registry and it will cascade down to each
  // existing logger.
  Registry::instance().SetLogFormat("TEST_PATTERN %v");
  auto &test_logger = Registry::instance().GetLogger("testing");
  ASLOG_TO_LOGGER(test_logger, error, "Hello");
  auto msg = test_sink->out_.str();
  EXPECT_THAT(msg.find("TEST_PATTERN"), Ne(std::string::npos));

  // Pushing a new sink, will also apply the format pattern to it.
  auto second_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(new TestSink_mt());
  Registry::instance().PushSink(second_sink_ptr);
  auto *second_sink = dynamic_cast<TestSink_mt *>(second_sink_ptr.get());
  ASLOG_TO_LOGGER(test_logger, error, "Hello");
  msg = second_sink->out_.str();
  EXPECT_THAT(msg.find("TEST_PATTERN"), Ne(std::string::npos));

  Registry::instance().SetLogFormat(DEFAULT_LOG_FORMAT);
  Registry::instance().PopSink();
}

// NOLINTNEXTLINE
TEST(Logging, LoggingFromMultipleThreadsWorks) {
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(new TestSink_mt());
  Registry::instance().PushSink(test_sink_ptr);
  spdlog::set_pattern("%v");

  std::thread th1([]() {
    constexpr int LOG_ITERATIONS = 5;
    for (auto ii = 0; ii < LOG_ITERATIONS; ++ii) {
      ASLOG_MISC(debug, "THREAD_1: {}", ii);
    }
  });
  std::thread th2([]() {
    constexpr int LOG_ITERATIONS = 5;
    auto &test_logger = Registry::instance().GetLogger("testing");
    for (auto ii = 0; ii < LOG_ITERATIONS; ++ii) {
      ASLOG_TO_LOGGER(test_logger, trace, "THREAD_2: {}", ii);
    }
  });
  th1.join();
  th2.join();

  auto *test_sink = dynamic_cast<TestSink_mt *>(test_sink_ptr.get());
  EXPECT_THAT(test_sink->called_, IsTrue());
  std::istringstream msg_reader(test_sink->out_.str());
  std::string line;
  auto expected_seq_th1 = 0;
  auto expected_seq_th2 = 0;
  while (std::getline(msg_reader, line)) {
    if (line.find("THREAD_1") != std::string::npos) {
      EXPECT_THAT(line.find(std::string("THREAD_1: ") + std::to_string(expected_seq_th1)),
          Ne(std::string::npos));
      ++expected_seq_th1;
    }
    if (line.find("THREAD_2") != std::string::npos) {
      EXPECT_THAT(line.find(std::string("THREAD_2: ") + std::to_string(expected_seq_th2)),
          Ne(std::string::npos));
      ++expected_seq_th2;
    }
  }
  EXPECT_THAT(expected_seq_th1, Eq(5));
  EXPECT_THAT(expected_seq_th2, Eq(5));

  Registry::instance().PopSink();
}

// NOLINTNEXTLINE
TEST(Logging, LogToLoggerWithParameters) {
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(new TestSink_mt());
  Registry::instance().PushSink(test_sink_ptr);

  auto &test_logger = Registry::instance().GetLogger("testing");

  auto *test_sink = dynamic_cast<TestSink_mt *>(test_sink_ptr.get());

  AS_DO_LOG(test_logger, debug);
  EXPECT_THAT(test_sink->called_, Eq(1));
  test_sink->Reset();

  AS_DO_LOG(test_logger, debug, "message");
  EXPECT_THAT(test_sink->called_, Eq(1));
  EXPECT_THAT(test_sink->out_.str(), ContainsRegex("message"));
  test_sink->Reset();

  AS_DO_LOG(test_logger, debug, "message {}", 1);
  EXPECT_THAT(test_sink->called_, Eq(1));
  EXPECT_THAT(test_sink->out_.str(), ContainsRegex("message 1"));
  test_sink->Reset();

  AS_DO_LOG(test_logger, debug, "message {} {}", 1, 2);
  EXPECT_THAT(test_sink->called_, Eq(1));
  EXPECT_THAT(test_sink->out_.str(), ContainsRegex("message 1 2"));
  test_sink->Reset();

  AS_DO_LOG(test_logger, debug, "message {} {} {}", 1, 2, 3);
  EXPECT_THAT(test_sink->called_, Eq(1));
  EXPECT_THAT(test_sink->out_.str(), ContainsRegex("message 1 2 3"));
  test_sink->Reset();

  AS_DO_LOG(test_logger, debug, "message {} {} {} {}", 1, 2, 3, 4);
  EXPECT_THAT(test_sink->called_, Eq(1));
  EXPECT_THAT(test_sink->out_.str(), ContainsRegex("message 1 2 3 4"));
  test_sink->Reset();

  Registry::instance().PopSink();
}

class MockSink : public spdlog::sinks::sink {
public:
  void log(const spdlog::details::log_msg & /*msg*/) override {
    ++called_;
  }
  void flush() override {
  }
  void set_pattern(const std::string & /*pattern*/) override {
  }
  void set_formatter(std::unique_ptr<spdlog::formatter> /*sink_formatter*/) override {
  }

  void Reset() {
    called_ = 0;
  }

  int called_{0};
};

// NOLINTNEXTLINE
TEST(Logging, SinkPushPop) {
  auto first_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(new MockSink());
  auto second_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(new MockSink());

  auto *first_mock = dynamic_cast<MockSink *>(first_sink_ptr.get());
  auto *second_mock = dynamic_cast<MockSink *>(second_sink_ptr.get());

  auto &test_logger = Registry::instance().GetLogger("testing");

  Registry::instance().PushSink(first_sink_ptr);
  ASLOG_TO_LOGGER(test_logger, debug, "message");
  EXPECT_THAT(first_mock->called_, Eq(1));
  first_mock->Reset();
  second_mock->Reset();

  Registry::instance().PushSink(second_sink_ptr);
  ASLOG_TO_LOGGER(test_logger, debug, "message");
  EXPECT_THAT(first_mock->called_, Eq(0));
  EXPECT_THAT(second_mock->called_, Eq(1));
  first_mock->Reset();
  second_mock->Reset();

  Registry::instance().PopSink();
  ASLOG_TO_LOGGER(test_logger, debug, "message");
  EXPECT_THAT(first_mock->called_, Eq(1));
  EXPECT_THAT(second_mock->called_, Eq(0));
  first_mock->Reset();
  second_mock->Reset();

  Registry::instance().PopSink();
  ASLOG_TO_LOGGER(test_logger, debug, "message");
  EXPECT_THAT(first_mock->called_, Eq(0));
  EXPECT_THAT(second_mock->called_, Eq(0));
}

#ifndef NDEBUG
// NOLINTNEXTLINE
TEST(Logging, FileNameShortening) {
  auto file_name = std::string{"0123456789----------"};
  // Make it really long......
  constexpr int NUMBER_OF_APPENDS = 10;
  for (int ii = 0; ii < NUMBER_OF_APPENDS; ii++) {
    file_name.append("++++++++++");
  }
  file_name.append("really_long_file_name");
  auto msg = asap::logging::FormatFileAndLine(file_name.c_str(), INTERNAL_LINE_STRING);
  EXPECT_THAT(msg.empty(), IsFalse());
  EXPECT_THAT(msg.find("0123456..."), Ne(std::string::npos));
  EXPECT_THAT(msg.find("really_long_file_name"), Ne(std::string::npos));
}
#endif // NDEBUG

// NOLINTNEXTLINE
TEST(Logging, DelegatingSinkFlush) {
  auto test_sink_ptr = std::shared_ptr<spdlog::sinks::sink>(new TestSink_mt());
  Registry::instance().PushSink(test_sink_ptr);
  auto *test_sink = dynamic_cast<TestSink_mt *>(test_sink_ptr.get());
  auto &test_logger = Registry::instance().GetLogger("testing");
  test_logger.flush();
  ASSERT_THAT(test_sink->flushed_, Eq(1));
}

} // namespace

} // namespace asap::logging
