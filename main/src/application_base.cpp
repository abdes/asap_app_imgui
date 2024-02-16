/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "application_base.h"

#include "app/imgui_runner.h"
#include "ui/fonts/material_design_icons.h"
#include "ui/log/sink.h"
#include "ui/style/theme.h"

#include <GLFW/glfw3.h>
#include <gsl/span>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <cmath> // for rounding frame rate
#include <sstream>

using asap::app::Application;
using asap::app::ImGuiRunner;
using asap::ui::Theme;

static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;

const char *const ApplicationBase::LOGGER_NAME = "main";

namespace {
constexpr float STATUS_BAR_HEIGHT = 16.0F;
constexpr float STATUS_BAR_FPS_WIDTH = 45.0F;
constexpr float HORIZONTAL_WINDOW_PADDING = 5.0F;
constexpr float VERTICAL_WINDOW_PADDING = 5.0F;
constexpr float TOOLBAR_HEIGHT = 30.0F;
constexpr float ICON_HEIGHT = 18.0F;
constexpr float ICON_WIDTH = 18.0F;
} // namespace

void ApplicationBase::Init(ImGuiRunner *runner) {
  runner_ = runner;
  sink_ = std::make_shared<asap::ui::ImGuiLogSink>();
  asap::logging::Registry::PushSink(sink_);

  sink_->LoadSettings();

  ASLOG(debug, "Initializing UI theme");
  Theme::Init();

  // Call for custom init operations from derived class
  AfterInit();
}

void ApplicationBase::ShutDown() {
  // Restore the original log sink
  asap::logging::Registry::PopSink();

  // Call derived class for any custom shutdown logic before we shutdown the
  // app. We do this before to stay consistent with the initialization order.
  BeforeShutDown();

  // Save configuration data:
  //  - Logging settings
  //  - Theme settings
  //  - Docks
  sink_->SaveSettings();
  Theme::SaveStyle();
}

auto ApplicationBase::DrawCommonElements() -> bool {
  static bool opt_fullscreen_persistant = true;
  ImGuiViewport *viewport = ImGui::GetMainViewport();

  if (ImGui::GetIO().DisplaySize.y > 0) {
    auto menu_height = DrawMainMenu();

    bool opt_fullscreen = opt_fullscreen_persistant;
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent
    // window not dockable into, because it would be confusing to have two
    // docking targets within each others.
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
      auto dockSpaceSize = viewport->Size;
      dockSpaceSize.y -= STATUS_BAR_HEIGHT; // remove the status bar
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(dockSpaceSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);
      window_flags |= ImGuiWindowFlags_NoTitleBar |
                      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                      ImGuiWindowFlags_NoMove;
      window_flags |=
          ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will
    // render our background and handle the pass-thru hole, so we ask Begin() to
    // not render a background.
    if ((opt_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0) {
      window_flags |= ImGuiWindowFlags_NoBackground;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen) {
      ImGui::PopStyleVar(2);
    }

    // Dockspace
    ImGuiIO &imgui_io = ImGui::GetIO();
    if ((imgui_io.ConfigFlags & ImGuiConfigFlags_DockingEnable) != 0) {
      ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0F, 0.0F), opt_flags);
    } else {
      // TODO(Abdessattar): emit a log message
    }

    //
    // Status bar
    //
    DrawStatusBar(viewport->Size.x, STATUS_BAR_HEIGHT, 0.0F,
        viewport->Size.y - menu_height);

    if (show_logs_) {
      DrawLogView();
    }
    if (show_settings_) {
      DrawSettings();
    }
    if (show_docks_debug_) {
      DrawDocksDebug();
    }
    if (show_imgui_metrics_) {
      DrawImGuiMetrics();
    }
    if (show_imgui_demos_) {
      DrawImGuiDemos();
    }
  }

  ImGui::End();

  // Return true to indicate that we are not doing any calculation and we can
  // sleep if the application window is not focused.
  return true;
}

