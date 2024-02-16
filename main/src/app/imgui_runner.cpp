/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "app/imgui_runner.h"
#include "app/application.h"
#include "config/config.h"

// clang-format off
// Include order is important
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
// clang-format on

#include <chrono> // for sleep timeout
#include <contract/contract.h>
#include <csignal> // for signal handling
#include <fstream>
#include <gsl/span>
#include <thread> // for access to this thread
#include <toml++/toml.hpp>
#include <utility>

namespace {

void glfw_error_callback(int error, const char *description) {
  auto &logger = asap::logging::Registry::GetLogger("main");
  ASLOG_TO_LOGGER(logger, critical, "Glfw Error {}: {}", error, description);
}

volatile std::sig_atomic_t gSignalInterrupt_;

void SignalHandler(int signal) {
  gSignalInterrupt_ = signal;
}

} // namespace

namespace asap::app {

const char *const ImGuiRunner::LOGGER_NAME = "main";

ImGuiRunner::ImGuiRunner(Application &app, shutdown_function_type func)
    : app_(app), shutdown_function_(std::move(func)) {
  InitGraphics();
  LoadSetting();
}

void ImGuiRunner::InitGraphics() {
  ASLOG(info, "Initialize graphical subsystem...");
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (glfwInit() == 0) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  // Decide GL version
#if __APPLE__
  // GL 3.2
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#elif _WIN32
  // GL 3.0 (as per ImGui demo)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  // only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#else
  // GL 3.2
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only
#endif

  ASLOG(debug, "  GLFW init done");
}

void ImGuiRunner::SetupContext() {
  ASAP_ASSERT(window_ != nullptr);
  glfwMakeContextCurrent(window_);

  if (gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress)) == 0) {
    ASLOG(error, "  failed to initialize OpenGL context!");
    throw std::runtime_error("failed to initialize OpenGL context");
  }

  ASLOG(debug, "  context setup done");
}

void ImGuiRunner::InitImGui() {
  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  static auto imguiSettingsPath =
      asap::config::GetPathFor(asap::config::Location::F_IMGUI_SETTINGS)
          .string();
  io.IniFilename = imguiSettingsPath.c_str();

  //
  // Various flags controlling ImGui IO behavior
  //

  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Enable Gamepad Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // Enable Multi-Viewport
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  // NOTE: Platform Windows
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

  ImGui_ImplGlfw_InitForOpenGL(window_, true);

  // Decide GLSL version
#if __APPLE__
  // GLSL 150
  const char *glsl_version = "#version 150";
#else
  // GLSL 130
  const char *glsl_version = "#version 130";
#endif
  ImGui_ImplOpenGL3_Init(glsl_version);

  ASLOG(debug, "  ImGui init done");
}

void ImGuiRunner::Windowed(int width, int height, const std::string &title) {
  window_title_ = title;

  if (windowed_ && !full_screen_) {
    saved_position_ = GetWindowPosition();
  }

  windowed_ = true;
  full_screen_ = false;

  if (window_ == nullptr) {
    ASLOG(debug, "  starting in 'Windowed' mode: w={}, h={}, t='{}'", width,
        height, title);
    glfwWindowHint(GLFW_SAMPLES, MultiSample()); // multisampling
    window_ = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    if (window_ == nullptr) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    SetupContext();
    glfwSwapInterval(Vsync() ? 1 : 0); // Enable vsync
    InitImGui();
  } else {
    ASLOG(debug, "setting 'Windowed' mode: w={}, h={}, t={}", width, height,
        title);
    // Restore the window position when you switch to windowed mode
    glfwSetWindowMonitor(window_, nullptr, saved_position_.first,
        saved_position_.second, width, height, GLFW_DONT_CARE);
    glfwSetWindowTitle(window_, title.data());
  }
}

