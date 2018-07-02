//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <common/logging.h>

#include <iomanip>  // std::setw
#include <sstream>  // std::ostringstream

#include <common/assert.h>

namespace asap {
namespace logging {

// ---------------------------------------------------------------------------
// Static members initialization
// ---------------------------------------------------------------------------

/// The default logging format
const char *Logger::DEFAULT_LOG_FORMAT =
    "[%Y-%m-%d %T.%e] [%t] [%^%l%$] [%n] %v";

// Synchronization mutex for sinks
std::mutex Registry::sinks_mutex_;
// Synchronization mutex for the loggers collection.
std::recursive_mutex Registry::loggers_mutex_;

// ---------------------------------------------------------------------------
// Helpers for dealing with Logger Id
// ---------------------------------------------------------------------------

namespace {

Id &operator++(Id &target) {
  target = static_cast<Id>(static_cast<int>(target) + 1);
  return target;
}

/// Get the corresponding logger name for a logger id.
inline constexpr const char *LoggerName(Id id) {
  switch (id) {
    // clang-format off
    case Id::MISC:     return "misc    ";
    case Id::TESTING:  return "testing ";
    case Id::COMMON:   return "common  ";
    case Id::MAIN:     return "main    ";
    case Id::INVALID_: return "__DO_NOT_USE__";
      // clang-format on
      // omit default case to trigger compiler warning for missing cases
  };
  return "__INVALID__";
}

}  // namespace

// ---------------------------------------------------------------------------
// Logger
// ---------------------------------------------------------------------------

Logger::Logger(std::string name, logging::Id id, spdlog::sink_ptr sink) : id_(id) {
  logger_ = std::make_shared<spdlog::logger>(name, sink);
  logger_->set_pattern(DEFAULT_LOG_FORMAT);
  logger_->set_level(spdlog::level::trace);
  // Ensure that critical errors, especially ASSERT/PANIC, get flushed
  logger_->flush_on(spdlog::level::critical);
}

spdlog::logger &Registry::GetLogger(Id id) {
  std::lock_guard<std::recursive_mutex> lock(loggers_mutex_);
  return *(Loggers()[static_cast<int>(id)].logger_);
}

// ---------------------------------------------------------------------------
// Registry
// ---------------------------------------------------------------------------

void Registry::PushSink(spdlog::sink_ptr sink) {
  std::lock_guard<std::mutex> lock(sinks_mutex_);
  auto &sinks = Sinks();
  // Push the current sink on the stack and use the new one
  sinks.emplace(delegating_sink()->SwapSink(sink));
}

void Registry::PopSink() {
  std::lock_guard<std::mutex> lock(sinks_mutex_);
  auto &sinks = Sinks();
  ASAP_ASSERT(
      !sinks.empty() &&
      "call to PopSink() not matching a previous call to PushSink()");
  if (!sinks.empty()) {
    auto &sink = sinks.top();
    // Assign this previous sink to the delegating sink
    delegating_sink()->SwapSink(sink);
    sinks.pop();
  }
}

std::stack<spdlog::sink_ptr> &Registry::Sinks() {
  static std::stack<spdlog::sink_ptr> sinks;
  return sinks;
}

void Registry::SetLogLevel(spdlog::level::level_enum log_level) {
  std::lock_guard<std::recursive_mutex> lock(loggers_mutex_);
  auto &loggers = Loggers();
  std::for_each(loggers.begin(), loggers.end(), [log_level](Logger &log) {
    // Thread safe
    log.Level(log_level);
  });
}

void Registry::SetLogFormat(const std::string &log_format) {
  std::lock_guard<std::recursive_mutex> lock(loggers_mutex_);
  auto &loggers = Loggers();
  std::for_each(loggers.begin(), loggers.end(), [log_format](Logger &log) {
    // Not thread safe
    std::lock_guard<std::mutex> log_lock(*log.logger_mutex_.get());
    log.logger_->set_pattern(log_format);
  });
}

std::vector<Logger> &Registry::Loggers() {
  static auto &all_loggers_static = all_loggers_();
  return all_loggers_static;
}

std::vector<Logger> &Registry::all_loggers_() {
  static auto *all_loggers = new std::vector<Logger>();
  for (auto id = Id::MISC; id < Id::INVALID_; ++id) {
    auto name = LoggerName(id);
    all_loggers->emplace_back(Logger(name, id, delegating_sink()));
  }
  return *all_loggers;
}

std::shared_ptr<DelegatingSink> &Registry::delegating_sink() {
  static auto sink_static = std::shared_ptr<DelegatingSink>(delegating_sink_());
  return sink_static;
}

DelegatingSink *Registry::delegating_sink_() {
  // Add a default console sink
#if defined _WIN32 && !defined(__cplusplus_winrt)
  auto default_sink =
      std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
  auto default_sink =
      std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

  static auto *sink = new DelegatingSink(default_sink);
  return sink;
}


// ---------------------------------------------------------------------------
// Helper for file name and line number formatting
// ---------------------------------------------------------------------------

std::string FormatFileAndLine(char const *file, char const *line) {
  constexpr static int FILE_MAX_LENGTH = 70;
  std::ostringstream ostr;
  std::string fstr(file);
  if (fstr.length() > FILE_MAX_LENGTH) {
    fstr = fstr.substr(0, 7).append("...").append(fstr.substr(
        fstr.length() - FILE_MAX_LENGTH + 10, FILE_MAX_LENGTH - 10));
  }
  ostr << "[" << std::setw(FILE_MAX_LENGTH) << std::right << fstr << ":"
       << std::setw(5) << std::setfill('0') << std::right << line << "] ";
  return ostr.str();
}

}  // namespace logging
}  // namespace asap
