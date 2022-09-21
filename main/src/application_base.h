/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include "app/application.h"
#include "ui/log/sink.h"

#include <logging/logging.h>

class ApplicationBase : public asap::app::Application,
                        protected asap::logging::Loggable<ApplicationBase> {
public:
  ApplicationBase(const ApplicationBase &) = delete;
  ApplicationBase(ApplicationBase &&) = delete;

  auto operator=(const ApplicationBase &) -> ApplicationBase & = delete;
  auto operator=(ApplicationBase &&) -> ApplicationBase & = delete;

  void Init(asap::app::ImGuiRunner *runner) final;
  void ShutDown() final;

  static const char *const LOGGER_NAME;

protected:
  ApplicationBase() = default;
  ~ApplicationBase() = default;

  virtual void AfterInit() {
  }
  virtual void DrawInsideMainMenu() {
  }
  virtual void DrawInsideStatusBar(float /*width*/, float /*height*/) {
  }
  virtual void BeforeShutDown() {
  }

  virtual auto DrawCommonElements() -> bool final;

private:
  auto DrawMainMenu() -> float;
  void DrawStatusBar(float width, float height, float pos_x, float pos_y);
  void DrawLogView();
  void DrawSettings();
  void DrawDocksDebug();
  void DrawImGuiMetrics();
  void DrawImGuiDemos();

  bool show_docks_debug_{false};
  bool show_logs_{true};
  bool show_settings_{true};
  bool show_imgui_metrics_{false};
  bool show_imgui_demos_{false};

  std::shared_ptr<asap::ui::ImGuiLogSink> sink_;
  asap::app::ImGuiRunner *runner_ =
      nullptr; // TODO(Abdessattar): convert to weak_ptr?
};
