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
set(META_VERSION_PATCH       "1")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_CMAKE_INIT_SHA      "${GIT_REV}")
# cmake-format: on

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)

# Determine if this is built as a subproject  or if it is the master project.
if(NOT DEFINED ${META_PROJECT_ID}_MASTER_PROJECT)
  set(${META_PROJECT_ID}_MASTER_PROJECT OFF)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(${META_PROJECT_ID}_MASTER_PROJECT ON)
  endif()
endif()

message(
  "=> Project : ${META_NAME_VERSION} - master: ${${META_PROJECT_ID}_MASTER_PROJECT}"
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

option(${META_PROJECT_ID}_INSTALL
       "Generate the install target for this project."
       ${${META_PROJECT_ID}_MASTER_PROJECT})

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
  LANGUAGES C CXX)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Create version file
file(WRITE "${PROJECT_BINARY_DIR}/VERSION" "${META_NAME_VERSION}")

# ------------------------------------------------------------------------------
# Additional CMake modules
# ------------------------------------------------------------------------------

# Register general cmake commands
include(AsapTargets)
include(BuildHelpers)
include(GenerateExportHeader)
include(CompileOptions)
include(GoogleSanitizers)
include(CodeCoverage)
include(Valgrind)
include(ClangFormat)
include(ClangTidy)

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

# ------------------------------------------------------------------------------
# Deployment/installation setup
# ------------------------------------------------------------------------------

# Get project name
set(project ${META_PROJECT_NAME})

# Check for system dir install
set(SYSTEM_DIR_INSTALL FALSE)
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr" OR "${CMAKE_INSTALL_PREFIX}"
                                                STREQUAL "/usr/local")
  set(SYSTEM_DIR_INSTALL TRUE)
  set(ASAP_INSTALL_PREFIX_FULL_PATH ${CMAKE_INSTALL_PREFIX})
else()
  set(ASAP_INSTALL_PREFIX_FULL_PATH ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX})
endif()

# Installation paths
include(GNUInstallDirs)
# cmake-format: off
if(UNIX AND SYSTEM_DIR_INSTALL)
  # Install into the system (/usr/bin or /usr/local/bin)
  set(ASAP_INSTALL_ROOT      "${CMAKE_INSTALL_DATAROOTDIR}/${project}")       # /usr/[local]/share/<project>
  set(ASAP_INSTALL_LIB       "${CMAKE_INSTALL_LIBDIR}")                       # /usr/[local]/lib
  set(ASAP_INSTALL_SHARED    "${ASAP_INSTALL_LIB}")                           # /usr/[local]/lib
  set(ASAP_INSTALL_CMAKE     "${CMAKE_INSTALL_DATAROOTDIR}/cmake/${project}") # /usr/[local]/share/cmake/<project>
  set(ASAP_INSTALL_PKGCONFIG "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig")        # /usr/[local]/share/pkgconfig
  set(ASAP_INSTALL_EXAMPLES  "${ASAP_INSTALL_ROOT}")                          # /usr/[local]/share/<project>
  set(ASAP_INSTALL_DATA      "${ASAP_INSTALL_ROOT}")                          # /usr/[local]/share/<project>/data
  set(ASAP_INSTALL_BIN       "${CMAKE_INSTALL_BINDIR}")                       # /usr/[local]/bin
  set(ASAP_INSTALL_INCLUDE   "${CMAKE_INSTALL_INCLUDEDIR}")                   # /usr/[local]/include
  set(ASAP_INSTALL_DOC       "${CMAKE_INSTALL_DOCDIR}")                       # /usr/[local]/share/doc/<project>
  set(ASAP_INSTALL_SHORTCUTS "${CMAKE_INSTALL_DATAROOTDIR}/applications")     # /usr/[local]/share/applications
  set(ASAP_INSTALL_ICONS     "${CMAKE_INSTALL_DATAROOTDIR}/pixmaps")          # /usr/[local]/share/pixmaps
  set(ASAP_INSTALL_INIT      "/etc/init")                                     # /etc/init (upstart init scripts)
else()
  # Install into local directory
  set(ASAP_INSTALL_ROOT      ".")                                           # ./
  set(ASAP_INSTALL_LIB       "lib")                                         # ./lib
  set(ASAP_INSTALL_SHARED    "${ASAP_INSTALL_LIB}")                         # ./lib
  set(ASAP_INSTALL_CMAKE     "${ASAP_INSTALL_ROOT}/share/cmake/${project}") # ./share/cmake/<project>
  set(ASAP_INSTALL_PKGCONFIG "${ASAP_INSTALL_ROOT}/share/pkgconfig")        # ./share/pkgconfig
  set(ASAP_INSTALL_EXAMPLES  "${ASAP_INSTALL_ROOT}")                        # ./
  set(ASAP_INSTALL_DATA      "${ASAP_INSTALL_ROOT}")                        # ./data
  set(ASAP_INSTALL_BIN       "bin")                                         # ./bin
  set(ASAP_INSTALL_INCLUDE   "include")                                     # ./include
  set(ASAP_INSTALL_DOC       "doc")                                         # ./doc
  set(ASAP_INSTALL_SHORTCUTS "misc")                                        # ./misc
  set(ASAP_INSTALL_ICONS     "misc")                                        # ./misc
  set(ASAP_INSTALL_INIT      "misc")                                        # ./misc
