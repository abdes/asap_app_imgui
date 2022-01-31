//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

/*!
 * \file
 *
 * \brief Implementation details of the text wrapper tokenizer.
 */

#include "tokenizer.h"

#include <common/compilers.h>
#include <common/overload.h>
#include <contract/contract.h>
#include <fsm/fsm.h>
#include <logging/logging.h>

// fmt causes a bunch of compiler warnings we can't do anything about except
// temporarily disabling them
ASAP_DIAGNOSTIC_PUSH
#if defined(__clang__)
ASAP_PRAGMA(clang diagnostic ignored "-Weverything")
#endif
#include <fmt/format.h>
ASAP_DIAGNOSTIC_POP

#include <magic_enum.hpp>

#include <algorithm>
#include <numeric>
#include <utility>

using asap::fsm::ByDefault;
using asap::fsm::Continue;
using asap::fsm::DoNothing;
using asap::fsm::On;
using asap::fsm::ReissueEvent;
using asap::fsm::StateMachine;
using asap::fsm::Status;
using asap::fsm::Terminate;
using asap::fsm::TerminateWithError;
using asap::fsm::TransitionTo;
using asap::fsm::Will;

using asap::wrap::detail::Token;
using asap::wrap::detail::TokenConsumer;
using asap::wrap::detail::TokenType;

// -----------------------------------------------------------------------------
//  TokenType formatting helpers
// -----------------------------------------------------------------------------

auto asap::wrap::detail::operator<<(
    std::ostream &out, const TokenType &token_type) -> std::ostream & {
  out << magic_enum::enum_name(token_type);
  return out;
}

#if !defined(DOXYGEN_DOCUMENTATION_BUILD)
template <> struct fmt::formatter<asap::wrap::detail::TokenType> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(
      const asap::wrap::detail::TokenType &token_type, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(token_type));
  }
};
#endif

// -----------------------------------------------------------------------------
//  Tokenizer state machine events
// -----------------------------------------------------------------------------

namespace {

struct NonWhiteSpaceChar {
  explicit NonWhiteSpaceChar(char character) : value{character} {
  }
  const char value;
};
struct WhiteSpaceChar {
  explicit WhiteSpaceChar(char character) : value{character} {
  }
  const char value;
};
struct InputEnd {};

} // namespace

// -----------------------------------------------------------------------------
//  Formatting helpers for the Tokenizer events
// -----------------------------------------------------------------------------

/*!
 * \brief Make a white space character safe for printing by adding a '\' if
 * needed.
 *
 * This function takes any character and if it is a special white space
 * character that needs escaping it transforms it into a safe string for
 * printing by adding a `\` in front of the character.
 *
 * Characters that get escaped are: `\n`, `\r`, `\f`, `\v`, `\t`.
 */
inline auto EscapeSpecialWhiteSpaces(const char &character) -> std::string {
  // Escape special white space characters
  std::string escaped;
  switch (character) {
  case '\n':
    escaped = "\\n";
    break;
  case '\f':
    escaped = "\\f";
    break;
  case '\r':
    escaped = "\\r";
    break;
  case '\t':
    escaped = "\\t";
    break;
  case '\v':
    escaped = "\\v";
    break;
  default:
    escaped = character;
  }
  return escaped;
}

/*!
 * \brief Make a string safe for printing by escaping any special white space
 * characters in it.
 *
 * \see EscapeSpecialWhiteSpaces
 */
inline auto EscapeSpecialWhiteSpaces(const std::string &str) -> std::string {
  return std::accumulate(
      str.cbegin(), str.cend(), std::string(), [](auto &dest, char value) {
        dest.append(EscapeSpecialWhiteSpaces(value));
        return dest;
      });
}

#if !defined(DOXYGEN_DOCUMENTATION_BUILD)
template <> struct fmt::formatter<NonWhiteSpaceChar> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const NonWhiteSpaceChar &event, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "NonWhiteSpaceChar('{}')", event.value);
  }
};

template <> struct fmt::formatter<WhiteSpaceChar> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const WhiteSpaceChar &event, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "WhiteSpaceChar('{}')",
        EscapeSpecialWhiteSpaces(event.value));
  }
};

template <> struct fmt::formatter<InputEnd> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const InputEnd & /*event*/, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "InputEnd");
  }
};
#endif

// -----------------------------------------------------------------------------
//  Tokenizer state machine states
// -----------------------------------------------------------------------------

