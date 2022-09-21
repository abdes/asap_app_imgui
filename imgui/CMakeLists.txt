# ~~~
# SPDX-License-Identifier: BSD-3-Clause

# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

set(my_name "imgui")
asap_push_module("${my_name}")

# --------------------------------------------------------------------------------------------------
# Meta information about the this module
# --------------------------------------------------------------------------------------------------

asap_declare_module(
  MODULE_NAME
  "${my_name}"
  DESCRIPTION
  "Imgui and related integration files"
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

# ==============================================================================
# Build instructions
# ==============================================================================

# --------------------------------------------------------------------------------------------------
# Main module target
# --------------------------------------------------------------------------------------------------

# Hardcode `asap` in the module name as we do not want this prefix to change
# with the forked project name.
set(MODULE_TARGET_NAME "asap_imgui")

asap_add_library(
  ${MODULE_TARGET_NAME}
  STATIC
  SHARED
  WARNING
  SOURCES
  # Imgui public Headers
  "imgui/imgui.h"
  "imgui/backends/imgui_impl_glfw.h"
  "imgui/backends/imgui_impl_opengl3.h"
  # Imgui private headers
  "imgui/imgui_internal.h"
  "imgui/imconfig.h"
  "imgui/imstb_rectpack.h"
  "imgui/imstb_textedit.h"
  "imgui/imstb_truetype.h"
  # Imgui Sources
  "imgui/imgui.cpp"
  "imgui/imgui_widgets.cpp"
  "imgui/imgui_tables.cpp"
  "imgui/imgui_draw.cpp"
  "imgui/imgui_demo.cpp"
  "imgui/backends/imgui_impl_glfw.cpp"
  "imgui/backends/imgui_impl_opengl3.cpp"
  # Integration public headers
  "include/KHR/khrplatform.h"
  "include/glad/gl.h"
  # Integration sources
  "src/glad/gl.cpp"
  # ImGui extensions wrappers for C++ standard library (STL) types (std::string,
  # etc.)
  "imgui/misc/cpp/imgui_stdlib.h"
  "imgui/misc/cpp/imgui_stdlib.cpp")

target_link_libraries(
  ${MODULE_TARGET_NAME}
  PRIVATE asap::common
  PUBLIC glfw)

target_include_directories(
  ${MODULE_TARGET_NAME}
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/imgui
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
          ${CMAKE_SOURCE_DIR}/third_party/glfw/include)

target_compile_definitions(
  ${MODULE_TARGET_NAME} PRIVATE IMGUI_USER_CONFIG="imgui_user_config.h"
                                IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_compile_features(${MODULE_TARGET_NAME} PUBLIC cxx_std_11)

# Add dll specific defines for glad
target_compile_definitions(
  ${MODULE_TARGET_NAME}
  PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:GLAD_API_CALL_EXPORT_BUILD>
  PUBLIC $<$<BOOL:${BUILD_SHARED_LIBS}>:GLAD_API_CALL_EXPORT>)

# Generate module config files for cmake and pkgconfig
asap_create_module_config_files()

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------
# ~~~
# if(ASAP_BUILD_TESTS)
#   asap_add_sanitizers(${MODULE_TARGET_NAME})
#   add_subdirectory(test)
# endif()
# ~~~

# --------------------------------------------------------------------------------------------------
# API Documentation
# --------------------------------------------------------------------------------------------------
asap_with_doxygen(
  MODULE_NAME
  ${MODULE_TARGET_NAME}
  VERSION
  ${META_MODULE_VERSION}
  TITLE
  "\"Imgui Module\""
  BRIEF
  "\"The Imgui library and additional integration files.\""
  INPUT_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include")

# asap_with_sphinx(${MODULE_TARGET_NAME})

# ==================================================================================================
# Deployment instructions
# ==================================================================================================
if(${META_PROJECT_ID}_INSTALL)
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

  # Docs
  if(EXISTS ${SPHINX_BUILD_DIR}/${MODULE_TARGET_NAME})
    install(
      DIRECTORY ${SPHINX_BUILD_DIR}/${MODULE_TARGET_NAME}
      DESTINATION ${ASAP_INSTALL_DOC}
      COMPONENT ${MODULE_TARGET_NAME}_docs)
  endif()
endif()

asap_pop_module("${my_name}")
