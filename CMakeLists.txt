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
if (ENABLE_CPPCHECK)
  cmake_minimum_required(VERSION 3.10)
else ()
  cmake_minimum_required(VERSION 3.6)
endif ()

# List of directories specifying a search path for CMake modules to be loaded by
# the the include() or find_package() commands before checking the default modules
# that come with CMake.
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/common/cmake)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Register general cmake commands
include(GenerateExportHeader)
include(WriteCompilerDetectionHeader)

# Register custom cmake commands
include(GetGitRevisionDescription)


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
set(META_AUTHOR_ORGANIZATION "OpenSource")
set(META_AUTHOR_DOMAIN       "https://github.com/abdes/asap")
set(META_AUTHOR_MAINTAINER   "abde.sassi@gmail.com")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "1")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_CMAKE_INIT_SHA      "${GIT_REV}")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)

message(STATUS "META_NAME_VERSION : ${META_NAME_VERSION}")
