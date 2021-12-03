# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

# Meta information about the project
# cmake-format: off
set(META_MODULE_NAME                "common")
set(META_MODULE_DESCRIPTION         "Common functionality for asap based projects")
set(META_MODULE_GITHUB_REPO         "asap_common")
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

# Project options
option(ASAP_USE_ASSERTS "Enable ASSERT macros." ON)
option(ASAP_USE_SYSTEM_ASSERTS
       "Use system assert() to implement ASSERT macros." OFF)

# This module's specific Environment detection
include(CheckIncludeFileCXX)
check_include_file_cxx("execinfo.h" ASAP_HAVE_EXECINFO_H)
include(CheckCXXSourceCompiles)
check_cxx_source_compiles(
  "
  #define _POSIX_C_SOURCE 200112L
  #include <unistd.h>
  #if !defined(_POSIX_VERSION) || (_POSIX_VERSION < 200112L)
  posix_200112L_unsupported
  #endif
  int main(int, char**) {
  }"
  ASAP_POSIX
  FAIL_REGEX
  "posix_200112L_unsupported")

# ------------------------------------------------------------------------------
# External dependencies
# ------------------------------------------------------------------------------

# find_package(THIRDPARTY REQUIRED)

# ==============================================================================
# Build instructions
# ==============================================================================

# Target name
set(target asap_common)
set(target_export common)

# Exit here if required dependencies are not met

# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/common")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(public_headers
    "include/common/platform.h"
    "include/common/compilers.h"
    "include/common/flag_ops.h"
    # traits module
    "include/common/traits/logical.h")

set(sources "src/place_holder_to_be_removed.cpp" ${public_headers})

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

# GSL is so basic that we make it available for any module that uses the 
# 'common' submodule.
set(public_libraries Microsoft.GSL::GSL)

if(WIN32)
  list(APPEND public_libraries dbghelp)
endif(WIN32)

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

# Generate config-header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/common/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/include/common/config.h)

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
  "\"Common Module\""
  "\"Provides common basic building blocks such as portability, assertions, etc.\""
  "${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include")

asap_with_sphinx(${target})
