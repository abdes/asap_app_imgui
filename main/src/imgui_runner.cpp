//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <chrono>   // for sleep timeout
#include <csignal>  // for signal handling
#include <thread>   // for access to this thread

#include <imgui_runner.h>

#include <fstream>

// clang-format off
// Include order is important
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <glfw/imgui_impl_glfw.h>
#include <glfw/imgui_impl_opengl3.h>
// clang-format on

#include <cpptoml.h>

#include <common/assert.h>
#include <application.h>
#include <config.h>

namespace {
void glfw_error_callback(int error, const char *description) {
  auto &logger = asap::logging::Registry::GetLogger("main");
  ASLOG_TO_LOGGER(logger, critical, "Glfw Error {}: {}", error, description);
}

volatile std::sig_atomic_t gSignalInterrupt_;

void SignalHandler(int signal) { gSignalInterrupt_ = signal; }

}  // namespace

namespace asap {

ImGuiRunner::ImGuiRunner(AbstractApplication &app,
                         RunnerBase::shutdown_function_type func)
    : RunnerBase(app, std::move(func)) {
  InitGraphics();
  LoadSetting();
}

void ImGuiRunner::InitGraphics() {
  ASLOG(info, "Initialize graphical subsystem...");
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  // Decide GL version
#if __APPLE__
  // GL 3.2
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Required on Mac
#elif _WIN32
  // GL 3.0 (as per ImGui demo)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  // only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // 3.0+ only
#else
  // GL 3.2
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

  ASLOG(debug, "  GLFW init done");
}

void ImGuiRunner::SetupContext() {
  ASAP_ASSERT(window_ != nullptr);
  glfwMakeContextCurrent(window_);
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    ASLOG(error, "  failed to initialize OpenGL context!");
    throw std::runtime_error("failed to initialize OpenGL context");
  } else {
    ASLOG(debug, "  context setup done");
  }
}

void ImGuiRunner::InitImGui() {
  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  static auto imguiSettingsPath =
      asap::fs::GetPathFor(asap::fs::Location::F_IMGUI_SETTINGS).string();
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

void ImGuiRunner::Windowed(int width, int height, char const *title) {
  window_title_ = title;

  if (windowed_ && !full_screen_) {
    GetWindowPosition(saved_position_);
  }

  windowed_ = true;
  full_screen_ = false;

  if (!window_) {
    ASLOG(debug, "  starting in 'Windowed' mode: w={}, h={}, t='{}'", width,
          height, title);
    glfwWindowHint(GLFW_SAMPLES, MultiSample());  // multisampling
    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    SetupContext();
    glfwSwapInterval(Vsync() ? 1 : 0);  // Enable vsync
    InitImGui();
  } else {
    ASLOG(debug, "setting 'Windowed' mode: w={}, h={}, t={}", width, height,
          title);
    // Restore the window position when you switch to windowed mode
    glfwSetWindowMonitor(window_, nullptr, saved_position_[0],
                         saved_position_[1], width, height, 60);
    glfwSetWindowTitle(window_, title);
  }
}

namespace {
GLFWmonitor *GetMonitorByNumber(int monitor) {
  GLFWmonitor *the_monitor{nullptr};
  if (monitor == 0)
    the_monitor = glfwGetPrimaryMonitor();
  else {
    int count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);
    if (monitor >= 0 && monitor < count)
      the_monitor = monitors[monitor];
    else {
      auto &logger = logging::Registry::GetLogger("main");
      ASLOG_TO_LOGGER(
          logger, error,
          "requested monitor {} is not connected, using primary monitor");
      the_monitor = monitors[0];
    }
  }
  return the_monitor;
}
}  // namespace

