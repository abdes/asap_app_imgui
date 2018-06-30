//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <stack>   // for stacking sinks
#include <string>  // for std::string
#include <thread>  // for std::mutex

#include <common/non_copiable.h>
#include <spdlog/fmt/ostr.h>  // for user defined objects logging
#include <spdlog/spdlog.h>

namespace asap {
namespace logging {

// ---------------------------------------------------------------------------
// Looger IDs
// ---------------------------------------------------------------------------

/*!
 * @brief Exhaustive list of logger ids that can be used in declaring
 * Loggable<ID> classes.
 *
 * Apart from this use case, the rest of the logging macros only use
 * logger names.
 */
enum class Id {
  MISC,
  TESTING,
  COMMON,
  MAIN,
  INVALID_  // MUST BE THE LAST ONE
};

// ---------------------------------------------------------------------------
// Logger
// ---------------------------------------------------------------------------

/**
 * @brief Logger wrapper for a spdlog logger.
 */
class Logger : private asap::NonCopiable {
 public:
  /* This is simple mapping between Logger severity levels and spdlog severity
   * levels. The only reason for this mapping is to go around the fact that
   * spdlog defines level as err but the method to log at err level is called
   * LOGGER.error not LOGGER.err. All other level are fine spdlog::info
   * corresponds to LOGGER.info method.
   */
  enum class Level {
    trace = spdlog::level::trace,
    debug = spdlog::level::debug,
    info = spdlog::level::info,
    warn = spdlog::level::warn,
    error = spdlog::level::err,
    critical = spdlog::level::critical,
    off = spdlog::level::off
  };

  /// Move constructor
  Logger(Logger &&other) noexcept
      : id_(other.id_), logger_(std::move(other.logger_)),
        logger_mutex_(std::move(other.logger_mutex_)){};

  /// Move assignment
  Logger &operator=(Logger &&other) noexcept {
    Logger(std::move(other)).swap(*this);
    return *this;
  };

  /// Default trivial destructor
  ~Logger() override = default;

  /*!
   * @brief Implementation of the swap operation.
   *
   * @param other Logger object to swap with.
   */
  void swap(Logger &other) { std::swap(logger_, other.logger_); }

  /*!
   * @brief Get this logger's name.
   *
   * @return the logger name.
   */
  const std::string &Name() const { return logger_->name(); }

  /*!
   * @brief Get this logger's id.
   *
   * @return the logger id.
   * @see Id
   */
  logging::Id Id() const { return id_; }

  /*!
   * @brief Set the logging level for this logger (e.g. debug, warning...).
   *
   * @param [in] level logging level.
   */
  void Level(spdlog::level::level_enum level) { logger_->set_level(level); }

  spdlog::level::level_enum Level() const { return logger_->level(); }

  /// Default format for all loggers.
  /// @see https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
  static const char *DEFAULT_LOG_FORMAT;

 private:
  /*!
   * @brief Create a Logger with the given name and make it use the given sink
   * for its log messages.
   *
   * Logger objects cannot be created directly. Instead, use the Registry class
   * to obtain a Logger for a specific ID.
   *
   * In spdlog, loggers get assigned a sink or several sinks only at creation
   * and have to continue using that sink for the rest of their lifetime.
   * The Registry class offer an extension feature that allows to dynamically
   * switch the current sink. See Registry::PushSink() and Registry::PopSink().
   *
   * @param [in] name the logger name.
   * @param [in] sink the sink to be used by this logger.
   *
   * @see Registry::GetLogger(Id)
   * @see Id
   */
  Logger(std::string name, logging::Id id, spdlog::sink_ptr sink);

  /// The logger id
  logging::Id id_;
  /// The underlying spdlog::logger instance.
  std::shared_ptr<spdlog::logger> logger_;
  /// Synchronization lock used to synchronize logging over this logger from
  /// multiple threads.
  std::unique_ptr<std::mutex> logger_mutex_ = std::make_unique<std::mutex>();

  /// Logger objects are created only by the Registry class.
  friend class Registry;
};

// ---------------------------------------------------------------------------
// DelegatingSink
// ---------------------------------------------------------------------------

/*!
 * @brief A logging sink implementation that delegates all its logging calls
 * to an encapsulated delegate.
 *
 * This class is used to work around the limitation of spdlog that forces the
 * same sink(s) to be used for the lifetime of a logger. Two important
 * application scenarios require the sink to be changed after the logger object
 * is created:
 *   - If the application starts logging early to console and then later needs
 *     to log to some different sink after the proper resources for that sink
 *     have been initialized (e.g. GUI),
 *   - If the application needs to temporarily switch logging output to a
 *     different sink (e.g. dumping diagnostic data) and then switch back to
 *     original sink after it's done. This requires two loggers in the current
 *     implementation.
 *
 * The DelegatingSink class supports switching its delegate at any time.
 */
class DelegatingSink : public spdlog::sinks::base_sink<std::mutex>,
                       private NonCopiable {
 public:
  /*!
   * @brief Create a DelegatingSink which delegates all its logging to the given
   * delegate sink.
   *
   * @param [in] delegate the sink to which logging calls will be delegated.
   */
  explicit DelegatingSink(spdlog::sink_ptr delegate)
      : sink_delegate_(std::move(delegate)) {}

  /// Move constructor
  DelegatingSink(DelegatingSink &&) = delete;

  /// Move assignment
  DelegatingSink &operator=(DelegatingSink &&) = delete;

  /// Default trivial destructor
  DelegatingSink() = default;

  /*!
   * @brief Use the given sink as a new delegate and return the old one.
   *
   * @param sink the new delegate.
   * @return the previously used delegate.
   */
  spdlog::sink_ptr SwapSink(spdlog::sink_ptr sink) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto tmp = sink_delegate_;
    sink_delegate_ = std::move(sink);
    return tmp;
  }

