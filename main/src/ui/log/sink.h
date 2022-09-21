/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <shared_mutex> // for locking the records vector
#include <vector>       // for the records vector

#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>

#include <imgui/imgui.h>
#include <logging/logging.h>

namespace asap::ui {

class ImGuiLogSink : public spdlog::sinks::base_sink<std::mutex>,
                     asap::logging::Loggable<ImGuiLogSink> {
public:
  void Clear();

  static void ShowLogLevelsPopup();

  void ShowLogFormatPopup();

  void ToggleWrap() {
    wrap_ = !wrap_;
  }

  void ToggleScrollLock() {
    scroll_lock_ = !scroll_lock_;
  }

  // TODO(Abdessattar) refactor this ugly interface to not use pointer for open
  void Draw(const char *title = nullptr, bool *p_open = nullptr);

  void LoadSettings();
  void SaveSettings();

  static const char *const LOGGER_NAME;

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override;

  void flush_() override;

private:
  static const ImVec4 COLOR_WARN;
  static const ImVec4 COLOR_ERROR;

  struct LogRecord {
    std::string properties_;
    std::string source_;
    std::string message_;
    std::size_t color_range_start_{0};
    std::size_t color_range_end_{0};
    const ImVec4 &color_;
    bool emphasis_{false};
  };
  std::vector<LogRecord> records_;
  mutable std::shared_timed_mutex records_mutex_;
  ImGuiTextFilter display_filter_;

  bool scroll_to_bottom_;
  bool wrap_{false};
  bool scroll_lock_{false};

  /// @name Log Format flags
  //@{
  bool show_time_{true};
  bool show_thread_{true};
  bool show_level_{true};
  bool show_logger_{true};
  //@}
};

} // namespace asap::ui