auto ApplicationBase::DrawMainMenu() -> float {
  auto menu_height = 0.0F;

  if (ImGui::BeginMainMenuBar()) {
    // Call the derived class to draw the application specific menus inside the
    // menu bar
    DrawInsideMainMenu();

    // Last menu bar items are the ones we create as baseline (i.e. the docking
    // control menu and the debug menu)

    if (ImGui::BeginMenu("Docking")) {
      // Disabling fullscreen would allow the window to be moved to the front of
      // other windows, which we can't undo at the moment without finer window
      // depth/z control.
      // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

      if (ImGui::MenuItem("Flag: NoSplit", "",
              (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0)) {
        opt_flags ^= ImGuiDockNodeFlags_NoSplit;
      }
      if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
              (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) {
        opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
      }
      if (ImGui::MenuItem("Flag: PassthruDockspace", "",
              (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0)) {
        opt_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Debug")) {
      if (ImGui::MenuItem("Show Logs", "CTRL+SHIFT+L", &show_logs_)) {
        DrawLogView();
      }
      if (ImGui::MenuItem(
              "Show Docks Debug", "CTRL+SHIFT+D", &show_docks_debug_)) {
        DrawDocksDebug();
      }
      if (ImGui::MenuItem("Show Settings", "CTRL+SHIFT+S", &show_settings_)) {
        DrawSettings();
      }

      ImGui::Separator();

      if (ImGui::MenuItem(
              "Show ImGui Metrics", "CTRL+SHIFT+M", &show_imgui_metrics_)) {
        DrawImGuiMetrics();
      }
      if (ImGui::MenuItem(
              "Show ImGui Demos", "CTRL+SHIFT+G", &show_imgui_demos_)) {
        DrawImGuiDemos();
      }

      ImGui::EndMenu();
    }
    menu_height = ImGui::GetWindowSize().y;

    ImGui::EndMainMenuBar();
  }

  return menu_height;
}

void ApplicationBase::DrawStatusBar(
    float width, float height, float pos_x, float pos_y) {
  // Draw status bar (no docking)
  ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Always);
  ImGui::Begin("statusbar", nullptr,
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings |
          ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize);

  // Call the derived class to add stuff to the status bar
  DrawInsideStatusBar(width - STATUS_BAR_FPS_WIDTH, height);

  // Draw the common stuff
  ImGui::SameLine(width - STATUS_BAR_FPS_WIDTH);
  ImGui::Text("FPS: %ld", std::lround(ImGui::GetIO().Framerate));
  ImGui::End();
}

void ApplicationBase::DrawLogView() {
  if (ImGui::Begin("Logs", &show_logs_)) {
    // Draw the log view docked
    sink_->Draw();
  }
  ImGui::End();
}

void ApplicationBase::DrawImGuiMetrics() {
  ImGui::ShowMetricsWindow();
}

void ApplicationBase::DrawImGuiDemos() {
  ImGui::ShowDemoWindow(&show_imgui_demos_);
}

namespace {

auto DrawDisplaySettingsTitle(ImGuiRunner *runner, std::string &title) -> bool {
  auto changed = false;
  if (ImGui::InputText(
          "Window Title", &title, ImGuiInputTextFlags_EnterReturnsTrue)) {
    changed = runner->GetWindowTitle() != title;
  }
  return changed;
}

auto DrawDisplaySettingsMode(int &display_mode) -> bool {
  static const std::array<const char *, 3> mode_items{
      "Windowed", "Full Screen", "Full Screen Windowed"};
  auto changed = ImGui::Combo(
      "Display Mode", &display_mode, mode_items.data(), mode_items.size());
  return changed;
}

auto DrawDisplaySettingsMonitor(ImGuiRunner *runner, GLFWmonitor *&monitor)
    -> bool {
  auto changed = false;
  if (ImGui::BeginCombo("Monitor", glfwGetMonitorName(monitor))) {
    int count = 0;
    GLFWmonitor **glfw_monitors = glfwGetMonitors(&count);
    auto monitors = gsl::span(glfw_monitors, count);
    for (int n = 0; n < count; n++) {
      bool is_selected = (monitors[n] == runner->GetMonitor());
      if (ImGui::Selectable(glfwGetMonitorName(monitors[n]), is_selected)) {
        monitor = monitors[n];
        changed = true;
      }
      if (is_selected) {
        // Set the initial focus when opening the combo (scrolling + for
        // keyboard navigation support in the upcoming navigation branch)
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  return changed;
}

auto DrawDisplaySettingsWindowSize(
    ImGuiRunner *runner, std::array<int, 2> &size) -> bool {
  auto input_changed = ImGui::InputInt2("Size", size.data());
  auto current_pos = runner->GetWindowSize();
  auto changed = input_changed || (current_pos.first != size[0]) ||
                 (current_pos.second != size[1]);
  return changed;
}

auto DrawDisplaySettingsResolution(
    GLFWmonitor *monitor, const GLFWvidmode *&resolution) -> bool {
  auto changed = false;
  auto ostr = std::ostringstream();
  ostr << resolution->width << 'x' << resolution->height << " @ "
       << resolution->refreshRate << " Hz";
  auto current_mode = ostr.str();
  if (ImGui::BeginCombo("Resolution", ostr.str().c_str())) {
    int count = 0;
    const GLFWvidmode *glfw_video_modes = glfwGetVideoModes(monitor, &count);
    auto const video_modes = gsl::span(glfw_video_modes, count);
    for (int n = 0; n < count; n++) {
      ostr = std::ostringstream();
      ostr << video_modes[n].width << 'x' << video_modes[n].height << " @ "
           << video_modes[n].refreshRate << " Hz";
      auto mode = ostr.str();
      bool is_selected = (mode == current_mode);
      if (ImGui::Selectable(mode.c_str(), is_selected)) {
        resolution = &video_modes[n];
        changed = true;
      }
      if (is_selected) {
        // Set the initial focus when opening the combo (scrolling + for
        // keyboard navigation support in the upcoming navigation branch)
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  return changed;
}

auto GetMonitorIndex(GLFWmonitor *monitor) -> int {
  int count = 0;
  GLFWmonitor **glfw_monitors = glfwGetMonitors(&count);
  auto monitors = gsl::span(glfw_monitors, count);
  for (int n = 0; n < count; n++) {
    if (monitors[n] == monitor) {
      return n;
    }
  }
  return 0;
}

void ShowDisplaySettings(ImGuiRunner *runner) {
  static std::string title;
  static int display_mode = 0;
  static GLFWmonitor *monitor = nullptr; // NOLINT
  static std::array<int, 2> size{0, 0};
  static const GLFWvidmode *resolution = nullptr;
  static int samples = 0;
  static bool vsync = false;

  static bool pending_changes = false;

  static bool reset_to_current = true;
  if (reset_to_current) {
    reset_to_current = false;

    title = runner->GetWindowTitle();

    if (runner->IsWindowed() && runner->IsFullScreen()) {
      display_mode = 2;
    } else if (runner->IsFullScreen()) {
      display_mode = 1;
    } else {
      display_mode = 0;
    }

    auto current_size = runner->GetWindowSize();
    size[0] = current_size.first;
    size[1] = current_size.second;

    monitor = runner->GetMonitor();
    if (monitor == nullptr) {
      monitor = glfwGetPrimaryMonitor();
    }

    resolution = glfwGetVideoMode(monitor);

    vsync = runner->Vsync();

    samples = runner->MultiSample();

    pending_changes = false;
  }

  // Toolbar
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
      {HORIZONTAL_WINDOW_PADDING, VERTICAL_WINDOW_PADDING});
  ImGui::PushStyleColor(ImGuiCol_ChildBg,
      (pending_changes) ? ImGui::GetStyleColorVec4(ImGuiCol_PlotLinesHovered)
                        : ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg));

  ImGui::BeginChild("Display Settings Toolbar",
      {ImGui::GetContentRegionAvail().x, TOOLBAR_HEIGHT}, true,
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
          ImGuiWindowFlags_NoScrollWithMouse);

  ImGui::TextUnformatted(pending_changes ? "Changed..." : "Active");

  if (pending_changes) {
    auto button_color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    button_color.w = 0.0F;
    ImGui::PushStyleColor(ImGuiCol_Button, button_color);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {1.0F, 1.0F});

    float right_align_pos = ImGui::GetContentRegionAvail().x;
    right_align_pos -= ICON_WIDTH;
    ImGui::SameLine(right_align_pos);
    if (ImGui::Button(ICON_MDI_RESTORE, {ICON_WIDTH, ICON_HEIGHT})) {
      reset_to_current = true;
    }
    right_align_pos -= ImGui::GetStyle().ItemSpacing.x;

    right_align_pos -= ICON_WIDTH;
    ImGui::SameLine(right_align_pos);
    if (ImGui::Button(ICON_MDI_CHECK_ALL, {ICON_WIDTH, ICON_HEIGHT})) {
      runner->EnableVsync(vsync);
      runner->MultiSample(samples);
      switch (display_mode) {
      case 0:
        runner->Windowed(size[0], size[1], title);
        break;
      case 1:
        runner->FullScreen(resolution->width, resolution->height, title,
            GetMonitorIndex(monitor), resolution->refreshRate);
        break;
      case 2:
        runner->FullScreenWindowed(title, GetMonitorIndex(monitor));
        break;
      default:;
      }
      runner->SetWindowTitle(title);
      pending_changes = false;
    }

    ImGui::PopStyleVar();
    // Restore the button color
    ImGui::PopStyleColor();
  }

  ImGui::EndChild();

  ImGui::PopStyleColor();
  ImGui::PopStyleVar();

  // Settings

  if (DrawDisplaySettingsTitle(runner, title)) {
    pending_changes = true;
  }
  if (DrawDisplaySettingsMode(display_mode)) {
    pending_changes = true;
  }
  if (display_mode == 1 || display_mode == 2) {
    if (DrawDisplaySettingsMonitor(runner, monitor)) {
      pending_changes = true;
    }
  }
  if (display_mode == 0) {
    if (DrawDisplaySettingsWindowSize(runner, size)) {
      pending_changes = true;
    }
  }
  if (display_mode == 1) {
    if (DrawDisplaySettingsResolution(monitor, resolution)) {
      pending_changes = true;
    }
  }

  if (ImGui::Checkbox("V-Sync", &vsync)) {
    pending_changes = true;
  }
  if (ImGui::SliderInt("Multi-sampling", &samples, -1, 4, "%d")) {
    pending_changes = true;
  }
}

void ShowStyleSettings() {
  static bool reset_to_current = false;
  if (reset_to_current) {
    reset_to_current = false;
    Theme::LoadStyle();
  }
  // Toolbar
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
        {HORIZONTAL_WINDOW_PADDING, VERTICAL_WINDOW_PADDING});
    ImGui::PushStyleColor(
        ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg));
    {
      ImGui::BeginChild("Style Settings Toolbar",
          {ImGui::GetContentRegionAvail().x, TOOLBAR_HEIGHT}, true,
          ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
              ImGuiWindowFlags_NoScrollWithMouse);
      {
        if (ImGui::SmallButton("Load Default Style")) {
          Theme::LoadDefaultStyle();
        }
      }
      {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {1.0F, 1.0F});
        auto button_color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        button_color.w = 0.0F;
        ImGui::PushStyleColor(ImGuiCol_Button, button_color);

        float right_align_pos = ImGui::GetContentRegionAvail().x;
        right_align_pos -= ICON_WIDTH;
        ImGui::SameLine(right_align_pos);
        if (ImGui::Button(ICON_MDI_RESTORE, {ICON_WIDTH, ICON_HEIGHT})) {
          reset_to_current = true;
        }

        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
      }

      ImGui::EndChild();
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
  }

  ImGui::ShowStyleEditor();
}

} // namespace

void ApplicationBase::DrawSettings() {
  if (ImGui::Begin("Settings", &show_settings_)) {
    static bool first_time = true;
    if (first_time) {
      ImGui::SetNextItemOpen(true);
      first_time = false;
    }
    if (ImGui::CollapsingHeader("Display")) {
      ShowDisplaySettings(runner_);
    }

    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Style")) {
      ShowStyleSettings();
    }
  }
  ImGui::End();
}

void ApplicationBase::DrawDocksDebug() {
  if (ImGui::Begin("Docks", &show_docks_debug_)) {
    ImGui::LabelText("TODO",
        "Get docking information from ImGui and populate this once the ImGui "
        "programmatic access to docking is published as a stable API");
    // TODO(Abdessattar): generate docking information
  }
  ImGui::End();
}

#if 0
// 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets
    // automatically appears in a window called "Debug".
    {
      ImGui::ColorEdit3(
          "clear color",
          (float *) &clear_color);  // Edit 3 floats representing a color

      ImGui::Checkbox("Demo Window",
                      &show_demo_window);  // Edit bools storing our windows
      // open/close state
      ImGui::Checkbox("Log Messages Window", &show_log_messages_window);

      ImGui::Text("Application average %.3F ms/frame (%.1F FPS)",
                  1000.0F / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    // 3. Show the ImGui demo window. Most of the sample code is in
    // ImGui::ShowDemoWindow(). Read its code to learn more about Dear
    // ImGui!
    if (show_demo_window) {
      ImGui::SetNextWindowPos(
          ImVec2(650, 20),
          ImGuiCond_FirstUseEver);  // Normally user code doesn't need/want
      // to
      // call this because positions are saved
      // in .ini file anyway. Here we just want
      // to make the demo initial state a bit
      // more friendly!
      ImGui::ShowDemoWindow(&show_demo_window);
    }

    if (show_log_messages_window) {
      sink->Draw("Log Messages", &show_log_messages_window);
    }

#endif
