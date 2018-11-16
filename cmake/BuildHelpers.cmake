
# ------------------------------------------------------------------------------
# Build Helpers to simplify target creation.
# ------------------------------------------------------------------------------

include(CMakeParseArguments)
include(GenerateTemplateExportHeader)


# Set policy if policy is available
function(set_policy POL VAL)

  if (POLICY ${POL})
    cmake_policy(SET ${POL} ${VAL})
  endif ()

endfunction(set_policy)


# Define function "source_group_by_path with three mandatory arguments
# (PARENT_PATH, REGEX, GROUP, ...) to group source files in folders (e.g. for
# MSVC solutions).
#
# Example:
# source_group_by_path("${CMAKE_CURRENT_SOURCE_DIR}/src"
#   "\\\\.h$|\\\\.inl$|\\\\.cpp$|\\\\.c$|\\\\.ui$|\\\\.qrc$"
#   "Source Files" ${sources})
#
function(source_group_by_path PARENT_PATH REGEX GROUP)

  foreach (FILENAME ${ARGN})

    get_filename_component(FILEPATH "${FILENAME}" REALPATH)
    file(RELATIVE_PATH FILEPATH ${PARENT_PATH} ${FILEPATH})
    get_filename_component(FILEPATH "${FILEPATH}" DIRECTORY)

    string(REPLACE "/" "\\" FILEPATH "${FILEPATH}")

    source_group("${GROUP}\\${FILEPATH}" REGULAR_EXPRESSION "${REGEX}" FILES ${FILENAME})

  endforeach ()

endfunction(source_group_by_path)


# ------------------------------------------------------------------------------
# Add a library to the project.
# ------------------------------------------------------------------------------

function(asap_library)
  set(options)
  set(oneValueArgs TARGET EXPORT_NAME)
  set(multiValueArgs
    SOURCES PUBLIC_LIBRARIES PUBLIC_INCLUDE_DIRS
    PRIVATE_COMPILE_DEFINITIONS PRIVATE_COMPILE_OPTIONS PRIVATE_INCLUDE_DIRS)
  cmake_parse_arguments(ASAP_LIB "${options}" "${oneValueArgs}" "${multiValueArgs}"
    ${ARGN})

  set(_NAME ${ASAP_LIB_TARGET})

  add_library(${_NAME} ${ASAP_LIB_SOURCES})

  # Set API export file and macro
  string(MAKE_C_IDENTIFIER ${_NAME} target_id)
  string(TOUPPER ${target_id} target_id)
  set(export_file "include/${ASAP_LIB_EXPORT_NAME}/${_NAME}_export.h")
  set(template_export_file "include/${ASAP_LIB_EXPORT_NAME}/${_NAME}_api.h")
  set(export_macro "${target_id}_API")

  # Create API export headers
  generate_export_header(${_NAME}
    EXPORT_FILE_NAME ${export_file}
    EXPORT_MACRO_NAME ${export_macro}
    )
  generate_template_export_header(${_NAME}
    ${target_id}
    ${template_export_file}
    )

  #
  # Compile definitions
  #

  target_compile_definitions(${_NAME}
    PRIVATE
      ${ASAP_LIB_PRIVATE_COMPILE_DEFINITIONS}

    PUBLIC
      ${DEFAULT_COMPILE_DEFINITIONS}
      $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_id}_STATIC_DEFINE>
    )


  #
  # Compile options
  #

  target_compile_options(${_NAME}
    PRIVATE
      ${ASAP_LIB_PRIVATE_COMPILE_OPTIONS}

    PUBLIC
      ${DEFAULT_COMPILE_OPTIONS}
    )

  #
  # Libraries
  #

  target_link_libraries(${_NAME}
    PRIVATE

    PUBLIC
      ${DEFAULT_LIBRARIES}
      ${ASAP_LIB_PUBLIC_LIBRARIES}
    )

  #
  # Linker flags
  #

  set_target_properties(${_NAME} PROPERTIES LINK_FLAGS "${DEFAULT_LINKER_OPTIONS}")


  #
  # Include directories
  #

  target_include_directories(${_NAME}
    PRIVATE
      # Those are build requirements, only used to build the library itselfi, and are not 
      # to be exported as part of the usage requirements.
      ${ASAP_LIB_PRIVATE_INCLUDE_DIRS}

    PUBLIC
      # The fiollowing are both build and usage requirements
      ${DEFAULT_INCLUDE_DIRECTORIES}
      # Adapt to the scenarios of the library being used from the build tree vs. from the
      # install location.
      $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )


  #
  # Project options
  #

  set_target_properties(${_NAME}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
    VERSION ${META_VERSION}
    SOVERSION ${META_VERSION_MAJOR}
  )

  if (ASAP_LIB_EXPORT_NAME)
    add_library(${META_PROJECT_NAME}::${ASAP_LIB_EXPORT_NAME} ALIAS ${_NAME})
  endif ()

  #
  # Deployment
  #

  # Library
  install(TARGETS ${_NAME}
    EXPORT "${_NAME}-export" COMPONENT dev
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT runtime
    LIBRARY DESTINATION ${INSTALL_SHARED} COMPONENT runtime
    ARCHIVE DESTINATION ${INSTALL_LIB} COMPONENT dev
    )

  # Header files
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include/${ASAP_LIB_EXPORT_NAME})
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${ASAP_LIB_EXPORT_NAME}
      DESTINATION ${INSTALL_INCLUDE}
      COMPONENT dev
      FILES_MATCHING PATTERN "*.h"
      )
  endif ()

  # Generated header files
  install(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/include/${ASAP_LIB_EXPORT_NAME} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
    )

  # CMake config
  install(EXPORT ${_NAME}-export
    NAMESPACE ${META_PROJECT_NAME}::
    DESTINATION ${INSTALL_CMAKE}/${ASAP_LIB_EXPORT_NAME}
    COMPONENT dev
    )

