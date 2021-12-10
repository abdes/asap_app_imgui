/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <filesystem>

namespace asap::fs {

enum class Location {
  D_USER_CONFIG,

  F_DISPLAY_SETTINGS,
  F_LOG_SETTINGS,
  F_IMGUI_SETTINGS,
  F_THEME_SETTINGS
};

std::filesystem::path GetPathFor(Location id);

void CreateDirectories();

} // namespace asap::fs