void ImGuiRunner::FullScreenWindowed(char const *title, int monitor) {
  window_title_ = title;
  windowed_ = true;
  full_screen_ = true;

  GLFWmonitor *the_monitor = GetMonitorByNumber(monitor);
  const GLFWvidmode *mode = glfwGetVideoMode(the_monitor);
  if (!window_) {
    ASLOG(debug,
          "  starting in 'Full Screen Windowed' mode: w={}, h={}, r={}, "
          "t='{}', m={}",
          mode->width, mode->height, mode->refreshRate, title, monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_SAMPLES, MultiSample());  // multisampling
    window_ = glfwCreateWindow(mode->width, mode->height, title, the_monitor,
                               nullptr);
    if (!window_) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    SetupContext();
    glfwSwapInterval(Vsync() ? 1 : 0);  // Enable vsync
    InitImGui();
  } else {
    ASLOG(
        debug,
        "setting 'Full Screen Windowed' mode: w={}, h={}, r= {}, t='{}', m={}",
        mode->width, mode->height, mode->refreshRate, title, monitor);

    // Save the window position
    GetWindowPosition(saved_position_);

    glfwSetWindowMonitor(window_, the_monitor, 0, 0, mode->width, mode->height,
                         mode->refreshRate);
    glfwSetWindowTitle(window_, title);
  }
}
void ImGuiRunner::FullScreen(int width, int height, char const *title,
                             int monitor, int refresh_rate) {
  window_title_ = title;
  windowed_ = false;
  full_screen_ = true;

  GLFWmonitor *the_monitor = GetMonitorByNumber(monitor);
  if (!window_) {
    ASLOG(debug,
          "  starting in 'Full Screen' mode: w={}, h={}, t='{}', m={}, r={}",
          width, height, title, monitor, refresh_rate);
    glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
    glfwWindowHint(GLFW_SAMPLES, MultiSample());  // multisampling
    window_ = glfwCreateWindow(width, height, title, the_monitor, nullptr);
    if (!window_) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    SetupContext();
    glfwSwapInterval(Vsync() ? 1 : 0);  // Enable vsync
    InitImGui();
  } else {
    ASLOG(debug, "setting 'Full Screen' mode: w={}, h={}, t='{}', m={}, r={}",
          width, height, title, monitor, refresh_rate);

    // Save the window position
    GetWindowPosition(saved_position_);

    glfwSetWindowMonitor(window_, the_monitor, 0, 0, width, height,
                         refresh_rate);
    glfwSetWindowTitle(window_, title);
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
  while (!glfwWindowShouldClose(window_) && (gSignalInterrupt_ == 0)) {
    static float wanted_fps;
    if (sleep_when_inactive && !glfwGetWindowAttrib(window_, GLFW_FOCUSED)) {
      wanted_fps = 20.0f;
    } else {
      wanted_fps = 90.0f;
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
    int size[2];
    GetWindowSize(size);
    if (size[0] == 0 || size[1] == 0) continue;

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw the Application
    sleep_when_inactive = app_.Draw();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwMakeContextCurrent(window_);
    glfwGetFramebufferSize(window_, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
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
  if (samples < 0 || samples > 4) samples = GLFW_DONT_CARE;
  samples_ = samples;
  glfwWindowHint(GLFW_SAMPLES, MultiSample());
}
std::string const &ImGuiRunner::GetWindowTitle() const { return window_title_; }
void ImGuiRunner::SetWindowTitle(char const *title) {
  if (window_) {
    window_title_ = title;
    glfwSetWindowTitle(window_, title);
  } else {
    ASLOG(error, "call SetWindowTitle() only after the window is created");
  }
}
GLFWmonitor *ImGuiRunner::GetMonitor() const {
  ASAP_ASSERT(window_ &&
              "don't call GetMonitor() before the window is created");
  return glfwGetWindowMonitor(window_);
}
void ImGuiRunner::GetWindowSize(int size[2]) const {
  ASAP_ASSERT(window_ &&
              "don't call GetWindowSize() before the window is created");
  glfwGetWindowSize(window_, &size[0], &size[1]);
}
void ImGuiRunner::GetWindowPosition(int position[2]) const {
  ASAP_ASSERT(window_ &&
              "don't call GetMonitor() before the window is created");
  glfwGetWindowPos(window_, &position[0], &position[1]);
}

int ImGuiRunner::RefreshRate() const {
  auto vid_mode = glfwGetVideoMode(GetMonitor());
  return vid_mode->refreshRate;
}

int ImGuiRunner::GetMonitorId() const {
  int count = 0;
  GLFWmonitor **monitors = glfwGetMonitors(&count);
  while (--count >= 0 && monitors[count] != GetMonitor())
    ;
  return count;
}

// -------------------------------------------------------------------------
// Settings load/save
// -------------------------------------------------------------------------

namespace {
void ConfigSanityChecks(std::shared_ptr<cpptoml::table> &config) {
  auto &logger = asap::logging::Registry::GetLogger("main");

  auto display = config->get_table("display");
  if (!display) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'display' in config");
  }

  if (!display->contains("mode")) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'display/mode' in config");
  } else {
    auto mode = *(display->get_as<std::string>("mode"));
    if (mode == "Full Screen") {
      if (!display->contains("size")) {
        ASLOG_TO_LOGGER(logger, warn, "missing 'display/size' in config");
      } else {
        auto size = display->get_table("size");
        if (!size->contains("width")) {
          ASLOG_TO_LOGGER(logger, warn,
                          "missing 'display/size/width' in config");
        }
        if (!size->contains("height")) {
          ASLOG_TO_LOGGER(logger, warn,
                          "missing 'display/size/height' in config");
        }
      }
      if (!display->contains("title")) {
        ASLOG_TO_LOGGER(logger, warn, "missing 'display/title' in config");
      }
      if (!display->contains("monitor")) {
        ASLOG_TO_LOGGER(logger, warn, "missing 'display/monitor' in config");
      }
      if (!display->contains("refresh-rate")) {
        ASLOG_TO_LOGGER(logger, warn,
                        "missing 'display/refresh-rate' in config");
      }
    } else if (mode == "Full Screen Windowed") {
      if (!display->contains("title")) {
        ASLOG_TO_LOGGER(logger, warn, "missing 'display/title' in config");
      }
      if (!display->contains("monitor")) {
        ASLOG_TO_LOGGER(logger, warn, "missing 'display/monitor' in config");
      }
    } else if (mode == "Windowed") {
      if (!display->contains("size")) {
        ASLOG_TO_LOGGER(logger, warn, "missing 'display/size' in config");
      } else {
        auto size = display->get_table("size");
        if (!size->contains("width")) {
          ASLOG_TO_LOGGER(logger, warn,
                          "missing 'display/size/width' in config");
        }
        if (!size->contains("height")) {
          ASLOG_TO_LOGGER(logger, warn,
                          "missing 'display/size/height' in config");
        }
      }
      if (!display->contains("title")) {
        ASLOG_TO_LOGGER(logger, warn, "missing 'display/title' in config");
      }
    } else {
      ASLOG_TO_LOGGER(logger, error, "invalid 'display/mode' ({})", mode);
    }
  }

  if (!display->contains("multi-sampling")) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'display/multi-sampling' in config");
  }
  if (!display->contains("vsync")) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'display/vsync' in config");
  }
}
}  // namespace

