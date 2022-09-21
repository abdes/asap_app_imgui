/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "app/imgui_runner.h"
#include "config/config.h"
#include "example_application.h"

#include <asap_app_imgui/version.h>
#include <logging/logging.h>

#include <iostream>

using asap::app::ImGuiRunner;

using asap_app_imgui::info::cNameVersion;
using asap_app_imgui::info::cProjectDescription;
using asap_app_imgui::info::cProjectName;

auto main(int argc, char **argv) -> int {
  auto &logger = asap::logging::Registry::GetLogger("main");

  asap::config::CreateDirectories();

  try {
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
