# ~~~
# SPDX-License-Identifier: BSD-3-Clause

# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

# --------------------------------------------------------------------------------------------------
# Meta information about the this module
# --------------------------------------------------------------------------------------------------

asap_declare_module(
  MODULE_NAME
  "main"
  DESCRIPTION
  "Main module for the application program"
  GITHUB_REPO
  "https://github.com/asap/asap_app_imgui"
  AUTHOR_MAINTAINER
  "Abdessattar Sassi"
  VERSION_MAJOR
  "1"
  VERSION_MINOR
  "0"
  VERSION_PATCH
  "0")

# ==============================================================================
# Build instructions
# ==============================================================================

# --------------------------------------------------------------------------------------------------
# Main module target
# --------------------------------------------------------------------------------------------------

set(MODULE_TARGET_NAME "${META_PROJECT_NAME}")

asap_add_executable(
  ${MODULE_TARGET_NAME}
  WARNING
  SOURCES
  # Headers
  src/ui/fonts/fonts.h
  src/ui/fonts/material_design_icons.h
  src/ui/style/theme.h
  src/ui/log/sink.h
  src/ui/application_base.h
  src/runner_base.h
  src/imgui_runner.h
  src/application.h
  src/config.h
  src/shader.h
  # Sources
  #FONTS
  src/ui/fonts/fonts.h
  src/ui/fonts/Roboto-Black.cpp
  src/ui/fonts/Roboto-BlackItalic.cpp
  src/ui/fonts/Roboto-Bold.cpp
  src/ui/fonts/Roboto-BoldItalic.cpp
  src/ui/fonts/Roboto-Italic.cpp
  src/ui/fonts/Roboto-Light.cpp
  src/ui/fonts/Roboto-LightItalic.cpp
  src/ui/fonts/Roboto-Medium.cpp
  src/ui/fonts/Roboto-MediumItalic.cpp
  src/ui/fonts/Roboto-Regular.cpp
  src/ui/fonts/Roboto-Thin.cpp
  src/ui/fonts/Roboto-ThinItalic.cpp
  src/ui/fonts/Inconsolata-Regular.cpp
  src/ui/fonts/Inconsolata-Bold.cpp
  src/ui/fonts/material_design_icons.cpp
  src/ui/fonts/material_design_icons.h
  #
  src/ui/log/sink.cpp
  #
  src/runner_base.cpp
  src/imgui_runner.cpp
  src/application.cpp
  src/config.cpp
  src/ui/application_base.cpp
  src/ui/style/theme.cpp
  #
  src/main.cpp
  )

target_link_libraries(
  ${MODULE_TARGET_NAME}
  PRIVATE
  asap::common
  asap::logging
  cxxopts
  glm
  ${META_PROJECT_NAME}::imgui
  cpptoml
  date_interface
)

target_include_directories(${MODULE_TARGET_NAME} PRIVATE ${CMAKE_BINARY_DIR}/include)
target_include_directories(${MODULE_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Hacking the include directories when building th executable simply because of 
# Imgui backend files including "imgui.h" literally like that. We add the imgui 
# directory to the includes so that it will work.
#
# In other files, such as the ones we provide, we don't do that shit like that.
# Instead, we include files from Imgui library as <imgui/xxxxx>
target_include_directories(${MODULE_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/imgui/imgui)

target_compile_features(${MODULE_TARGET_NAME} PUBLIC cxx_std_17)

asap_add_sanitizers(${MODULE_TARGET_NAME})

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

#if (OPTION_BUILD_TESTS)
#  add_subdirectory(test)
#endif()


# --------------------------------------------------------------------------------------------------
# API Documentation
# --------------------------------------------------------------------------------------------------

asap_with_doxygen(
  ${META_PROJECT_NAME}_${META_MODULE_NAME} "\"Main Module\""
  "\"The main module for the application program.\""
  "${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include")

# asap_with_sphinx(${META_PROJECT_NAME}_${META_MODULE_NAME})

# ==================================================================================================
# Deployment instructions
# ==================================================================================================

#
# Deployment
#
# Executable
install(
  TARGETS ${MODULE_TARGET_NAME}
  RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT programs
  BUNDLE DESTINATION ${INSTALL_BIN} COMPONENT programs)