 protected:
  /// @name base_sink interface
  //@{
  /*!
   * @brief Process the given log message.
   *
   * @param msg log message to be processed.
   */
  void _sink_it(const spdlog::details::log_msg &msg) override {
    sink_delegate_->log(msg);
  }

  /// Called when this sink needs to flush any buffered log messages.
  void _flush() override { sink_delegate_->flush(); }
  //@}

 private:
  /// The deleagte sink.
  spdlog::sink_ptr sink_delegate_;
};

// ---------------------------------------------------------------------------
// Registry
// ---------------------------------------------------------------------------

/*!
 * @brief A registry of all named loggers and the single point of access to the
 * logging API.
 *
 * The logging registry creates and manages all the named loggers in the
 * application. It can be used to:
 *   - obtain any registered logger by its ID,
 *   - set logging level for all registered loggers,
 *   - change the logging format,
 *   - manage a stack of sinks where the current sink can be temporarily
 *     swapped with another sink, to be restored later
 *
 * The Registry creates a default sink at startup to be used by all registered
 * loggers, until an explicit call to PushSink() is made. The default sink is
 * a console logger (color).
 *
 * Example:
 * ```
 * {
 *   auto &logger =
 *     asap::logging::Registry::GetLogger(asap::logging::Id::TESTING);
 *   ASLOG_TO_LOGGER(debug, "starting...");
 *
 *   // Initialize a complex GUI system
 *   ...
 *
 *   // Start logging to the graphical console
 *   auto ui_console = std::make_shared<MyCustomSink>();
 *   asap::logging::Registry::PushSink(ui_console);
 *
 *   ...
 *
 *   // Shutdown the GUI, switch back to the primitive logging sink
 *   asap::logging::Registry::PopSink()
 * ```
 *
 * @todo TODO: Add Init() method with format, sinks and level
 */
class Registry {
 public:
  /*!
   * @brief Sets the minimum log severity required to print messages.
   *
   * Messages below this loglevel will be suppressed.
   *
   * @param [in] log_level the logging severity to be applied to all registered
   * loggers.
   */
  static void SetLogLevel(spdlog::level::level_enum log_level);

  /*!
   * @brief Change the format string used by the registered loggers.
   *
   * @param [in] log_format new format string.
   *
   * @see https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
   */
  static void SetLogFormat(const std::string &log_format);

  /*!
   * Get a registered logger by ID.
   *
   * @param [in] id the unique identifier of the logger to fetch. Logger ids are
   * defined once as part of the Id enum class.
   *
   * @return The logger corresponding to the given id. It is always guaranteed
   * to succeed as all loggers are known at compile time and are created as soon
   * as any attempt is made to use the registry.
   */
  static spdlog::logger &GetLogger(Id id);

  /// API access to the collection of registered loggers.
  static std::vector<Logger> &Loggers();

  /*!
   * @brief Use the given sink for all subsequent logging operations until a
   * call to PopSink() is made.
   *
   * This method, together with PopSink(), enable to switch sinks at runtime
   * temporarily or permanently for all registered loggers.
   *
   * @param [in] sink new sink to replace the existing one. The existing one is
   * pushed on top of an internally managed stack of sinks.
   *
   * @see PopSink()
   */
  static void PushSink(spdlog::sink_ptr sink);

  /*!
   * @brief Restore the top of sinks stack as the current sink for all
   * registered loggers.
   *
   * @see PushSink(spdlog::sink_ptr)
   */
  static void PopSink();

 private:
  // The following methods all use a simple pattern to implement static data
  // members for this singleton class. An implementation detail method does the
  // expensive initialization of the static data member. That detail method is
  // never used in the rest of the class except from the second method which
  // provides the API to access the static data member. That second method also
  // caches the static member to optimize the call.

  /// Internal initialization of the static collection of loggers.
  static std::vector<Logger> &all_loggers_();
  /// A synchronization object for concurrent access to the collection of
  /// loggers.
  static std::recursive_mutex loggers_mutex_;

  /// API access to the stack of sinks. We don't do any expensive initialization
  /// here, so no need for a second level of access.
  static std::stack<spdlog::sink_ptr> &Sinks();
  /// A sunchronization object for concurrent access to the collection of sinks.
  static std::mutex sinks_mutex_;

