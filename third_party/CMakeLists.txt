# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

message(STATUS "${ASAP_LOG_PROJECT_HIERARCHY} >> third-party modules")

# Call `asap_add_package` to add third party packages using CPM, while
# automatically adding/removing them to the project hierarchy stack.
#
# Refer to https://github.com/cpm-cmake/CPM.cmake for the full usage.
#
# Example:
# ~~~
# asap_add_package(
#   NAME
#   magic_enum
#   GIT_TAG
#   master
#   GITHUB_REPOSITORY
#   abdes/magic_enum
#   OPTIONS
#   "MAGIC_ENUM_OPT_INSTALL ON")
# ~~~

# ------------------------------------------------------------------------------
# GSL
# ------------------------------------------------------------------------------

asap_add_package(NAME gsl GIT_TAG v4.0.0 GITHUB_REPOSITORY microsoft/GSL)

# ------------------------------------------------------------------------------
# fmt::fmt-header-only
# ------------------------------------------------------------------------------

asap_add_package(
  NAME
  fmtlib
  GIT_TAG
  10.2.1
  GITHUB_REPOSITORY
  fmtlib/fmt
  OPTIONS
  "FMT_INSTALL ON")

# ------------------------------------------------------------------------------
# asap::common
# ------------------------------------------------------------------------------

asap_add_package(
  NAME
  asap_common
  GIT_TAG
  master
  GITHUB_REPOSITORY
  asap-projects/asap-common
  OPTIONS
  "ASAP_COMMON_INSTALL ON"
  "ASAP_BUILD_TESTS OFF"
  "ASAP_BUILD_EXAMPLES OFF")

# ------------------------------------------------------------------------------
# asap::contract
# ------------------------------------------------------------------------------

asap_add_package(
  NAME
  asap_contract
  GIT_TAG
  master
  GITHUB_REPOSITORY
  asap-projects/asap-contract
  OPTIONS
  "ASAP_CONTRACT_INSTALL ON"
  "ASAP_BUILD_TESTS OFF"
  "ASAP_BUILD_EXAMPLES OFF")

# ------------------------------------------------------------------------------
# asap::logging
# ------------------------------------------------------------------------------

asap_add_package(
  NAME
  asap_logging
  GIT_TAG
  master
  GITHUB_REPOSITORY
  asap-projects/asap-logging
  OPTIONS
  "ASAP_LOGGING_INSTALL ON"
  "ASAP_BUILD_TESTS OFF"
  "ASAP_BUILD_EXAMPLES OFF")

# --------------------------------------------------------------------------------------------------
# GLFW
# --------------------------------------------------------------------------------------------------

asap_add_package(
  NAME
  glfw
  GIT_TAG
  master
  GITHUB_REPOSITORY
  glfw/glfw
  OPTIONS
  "GLFW_BUILD_EXAMPLES OFF"
  "GLFW_BUILD_TESTS OFF"
  "GLFW_BUILD_DOCS OFF")

# --------------------------------------------------------------------------------------------------
# GLM
# --------------------------------------------------------------------------------------------------

asap_add_package(
  NAME
  glm
  GIT_TAG
  master
  GITHUB_REPOSITORY
  g-truc/glm
  OPTIONS
  "GLM_ENABLE_CXX_17 ON"
  "GLM_BUILD_LIBRARY OFF")

# --------------------------------------------------------------------------------------------------
# date - used for time formatting in the logging widget
# --------------------------------------------------------------------------------------------------

asap_add_package(
  NAME
  date
  GIT_TAG
  master
  GITHUB_REPOSITORY
  HowardHinnant/date
  OPTIONS
  "USE_SYSTEM_TZ_DB ON"
  "ENABLE_DATE_TESTING OFF"
  "CMAKE_CXX_STANDARD 14")

# --------------------------------------------------------------------------------------------------
# cpptoml - used to save/load the configuration files
# --------------------------------------------------------------------------------------------------

asap_add_package(
  NAME
  tomlplusplus
  GIT_TAG
  v3.4.0
  GITHUB_REPOSITORY
  marzer/tomlplusplus
  OPTIONS
  "PROJECT_IS_TOP_LEVEL ON")
find_package(tomlplusplus REQUIRED)
