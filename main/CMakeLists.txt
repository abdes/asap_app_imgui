# ~~~
# SPDX-License-Identifier: BSD-3-Clause

# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

set(my_name "main")
asap_push_module("${my_name}")

# --------------------------------------------------------------------------------------------------
# Meta information about the this module
# --------------------------------------------------------------------------------------------------

asap_declare_module(
  MODULE_NAME
  "${my_name}"
  DESCRIPTION
  "Main module for the application program"
  GITHUB_REPO
  "https://github.com/asap/asap_app_imgui"
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

set(MODULE_TARGET_NAME "${META_PROJECT_NAME}")

asap_add_executable(
  ${MODULE_TARGET_NAME}
  WARNING
  SOURCES
  # Headers
  src/app/application.h
  src/app/imgui_runner.h
  src/config/config.h
  src/ui/fonts/fonts.h
  src/ui/fonts/material_design_icons.h
  src/ui/log/sink.h
  src/ui/style/theme.h
  # Sources FONTS
  src/ui/fonts/material_design_icons.cpp
  src/ui/fonts/material_design_icons.h
  #
  src/config/config.cpp
  #
  src/ui/log/sink.cpp
  src/ui/style/theme.cpp
  #
  src/app/imgui_runner.cpp
  #
  src/application_base.h
  src/application_base.h
  src/application_base.cpp
  src/example_application.h
  src/example_application.cpp
  src/main.cpp)

target_link_libraries(
  ${MODULE_TARGET_NAME}
  PRIVATE GSL
          asap::common
          asap::contract
          asap::logging
          glm::glm
          ${META_PROJECT_NAME}::imgui
          tomlplusplus::tomlplusplus
          date::date)

target_include_directories(${MODULE_TARGET_NAME}
                           PRIVATE ${CMAKE_BINARY_DIR}/include)
target_include_directories(${MODULE_TARGET_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Hacking the include directories when building th executable simply because of
# Imgui backend files including "imgui.h" literally like that. We add the imgui
# directory to the includes so that it will work.
#
# In other files, such as the ones we provide, we don't do that shit like that.
# Instead, we include files from Imgui library as <imgui/xxxxx>
target_include_directories(${MODULE_TARGET_NAME}
                           PRIVATE ${CMAKE_SOURCE_DIR}/imgui/imgui)

target_compile_features(${MODULE_TARGET_NAME} PUBLIC cxx_std_17)

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
  "\"Main Module\""
  BRIEF
  "\"The main module for the application program.\""
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

  # Binaries
  install(
    TARGETS ${MODULE_TARGET_NAME}
    EXPORT "${TARGETS_EXPORT_NAME}"
    COMPONENT dev
    RUNTIME DESTINATION ${ASAP_INSTALL_BIN} COMPONENT ${runtime}
    LIBRARY DESTINATION ${ASAP_INSTALL_SHARED} COMPONENT ${runtime}
    ARCHIVE DESTINATION ${ASAP_INSTALL_LIB} COMPONENT ${dev})

  # Docs
  if(EXISTS ${SPHINX_BUILD_DIR}/${MODULE_TARGET_NAME})
    install(
      DIRECTORY ${SPHINX_BUILD_DIR}/${MODULE_TARGET_NAME}
      DESTINATION ${ASAP_INSTALL_DOC}
      COMPONENT ${MODULE_TARGET_NAME}_docs)
  endif()
endif()

asap_pop_module("${my_name}")
