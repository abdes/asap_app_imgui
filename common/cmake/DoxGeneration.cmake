
# ------------------------------------------------------------------------------
# API Documentation
# ------------------------------------------------------------------------------

# To avoid indiscriminately generating documentation for all modules in the
# project, including third party modules and stuff for which we don't want
# documentation to be generated, we provide here the basic tools to add
# doxygen capabilities to a module.
#
# To use in a specific CmakeLists.txt add the following:
#
# 1. Call configure_doxyfile(...)
#      configure_doxyfile(
#        ${target}
#        "\"<Title>\""
#        "\"<Desscription>\""
#        "<List of directories to include>")
#
# 2. Call add_doxygen_target(...) to add this module under the 'dox' target
#      add_doxygen_target(${target})
#
# 3. Add install instructions
#      install(
#        DIRECTORY ${dox_root}/${target}
#        DESTINATION ${INSTALL_DOC}
#        COMPONENT docs
#      )
#
# Use 'make dox' to generate documentation. (not done by default)
#


macro(configure_doxyfile TARGET_NAME TARGET_TITLE TARGET_BRIEF TARGET_INPUT_PATH)
  if (EXISTS "${CMAKE_SOURCE_DIR}/Doxyfile.in")
    set(DOXY_TARGET_OUTPUT_DIR "${TARGET_NAME}")
    set(DOXY_TARGET_ROOT_DIR "") # ${DOXTOSRCDIR} - set(DOXTOSRCDIR "../src")
    set(DOXY_TARGET_NAME "${TARGET_NAME}")
    set(DOXY_TARGET_TITLE "${TARGET_TITLE}")
    set(DOXY_TARGET_BRIEF "${TARGET_BRIEF}")
    set(DOXY_TARGET_INPUT_PATH "${TARGET_INPUT_PATH}")
    set(DOXY_TARGET_FILE_VERSION_FILTER "\"${PRINT_FILE_DATE_COMMAND_STR} \"")
    if (NOT EXISTS "${dox_root}/${DOXY_TARGET_OUTPUT_DIR}")
      file(MAKE_DIRECTORY "${dox_root}/${DOXY_TARGET_OUTPUT_DIR}")
    endif ()
    configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.in" "${dox_root}/${TARGET_NAME}_Doxyfile" @ONLY)
    message(STATUS "== Copy Doxyfile.in --> '${dox_root}/${TARGET_NAME}_Doxyfile'")
  else ()
    message(STATUS "WARNING : The '${CMAKE_SOURCE_DIR}/Doxyfile.in' file does not exist!")
  endif ()
endmacro(configure_doxyfile)

if (DOXYGEN_FOUND)
  message(STATUS "Doxygen package was found.")
  add_custom_target(dox)
  macro(add_doxygen_target TARGET_NAME)
    add_custom_target(${TARGET_NAME}_dox
      ${CMAKE_COMMAND} -E remove -f "${TARGET_NAME}_Doxyfile.out"
      COMMAND ${CMAKE_COMMAND} -E copy "${TARGET_NAME}_Doxyfile" "${TARGET_NAME}_Doxyfile.out"
      COMMAND ${DOXYGEN_EXECUTABLE} "${TARGET_NAME}_Doxyfile.out"
      COMMAND ${CMAKE_COMMAND} -E remove -f "${TARGET_NAME}_Doxyfile.out"
      WORKING_DIRECTORY "${dox_root}" VERBATIM)
    add_dependencies(dox ${TARGET_NAME}_dox)
  endmacro()
  set_target_properties(dox PROPERTIES EXCLUDE_FROM_ALL TRUE)
else ()
  message(STATUS "WARNING : Doxygen package is not available on this system!")
  macro(add_doxygen_target TARGET_NAME)
  endmacro()
endif ()
