#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)

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
set(target asap_common)
set(target_export common)

# Exit here if required dependencies are not met
message(STATUS "Lib ${target}")


# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/common")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(public_headers
  "include/common/platform.h"
  "include/common/assert.h"
  "include/common/non_copiable.h"
  "include/common/logging.h"
  )

set(sources
  "src/assert.cpp"
  "src/logging.cpp"
  ${public_headers}
  )

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$"
  ${header_group} ${public_headers})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$"
  ${source_group} ${sources})


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

set(public_libraries spdlog Boost::boost)
if (WIN32)
  list(APPEND public_libraries dbghelp)
endif (WIN32)

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
  ${private_compile_options}
)


# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

if (OPTION_BUILD_TESTS)
  add_subdirectory(test)
endif()


# ------------------------------------------------------------------------------
# Code analysis
# ------------------------------------------------------------------------------

enable_cppcheck()
tidy_target(${target})


# ------------------------------------------------------------------------------
# API Documentation
# ------------------------------------------------------------------------------

configure_doxyfile(${target}
  "\"Common Module\""
  "\"A module to provide the absolute minimum required building blocks for the other modules such as portability, logging, assertions, etc...\""
  "${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include")

add_doxygen_target(${target})

install(
  DIRECTORY ${dox_root}/${target}
  DESTINATION ${INSTALL_DOC}
  COMPONENT docs
)