endif()
# cmake-format: on

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
# Third party modules
# ------------------------------------------------------------------------------

add_subdirectory(third_party)

# ------------------------------------------------------------------------------
# Clean compiler settings and options
# ------------------------------------------------------------------------------

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  # remove default warning level from CMAKE_CXX_FLAGS_INIT CMake adds compiler
  # warning levels by default and for MSVC, it uses /W3 which we want to
  # override with /W4. The override does make MSVC complain though, so we just
  # strip any argument already added by cmake before we set ours.
  string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
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
# Test targets
# ------------------------------------------------------------------------------

# enable CTest.  This will set BUILD_TESTING to ON unless otherwise specified on
# the command line
include(CTest)
enable_testing()
asap_add_code_coverage_all_targets(EXCLUDE */test/* *googlemock* *googletest*
                                   /usr/*)

include(FetchContent)
# We make sure that we have 'third_party' in the name so that the targets get
# excluded from the generated target lists for the various tools.
set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/third_party_deps)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG # GoogleTest now follows the Abseil Live at Head philosophy. We
          # recommend using the latest commit in the main branch in your
          # projects.
          origin/main)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
set(INSTALL_GTEST
    OFF
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# ==============================================================================
# Documentation - doxygen, sphinx/breathe/exhale
# ==============================================================================

# Doxygen
set(DOXYGEN_BUILD_DIR "${CMAKE_BINARY_DIR}/dox")
include(DoxGeneration)

# Sphinx/breathe/exhale
set(SPHINX_BUILD_DIR "${CMAKE_BINARY_DIR}/sphinx")
include(SphinxGeneration)

# Setup sphinx doc master target and add other submodules as dependencies
if(SPHINX_FOUND)
  asap_with_sphinx(master)

  # Add a target for copying the index.html from the doc dir to the sphinx build
  # dir. A dependency on this target will be added to the master sphinx target.
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sphinx/index.html
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/doc/index.html
            ${CMAKE_CURRENT_BINARY_DIR}/sphinx/index.html
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/doc/index.html)
  add_custom_target(copy_doc_index ALL
                    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/sphinx/index.html)

  add_dependencies(
    master_sphinx
    copy_doc_index
    # Add more submodule documentation targets after this, using variables in
    # the target names consistently with the module's CMakeLists.txt.
  )

endif()

# ==============================================================================
# Project modules
# ==============================================================================

# Add project modules here

# ==============================================================================
# Code analyzers: clang-tidy, cppcheck, valgrind, sanitizers, etc...
# ==============================================================================

asap_setup_clang_format()
# Enable this target when the project has library with source files (.cpp)
#asap_create_clang_tidy_targets()

# ==============================================================================
# Deployment (global project files)
# ==============================================================================

if(${META_PROJECT_ID}_INSTALL)
  set(runtime "${META_PROJECT_NAME}_runtime")
  set(dev "${META_PROJECT_NAME}_dev")
  set(meta "${META_PROJECT_NAME}_meta")
  set(data "${META_PROJECT_NAME}_date")

  # Install version file
  install(
    FILES "${PROJECT_BINARY_DIR}/VERSION"
    DESTINATION ${ASAP_INSTALL_ROOT}
    COMPONENT ${runtime})

  # Deploy generated top level headers (always in `asap` directory even when
  # forked)
  install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/asap
    DESTINATION ${ASAP_INSTALL_INCLUDE}
    COMPONENT ${dev})

  # Install the project meta files
  install(
    FILES AUTHORS
    DESTINATION ${ASAP_INSTALL_ROOT}
    COMPONENT meta)
  install(
    FILES LICENSE
    DESTINATION ${ASAP_INSTALL_ROOT}
    COMPONENT meta)
  install(
    FILES README.md
    DESTINATION ${ASAP_INSTALL_ROOT}
    COMPONENT meta)

  # Install master docs
  install(
    DIRECTORY ${SPHINX_BUILD_DIR}/master
    DESTINATION ${ASAP_INSTALL_DOC}
    COMPONENT master_docs)

  # Install data
  if (EXISTS ${PROJECT_SOURCE_DIR}/data)
    install(
      DIRECTORY ${PROJECT_SOURCE_DIR}/data
      DESTINATION ${ASAP_INSTALL_DATA}
      COMPONENT ${data})
  endif()
endif()