  /// API access to the delegating sink.
  static std::shared_ptr<DelegatingSink> &delegating_sink();
  /// Internal initialization of the static delegating sink.
  static DelegatingSink *delegating_sink_();
};

// ---------------------------------------------------------------------------
// Loggable
// ---------------------------------------------------------------------------

/*!
 * @brief Mixin class that allows any class to peform logging with a logger of a
 * particular ID.
 */
template <Id ID>
class Loggable {
 protected:
  /*!
   * @brief Do not use this directly, use macros defined below.
   * @return spdlog::logger& the static log instance to use for class local
   * logging.
   */
  static spdlog::logger &__log_do_not_use_read_comment() {
    static spdlog::logger &instance = Registry::GetLogger(ID);
    return instance;
  }
};

// Convert the line macro to a string literal for concatenation in log macros.
#define DO_STRINGIZE(x) STRINGIZE(x)
#define STRINGIZE(x) #x
#define LINE_STRING DO_STRINGIZE(__LINE__)
#ifndef NDEBUG
std::string FormatFileAndLine(char const *file, char const *line);
//#define LOG_PREFIX "[" __FILE__ ":" LINE_STRING "] "
//#define LOG_PREFIX " "
#define LOG_PREFIX asap::logging::FormatFileAndLine(__FILE__, LINE_STRING)
#else
#define LOG_PREFIX " "
#endif  // NDEBUG

// ---------------------------------------------------------------------------
// Helper macros
// ---------------------------------------------------------------------------

/// @name Logging macros
//@{
/*!
 * Base logging macros. It is expected that users will use the
 * convenience macros below rather than invoke these directly.
 */

#define ASLOG_COMP_LEVEL(LOGGER, LEVEL)    \
  (static_cast<spdlog::level::level_enum>( \
       asap::logging::Logger::Level::LEVEL) >= LOGGER.level())

// Compare levels before invoking logger. This is an optimization to avoid
// executing expressions computing log contents when they would be suppressed.
// The same filtering will also occur in spdlog::logger.

#define _SELECT(PREFIX, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, \
                _1, SUFFIX, ...)                                            \
  PREFIX##_##SUFFIX
#define _SELECT_IMPL(args) _SELECT args
#define AS_DO_LOG(...)                                                       \
  _SELECT_IMPL((_ASLOG, __VA_ARGS__, N, N, N, N, N, N, N, N, N, N, 3, 2, 1)) \
  (__VA_ARGS__)

#define _ASLOG_1(LOGGER) LOGGER.debug("no logger level - no message")
#define _ASLOG_2(LOGGER, LEVEL) LOGGER.LEVEL("no message")
#define _ASLOG_3(LOGGER, LEVEL, MSG) LOGGER.LEVEL("{}" MSG, LOG_PREFIX)
#define _ASLOG_N(LOGGER, LEVEL, MSG, ...) \
  LOGGER.LEVEL("{}" MSG, LOG_PREFIX, __VA_ARGS__)

#ifndef NDEBUG
#define ASLOG_COMP_AND_LOG(LOGGER, LEVEL, ...) \
  do {                                         \
    if (ASLOG_COMP_LEVEL(LOGGER, LEVEL)) {     \
      AS_DO_LOG(LOGGER, LEVEL, __VA_ARGS__);   \
    }                                          \
  } while (0)
#else  // NDEBUG
#define ASLOG_COMP_AND_LOG(LOGGER, LEVEL, ...) \
  do {                                         \
    if (ASLOG_COMP_LEVEL(LOGGER, LEVEL)) {     \
      LOGGER.LEVEL(__VA_ARGS__);               \
    }                                          \
  } while (0)
#endif  // NDEBUG

#define ASLOG_CHECK_LEVEL(LEVEL) ASLOG_COMP_LEVEL(ASLOGGER(), LEVEL)

/**
 * Convenience macro to log to a user-specified logger.
 * Maps directly to ASLOG_COMP_AND_LOG - it could contain macro logic itself,
 * without redirection, but left in case various implementations are required in
 * the future (based on log level for example).
 */
#define ASLOG_TO_LOGGER(LOGGER, LEVEL, ...) \
  ASLOG_COMP_AND_LOG(LOGGER, LEVEL, __VA_ARGS__)

/**
 * Convenience macro to get logger.
 */
#define ASLOGGER() __log_do_not_use_read_comment()

/**
 * Convenience macro to flush logger.
 */
#define ASFLUSH_LOG() ASLOGGER().flush()

/**
 * Convenience macro to log to the class' logger.
 */
#define ASLOG(LEVEL, ...) ASLOG_TO_LOGGER(ASLOGGER(), LEVEL, __VA_ARGS__)

/**
 * Convenience macro to log to the misc logger, which allows for logging without
 * of direct access to a logger.
 */
#define GET_MISC_LOGGER() \
  asap::logging::Registry::GetLogger(asap::logging::Id::MISC)
#define ASLOG_MISC(LEVEL, ...) \
  ASLOG_TO_LOGGER(GET_MISC_LOGGER(), LEVEL, __VA_ARGS__)

//@}

}  // namespace logging
}  // namespace asap
