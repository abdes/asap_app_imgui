
# ------------------------------------------------------------------------------
# API Documentation with Sphinx/Breathe/Exhale
# ------------------------------------------------------------------------------

# To avoid indiscriminately generating documentation for all modules in the
# project, including third party modules and stuff for which we don't want
# documentation to be generated, we provide here the basic tools to add
# breathe/exhale capabilities to a module. Additionally, exhale can only process
# one module at a time, so we need to have the generation run for each module.
#
# Doxygen is run separately NOT by exhale. Sphinx targets must be run after
# doxygen.
#
# To use in a specific CmakeLists.txt add the following:
#
# 1. Call configure_sphinx_files(...)
#      configure_sphinx_files(
#        ${target}
#        "\"<Title>\""
#        "\"<Desscription>\""
#        "<List of directories to include>")
#
# 2. Call add_sphinx_target(...) to add this module under the 'sphinx' target
#      add_sphinx_target(${target})
#
# 3. Add install instructions
#      install(
#        DIRECTORY ${dox_root}/${target}
#        DESTINATION ${INSTALL_DOC}
#        COMPONENT docs
#      )
#
# Use 'make sphinx' to generate documentation. (not done by default)
#


if (SPHINX_FOUND)
  message(STATUS "System has sphinx.")

  # Copy the master conf with @ substitutions to the sphinx build dir
  #  configure_file("${CMAKE_SOURCE_DIR}/doc/master_conf.py.in"
  #    "${SPHINX_BUILD_DIR}/conf.py" @ONLY)
  # Copy all reST documents from doc dir to the sphinx build dir
  #  file(GLOB_RECURSE master_documentation_pages "${CMAKE_SOURCE_DIR}/doc/*.rst")
  #  file(COPY ${master_documentation_pages} DESTINATION
  #    "${SPHINX_BUILD_DIR}")
  # Add a master sphinx target to collect all submodules
  add_custom_target(sphinx)
  #  add_custom_target(sphinx
  #    ${SPHINX_EXECUTABLE} -q -b html -c "${SPHINX_BUILD_DIR}" -d
  #    "${SPHINX_CACHE_DIR}" "${SPHINX_BUILD_DIR}" "${SPHINX_HTML_DIR}"
  #    WORKING_DIRECTORY "${SPHINX_BUILD_DIR}" VERBATIM)

  # The macro to add a submodule as a sphinx target.
  macro(add_sphinx_target TARGET_NAME)
    # Setup work directory for the target module
    set(SPHINX_TARGET_WORKDIR "${SPHINX_BUILD_DIR}/${TARGET_NAME}")
    if (NOT EXISTS "${SPHINX_TARGET_WORKDIR}")
      file(MAKE_DIRECTORY "${SPHINX_TARGET_WORKDIR}")
    endif ()
    # Do @ substitutions in the sphinx config file for the module
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/doc/conf.py.in"
      "${CMAKE_CURRENT_SOURCE_DIR}/doc/conf.py" @ONLY)

    # Add a target for building the sphinx documentation of the module
    add_custom_target(${TARGET_NAME}_sphinx
      ${SPHINX_EXECUTABLE} -b html -c "${EXHALE_TARGET_WORKDIR}" -d
      "${SPHINX_CACHE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/doc" "${SPHINX_TARGET_WORKDIR}/html"
      WORKING_DIRECTORY "${SPHINX_TARGET_WORKDIR}" VERBATIM)
    set_target_properties(${TARGET_NAME}_sphinx PROPERTIES EXCLUDE_FROM_ALL TRUE)
    # Finally add the module sphinx target as a dependency for the overall
    # sphinx target
    add_dependencies(sphinx ${TARGET_NAME}_sphinx)
  endmacro()
  # We only build documentation through explicit invocation of the sphinx target
  # as it is pretty heavy and requires doxygen to be run before it is invoked.
  set_target_properties(sphinx PROPERTIES EXCLUDE_FROM_ALL TRUE)

else (SPHINX_FOUND)
  message(STATUS "WARNING: sphinx is not available on this system!")
  macro(add_sphinx_target TARGET_NAME)
  endmacro()
endif (SPHINX_FOUND)
