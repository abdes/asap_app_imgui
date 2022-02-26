# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

# ------------------------------------------------------------------------------
# Documentation with Sphinx/Breathe/Exhale
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
# To use in a submodule or in the master module add the following:
#
# ~~~
#      asap_with_sphinx(${target})
# ~~~
#
# Use 'make sphinx' to generate documentation. (not done by default)
#

include(FindSphinx)

if(SPHINX_FOUND)
  message(STATUS "System has sphinx.")

  # Add a master sphinx target to collect all submodules
  add_custom_target(sphinx)
  set_target_properties(sphinx PROPERTIES EXCLUDE_FROM_ALL TRUE)

  # The macro to add a submodule as a sphinx target.
  macro(asap_with_sphinx TARGET_NAME)
    # Setup work directory for the target module
    set(SPHINX_TARGET_WORKDIR "${SPHINX_BUILD_DIR}/${TARGET_NAME}")
    if(NOT EXISTS "${SPHINX_TARGET_WORKDIR}")
      file(MAKE_DIRECTORY "${SPHINX_TARGET_WORKDIR}")
    endif()
    # Do @ substitutions in the sphinx config file for the module
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/doc/conf.py.in"
                   "${CMAKE_CURRENT_SOURCE_DIR}/doc/conf.py" @ONLY)

    # Add a target for building the sphinx documentation of the module
    add_custom_target(
      ${TARGET_NAME}_sphinx
      COMMAND
        ${SPHINX_EXECUTABLE} -q -b html -c "${EXHALE_TARGET_WORKDIR}" -d
        "${SPHINX_TARGET_WORKDIR}/_doctrees" "${CMAKE_CURRENT_SOURCE_DIR}/doc"
        "${SPHINX_TARGET_WORKDIR}/html"
      WORKING_DIRECTORY "${SPHINX_TARGET_WORKDIR}"
      VERBATIM
      COMMENT "Generating `sphinx` documentation for `${TARGET_NAME}`")
    set_target_properties(${TARGET_NAME}_sphinx PROPERTIES EXCLUDE_FROM_ALL
                                                           TRUE)
    # Finally add the module sphinx target as a dependency for the overall
    # sphinx target
    add_dependencies(sphinx ${TARGET_NAME}_sphinx)

  endmacro()
  # We only build documentation through explicit invocation of the sphinx target
  # as it is pretty heavy and requires doxygen to be run before it is invoked.
  set_target_properties(sphinx PROPERTIES EXCLUDE_FROM_ALL TRUE)

else(SPHINX_FOUND)
  message(STATUS "WARNING: sphinx is not available on this system!")

  macro(asap_with_sphinx TARGET_NAME)

  endmacro()
endif(SPHINX_FOUND)