namespace {

struct InitialState;
struct WordState;
struct WhiteSpaceState;
struct FinalState;

/*!
 * \brief Pass a token to a token consumer.
 *
 * This utility function passes the token to the token consumer and clears the
 * token before returning.
 */
void DispatchTokenToConsumer(const TokenConsumer &consume_token,
    TokenType token_type, std::string &token, spdlog::logger &logger) {
  ASLOG_TO_LOGGER(logger, info, "== new token: {} / \"{}\"", TokenType::Chunk,
      EscapeSpecialWhiteSpaces(token));
  consume_token(token_type, token);
  token.clear();
}

/*!
 * \brief The final state of the tokenizer state machine.
 *
 * When the state machine enters this state, a final token event of type
 * TokenType::EndOfInput will be dispatched to the token consumer. After that,
 * the state machine is a completed execution state and no more tokens will be
 * produced. The tokenizer can be reused for a new input text.
 */
struct FinalState : public Will<ByDefault<DoNothing>> {
  FinalState(spdlog::logger &logger, TokenConsumer callback)
      : logger_(logger), consume_token_{std::move(callback)} {
  }

  auto OnEnter(const InputEnd & /*event*/) -> Status {
    ASLOG_TO_LOGGER(logger_, debug, "enter(FinalState) with InputEnd");
    consume_token_(TokenType::EndOfInput, "");
    return Terminate{};
  }

private:
  spdlog::logger &logger_;
  TokenConsumer consume_token_;
};

/*!
 * \brief The initial state of the tokenizer state machine.
 *
 * This is the starting state of the tokenizer. Based on which first token will
 * be extracted from the input text, this state will transition to one of the
 * next states.
 */
struct InitialState
    : public Will<On<NonWhiteSpaceChar, TransitionTo<WordState>>,
          On<WhiteSpaceChar, TransitionTo<WhiteSpaceState>>,
          On<InputEnd, TransitionTo<FinalState>>> {
  using Will::Handle;

  explicit InitialState(spdlog::logger &logger) : logger_(logger) {
  }

  template <typename Event> auto OnLeave(const Event &event) -> Status {
    ASLOG_TO_LOGGER(logger_, debug, "leave(InitialState) with {}", event);
    return Continue{};
  }

private:
  spdlog::logger &logger_;
};

/*!
 * \brief Tokenizer state while forming a chunk of non-white-space text.
 *
 * In this state, the tokenizer accumulates input characters into a chunk until
 * it encounters a white space character, a hyphen when break_on_hyphens is
 * `true` or the end of input. At that moment, it dispatches the chunk as a
 * token to the token consumer and transitions to the next state corresponding
 * to the last event.
 */
struct WordState : public Will<On<InputEnd, TransitionTo<FinalState>>,
                       On<WhiteSpaceChar, TransitionTo<WhiteSpaceState>>> {
  using Will::Handle;

  WordState(
      spdlog::logger &logger, TokenConsumer callback, bool break_on_hyphens)
      : logger_(logger), consume_token_{std::move(callback)},
        break_on_hyphens_{break_on_hyphens} {
  }

  auto OnEnter(const NonWhiteSpaceChar &event) -> Status {
    ASLOG_TO_LOGGER(logger_, debug, "enter(WordState) with {}", event);
    return ReissueEvent{};
  }

  template <typename Event> auto OnLeave(const Event &event) -> Status {
    if (!token_.empty()) {
      DispatchTokenToConsumer(
          consume_token_, TokenType::Chunk, token_, logger_);
    }
    ASLOG_TO_LOGGER(logger_, debug, "leave(WordState) with {}", event);
    return Continue{};
  }

  auto Handle(const NonWhiteSpaceChar &event) -> DoNothing {
    ASLOG_TO_LOGGER(logger_, debug, "WordState.Handle {}", event);
    if (break_on_hyphens_ && event.value == '-' && !token_.empty() &&
        (std::isalpha(token_.back()) != 0)) {
      ASLOG_TO_LOGGER(logger_, debug, "found '-' and must break on hyphens");
      token_.push_back(event.value);
      DispatchTokenToConsumer(
          consume_token_, TokenType::Chunk, token_, logger_);
    } else {
      token_.push_back(event.value);
    }
    return DoNothing{};
  }

private:
  spdlog::logger &logger_;
  std::string token_;
  TokenConsumer consume_token_;
  const bool break_on_hyphens_;
};

/*!
 * \brief Tokenizer state while getting white-space as input characters.
 *
 * In this state, the tokenizer is waiting for a non-white-space to start
 * forming a new chunk or for the end of input to terminate. Any white space
 * characters are collected into a temporary token to be transformed as per the
 * configured parameters (see Tokenizer class description for a detailed
 * description of these parameters). The white space token is then dispatched to
 * the token consumer and the state machine transitions into the next state
 * based on the last event.
 */
struct WhiteSpaceState : public Will<On<InputEnd, TransitionTo<FinalState>>,
                             On<NonWhiteSpaceChar, TransitionTo<WordState>>> {
  using Will::Handle;

  explicit WhiteSpaceState(spdlog::logger &logger, TokenConsumer callback,
      const std::string &tab, bool replace_ws, bool collapse_ws)
      : logger_(logger), consume_token_{std::move(callback)}, tab_{tab},
        replace_ws_{replace_ws}, collapse_ws_{collapse_ws} {
  }

  auto OnEnter(const WhiteSpaceChar &event) -> Status {
    ASLOG_TO_LOGGER(logger_, debug, "enter(WhiteSpaceState) with {}", event);
    return ReissueEvent{};
  }

  template <typename Event> auto OnLeave(const Event &event) -> Status {
    if (!token_.empty()) {
      DispatchToConsumer(TokenType::WhiteSpace);
    } else {
      // We're leaving after a paragraph mark
      ASLOG_TO_LOGGER(
          logger_, debug, "leave(WhiteSpaceState) with {} -> no token", event);
    }
    last_was_newline_ = false;
    return Continue{};
  }

  auto Handle(const WhiteSpaceChar &event) -> DoNothing {
    ASLOG_TO_LOGGER(logger_, debug, "WhiteSpaceState.Handle {}", event);
    if (event.value == '\t') {
      if (tab_ == "\t" && replace_ws_) {
        token_.push_back(' ');
      } else {
        token_.append(tab_);
      }
      return DoNothing{};
    }
    if (event.value == '\n') {
      if (last_was_newline_) {
        ASLOG_TO_LOGGER(
            logger_, debug, "WhiteSpaceState.Handle - new paragraph");
        if (!token_.empty()) { // cases with 3+ successive `\n`
          token_.pop_back();
        }
        if (!token_.empty()) {
          DispatchToConsumer(TokenType::WhiteSpace);
        }
        token_ = "";
        DispatchToConsumer(TokenType::ParagraphMark);
        return DoNothing{};
      }
      last_was_newline_ = true;
    }
    token_.push_back(event.value);
    return DoNothing{};
  }

private:
  void DispatchToConsumer(TokenType token_type) {
    // If the token is a white space, and we need to collapse white spaces, do
    // it now.
    if (token_type == TokenType::WhiteSpace) {
      if (collapse_ws_) {
        token_ = " ";
      } else if (replace_ws_) {
        // Tab expansion may produce non-white-space characters, that are still
        // par of a white space token. When replace white space, we still stick
        // to the pure definition of white space characters and only replace
        // those.
        std::transform(token_.begin(), token_.end(), token_.begin(),
            [](char a_char) -> char {
              return (std::isspace(a_char) != 0) ? ' ' : a_char;
            });
      }
      DispatchTokenToConsumer(
          consume_token_, TokenType::WhiteSpace, token_, logger_);
    } else {
      DispatchTokenToConsumer(
          consume_token_, TokenType::ParagraphMark, token_, logger_);
    }
  }

  spdlog::logger &logger_;
  bool last_was_newline_{false};
  std::string token_;
  TokenConsumer consume_token_;
  const std::string &tab_;
  const bool replace_ws_;
  const bool collapse_ws_;
};

} // namespace

