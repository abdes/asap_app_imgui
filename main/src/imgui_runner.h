//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <runner_base.h>

struct GLFWwindow;
struct GLFWmonitor;

namespace asap {

class ImGuiRunner : public RunnerBase {
 public:
  ImGuiRunner(AbstractApplication &app, shutdown_function_type func);

  ~ImGuiRunner() override = default;
  ImGuiRunner(const ImGuiRunner &) = delete;
  ImGuiRunner &operator=(const ImGuiRunner &) = delete;

  void LoadSetting();
  void SaveSetting();

  void Windowed(int width, int height, char const *title);
  void FullScreenWindowed(char const *title, int monitor);
  void FullScreen(int width, int height, char const *title, int monitor,
                  int refresh_rate);

  void EnableVsync(bool state = true);
  void MultiSample(int samples);
  void SetWindowTitle(char const *title);

  void Run() override;

  std::string const &GetWindowTitle() const;
  bool IsFullScreen() const { return full_screen_; };
  bool IsWindowed() const { return windowed_; };
  GLFWmonitor *GetMonitor() const;
  int GetMonitorId() const;
  int RefreshRate() const;
  void GetWindowSize(int size[2]) const;
  void GetWindowPosition(int position[2]) const;

  bool Vsync() const { return vsync_; };
  int MultiSample() const { return samples_; }

 private:
  void InitGraphics();
  void SetupContext();
  void InitImGui();
  void CleanUp();

  GLFWwindow *window_{nullptr};

  std::string window_title_;
  bool full_screen_{false};
  bool windowed_{false};

  bool vsync_{true};
  int samples_{-1};

  mutable int saved_position_[2]{-1, -1};
};

}  // namespace asap
