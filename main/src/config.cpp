//    Copyright The asap Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <config.h>

namespace bfs = boost::filesystem;

namespace asap {
namespace fs {


bfs::path GetPathFor(Location id) {
  switch (id) {
    case Location::D_USER_CONFIG: {
      auto p = bfs::current_path();
      p /= ".asap";
      return p;
    }
    case Location::F_DISPLAY_SETTINGS: {
      auto p = GetPathFor(Location::D_USER_CONFIG);
      p /= "display.yaml";
      return p;
    }
    case Location::F_LOG_SETTINGS: {
      auto p = GetPathFor(Location::D_USER_CONFIG);
      p /= "logging.yaml";
      return p;
    }
    case Location::F_DOCK_SETTINGS: {
      auto p = GetPathFor(Location::D_USER_CONFIG);
      p /= "docks.yaml";
      return p;
    }
    case Location::F_THEME_SETTINGS: {
      auto p = GetPathFor(Location::D_USER_CONFIG);
      p /= "theme.yaml";
      return p;
    }
  }
  // Worakround only for MSVC complaining
  return bfs::current_path().append("__unreachable__");
}

void CreateDirectories() {
  bfs::create_directories(GetPathFor(Location::D_USER_CONFIG));
}

}  // namespace fs
}  // namespace asap
