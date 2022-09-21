# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

macro(asap_create_format_targets)
  cpmaddpackage(NAME Format.cmake VERSION 1.7.3 GITHUB_REPOSITORY
                TheLartians/Format.cmake)
endmacro()