namespace {
auto GetMonitorByNumber(int monitor) -> GLFWmonitor * {
  GLFWmonitor *the_monitor{nullptr};
  if (monitor == 0) {
    the_monitor = glfwGetPrimaryMonitor();
  } else {
    int count = 0;
    GLFWmonitor **glfw_monitors = glfwGetMonitors(&count);
    auto monitors = gsl::span(glfw_monitors, count);
    if (monitor >= 0 && monitor < count) {
      the_monitor = monitors[monitor];
    } else {
      auto &logger = logging::Registry::GetLogger("main");
      ASLOG_TO_LOGGER(logger, error,
          "requested monitor {} is not connected, using primary monitor");
      the_monitor = monitors[0];
    }
  }
  return the_monitor;
}
} // namespace

void ImGuiRunner::FullScreenWindowed(const std::string &title, int monitor) {
  window_title_ = title;
  windowed_ = true;
  full_screen_ = true;

  GLFWmonitor *the_monitor = GetMonitorByNumber(monitor);
  const GLFWvidmode *mode = glfwGetVideoMode(the_monitor);
  if (window_ == nullptr) {
    ASLOG(debug,
        "  starting in 'Full Screen Windowed' mode: w={}, h={}, r={}, "
        "t='{}', m={}",
        mode->width, mode->height, mode->refreshRate, title, monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_SAMPLES, MultiSample()); // multisampling
    window_ = glfwCreateWindow(
        mode->width, mode->height, title.data(), the_monitor, nullptr);
    if (window_ == nullptr) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    SetupContext();
    glfwSwapInterval(Vsync() ? 1 : 0); // Enable vsync
    InitImGui();
  } else {
    ASLOG(debug,
        "setting 'Full Screen Windowed' mode: w={}, h={}, r= {}, t='{}', m={}",
        mode->width, mode->height, mode->refreshRate, title, monitor);

    // Save the window position
    saved_position_ = GetWindowPosition();

    glfwSetWindowMonitor(window_, the_monitor, 0, 0, mode->width, mode->height,
        mode->refreshRate);
    glfwSetWindowTitle(window_, title.data());
  }
}
void ImGuiRunner::FullScreen(int width, int height, const std::string &title,
    int monitor, int refresh_rate) {
  window_title_ = title;
  windowed_ = false;
  full_screen_ = true;

  GLFWmonitor *the_monitor = GetMonitorByNumber(monitor);
  if (window_ == nullptr) {
    ASLOG(debug,
        "  starting in 'Full Screen' mode: w={}, h={}, t='{}', m={}, r={}",
        width, height, title, monitor, refresh_rate);
    glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
    glfwWindowHint(GLFW_SAMPLES, MultiSample()); // multisampling
    window_ =
        glfwCreateWindow(width, height, title.data(), the_monitor, nullptr);
    if (window_ == nullptr) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    SetupContext();
    glfwSwapInterval(Vsync() ? 1 : 0); // Enable vsync
    InitImGui();
  } else {
    ASLOG(debug, "setting 'Full Screen' mode: w={}, h={}, t='{}', m={}, r={}",
        width, height, title, monitor, refresh_rate);

    // Save the window position
    saved_position_ = GetWindowPosition();

    glfwSetWindowMonitor(
        window_, the_monitor, 0, 0, width, height, refresh_rate);
    glfwSetWindowTitle(window_, title.data());
  }
}

void ImGuiRunner::CleanUp() {
  ASLOG(info, "Cleanup graphical subsystem...");

  // Cleanup ImGui
  ASLOG(debug, "  shutdown OpenGL3");
  ImGui_ImplOpenGL3_Shutdown();
  ASLOG(debug, "  shutdown ImGui/GLFW");
  ImGui_ImplGlfw_Shutdown();
  ASLOG(debug, "  destroy ImGui context");
  ImGui::DestroyContext();

  ASLOG(debug, "  destroy window");
  glfwDestroyWindow(window_);
  ASLOG(debug, "  terminate GLFW");
  glfwTerminate();

  // Call the shutdown hook
  ASLOG(debug, "  call shutdown handler");
  shutdown_function_();
}

