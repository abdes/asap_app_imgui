//    Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <common/logging.h>

#include <abstract_application.h>
#include <ui/log/sink.h>

namespace asap {
namespace ui {


class ApplicationBase : public AbstractApplication,
                        asap::NonCopiable,
                        protected asap::logging::Loggable<ApplicationBase> {
 public:
  ApplicationBase() = default;

  /// Not move constructible
  ApplicationBase(ApplicationBase &&) = delete;
  /// Not move assignable
  ApplicationBase &operator=(ApplicationBase &&) = delete;

  ~ApplicationBase() override = default;

  void Init(ImGuiRunner *runner) final;
  bool Draw() override;
  void ShutDown() final;

  static const char * LOGGER_NAME;

 protected:
  virtual void AfterInit() {}
  virtual void DrawInsideMainMenu() {}
  virtual void DrawInsideStatusBar(float /*width*/, float /*height*/) {}
  virtual void BeforeShutDown() {}

 private:
  float DrawMainMenu();
  void DrawStatusBar(float width, float height, float pos_x, float pos_y);
  void DrawLogView();
  void DrawSettings();
  void DrawDocksDebug();
  void DrawImGuiMetrics();
  void DrawImGuiDemos();

 private:
  bool show_docks_debug_{false};
  bool show_logs_{true};
  bool show_settings_{true};
  bool show_imgui_metrics_{false};
  bool show_imgui_demos_{false};

  std::shared_ptr<ImGuiLogSink> sink_;
  ImGuiRunner *runner_ = nullptr; // TODO: convert to weak_ptr?
};

}  // namespace ui
}  // namespace asap
