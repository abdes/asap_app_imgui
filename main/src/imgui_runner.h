//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <runner_base.h>

namespace boost {
namespace asio {
class io_context;
class signal_set;
}  // namespace asio
}  // namespace boost

struct GLFWwindow;
struct GLFWmonitor;

namespace asap {

class ImGuiRunner : public RunnerBase {
 public:
  ImGuiRunner(AbstractApplication &app, shutdown_function_type func);

  ~ImGuiRunner() override;
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
  bool IsFullScreenWindowed() const { return windowed_ && full_screen_; };
  GLFWmonitor *GetMonitor() const;
  int GetMonitorId() const;
  int RefreshRate() const;
  void GetWindowSize(int size[2]) const;
  void GetWindowPosition(int position[2]) const;

  bool Vsync() const { return vsync_; };
  int MultiSample() const { return samples_; }


 private:
  void SetupSignalHandler();
  void InitGraphics();
  void SetupContext();
  void InitImGui();
  void CleanUp();

  GLFWwindow *window{nullptr};

  boost::asio::io_context *io_context_;
  /// The signal_set is used to register for process termination notifications.
  boost::asio::signal_set *signals_;

  std::string window_title_;
  bool full_screen_{false};
  bool windowed_{false};

  bool vsync_{true};
  int samples_{-1};

  mutable int saved_position_[2]{-1, -1};
};

}  // namespace asap