void ImGuiRunner::Run() {
  // Register to handle the signals that indicate when the server should exit.
  // It is safe to register for the same signal multiple times in a program,
  // provided all registration for the specified signal is made through Asio.
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  app_.Init(this);

  // Main loop
  bool sleep_when_inactive = true;
  while ((glfwWindowShouldClose(window_) == 0) && (gSignalInterrupt_ == 0)) {
    static float wanted_fps;
    if (sleep_when_inactive &&
        (glfwGetWindowAttrib(window_, GLFW_FOCUSED) == 0)) {
      wanted_fps = 20.0F;
    } else {
      wanted_fps = 90.0F;
    }
    float current_fps = ImGui::GetIO().Framerate;
    float frame_time = 1000 / current_fps;
    auto wait_time = std::lround(1000 / wanted_fps - frame_time);
    if (wanted_fps < current_fps) {
      std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
    }

    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data
    // to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application. Generally you may always pass all
    // inputs to dear imgui, and hide them from your application based on
    // those two flags.
    glfwPollEvents();

    // Skip frame rendering if the window width or heigh is 0
    // Not doing so will cause the docking system to lose its mind
    auto size = GetWindowSize();
    if (size.first == 0 || size.second == 0) {
      continue;
    }

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw the Application
    sleep_when_inactive = app_.Draw();

    // Rendering
    ImGui::Render();
    int display_w = 0;
    int display_h = 0;
    glfwMakeContextCurrent(window_);
    glfwGetFramebufferSize(window_, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
    }

    glfwMakeContextCurrent(window_);
    glfwSwapBuffers(window_);
  }

  SaveSetting();

  app_.ShutDown();
  CleanUp();
}
void ImGuiRunner::EnableVsync(bool state) {
  glfwSwapInterval(state ? 1 : 0);
  vsync_ = state;
}
void ImGuiRunner::MultiSample(int samples) {
  if (samples < 0 || samples > 4) {
    samples = GLFW_DONT_CARE;
  }
  samples_ = samples;
  glfwWindowHint(GLFW_SAMPLES, MultiSample());
}
auto ImGuiRunner::GetWindowTitle() const -> std::string const & {
  return window_title_;
}
void ImGuiRunner::SetWindowTitle(const std::string &title) {
  if (window_ != nullptr) {
    window_title_ = title;
    glfwSetWindowTitle(window_, title.data());
  } else {
    ASLOG(error, "call SetWindowTitle() only after the window is created");
  }
}
auto ImGuiRunner::GetMonitor() const -> GLFWmonitor * {
  ASAP_ASSERT(
      window_, "don't call GetMonitor() before the window is created", nullptr);
  return glfwGetWindowMonitor(window_);
}
auto ImGuiRunner::GetWindowSize() const -> std::pair<int, int> {
  ASAP_ASSERT(window_,
      "don't call GetWindowSize() before the window is created",
      std::make_pair(-1, -1));
  auto size = std::make_pair(-1, -1);
  glfwGetWindowSize(window_, &size.first, &size.second);
  return size;
}
auto ImGuiRunner::GetWindowPosition() const -> std::pair<int, int> {
  ASAP_ASSERT(window_, "don't call GetMonitor() before the window is created",
      std::make_pair(-1, -1));
  auto position = std::make_pair(-1, -1);
  glfwGetWindowPos(window_, &position.first, &position.second);
  return position;
}

auto ImGuiRunner::RefreshRate() const -> int {
  const auto *vid_mode = glfwGetVideoMode(GetMonitor());
  return vid_mode->refreshRate;
}

auto ImGuiRunner::GetMonitorId() const -> int {
  int count = 0;
  GLFWmonitor **glfw_monitors = glfwGetMonitors(&count);
  auto monitors = gsl::span(glfw_monitors, count);
  while (--count >= 0 && monitors[count] != GetMonitor()) {
    // DO NOTHING
  }
  return count;
}

