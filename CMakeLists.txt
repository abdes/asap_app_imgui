#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)

# ------------------------------------------------------------------------------
# CMake basic options
# ------------------------------------------------------------------------------

# We require 3.0 for modern, target-based CMake.
# We require 3.1 for the use of CXX_STANDARD in our targets.
# We require 3.6 for CXX_CLANG_TIDY
# We require 3.10 for CMAKE_CXX_CPPCHECK
if (OPTION_CPPCHECK)
  cmake_minimum_required(VERSION 3.10)
elseif(OPTION_CLANG_TIDY)
  cmake_minimum_required(VERSION 3.6)
else()
  cmake_minimum_required(VERSION 3.5)
endif()

# List of directories specifying a search path for CMake modules to be loaded by
# the the include() or find_package() commands before checking the default modules
# that come with CMake.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/common/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Register general cmake commands
include(GenerateExportHeader)
include(WriteCompilerDetectionHeader)
include(ExternalProject)
find_package(Git REQUIRED)

# Register custom cmake commands
include(GetGitRevisionDescription)
include(DownloadProject)
include(BuildHelpers)


# ------------------------------------------------------------------------------
# Project description and (meta) information
# ------------------------------------------------------------------------------

# Get git revision
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
if(NOT GIT_SHA1)
  set(GIT_REV "0")
endif()

# Meta information about the project
set(META_PROJECT_NAME        "asap")
set(META_PROJECT_DESCRIPTION "Instantly start with a fully loaded CMake project")
set(META_AUTHOR_ORGANIZATION "The Authors")
set(META_GITHUB_USER         "abdes")
set(META_GITHUB_REPO         "asap")
set(META_AUTHOR_DOMAIN       "https://github.com/abdes/asap")
set(META_AUTHOR_MAINTAINER   "Abdessattar Sassi (abde.sassi gmail account)")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "9")
set(META_VERSION_PATCH       "2")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_CMAKE_INIT_SHA      "${GIT_REV}")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)

message("=> Project : ${META_NAME_VERSION}")



# ------------------------------------------------------------------------------
# Project configuration options
# ------------------------------------------------------------------------------

# Project options
option(BUILD_SHARED_LIBS     "Build shared instead of static libraries."              ON)
option(OPTION_SELF_CONTAINED "Create a self-contained install with all dependencies." OFF)
option(OPTION_BUILD_TESTS    "Build tests."                                           ON)
option(OPTION_BUILD_DOCS     "Build documentation."                                   OFF)
option(OPTION_BUILD_EXAMPLES "Build examples."                                        OFF)
option(OPTION_CLANG_TIDY     "Analyze code with clang-tidy."                          OFF)
option(OPTION_CPPCHECK       "Analyze code with CppCheck."                            OFF)
option(OPTION_GOOGLE_ASAN    "Instrument code with address sanitizer"                 OFF)
option(OPTION_GOOGLE_USAN    "Instrument code with memory sanitizer"                  OFF)
option(OPTION_GOOGLE_TSAN    "Instrument code with thread sanitizer"                  OFF)


# The default build type provided by CMake is to include no compiler flags for
# optimization. For some projects you may want to set a default build type so
# that you do not have to remember to set it.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()


# ------------------------------------------------------------------------------
# Project Declaration
# ------------------------------------------------------------------------------

# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "${META_PROJECT_NAME}")

# Declare project
project(${META_PROJECT_NAME} VERSION ${META_VERSION} LANGUAGES C CXX)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Create version file
file(WRITE "${PROJECT_BINARY_DIR}/VERSION" "${META_NAME_VERSION}")


# ------------------------------------------------------------------------------
# Third party modules
# ------------------------------------------------------------------------------

add_subdirectory(third_party)


# ------------------------------------------------------------------------------
# Compiler settings and options
# ------------------------------------------------------------------------------

include(CompileOptions)


# ------------------------------------------------------------------------------
# Code analyzers: clang-tidy, cppcheck, valgrind, sanitizers, etc...
# ------------------------------------------------------------------------------

include(ClangTidy)
include(CppCheck)
include(GoogleSanitizers)
include(Valgrind)


# ------------------------------------------------------------------------------
# Top level code generation
# ------------------------------------------------------------------------------

# Generate version-header
configure_file(
  templates/version.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h
)

# Generate feature-header
message(STATUS "Generating compiler feature detection header...")
set(feature_file "${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-features.h")
write_compiler_detection_header(
  FILE ${feature_file}
  PREFIX ${META_PROJECT_ID}
  COMPILERS AppleClang Clang GNU MSVC
  FEATURES cxx_constexpr cxx_final cxx_noexcept cxx_nullptr cxx_sizeof_member cxx_thread_local
  ALLOW_UNKNOWN_COMPILERS
  ALLOW_UNKNOWN_COMPILER_VERSIONS
)


# ------------------------------------------------------------------------------
# Test targets
# ------------------------------------------------------------------------------

# enable CTest.  This will set BUILD_TESTING to ON unless otherwise specified
# on the command line
include(CTest)
include(third_party/catch2/contrib/Catch.cmake)
enable_testing()


