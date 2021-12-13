/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "app/imgui_runner.h"
#include "config/config.h"
#include "example_application.h"

#include <cxxopts.hpp>

#include <asap/asap-version.h>
#include <logging/logging.h>

#include <iostream>

using asap::app::ImGuiRunner;

auto main(int argc, char **argv) -> int {
  auto &logger = asap::logging::Registry::GetLogger("main");

  asap::config::CreateDirectories();

  try {
    //
    // Handle program options
    //
    cxxopts::Options options(ASAP_PROJECT_NAME, ASAP_PROJECT_DESCRIPTION);
    options.add_options()
        // clang-format off
      ("v,version", "Show version")
      ("h,help", "Show usage information")
      ("t,test", "Test option with value", cxxopts::value<std::string>())
        // clang-format on
        ;
    auto result = options.parse(argc, argv);

    if (result.count("help") != 0U) {
      std::cout << options.help({}) << std::endl;
      exit(0);
    }

    if (result.count("version") != 0U) {
      std::cout << ASAP_NAME_VERSION << std::endl;
      exit(0);
    }

    // Test code for the option with value
    if (result.count("test") != 0U) {
      std::cout << "test = " << result["test"].as<std::string>() << std::endl;
    }
    // End Test code

    ASLOG_TO_LOGGER(logger, info, "starting ImGui application...");
    ExampleApplication app;
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