auto asap::wrap::detail::Tokenizer::Tokenize(
    const std::string &text, const TokenConsumer &consume_token) const -> bool {
  auto &logger =
      asap::logging::Registry::instance().GetLogger("TextWrapper-Tokenizer");

  using Machine =
      StateMachine<InitialState, WordState, WhiteSpaceState, FinalState>;
  Machine machine{InitialState(logger),
      WordState(logger, consume_token, break_on_hyphens_),
      WhiteSpaceState(logger, consume_token, tab_, replace_ws_, collapse_ws_),
      FinalState(logger, consume_token)};

  bool continue_running{true};
  bool no_errors{true};
  bool reissue = false;

  auto cursor = text.begin();
  while (cursor != text.end()) {
    Status execution_status;

    if (std::isspace(*cursor) != 0) {
      execution_status = machine.Handle(WhiteSpaceChar{*cursor});
    } else {
      execution_status = machine.Handle(NonWhiteSpaceChar{*cursor});
    }
    // https://en.cppreference.com/w/cpp/utility/variant/visit
    std::visit(Overload{
                   [&continue_running](const Continue & /*status*/) noexcept {
                     continue_running = true;
                   },
                   [&continue_running](const Terminate & /*status*/) noexcept {
                     continue_running = false;
                   },
                   [&continue_running, &no_errors, &logger](
                       const TerminateWithError &status) noexcept {
                     ASLOG_TO_LOGGER(logger, error, "{}", status.error_message);
                     continue_running = false;
                     no_errors = false;
                   },
                   [&continue_running, &reissue](
                       const ReissueEvent & /*status*/) noexcept {
                     reissue = true;
                     continue_running = true;
                   },
               },
        execution_status);
    if (continue_running) {
      if (reissue) {
        reissue = false;
        // reuse the same token again
        ASLOG_TO_LOGGER(logger, debug, "re-issuing event as requested");
      } else {
        cursor++;
      }
    }
  }
  machine.Handle(InputEnd{});

  return no_errors;
}
