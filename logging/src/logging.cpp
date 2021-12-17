/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

/*!
 * \file logging.cpp
 *
 * \brief Implementation details for the logging API.
 */

#include "logging/logging.h"

#include <common/compilers.h>
#include <contract/contract.h>

#include <iomanip> // std::setw
#include <sstream> // std::ostringstream
#include <utility>

// spdlog causes a bunch of compiler warnings we can't do anything about except temporarily
// disabling them
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__)
ASAP_PRAGMA(GCC diagnostic ignored "-Weverything")
#endif
#if defined _WIN32 && !defined(__cplusplus_winrt)
#include <spdlog/sinks/wincolor_sink.h>
#else
#include <spdlog/sinks/ansicolor_sink.h>
#endif
ASAP_DIAGNOSTIC_POP

namespace asap::logging {

// -------------------------------------------------------------------------------------------------
// Static members initialization
// -------------------------------------------------------------------------------------------------

/// The default logging format
const char *const Logger::DEFAULT_LOG_FORMAT = "[%Y-%m-%d %T.%e] [%t] [%^%l%$] [%n] %v";

// -------------------------------------------------------------------------------------------------
// Logger
// -------------------------------------------------------------------------------------------------

Logger::Logger(const std::string &name, const spdlog::sink_ptr &sink) {
  logger_ = std::make_shared<spdlog::logger>(name, sink);
  logger_->set_pattern(DEFAULT_LOG_FORMAT);
  logger_->set_level(spdlog::level::trace);
  // Ensure that critical errors, especially ASSERT/PANIC, get flushed
  logger_->flush_on(spdlog::level::critical);
}

Logger::~Logger() = default;

auto Registry::GetLogger(std::string const &name) -> spdlog::logger & {
  std::lock_guard<std::recursive_mutex> lock(loggers_mutex_);
  auto search = all_loggers_.find(name);
  if (search == all_loggers_.end()) {
    auto new_logger = all_loggers_.emplace(name, Logger(name, delegating_sink_));
    search = new_logger.first;
  }
  return *(search->second.logger_);
}

// -------------------------------------------------------------------------------------------------
// DelegatingSink
// -------------------------------------------------------------------------------------------------

// This mess is created because of the way spdlog is organizing its source code based on header only
// builds vs library builds. The issue is that spdlog places the template definitions in a separate
// file and explicitly instantiates them, so we have no problem at link, but we do have a problem
// with clang (rightfully) complaining that the template definitions are not available when the
// template needs to be instantiated here.
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__)
ASAP_PRAGMA(GCC diagnostic ignored "-Wundefined-func-template")
#endif
DelegatingSink::DelegatingSink(spdlog::sink_ptr delegate) : sink_delegate_(std::move(delegate)) {
}
ASAP_DIAGNOSTIC_POP

DelegatingSink::~DelegatingSink() = default;

// -------------------------------------------------------------------------------------------------
// Registry
// -------------------------------------------------------------------------------------------------

namespace {
auto CreateDelegatingSink() -> DelegatingSink * {
  // Add a default console sink
#if defined _WIN32 && !defined(__cplusplus_winrt)
  auto default_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
  auto default_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

  return new DelegatingSink(default_sink);
}
} // namespace

Registry::Registry(typename asap::Singleton<Registry>::token /*unused*/)
    : delegating_sink_(CreateDelegatingSink()) {
  CreatePredefinedLoggers();
}

void Registry::PushSink(spdlog::sink_ptr sink) {
  std::lock_guard<std::mutex> lock(sinks_mutex_);
  auto &sinks = sinks_;
  // Push the current sink on the stack and use the new one
  sinks.emplace(delegating_sink_->SwapSink(std::move(sink)));
}

void Registry::PopSink() {
  std::lock_guard<std::mutex> lock(sinks_mutex_);
  ASAP_ASSERT(!sinks_.empty() && "call to PopSink() not matching a previous call to PushSink()");
  if (!sinks_.empty()) {
    auto &sink = sinks_.top();
    // Assign this previous sink to the delegating sink
    delegating_sink_->SwapSink(sink);
    sinks_.pop();
  }
}

void Registry::SetLogLevel(spdlog::level::level_enum log_level) {
  std::lock_guard<std::recursive_mutex> lock(loggers_mutex_);
  for (auto &log : all_loggers_) {
    // Thread safe
    log.second.SetLevel(log_level);
  }
}

void Registry::SetLogFormat(const std::string &log_format) {
  std::lock_guard<std::recursive_mutex> lock(loggers_mutex_);
  for (auto &log : all_loggers_) {
    log.second.logger_->set_pattern(log_format);
  }
}

void Registry::CreatePredefinedLoggers() {
  all_loggers_.emplace("misc", Logger("misc", delegating_sink_));
  all_loggers_.emplace("testing", Logger("testing", delegating_sink_));
  all_loggers_.emplace("main", Logger("main", delegating_sink_));
}

// -------------------------------------------------------------------------------------------------
// Helper for file name and line number formatting
// -------------------------------------------------------------------------------------------------
#ifndef NDEBUG
/*!
 * \brief Make a string with the soruce code file name and line number at which the log message was
 * produced.
 *
 * \param file source code file name.
 * \param line source code line number.
 * \return a formatted string with the file name and line number.
 */
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto FormatFileAndLine(char const *file, char const *line) -> std::string {
  static constexpr int FILE_MAX_LENGTH = 60;
  static constexpr int FILE_TOO_LONG_PREFIX_LENGTH = 7;
  static constexpr int FILE_TOO_LONG_PREFIX_FULL_LENGTH = 10;
  static constexpr int LINE_NUMBER_WIDTH = 5;
  std::ostringstream ostr;
  std::string fstr(file);
  if (fstr.length() > FILE_MAX_LENGTH) {
    fstr =
        fstr.substr(0, FILE_TOO_LONG_PREFIX_LENGTH)
            .append("...")
            .append(fstr.substr(fstr.length() - FILE_MAX_LENGTH + FILE_TOO_LONG_PREFIX_FULL_LENGTH,
                FILE_MAX_LENGTH - FILE_TOO_LONG_PREFIX_FULL_LENGTH));
  }
  ostr << "[" << std::setw(FILE_MAX_LENGTH) << std::right << fstr << ":"
       << std::setw(LINE_NUMBER_WIDTH) << std::setfill('0') << std::right << line << "] ";
  return ostr.str();
}
#endif // NDEBUG

} // namespace asap::logging