endfunction()


# ------------------------------------------------------------------------------
# Add a Header-Only library to the project.
# ------------------------------------------------------------------------------

function(asap_header_library)
  set(options)
  set(oneValueArgs TARGET EXPORT_NAME)
  set(multiValueArgs SOURCES PUBLIC_LIBRARIES PUBLIC_INCLUDE_DIRS)
  cmake_parse_arguments(ASAP_HO "${options}" "${oneValueArgs}" "${multiValueArgs}"
    ${ARGN})

  set(_NAME ${ASAP_HO_TARGET})

  add_library(${_NAME} ${ASAP_HO_SOURCES})

  # Set API export file and macro
  string(MAKE_C_IDENTIFIER ${_NAME} target_id)
  string(TOUPPER ${target_id} target_id)
  set(export_file "include/${ASAP_HO_EXPORT_NAME}/${_NAME}_export.h")
  set(template_export_file "include/${ASAP_HO_EXPORT_NAME}/${_NAME}_api.h")
  set(export_macro "${target_id}_API")

  # Create API export headers
  generate_export_header(${_NAME}
    EXPORT_FILE_NAME ${export_file}
    EXPORT_MACRO_NAME ${export_macro}
    )
  generate_template_export_header(${_NAME}
    ${target_id}
    ${template_export_file}
    )

  #
  # Libraries
  #

  target_link_libraries(${_NAME}
    PRIVATE

    PUBLIC
    ${DEFAULT_LIBRARIES}
    ${ASAP_HO_PUBLIC_LIBRARIES}

    INTERFACE
    )


  #
  # Include directories
  #

  target_include_directories(${_NAME}
    PRIVATE
    ${PROJECT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${ASAP_HO_PRIVATE_INCLUDE_DIRS}

    PUBLIC
    ${DEFAULT_INCLUDE_DIRECTORIES}

    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )


  #
  # Project options
  #

  set_target_properties(${_NAME}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
    VERSION ${META_VERSION}
    SOVERSION ${META_VERSION_MAJOR}
    )

  if (ASAP_HO_EXPORT_NAME)
    add_library(${META_PROJECT_NAME}::${ASAP_HO_EXPORT_NAME} ALIAS ${_NAME})
  endif ()

  #
  # Deployment
  #

  # Header files
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include/${ASAP_LIB_EXPORT_NAME})
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${ASAP_LIB_EXPORT_NAME}
      DESTINATION ${INSTALL_INCLUDE}
      COMPONENT dev
      FILES_MATCHING PATTERN "*.h"
      )
  endif ()

  # Generated header files
  install(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/include/${ASAP_LIB_EXPORT_NAME} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
    )

  # CMake config
  install(EXPORT ${_NAME}-export
    NAMESPACE ${META_PROJECT_NAME}::
    DESTINATION ${INSTALL_CMAKE}/${ASAP_LIB_EXPORT_NAME}
    COMPONENT dev
    )

