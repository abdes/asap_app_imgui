# ~~~
# SPDX-License-Identifier: BSD-3-Clause

# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

message("=> [third-party modules]")

# --------------------------------------------------------------------------------------------------
# gsl
# --------------------------------------------------------------------------------------------------

add_subdirectory(gsl)

# --------------------------------------------------------------------------------------------------
# fmt (must come before spdlog)
# --------------------------------------------------------------------------------------------------

# We want the spdlog install target to be generated even though we are using it as a submodule. The
# reason for that is that is spdlog and also we want our project to be self contained with all
# dependencies included.
set(FMT_INSTALL
    ON
    CACHE BOOL "Generate the install target.")

add_subdirectory(fmt)

# --------------------------------------------------------------------------------------------------
# spdlog
# --------------------------------------------------------------------------------------------------

# We want the spdlog install target to be generated even though we are using it as a submodule. The
# reason for that is that we want our project to be self contained with all dependencies included.
set(SPDLOG_INSTALL
    ON
    CACHE BOOL "Generate the spdlog install target")

set(SPDLOG_FMT_EXTERNAL
    ON
    CACHE BOOL "Use external fmt library instead of bundled")

if(WIN32)
  set(SPDLOG_WCHAR_SUPPORT
      ON
      CACHE BOOL "Support wchar api")
  set(SPDLOG_WCHAR_FILENAMES
      ON
      CACHE BOOL "Support wchar filenames")
endif()

add_subdirectory(spdlog)
