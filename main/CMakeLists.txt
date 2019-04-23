#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)

# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------

set(IDE_FOLDER "Main")

# ------------------------------------------------------------------------------
# External dependencies
# ------------------------------------------------------------------------------

# find_package(THIRDPARTY REQUIRED)


# ==============================================================================
# Build instructions
# ==============================================================================

# Target name
set(target asap_app)

# Exit here if required dependencies are not met
message(STATUS "Program ${target}")


# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

list(APPEND headers
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
  )

set(sources
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
  ${headers}
  )

# Group source files
set(header_group "Header Files")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$"
  ${header_group} ${headers})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$"
  ${source_group} ${sources})


# ------------------------------------------------------------------------------
# Include dirs
# ------------------------------------------------------------------------------

set(include_dirs ${include_path})

# ------------------------------------------------------------------------------
# Compile definitions / options
# ------------------------------------------------------------------------------

set(compile_definitions)
set(compile_options)

# ------------------------------------------------------------------------------
# Libraries
# ------------------------------------------------------------------------------

set(libraries
  asap::common
  asap::filesystem
  cxxopts
  glm
  asap::imgui
  cpptoml
  date_interface
  )


# ------------------------------------------------------------------------------
# Create targets
# ------------------------------------------------------------------------------

asap_executable(
  TARGET
  ${target}
  SOURCES
  ${sources}
  INCLUDE_DIRS
  ${include_dirs}
  LIBRARIES
  ${libraries}
  COMPILE_DEFINITIONS
  ${compile_definitions}
  COMPILE_OPTIONS
  ${compile_options}
)


# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

#if (OPTION_BUILD_TESTS)
#  add_subdirectory(test)
#endif()


# ------------------------------------------------------------------------------
# Code analysis
# ------------------------------------------------------------------------------

enable_cppcheck()
tidy_target(${target})


# ------------------------------------------------------------------------------
# API Documentation
# ------------------------------------------------------------------------------

configure_doxyfile(${target}
  "\"Main\""
  "\"The application main entry point.\""
  "${source_path}")

add_doxygen_target(${target})

install(
  DIRECTORY ${DOXYGEN_BUILD_DIR}/${target}
  DESTINATION ${INSTALL_DOC}
  COMPONENT docs
)
