# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

# ------------------------------------------------------------------------------
# Meta information about the this module
# ------------------------------------------------------------------------------

asap_declare_module(
  MODULE_NAME
  "common"
  DESCRIPTION
  "Common functionality for asap based projects"
  GITHUB_REPO
  "https://github.com/abdes/asap"
  AUTHOR_MAINTAINER
  "Abdessattar Sassi"
  VERSION_MAJOR
  "1"
  VERSION_MINOR
  "1"
  VERSION_PATCH
  "0")

# ------------------------------------------------------------------------------
# Code generation
# ------------------------------------------------------------------------------

# Generate config-header
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/${META_MODULE_NAME}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/${META_MODULE_NAME}/config.h)

# ==============================================================================
# Build instructions
# ==============================================================================

# ------------------------------------------------------------------------------
# Main module target
# ------------------------------------------------------------------------------

# Hardcode `asap` in the module name as we do not want this prefix to change
# with the forked project name.
set(MODULE_TARGET_NAME "asap_${META_MODULE_NAME}")

asap_add_library(
  ${MODULE_TARGET_NAME}
  STATIC
  SHARED
  WARNING
  SOURCES
  # Headers
  "include/common/platform.h"
  "include/common/compilers.h"
  "include/common/singleton.h"
  "include/common/flag_ops.h"
  "include/common/overload.h"
  "include/common/mixin.h"
  # contract module
  "include/contract/contract.h"
  "include/contract/ut/gtest.h"
  "include/contract/ut/framework.h"
  # Sources
  "src/contract.cpp")

target_link_libraries(${MODULE_TARGET_NAME} PRIVATE Microsoft.GSL::GSL)

target_include_directories(
  ${MODULE_TARGET_NAME}
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(${MODULE_TARGET_NAME} PUBLIC cxx_std_17)

# common comes directly from asap and is usually kept as is. It is more
# convenient to simply use it as asap::common
add_library(asap::${META_MODULE_NAME} ALIAS ${MODULE_TARGET_NAME})

# Add support for (optional) code quality tools
asap_add_sanitizers(${MODULE_TARGET_NAME})

# Generate module config files for cmake and pkgconfig
asap_create_module_config_files()

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

if(ASAP_BUILD_TESTS)
  add_subdirectory(test)
endif()

# ------------------------------------------------------------------------------
# API Documentation
# ------------------------------------------------------------------------------

asap_with_doxygen(
  MODULE_NAME
  ${MODULE_TARGET_NAME}
  VERSION
  ${META_MODULE_VERSION}
  TITLE
  "\"Common Module\""
  BRIEF
  "\"Provides common basic building blocks such as portability, assertions, etc.\""
  INPUT_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include")

asap_with_sphinx(${MODULE_TARGET_NAME})

# ==============================================================================
# Deployment instructions
# ==============================================================================

set(TARGETS_EXPORT_NAME "${MODULE_TARGET_NAME}Targets")
set(runtime "${MODULE_TARGET_NAME}_runtime")
set(dev "${MODULE_TARGET_NAME}_dev")

# Library
install(
  TARGETS ${MODULE_TARGET_NAME}
  EXPORT "${TARGETS_EXPORT_NAME}"
  COMPONENT dev
  RUNTIME DESTINATION ${ASAP_INSTALL_BIN} COMPONENT ${runtime}
  LIBRARY DESTINATION ${ASAP_INSTALL_SHARED} COMPONENT ${runtime}
  ARCHIVE DESTINATION ${ASAP_INSTALL_LIB} COMPONENT ${dev})

# Header files
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${META_MODULE_NAME}
  DESTINATION ${ASAP_INSTALL_INCLUDE}
  COMPONENT ${dev}
  FILES_MATCHING
  PATTERN "*.h")

# Contract library Header files
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/contract
  DESTINATION ${ASAP_INSTALL_INCLUDE}
  COMPONENT ${dev}
  FILES_MATCHING
  PATTERN "*.h")

# Generated header files
install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_MODULE_NAME}
  DESTINATION ${ASAP_INSTALL_INCLUDE}
  COMPONENT ${dev})

# Target config
install(
  EXPORT ${TARGETS_EXPORT_NAME}
  NAMESPACE ${META_PROJECT_NAME}::
  DESTINATION ${ASAP_INSTALL_CMAKE}/${META_MODULE_NAME}
  COMPONENT ${dev})

# Package configuration files
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET_NAME}ConfigVersion.cmake
  DESTINATION ${ASAP_INSTALL_CMAKE}/${META_MODULE_NAME})
