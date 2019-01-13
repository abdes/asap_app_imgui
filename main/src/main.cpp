//    Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <iostream>

#include <cxxopts.hpp>

#include <asap/asap-version.h>
#include <common/logging.h>
#include <imgui_runner.h>
#include <config.h>
#include <application.h>


using asap::ImGuiRunner;


int main(int argc, char **argv) {
  auto &logger = asap::logging::Registry::GetLogger("main");

  asap::fs::CreateDirectories();

  try {
    //
    // Handle program options
    //
    // TODO: UTF-8 to be tested on Windows
    cxxopts::Options options(ASAP_PROJECT_NAME, ASAP_PROJECT_DESCRIPTION);
    options.add_options()
      ("v,version", "Show version")
      ("h,help", "Show usage information")
      ("t,test", "Test option with value", cxxopts::value<std::string>())
      ("c,chinese", "中文帮助文本", cxxopts::value<std::string>())
      ;
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help({}) << std::endl;
      exit(0);
    }

    if (result.count("version")) {
      std::cout << ASAP_NAME_VERSION << std::endl;
      exit(0);
    }

    // Test code for the option with value
    if (result.count("test")) {
      std::cout << "test = " << result["test"].as<std::string>() << std::endl;
    }
    // End Test code


    ASLOG_TO_LOGGER(logger, info, "starting ImGui application...");
    asap::Application app;
    //
    // Start the ImGui runner
    //
    ImGuiRunner runner(app, [&]() {
      // Shutdown
      ASLOG_TO_LOGGER(logger, info, "shutdown complete");
    });
    runner.Run();
  } catch (std::exception &e) {
    ASLOG_TO_LOGGER(logger, error, "Error: {}", e.what());
    return -1;
  } catch (...) {
    ASLOG_TO_LOGGER(logger, error, "Unknown error!");
    return -1;
  }

  return 0;
}
