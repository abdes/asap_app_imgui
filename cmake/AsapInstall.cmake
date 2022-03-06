# Determine if this is built as a subproject  or if it is the master project.

if(NOT DEFINED ${META_PROJECT_ID}_INSTALL)
  option(${META_PROJECT_ID}_INSTALL
         "Generate the install target for this project."
         ${${META_PROJECT_ID}_IS_MASTER_PROJECT})
endif()

macro(_setup_install_dirs)
  # Check for system dir install
  set(_system_dir_install FALSE)
  if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr" OR "${CMAKE_INSTALL_PREFIX}"
                                                  STREQUAL "/usr/local")
    set(_system_dir_install TRUE)
    set(ASAP_INSTALL_PREFIX_FULL_PATH ${CMAKE_INSTALL_PREFIX})
  else()
    set(ASAP_INSTALL_PREFIX_FULL_PATH
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX})
  endif()

  # Installation paths
  include(GNUInstallDirs)
  # cmake-format: off
  if(UNIX AND _system_dir_install)
    # Install into the system (/usr/bin or /usr/local/bin)
    set(ASAP_INSTALL_ROOT      "${CMAKE_INSTALL_DATAROOTDIR}/${META_PROJECT_NAME}")       # /usr/[local]/share/<project>
    set(ASAP_INSTALL_LIB       "${CMAKE_INSTALL_LIBDIR}")                       # /usr/[local]/lib
    set(ASAP_INSTALL_SHARED    "${ASAP_INSTALL_LIB}")                           # /usr/[local]/lib
    set(ASAP_INSTALL_CMAKE     "${CMAKE_INSTALL_DATAROOTDIR}/cmake/${META_PROJECT_NAME}") # /usr/[local]/share/cmake/<project>
    set(ASAP_INSTALL_PKGCONFIG "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig")        # /usr/[local]/share/pkgconfig
    set(ASAP_INSTALL_EXAMPLES  "${ASAP_INSTALL_ROOT}")                          # /usr/[local]/share/<project>
    set(ASAP_INSTALL_DATA      "${ASAP_INSTALL_ROOT}")                          # /usr/[local]/share/<project>/data
    set(ASAP_INSTALL_BIN       "${CMAKE_INSTALL_BINDIR}")                       # /usr/[local]/bin
    set(ASAP_INSTALL_INCLUDE   "${CMAKE_INSTALL_INCLUDEDIR}")                   # /usr/[local]/include
    set(ASAP_INSTALL_DOC       "${CMAKE_INSTALL_DOCDIR}")                       # /usr/[local]/share/doc/<project>
    set(ASAP_INSTALL_SHORTCUTS "${CMAKE_INSTALL_DATAROOTDIR}/applications")     # /usr/[local]/share/applications
    set(ASAP_INSTALL_ICONS     "${CMAKE_INSTALL_DATAROOTDIR}/pixmaps")          # /usr/[local]/share/pixmaps
    set(ASAP_INSTALL_INIT      "/etc/init")                                     # /etc/init (upstart init scripts)
  else()
    # Install into local directory
    set(ASAP_INSTALL_ROOT      ".")                                           # ./
    set(ASAP_INSTALL_LIB       "lib")                                         # ./lib
    set(ASAP_INSTALL_SHARED    "${ASAP_INSTALL_LIB}")                         # ./lib
    set(ASAP_INSTALL_CMAKE     "${ASAP_INSTALL_ROOT}/share/cmake/${META_PROJECT_NAME}") # ./share/cmake/<project>
    set(ASAP_INSTALL_PKGCONFIG "${ASAP_INSTALL_ROOT}/share/pkgconfig")        # ./share/pkgconfig
    set(ASAP_INSTALL_EXAMPLES  "${ASAP_INSTALL_ROOT}")                        # ./
    set(ASAP_INSTALL_DATA      "${ASAP_INSTALL_ROOT}")                        # ./data
    set(ASAP_INSTALL_BIN       "bin")                                         # ./bin
    set(ASAP_INSTALL_INCLUDE   "include")                                     # ./include
    set(ASAP_INSTALL_DOC       "doc")                                         # ./doc
    set(ASAP_INSTALL_SHORTCUTS "misc")                                        # ./misc
    set(ASAP_INSTALL_ICONS     "misc")                                        # ./misc
    set(ASAP_INSTALL_INIT      "misc")                                        # ./misc
  endif()
  # cmake-format: on
endmacro()

if(${META_PROJECT_ID}_INSTALL)
  # By default we only add the install target if this is the master project. If
  # it is used as a sub-project, then the user can request to add the install
  # targets by overriding this option.

  _setup_install_dirs()

  function(asap_top_level_install)
    if(NOT ${META_PROJECT_ID}_IS_MASTER_PROJECT)
      return()
    endif()

    set(runtime "${META_PROJECT_NAME}_runtime")
    set(dev "${META_PROJECT_NAME}_dev")
    set(meta "${META_PROJECT_NAME}_meta")
    set(data "${META_PROJECT_NAME}_data")
    set(docs "${META_PROJECT_NAME}_docs")

    # Install the project meta files
    install(
      FILES AUTHORS
      DESTINATION ${ASAP_INSTALL_ROOT}
      COMPONENT ${meta})
    install(
      FILES LICENSE
      DESTINATION ${ASAP_INSTALL_ROOT}
      COMPONENT ${meta})
    install(
      FILES README.md
      DESTINATION ${ASAP_INSTALL_ROOT}
      COMPONENT ${meta})

    # Install master docs
    string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} project_id)
    string(TOLOWER ${project_id} project_id)
    set(master_sphinx_target ${project_id}_master)
    if(EXISTS ${SPHINX_BUILD_DIR}/${master_sphinx_target})
      install(
        DIRECTORY ${SPHINX_BUILD_DIR}/${master_sphinx_target}
        DESTINATION ${ASAP_INSTALL_DOC}
        COMPONENT ${docs})
    endif()

    # Install data
    if(EXISTS ${PROJECT_SOURCE_DIR}/data)
      install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/data
        DESTINATION ${ASAP_INSTALL_DATA}
        COMPONENT ${data})
    endif()
  endfunction()
else()
  function(asap_top_level_install)

  endfunction()
endif()
