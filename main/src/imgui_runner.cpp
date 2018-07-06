//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <imgui_runner.h>

#include <fstream>

#include <boost/asio.hpp>

// clang-format off
// Include order is important
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <glfw/imgui_impl_glfw.h>
#include <glfw/imgui_impl_opengl3.h>
// clang-format on

#include <yaml-cpp/yaml.h>

#include <common/assert.h>
#include <application.h>
#include <config.h>

namespace bfs = boost::filesystem;

namespace {
void glfw_error_callback(int error, const char *description) {
  auto &logger = asap::logging::Registry::GetLogger(asap::logging::Id::MAIN);
  ASLOG_TO_LOGGER(logger, critical, "Glfw Error {}: {}", error, description);
}
}  // namespace

namespace asap {

ImGuiRunner::ImGuiRunner(AbstractApplication &app,
                         RunnerBase::shutdown_function_type func)
    : RunnerBase(app, std::move(func)) {
  SetupSignalHandler();
  InitGraphics();
}

ImGuiRunner::~ImGuiRunner() {
  if (!io_context_->stopped()) io_context_->stop();
  delete signals_;
  delete io_context_;
}

void ImGuiRunner::SetupSignalHandler() {
  ASLOG(info, "Setup termination signal handlers");
  // Set signal handler
  io_context_ = new boost::asio::io_context(1);
  signals_ = new boost::asio::signal_set(*io_context_);
  // Register to handle the signals that indicate when the server should exit.
  // It is safe to register for the same signal multiple times in a program,
  // provided all registration for the specified signal is made through Asio.
  signals_->add(SIGINT);
  signals_->add(SIGTERM);
}

void ImGuiRunner::InitGraphics() {
  ASLOG(info, "Initialize graphical subsystem...");
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  ASLOG(debug, "  GLFW init done");
}

void ImGuiRunner::SetupContext() {
  ASAP_ASSERT(window != nullptr);
  glfwMakeContextCurrent(window);
  gladLoadGL((GLADloadfunc) glfwGetProcAddress);
  ASLOG(debug, "  context setup done");
}

void ImGuiRunner::InitImGui() {
  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  // // Enable Gamepad Controls

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
  ASLOG(debug, "  ImGui init done");
}

void ImGuiRunner::Windowed(int width, int height, char const *title) {
  window_title_ = title;

  if (windowed_ && !full_screen_) {
    GetWindowPosition(saved_position_);
  }

  windowed_ = true;
  full_screen_ = false;

  if (!window) {
    ASLOG(debug, "  starting in 'Windowed' mode: w={}, h={}, t='{}'", width,
          height, title);
    glfwWindowHint(GLFW_SAMPLES, MultiSample());  // multisampling
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    SetupContext();
    glfwSwapInterval(Vsync()?1:0);  // Enable vsync
    InitImGui();
  } else {
    ASLOG(debug, "setting 'Windowed' mode: w={}, h={}, t={}", width, height,
          title);
    // Restore the window position when you switch to windowed mode
    glfwSetWindowMonitor(window, nullptr, saved_position_[0],
                         saved_position_[1], width, height, 60);
    glfwSetWindowTitle(window, title);
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
      auto &logger = logging::Registry::GetLogger(logging::Id::MAIN);
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
  if (!window) {
    ASLOG(debug,
          "  starting in 'Full Screen Windowed' mode: w={}, h={}, r={}, "
          "t='{}', m={}",
          mode->width, mode->height, mode->refreshRate, title, monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_SAMPLES, MultiSample());  // multisampling
    window = glfwCreateWindow(mode->width, mode->height, title, the_monitor,
                              nullptr);
    if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    SetupContext();
    glfwSwapInterval(Vsync()?1:0);  // Enable vsync
    InitImGui();
  } else {
    ASLOG(
        debug,
        "setting 'Full Screen Windowed' mode: w={}, h={}, r= {}, t='{}', m={}",
        mode->width, mode->height, mode->refreshRate, title, monitor);

    // Save the window position
    GetWindowPosition(saved_position_);

    glfwSetWindowMonitor(window, the_monitor, 0, 0, mode->width, mode->height,
                         mode->refreshRate);
    glfwSetWindowTitle(window, title);
  }
}
void ImGuiRunner::FullScreen(int width, int height, char const *title,
                             int monitor, int refresh_rate) {
  window_title_ = title;
  windowed_ = false;
  full_screen_ = true;

  GLFWmonitor *the_monitor = GetMonitorByNumber(monitor);
  if (!window) {
    ASLOG(debug,
          "  starting in 'Full Screen' mode: w={}, h={}, t='{}', m={}, r={}",
          width, height, title, monitor, refresh_rate);
    glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
    glfwWindowHint(GLFW_SAMPLES, MultiSample());  // multisampling
    window = glfwCreateWindow(width, height, title, the_monitor, nullptr);
    if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    SetupContext();
    glfwSwapInterval(Vsync()?1:0);  // Enable vsync
    InitImGui();
  } else {
    ASLOG(debug, "setting 'Full Screen' mode: w={}, h={}, t='{}', m={}, r={}",
          width, height, title, monitor, refresh_rate);

    // Save the window position
    GetWindowPosition(saved_position_);

    glfwSetWindowMonitor(window, the_monitor, 0, 0, width, height,
                         refresh_rate);
    glfwSetWindowTitle(window, title);
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
  glfwDestroyWindow(window);
  ASLOG(debug, "  terminate GLFW");
  glfwTerminate();

  // Call the shutdown hook
  ASLOG(debug, "  call shutdown handler");
  shutdown_function_();
}

void ImGuiRunner::Run() {
  app_.Init(this);

  // Main loop
  bool interrupted = false;
  bool sleep_when_inactive = true;
  while (!glfwWindowShouldClose(window) && !interrupted) {
    signals_->async_wait(
        [this, &interrupted](boost::system::error_code /*ec*/, int /*signo*/) {
          ASLOG(info, "Signal caught");
          // Once all operations have finished the io_context::run() call will
          // exit.
          interrupted = true;
          io_context_->stop();
        });

    if (sleep_when_inactive && !glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
      static float wanted_fps = 5.0f;
      float current_fps = ImGui::GetIO().Framerate;
      float frame_time = 1000 / current_fps;
      auto wait_time = std::lround(1000 / wanted_fps - frame_time);
      if (wanted_fps < current_fps) {
        io_context_->run_for(std::chrono::milliseconds(wait_time));
      }
    } else {
      io_context_->poll_one();
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
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
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
  if (window) {
    window_title_ = title;
    glfwSetWindowTitle(window, title);
  } else {
    ASLOG(error, "call SetWindowTitle() only after the window is created");
  }
}
GLFWmonitor *ImGuiRunner::GetMonitor() const {
  ASAP_ASSERT(window && "don't call GetMonitor() before the window is created");
  return glfwGetWindowMonitor(window);
}
void ImGuiRunner::GetWindowSize(int size[2]) const {
  ASAP_ASSERT(window &&
                  "don't call GetWindowSize() before the window is created");
  glfwGetWindowSize(window, &size[0], &size[1]);
}
void ImGuiRunner::GetWindowPosition(int position[2]) const {
  ASAP_ASSERT(window && "don't call GetMonitor() before the window is created");
  glfwGetWindowPos(window, &position[0], &position[1]);
}

int ImGuiRunner::RefreshRate() const {
  auto vid_mode = glfwGetVideoMode(GetMonitor());
  return vid_mode->refreshRate;
}

int ImGuiRunner::GetMonitorId() const {
  int count = 0;
  GLFWmonitor **monitors = glfwGetMonitors(&count);
  while (--count >= 0 && monitors[count] != GetMonitor());
  return count;
}

// -------------------------------------------------------------------------
// Settings load/save
// -------------------------------------------------------------------------

namespace {
void ConfigSanityChecks(YAML::Node &config) {
  auto &logger = asap::logging::Registry::GetLogger(asap::logging::Id::MAIN);

  auto display = config["display"];
  if ( !display) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'display' in config");
  }

  if ( !display["mode"]) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'display/mode' in config");
  }

  if (!display["multi-sampling"]) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'display/multi-sampling' in config");
  }
  if (!display["vsync"]) {
    ASLOG_TO_LOGGER(logger, warn, "missing 'display/vsync' in config");
  }

  auto mode = display["mode"].as<std::string>();
  if (mode == "Full Screen") {
    if (!display["size"]["width"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/size/width' in config");
    }
    if (!display["size"]["height"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/size/height' in config");
    }
    if (!display["title"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/title' in config");
    }
    if (!display["monitor"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/monitor' in config");
    }
    if (!display["refresh-rate"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/refresh-rate' in config");
    }
  } else if (mode == "Full Screen Windowed") {
    if (!display["title"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/title' in config");
    }
    if (!display["monitor"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/monitor' in config");
    }
  } else if (mode == "Windowed") {
    if (!display["size"]["width"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/size/width' in config");
    }
    if (!display["size"]["height"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/size/height' in config");
    }
    if (!display["title"]) {
      ASLOG_TO_LOGGER(logger, warn, "missing 'display/title' in config");
    }
  } else {
    ASLOG_TO_LOGGER(logger, error, "invalid 'display/mode' ({})", mode);
  }
}
}

void ImGuiRunner::LoadSetting() {
  YAML::Node config;
  auto display_settings =
      asap::fs::GetPathFor(asap::fs::Location::F_DISPLAY_SETTINGS);
  auto has_config = false;
  if (bfs::exists(display_settings)) {
    try {
      config = YAML::LoadFile(display_settings.string());
      ASLOG(info, "display settings loaded from {}",
            display_settings);
      has_config = true;
    } catch (std::exception const &ex) {
      ASLOG(error,
            "error () while loading settings from {}", ex.what(),
            display_settings);
    }
  } else {
    ASLOG(info, "file {} does not exist", display_settings);
  }

  if (has_config) {
    ConfigSanityChecks(config);

    auto display = config["display"];
    if (display["multi-sampling"]) {
      MultiSample(display["multi-sampling"].as<int>());
    }
    auto mode = display["mode"].as<std::string>();
    if (mode == "Full Screen") {
      FullScreen(display["size"]["width"].as<int>(),
                 display["size"]["height"].as<int>(),
                 display["title"].as<std::string>().c_str(),
                 display["monitor"].as<int>(),
                 display["refresh-rate"].as<int>());
    } else if (mode == "Full Screen Windowed") {
      FullScreenWindowed(display["title"].as<std::string>().c_str(),
                         display["monitor"].as<int>());
    } else if (mode == "Windowed") {
      Windowed(display["size"]["width"].as<int>(),
               display["size"]["height"].as<int>(),
               display["title"].as<std::string>().c_str());
    } else {
      FullScreenWindowed("ASAP Application", 0);
    }
    if (display["vsync"]) {
      EnableVsync(display["vsync"].as<bool>());
    }
  } else {
    FullScreenWindowed("ASAP Application", 0);
  }
}

void ImGuiRunner::SaveSetting() {
  YAML::Emitter out;
  out << YAML::BeginMap;
  {
    out << YAML::Key << "display";
    out << YAML::BeginMap;
    {
      out << YAML::Key << "title";
      out << YAML::Value << GetWindowTitle();
      out << YAML::Key << "mode";
      if (IsFullScreen()) {
        if (IsWindowed()) {
          out << YAML::Value << "Full Screen Windowed";
          out << YAML::Key << "monitor";
          out << YAML::Value << GetMonitorId();
        } else {
          out << YAML::Value << "Full Screen";
          out << YAML::Key << "size";
          int size[2];
          GetWindowSize(size);
          out << YAML::BeginMap;
          {
            out << YAML::Key << "width";
            out << YAML::Value << size[0];
            out << YAML::Key << "height";
            out << YAML::Value << size[1];
          }
          out << YAML::EndMap;
          out << YAML::Key << "monitor";
          out << YAML::Value << GetMonitorId();
          out << YAML::Key << "refresh-rate";
          out << YAML::Value << RefreshRate();
        }
      } else {
        out << YAML::Value << "Windowed";
        out << YAML::Key << "size";
        int size[2];
        GetWindowSize(size);
        out << YAML::BeginMap;
        {
          out << YAML::Key << "width";
          out << YAML::Value << size[0];
          out << YAML::Key << "height";
          out << YAML::Value << size[1];
        }
        out << YAML::EndMap;
      }
    }
    out << YAML::Key << "multi-sampling";
    out << YAML::Value << MultiSample();
    out << YAML::Key << "vsync";
    out << YAML::Value << Vsync();
    out << YAML::EndMap;
  }
  out << YAML::EndMap;

  auto display_settings =
      asap::fs::GetPathFor(asap::fs::Location::F_DISPLAY_SETTINGS);
  auto ofs = std::ofstream();
  ofs.open(display_settings.string());
  ofs << out.c_str() << std::endl;
  ofs.close();
}

}  // namespace asap