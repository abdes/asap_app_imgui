# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

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

# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")

set(public_headers)

set(sources
    "traits_logical_test.cpp"
    "flag_ops_test.cpp"
    "main.cpp"
    ${public_headers})

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

set(libraries ${META_PROJECT_NAME}::common gtest gmock)

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
  ${compile_options})

set_target_properties(${target} PROPERTIES
   CXX_STANDARD 11
   CXX_STANDARD_REQUIRED YES
   CXX_EXTENSIONS NO
)

# ------------------------------------------------------------------------------
# Add support for (optional) code quality tools
# ------------------------------------------------------------------------------

asap_configure_sanitizers(${target})
