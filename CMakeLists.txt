# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

# ------------------------------------------------------------------------------
# CMake basic options
# ------------------------------------------------------------------------------

# It's time to move on! We require 3.14 for modern CMake with nice quality of
# life features and simpler scripts.
cmake_minimum_required(VERSION 3.14)

# List of directories specifying a search path for CMake modules to be loaded by
# the the include() or find_package() commands before checking the default
# modules that come with CMake.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# ------------------------------------------------------------------------------
# Project description and (meta) information
# ------------------------------------------------------------------------------

# Get git revision
find_package(Git REQUIRED)
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
if(NOT GIT_SHA1)
  set(GIT_REV "0")
endif()

# Meta information about the project
# cmake-format: off
set(META_PROJECT_NAME        "asap")
set(META_PROJECT_DESCRIPTION "Instantly start with a fully loaded CMake project")
set(META_AUTHOR_ORGANIZATION "The Authors")
set(META_GITHUB_REPO         "https://github.com/abdes/asap")
set(META_AUTHOR_DOMAIN       "https://github.com/abdes/asap")
set(META_AUTHOR_MAINTAINER   "Abdessattar Sassi")
set(META_VERSION_MAJOR       "4")
set(META_VERSION_MINOR       "1")
set(META_VERSION_PATCH       "7")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_CMAKE_INIT_SHA      "${GIT_REV}")
# cmake-format: on

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)

# Determine if this is built as a subproject  or if it is the master project.
if(NOT DEFINED ${META_PROJECT_ID}_IS_MASTER_PROJECT)
  set(${META_PROJECT_ID}_IS_MASTER_PROJECT OFF)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(${META_PROJECT_ID}_IS_MASTER_PROJECT ON)
  endif()
endif()

message(
  "=> Project : ${META_NAME_VERSION} (IS_MASTER: ${${META_PROJECT_ID}_IS_MASTER_PROJECT})"
)

# ------------------------------------------------------------------------------
# Project configuration options
# ------------------------------------------------------------------------------

# Project options
# cmake-format: off
option(BUILD_SHARED_LIBS        "Build shared instead of static libraries."              ON)
option(ASAP_BUILD_TESTS         "Build tests."                                           OFF)
option(ASAP_BUILD_EXAMPLES      "Build examples."                                        OFF)
option(ASAP_WITH_GOOGLE_ASAN    "Instrument code with address sanitizer"                 OFF)
option(ASAP_WITH_GOOGLE_UBSAN   "Instrument code with undefined behavior sanitizer"      OFF)
option(ASAP_WITH_GOOGLE_TSAN    "Instrument code with thread sanitizer"                  OFF)
option(ASAP_WITH_VALGRIND       "Builds targets with valgrind profilers added"           OFF)
# cmake-format: on

# ------------------------------------------------------------------------------
# Project Declaration
# ------------------------------------------------------------------------------

# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "${META_PROJECT_NAME}")

# Declare project
project(
  ${META_PROJECT_NAME}
  VERSION ${META_VERSION}
  DESCRIPTION ${META_PROJECT_DESCRIPTION}
  HOMEPAGE_URL ${META_GITHUB_REPO}
  LANGUAGES C CXX)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ------------------------------------------------------------------------------
# Additional CMake modules
# ------------------------------------------------------------------------------

# Register general cmake commands
include(AsapTargets)
include(BuildHelpers)
include(GenerateExportHeader)
include(CompileOptions)

# Override the ${META_PROJECT_ID}_INSTALL option to ON/OFF to respectively force
# install/no-install behavior for this project. This is particularly useful when
# this project is used as a sub-project and the user publicly depends on it and
# wants to have a self-contained install.
include(AsapInstall)

# The default build type provided by CMake is to include no compiler flags for
# optimization. For some projects you may want to set a default build type so
# that you do not have to remember to set it.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE
      RelWithDebInfo
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

# If no contract mode is specified, we set the contract mode based on the build
# type.
if(NOT OPTION_CONTRACT_MODE)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPTION_CONTRACT_MODE
        "AUDIT"
        CACHE STRING "Contract mode for assertions (common module).")
  elseif(CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
    set(OPTION_CONTRACT_MODE
        "DEFAULT"
        CACHE STRING "Contract mode for assertions (common module).")
  else()
    set(OPTION_CONTRACT_MODE
        "OFF"
        CACHE STRING "Contract mode for assertions (common module).")
  endif()
endif()
message(
  "-- Building for ${CMAKE_BUILD_TYPE} with OPTION_CONTRACT_MODE : ${OPTION_CONTRACT_MODE}"
)

include(FetchContent)
# We make sure that we have 'third_party' in the name so that the targets get
# excluded from the generated target lists for the various tools.
set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/third_party_deps)

# ------------------------------------------------------------------------------
# Testing
# ------------------------------------------------------------------------------

if(ASAP_BUILD_TESTS)
  include(GoogleSanitizers)
  include(CodeCoverage)
  # Enable this target when the project has test cases
  # asap_add_code_coverage_all_targets( EXCLUDE */test/* *googlemock*
  # *googletest* /usr/*)
  include(Valgrind)
  include(FetchGoogleTest)
  include(GoogleTest)
  include(CTest)
endif()

# ------------------------------------------------------------------------------
# RPATH setup
# ------------------------------------------------------------------------------

# Set runtime path
set(CMAKE_SKIP_BUILD_RPATH FALSE) # Add absolute path to all dependencies for
                                  # BUILD
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

if(NOT SYSTEM_DIR_INSTALL)
  # Find libraries relative to binary
  if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path/../../../${ASAP_INSTALL_LIB}")
  else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN/${ASAP_INSTALL_LIB}")
  endif()
endif()

# ------------------------------------------------------------------------------
# Top level code generation
# ------------------------------------------------------------------------------

# Generate version-header
configure_file(templates/version.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/include/asap/asap-version.h)

# Generate a clangd configuration file that points to the compilation database
# in the cmake build directory. We need to do this as the build directory is
# different for every preset and can be different as well when the user decides
# to build somewhere else. Currently we cannot configure this properly in vscode
# settings. See https://github.com/clangd/vscode-clangd/issues/48
configure_file(.clangd.in ${CMAKE_SOURCE_DIR}/.clangd @ONLY)

# ------------------------------------------------------------------------------
# Documentation - doxygen, sphinx/breathe/exhale
# ------------------------------------------------------------------------------

# Doxygen
set(DOXYGEN_BUILD_DIR "${CMAKE_BINARY_DIR}/dox")
include(DoxGeneration)

# Sphinx/breathe/exhale
set(SPHINX_BUILD_DIR "${CMAKE_BINARY_DIR}/sphinx")
include(SphinxGeneration)

# ==============================================================================
# Third party modules
# ==============================================================================

add_subdirectory(third_party)

# ==============================================================================
# Project modules
# ==============================================================================

# Add project modules here

# ==============================================================================
# Code analyzers: clang-tidy, cppcheck, valgrind, sanitizers, etc...
#
# Call after sub-modules have been added so that source code files can be
# properly collected for analysis.
# ==============================================================================

if(${META_PROJECT_ID}_IS_MASTER_PROJECT)
  include(ClangFormat)
  include(ClangTidy)

  # Enable this target when the project has header or source files (.h or .cpp)
  # asap_setup_clang_format()

  # Enable this target when the project has source files (.cpp)
  # asap_create_clang_tidy_targets()
endif()

# ==============================================================================
# Deployment (global project files)
# ==============================================================================

asap_top_level_install()