void ImGuiRunner::LoadSetting() {
  std::shared_ptr<cpptoml::table> config;
  auto display_settings =
      asap::fs::GetPathFor(asap::fs::Location::F_DISPLAY_SETTINGS);
  auto has_config = false;
  if (asap::filesystem::exists(display_settings)) {
    try {
      config = cpptoml::parse_file(display_settings.string());
      ASLOG(info, "display settings loaded from {}", display_settings);
      has_config = true;
    } catch (std::exception const &ex) {
      ASLOG(error, "error () while loading settings from {}", ex.what(),
            display_settings);
    }
  } else {
    ASLOG(info, "file {} does not exist", display_settings);
  }

  int width = 800;
  int height = 600;
  if (has_config) {
    ConfigSanityChecks(config);

    auto display = config->get_table("display");
    if (display->contains("multi-sampling")) {
      MultiSample(*(display->get_as<int>("multi-sampling")));
    }
    auto mode =
        display->get_as<std::string>("mode").value_or("Full Screen Windowed");
    if (mode == "Full Screen") {
      auto size = display->get_table("size");
      if (size) {
        if (size->contains("width")) {
          width = *(size->get_as<int>("width"));
        }
        if (size->contains("height")) {
          height = *(size->get_as<int>("height"));
        }
      }
      FullScreen(width, height,
                 display->get_as<std::string>("title")
                     .value_or("ASAP Application")
                     .c_str(),
                 display->get_as<int>("monitor").value_or(0),
                 display->get_as<int>("refresh-rate").value_or(0));
    } else if (mode == "Full Screen Windowed") {
      FullScreenWindowed(display->get_as<std::string>("title")
                             .value_or("ASAP Application")
                             .c_str(),
                         display->get_as<int>("monitor").value_or(0));
    } else if (mode == "Windowed") {
      auto size = display->get_table("size");
      if (size) {
        if (size->contains("width")) {
          width = *(size->get_as<int>("width"));
        }
        if (size->contains("height")) {
          height = *(size->get_as<int>("height"));
        }
      }
      Windowed(width, height,
               display->get_as<std::string>("title")
                   .value_or("ASAP Application")
                   .c_str());
    } else {
      Windowed(width, height, "ASAP Application");
    }
    if (display->contains("vsync")) {
      EnableVsync(*(display->get_as<bool>("vsync")));
    }
  } else {
    Windowed(width, height, "ASAP Application");
  }
}

void ImGuiRunner::SaveSetting() {
  std::shared_ptr<cpptoml::table> root = cpptoml::make_table();

  auto display_settings = cpptoml::make_table();
  display_settings->insert("title", GetWindowTitle());
  if (IsFullScreen()) {
    if (IsWindowed()) {
      display_settings->insert("mode", "Full Screen Windowed");
      display_settings->insert("monitor", GetMonitorId());
    } else {
      display_settings->insert("mode", "Full Screen");
      // size
      {
        auto size_settings = cpptoml::make_table();
        int size[2];
        GetWindowSize(size);
        size_settings->insert("width", size[0]);
        size_settings->insert("height", size[1]);
        display_settings->insert("size", size_settings);
      }
      display_settings->insert("monitor", GetMonitorId());
      display_settings->insert("refresh-rate", RefreshRate());
    }
  } else {
    display_settings->insert("mode", "Windowed");
    // size
    {
      auto size_settings = cpptoml::make_table();
      int size[2];
      GetWindowSize(size);
      size_settings->insert("width", size[0]);
      size_settings->insert("height", size[1]);
      display_settings->insert("size", size_settings);
    }
  }
  display_settings->insert("multi-sampling", MultiSample());
  display_settings->insert("vsync", Vsync());

  root->insert("display", display_settings);

  auto settings_path =
      asap::fs::GetPathFor(asap::fs::Location::F_DISPLAY_SETTINGS);
  auto ofs = std::ofstream();
  ofs.open(settings_path.string());
  ofs << (*root) << std::endl;
  ofs.close();
}

}  // namespace asap