# ------------------------------------------------------------------------------
# Documentation - doxygen, sphinx/breathe/exhale
# ------------------------------------------------------------------------------

## Doxygen
include(FindDoxygen)
set(DOXYGEN_BUILD_DIR "${CMAKE_BINARY_DIR}/dox")
include (DoxGeneration)

## Sphinx/breathe/exhale
include(FindSphinx)
# sphinx build dir
set(SPHINX_BUILD_DIR "${CMAKE_BINARY_DIR}/sphinx")
# Sphinx cache with pickled ReST documents
set(SPHINX_CACHE_DIR "${SPHINX_BUILD_DIR}/_doctrees")
# HTML output directory
set(SPHINX_HTML_DIR "${SPHINX_BUILD_DIR}/html")
include (SphinxGeneration)


# ------------------------------------------------------------------------------
# Deployment/installation setup
# ------------------------------------------------------------------------------

# Get project name
set(project ${META_PROJECT_NAME})

# Check for system dir install
set(SYSTEM_DIR_INSTALL FALSE)
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr" OR "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  set(SYSTEM_DIR_INSTALL TRUE)
endif()

# Installation paths
if(UNIX AND SYSTEM_DIR_INSTALL)
  # Install into the system (/usr/bin or /usr/local/bin)
  set(INSTALL_ROOT      "share/${project}")       # /usr/[local]/share/<project>
  set(INSTALL_CMAKE     "share/${project}/cmake") # /usr/[local]/share/<project>/cmake
  set(INSTALL_EXAMPLES  "share/${project}")       # /usr/[local]/share/<project>
  set(INSTALL_DATA      "share/${project}")       # /usr/[local]/share/<project>
  set(INSTALL_BIN       "bin")                    # /usr/[local]/bin
  set(INSTALL_SHARED    "lib")                    # /usr/[local]/lib
  set(INSTALL_LIB       "lib")                    # /usr/[local]/lib
  set(INSTALL_INCLUDE   "include")                # /usr/[local]/include
  set(INSTALL_DOC       "share/doc/${project}")   # /usr/[local]/share/doc/<project>
  set(INSTALL_SHORTCUTS "share/applications")     # /usr/[local]/share/applications
  set(INSTALL_ICONS     "share/pixmaps")          # /usr/[local]/share/pixmaps
  set(INSTALL_INIT      "/etc/init")              # /etc/init (upstart init scripts)
else()
  # Install into local directory
  set(INSTALL_ROOT      ".")                      # ./
  set(INSTALL_CMAKE     "cmake")                  # ./cmake
  set(INSTALL_EXAMPLES  ".")                      # ./
  set(INSTALL_DATA      ".")                      # ./
  set(INSTALL_BIN       ".")                      # ./
  set(INSTALL_SHARED    "lib")                    # ./lib
  set(INSTALL_LIB       "lib")                    # ./lib
  set(INSTALL_INCLUDE   "include")                # ./include
  set(INSTALL_DOC       "doc")                    # ./doc
  set(INSTALL_SHORTCUTS "misc")                   # ./misc
  set(INSTALL_ICONS     "misc")                   # ./misc
  set(INSTALL_INIT      "misc")                   # ./misc
endif()

# Set runtime path
set(CMAKE_SKIP_BUILD_RPATH            FALSE) # Add absolute path to all dependencies for BUILD
set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE) # Use CMAKE_INSTALL_RPATH for INSTALL
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE) # Do NOT add path to dependencies for INSTALL

if(NOT SYSTEM_DIR_INSTALL)
  # Find libraries relative to binary
  if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path/../../../${INSTALL_LIB}")
  else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN/${INSTALL_LIB}")
  endif()
endif()


# ------------------------------------------------------------------------------
# Project modules
# ------------------------------------------------------------------------------

add_subdirectory(common)
add_subdirectory(filesystem)
add_subdirectory(imgui)
add_subdirectory(main)

# For packaging
add_subdirectory(deploy)

# Setup sphinx doc master target and add other submodules as dependencies
if(SPHINX_FOUND)
  add_sphinx_target("master")
  add_dependencies(master_sphinx asap_common_sphinx asap_filesystem_sphinx)
endif()


# ------------------------------------------------------------------------------
# Deployment (global project files)
# ------------------------------------------------------------------------------

# Install version file
install(FILES "${PROJECT_BINARY_DIR}/VERSION" DESTINATION ${INSTALL_ROOT} COMPONENT runtime)

# Deploy generated top level headers
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)

# Install cmake find script for the project
install(FILES ${META_PROJECT_NAME}-config.cmake DESTINATION ${INSTALL_ROOT} COMPONENT dev)

# Install the project meta files
install(FILES AUTHORS   DESTINATION ${INSTALL_ROOT} COMPONENT runtime)
install(FILES LICENSE   DESTINATION ${INSTALL_ROOT} COMPONENT runtime)
install(FILES README.md DESTINATION ${INSTALL_ROOT} COMPONENT runtime)

# Install runtime data
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data DESTINATION ${INSTALL_DATA} COMPONENT runtime)
