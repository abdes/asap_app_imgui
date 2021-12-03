# ~~~
# SPDX-License-Identifier: BSD-3-Clause

# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

# --------------------------------------------------------------------------------------------------
# Meta information about the this module
# --------------------------------------------------------------------------------------------------

asap_declare_module(
  MODULE_NAME
  "logging"
  DESCRIPTION
  "Logging functionality for asap based projects"
  GITHUB_REPO
  "https://github.com/abdes/asap"
  AUTHOR_MAINTAINER
  "Abdessattar Sassi"
  VERSION_MAJOR
  "1"
  VERSION_MINOR
  "0"
  VERSION_PATCH
  "0")

# ==================================================================================================
# Build instructions
# ==================================================================================================

# --------------------------------------------------------------------------------------------------
# Main module target
# --------------------------------------------------------------------------------------------------

set(MODULE_TARGET_NAME "${META_PROJECT_NAME}_${META_MODULE_NAME}")

asap_add_library(
  ${MODULE_TARGET_NAME}
  STATIC
  SHARED
  WARNING
  SOURCES
  # Headers
  "include/logging/logging.h"
  # Sources
  "src/logging.cpp")

target_link_libraries(
  ${MODULE_TARGET_NAME}
  PRIVATE Microsoft.GSL::GSL
  PUBLIC ${META_PROJECT_NAME}::common spdlog::spdlog)

target_include_directories(
  ${MODULE_TARGET_NAME}
  PUBLIC $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

asap_configure_sanitizers(${MODULE_TARGET_NAME})

# --------------------------------------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------------------------------------

if(OPTION_BUILD_TESTS)
  add_subdirectory(test)
endif()

# --------------------------------------------------------------------------------------------------
# API Documentation
# --------------------------------------------------------------------------------------------------

asap_with_doxygen(
  ${MODULE_TARGET_NAME} "\"Logging Module\"" "\"Provides logging functionality based on spdlog.\""
  "${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include")

asap_with_sphinx(${MODULE_TARGET_NAME})

# ==================================================================================================
# Deployment instructions
# ==================================================================================================

# Library
install(
  TARGETS ${MODULE_TARGET_NAME}
  EXPORT "${TARGETS_EXPORT_NAME}"
  COMPONENT dev
  RUNTIME DESTINATION ${ASAP_INSTALL_BIN} COMPONENT runtime
  LIBRARY DESTINATION ${ASAP_INSTALL_SHARED} COMPONENT runtime
  ARCHIVE DESTINATION ${ASAP_INSTALL_LIB} COMPONENT dev)

# Header files
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${META_MODULE_NAME}
  DESTINATION ${ASAP_INSTALL_INCLUDE}
  COMPONENT dev
  FILES_MATCHING
  PATTERN "*.h")

# Generated header files
install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_MODULE_NAME}
  DESTINATION ${ASAP_INSTALL_INCLUDE}
  COMPONENT dev)

# Target config
install(
  EXPORT ${TARGETS_EXPORT_NAME}
  NAMESPACE ${META_PROJECT_NAME}::
  DESTINATION ${ASAP_INSTALL_CMAKE}/${META_MODULE_NAME}
  COMPONENT dev)

# Package configuration files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET_NAME}ConfigVersion.cmake
        DESTINATION ${ASAP_INSTALL_CMAKE}/${META_MODULE_NAME})
