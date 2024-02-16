/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "./config.h"

namespace asap::config {

auto GetPathFor(Location id) -> std::filesystem::path {
  switch (id) {
  case Location::D_USER_CONFIG: {
    auto p = std::filesystem::current_path();
    p /= ".asap";
    return p;
  }
  case Location::F_DISPLAY_SETTINGS: {
    auto p = GetPathFor(Location::D_USER_CONFIG);
    p /= "display.toml";
    return p;
  }
  case Location::F_LOG_SETTINGS: {
    auto p = GetPathFor(Location::D_USER_CONFIG);
    p /= "logging.toml";
    return p;
  }
  case Location::F_IMGUI_SETTINGS: {
    auto p = GetPathFor(Location::D_USER_CONFIG);
    p /= "imgui.ini";
    return p;
  }
  case Location::F_THEME_SETTINGS: {
    auto p = GetPathFor(Location::D_USER_CONFIG);
    p /= "theme.toml";
    return p;
  }
  }
  // Workaround only for MSVC complaining
  return std::filesystem::current_path().append("__unreachable__");
}

void CreateDirectories() {
  std::filesystem::create_directories(GetPathFor(Location::D_USER_CONFIG));
}

} // namespace asap::config
