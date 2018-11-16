
# ------------------------------------------------------------------------------
# CppCheck
# ------------------------------------------------------------------------------

if(OPTION_CPPCHECK)

  list(APPEND CPPCHECK_CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
    )

  ExternalProject_Add(
    cppcheck
    GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
    GIT_TAG             1.85
    GIT_SHALLOW         1
    CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
    PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
    TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
    STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
    DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
    SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
    BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
  )
  set(CMAKE_CXX_CPPCHECK "${CMAKE_BINARY_DIR}/bin/cppcheck")

  macro(enable_cppcheck)
    list(APPEND CMAKE_CXX_CPPCHECK
      "--enable=all"
      "--language=c++"
      "--std=c++14"
      "--error-exitcode=1"
      "--inline-suppr"
      "--suppress=*:*_build*"
      "--suppress=*:*third_party*"
      "--suppress=unmatchedSuppression:*"
      )
  endmacro()

else()
  macro(enable_cppcheck)
  endmacro()
endif()
