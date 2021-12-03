# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

# Meta information about the project
# cmake-format: off
set(META_MODULE_NAME                "logging")
set(META_MODULE_DESCRIPTION         "Logging functionality for asap based projects")
set(META_MODULE_GITHUB_REPO         "asap_logging")
set(META_MODULE_VERSION_MAJOR       "0")
set(META_MODULE_VERSION_MINOR       "1")
set(META_MODULE_VERSION_PATCH       "1")
set(META_MODULE_VERSION_REVISION    "${MODULE_GIT_REV}")
set(META_MODULE_VERSION             "${META_MODULE_VERSION_MAJOR}.${META_MODULE_VERSION_MINOR}.${META_MODULE_VERSION_PATCH}")
set(META_MODULE_NAME_VERSION        "${META_MODULE_PROJECT_NAME} v${META_MODULE_VERSION}")
# cmake-format: on
message("=> [module: common ${META_MODULE_VERSION}]")

# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------

set(IDE_FOLDER "Libraries")

# ------------------------------------------------------------------------------
# External dependencies
# ------------------------------------------------------------------------------

# find_package(THIRDPARTY REQUIRED)

# ==============================================================================
# Build instructions
# ==============================================================================

# Target name
set(target asap_logging)
set(target_export logging)

# Exit here if required dependencies are not met

# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/logging")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(public_headers
    "include/logging/logging.h"
)

set(sources "src/logging.cpp" ${public_headers})

# ------------------------------------------------------------------------------
# Include dirs
# ------------------------------------------------------------------------------

set(public_include_dirs)
set(private_include_dirs)

# ------------------------------------------------------------------------------
# Compile definitions / options
# ------------------------------------------------------------------------------

set(private_compile_definitions)
set(private_compile_options)

# ------------------------------------------------------------------------------
# Libraries
# ------------------------------------------------------------------------------

set(public_libraries ${META_PROJECT_NAME}::common spdlog::spdlog)

# ------------------------------------------------------------------------------
# Create targets
# ------------------------------------------------------------------------------

asap_library(
  TARGET
  ${target}
  EXPORT_NAME
  ${target_export}
  SOURCES
  ${sources}
  PUBLIC_INCLUDE_DIRS
  ${public_include_dirs}
  PRIVATE_INCLUDE_DIRS
  ${private_include_dirs}
  PUBLIC_LIBRARIES
  ${public_libraries}
  PRIVATE_COMPILE_DEFINITIONS
  ${private_compile_definitions}
  PRIVATE_COMPILE_OPTIONS
  ${private_compile_options})

set_target_properties(${target} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

if(OPTION_BUILD_TESTS)
  add_subdirectory(test)
endif()

# ------------------------------------------------------------------------------
# Code generation
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# Add support for (optional) code quality tools
# ------------------------------------------------------------------------------

asap_configure_sanitizers(${target})
tidy_target(${target})

# ------------------------------------------------------------------------------
# API Documentation
# ------------------------------------------------------------------------------

asap_with_doxygen(
  ${target}
  "\"Logging Module\""
  "\"Provides logging functionality based on spdlog.\""
  "${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include")

asap_with_sphinx(${target})