// -------------------------------------------------------------------------
// Settings load/save
// -------------------------------------------------------------------------

void ImGuiRunner::LoadSetting() {
  toml::parse_result config;
  auto display_settings =
      asap::config::GetPathFor(asap::config::Location::F_DISPLAY_SETTINGS);
  auto has_config = false;
  if (std::filesystem::exists(display_settings)) {
    try {
      config = toml::parse_file(display_settings.string());
      ASLOG(info, "display settings loaded from {}", display_settings.string());
      has_config = true;
    } catch (std::exception const &ex) {
      ASLOG(error, "error {} while loading settings from {}", ex.what(),
          display_settings.string());
    }
  } else {
    ASLOG(info, "file {} does not exist", display_settings.string());
  }

  constexpr int DEFAULT_WINDOW_WIDTH = 800;
  constexpr int DEFAULT_WINDOW_HEIGHT = 600;

  int width = DEFAULT_WINDOW_WIDTH;
  int height = DEFAULT_WINDOW_HEIGHT;
  if (has_config) {
    auto display = config["display"];
    auto multiSampling = display["multi-sampling"];
    if (multiSampling) {
      MultiSample(multiSampling.value_or<int>(1));
    }
    auto mode = display["mode"].value_or<std::string>("Full Screen Windowed");
    if (mode == "Full Screen") {
      auto size = display["size"];
      if (size) {
        if (size["width"]) {
          width = *(size[width].value<int>());
        }
        if (size["height"]) {
          height = *(size[height].value<int>());
        }
      }
      FullScreen(width, height, display["title"].value_or("ASAP Application"),
          display["monitor"].value_or(0), display["refresh-rate"].value_or(0));
    } else if (mode == "Full Screen Windowed") {
      FullScreenWindowed(display["title"].value_or("ASAP Application"),
          display["monitor"].value_or(0));
    } else if (mode == "Windowed") {
      auto size = display["size"];
      if (size) {
        if (size["width"]) {
          width = *(size["width"].value<int>());
        }
        if (size["height"]) {
          height = *(size["height"].value<int>());
        }
      }
      Windowed(width, height, display["title"].value_or("ASAP Application"));
    } else {
      Windowed(width, height, "ASAP Application");
    }
    EnableVsync(display["vsync"].value_or(0));
  } else {
    Windowed(width, height, "ASAP Application");
  }
}

void ImGuiRunner::SaveSetting() const {
  toml::table display_settings;
  display_settings.insert("title", GetWindowTitle());
  if (IsFullScreen()) {
    if (IsWindowed()) {
      display_settings.insert("mode", "Full Screen Windowed");
      display_settings.insert("monitor", GetMonitorId());
    } else {
      display_settings.insert("mode", "Full Screen");
      // size
      {
        toml::table size_settings;
        auto size = GetWindowSize();
        size_settings.insert("width", size.first);
        size_settings.insert("height", size.second);
        display_settings.insert("size", size_settings);
      }
      display_settings.insert("monitor", GetMonitorId());
      display_settings.insert("refresh-rate", RefreshRate());
    }
  } else {
    display_settings.insert("mode", "Windowed");
    // size
    {
      toml::table size_settings;
      auto size = GetWindowSize();
      size_settings.insert("width", size.first);
      size_settings.insert("height", size.second);
      display_settings.insert("size", size_settings);
    }
  }
  display_settings.insert("multi-sampling", MultiSample());
  display_settings.insert("vsync", Vsync());

  toml::table root;
  root.insert("display", display_settings);

  auto settings_path =
      asap::config::GetPathFor(asap::config::Location::F_DISPLAY_SETTINGS);
  auto ofs = std::ofstream();
  ofs.open(settings_path.string());
  ofs << root << std::endl;
  ofs.close();
}

} // namespace asap::app
