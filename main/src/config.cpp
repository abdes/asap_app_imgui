//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <config.h>

namespace asap {
namespace fs {

asap::filesystem::path GetPathFor(Location id) {
  switch (id) {
    case Location::D_USER_CONFIG: {
      auto p = asap::filesystem::current_path();
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
  // Worakround only for MSVC complaining
  return asap::filesystem::current_path().append("__unreachable__");
}

void CreateDirectories() {
  asap::filesystem::create_directories(GetPathFor(Location::D_USER_CONFIG));
}

}  // namespace fs
}  // namespace asap
