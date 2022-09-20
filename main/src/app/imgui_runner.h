/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include "app/application.h"
#include <logging/logging.h>

#include <functional> // for std::function
#include <utility>

struct GLFWwindow;
struct GLFWmonitor;

namespace asap::app {

class Application;

class ImGuiRunner : public asap::logging::Loggable<ImGuiRunner> {
public:
  using shutdown_function_type = std::function<void()>;

  static const char *const LOGGER_NAME;

  ImGuiRunner(Application &app, shutdown_function_type func);
  ~ImGuiRunner() = default;

  ImGuiRunner(const ImGuiRunner &) = delete;

  ImGuiRunner(const ImGuiRunner &&) = delete;

  auto operator=(const ImGuiRunner &) -> ImGuiRunner & = delete;

  auto operator=(const ImGuiRunner &&) -> ImGuiRunner & = delete;

  void LoadSetting();
  void SaveSetting() const;

  void Windowed(int width, int height, const std::string &title);
  void FullScreenWindowed(const std::string &title, int monitor);
  void FullScreen(int width, int height, const std::string &title, int monitor,
      int refresh_rate);

  void EnableVsync(bool state = true);
  void MultiSample(int samples);
  void SetWindowTitle(const std::string &title);

  void Run();

  [[nodiscard]] auto GetWindowTitle() const -> std::string const &;
  [[nodiscard]] auto IsFullScreen() const -> bool {
    return full_screen_;
  }
  [[nodiscard]] auto IsWindowed() const -> bool {
    return windowed_;
  }
  [[nodiscard]] auto GetMonitor() const -> GLFWmonitor *;
  [[nodiscard]] auto GetMonitorId() const -> int;
  [[nodiscard]] auto RefreshRate() const -> int;
  [[nodiscard]] auto GetWindowSize() const -> std::pair<int, int>;
  [[nodiscard]] auto GetWindowPosition() const -> std::pair<int, int>;

  [[nodiscard]] auto Vsync() const -> bool {
    return vsync_;
  }
  [[nodiscard]] auto MultiSample() const -> int {
    return samples_;
  }

private:
  static void InitGraphics();
  void SetupContext();
  void InitImGui();
  void CleanUp();

  GLFWwindow *window_{nullptr};

  std::string window_title_;
  bool full_screen_{false};
  bool windowed_{false};

  bool vsync_{true};
  int samples_{-1};

  std::pair<int, int> saved_position_{-1, -1};

  Application &app_;
  shutdown_function_type shutdown_function_;
};

} // namespace asap::app