endfunction()


# ------------------------------------------------------------------------------
# Add an application executable program to the project.
# ------------------------------------------------------------------------------

function(asap_executable)
  set(options)
  set(oneValueArgs TARGET)
  set(multiValueArgs
    SOURCES LIBRARIES INCLUDE_DIRS COMPILE_DEFINITIONS COMPILE_OPTIONS)
  cmake_parse_arguments(ASAP_EXE "${options}" "${oneValueArgs}" "${multiValueArgs}"
    ${ARGN})

  set(_NAME ${ASAP_EXE_TARGET})

  add_executable(${_NAME} ${ASAP_EXE_SOURCES})
  # Create namespaced alias
  add_executable(${META_PROJECT_NAME}::${_NAME} ALIAS ${_NAME})

  #
  # Compile definitions
  #

  target_compile_definitions(${_NAME}
    PRIVATE
    ${ASAP_EXE_COMPILE_DEFINITIONS}
    ${DEFAULT_COMPILE_DEFINITIONS}
    )


  #
  # Compile options
  #

  target_compile_options(${_NAME}
    PRIVATE
    ${ASAP_EXE_COMPILE_OPTIONS}
    ${DEFAULT_COMPILE_OPTIONS}
    )

  #
  # Libraries
  #

  target_link_libraries(${_NAME}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${ASAP_EXE_LIBRARIES}
    )

  #
  # Linker flags
  #

  set_target_properties(${_NAME} PROPERTIES LINK_FLAGS "${DEFAULT_LINKER_OPTIONS}")


  #
  # Include directories
  #

  target_include_directories(${_NAME}
    PRIVATE
    ${PROJECT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${ASAP_EXE_INCLUDE_DIRS}
    )


  #
  # Project options
  #

  set_target_properties(${_NAME}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
  )

  #
  # Deployment
  #
  # Executable
  install(TARGETS ${_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT programs
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT programs
  )

endfunction()


# ------------------------------------------------------------------------------
# Add a test program to the project.
# ------------------------------------------------------------------------------

function(asap_test_executable)
  set(options)
  set(oneValueArgs TARGET)
  set(multiValueArgs
    SOURCES LIBRARIES INCLUDE_DIRS COMPILE_DEFINITIONS COMPILE_OPTIONS)
  cmake_parse_arguments(ASAP_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}"
    ${ARGN})

  set(_NAME ${ASAP_TEST_TARGET})

  add_executable(${_NAME} ${ASAP_TEST_SOURCES})

  #
  # Compile definitions
  #

  target_compile_definitions(${_NAME}
    PRIVATE
    ${ASAP_TEST_COMPILE_DEFINITIONS}
    ${DEFAULT_COMPILE_DEFINITIONS}
    )


  #
  # Compile options
  #

  target_compile_options(${_NAME}
    PRIVATE
    ${ASAP_TEST_COMPILE_OPTIONS}
    ${DEFAULT_COMPILE_OPTIONS}
    )

  #
  # Libraries
  #

  target_link_libraries(${_NAME}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${ASAP_TEST_LIBRARIES}
    Catch2
    )

  #
  # Linker flags
  #

  set_target_properties(${_NAME} PROPERTIES LINK_FLAGS "${DEFAULT_LINKER_OPTIONS}")


  #
  # Include directories
  #

  target_include_directories(${_NAME}
    PRIVATE
    ${PROJECT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${ASAP_TEST_INCLUDE_DIRS}
    )


  #
  # Project options
  #

  set_target_properties(${_NAME}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
    )

if (TARGET Catch2)
  catch_discover_tests(${_NAME} TEST_PREFIX "${_NAME}::")
else ()
  add_test(${_NAME} ${PROJECT_BINARY_DIR}/${_NAME})
endif ()

endfunction()
