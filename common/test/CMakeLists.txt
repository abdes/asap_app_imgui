#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)

# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------

set(IDE_FOLDER "Tests")

# ------------------------------------------------------------------------------
# External dependencies
# ------------------------------------------------------------------------------

# find_package(THIRDPARTY REQUIRED)


# ==============================================================================
# Build instructions
# ==============================================================================

# Target name
set(target asap_common_test)

# Exit here if required dependencies are not met
message(STATUS "Test Program ${target}")


# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(public_headers)

set(sources
  "logging_test.cpp"
  "main.cpp"
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

set(include_dirs)

# ------------------------------------------------------------------------------
# Compile definitions / options
# ------------------------------------------------------------------------------

set(compile_definitions)
set(compile_options)

# ------------------------------------------------------------------------------
# Libraries
# ------------------------------------------------------------------------------

set(libraries ${META_PROJECT_NAME}::common Catch2)

# ------------------------------------------------------------------------------
# Create targets
# ------------------------------------------------------------------------------

asap_test_executable(
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
