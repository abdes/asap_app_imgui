

macro(configure_doxyfile TARGET_NAME TARGET_TITLE TARGET_BRIEF TARGET_INPUT_PATH)
  if (EXISTS "${CMAKE_SOURCE_DIR}/Doxyfile.in")
    set(DOXY_TARGET_OUTPUT_DIR "${TARGET_NAME}")
    set(DOXY_TARGET_ROOT_DIR "") # ${DOXTOSRCDIR} - set(DOXTOSRCDIR "../src")
    set(DOXY_TARGET_NAME "${TARGET_NAME}")
    set(DOXY_TARGET_TITLE "${TARGET_TITLE}")
    set(DOXY_TARGET_BRIEF "${TARGET_BRIEF}")
    set(DOXY_TARGET_INPUT_PATH "${TARGET_INPUT_PATH}")
    set(DOXY_TARGET_FILE_VERSION_FILTER "\"${PRINT_FILE_DATE_COMMAND_STR} \"")
    if (NOT EXISTS "${DOXROOT}/${DOXY_TARGET_OUTPUT_DIR}")
      file(MAKE_DIRECTORY "${DOXROOT}/${DOXY_TARGET_OUTPUT_DIR}")
    endif ()
    configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.in" "${DOXROOT}/${TARGET_NAME}_Doxyfile" @ONLY)
    message(STATUS "== Copy Doxyfile.in --> '${DOXROOT}/${TARGET_NAME}_Doxyfile'")
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
      WORKING_DIRECTORY "${DOXROOT}" VERBATIM)
    add_dependencies(dox ${TARGET_NAME}_dox)
  endmacro()
  set_target_properties(dox PROPERTIES EXCLUDE_FROM_ALL TRUE)
else ()
  message(STATUS "WARNING : Doxygen package is not available on this system!")
  macro(add_doxygen_target TARGET_NAME)
  endmacro()
endif ()
