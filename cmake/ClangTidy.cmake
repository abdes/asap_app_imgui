# ------------------------------------------------------------------------------
# Clang Tidy
# ------------------------------------------------------------------------------

if(OPTION_CLANG_TIDY)

  find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy" "clang-tidy-4.0"
    DOC "Path to clang-tidy executable")

  if(NOT CLANG_TIDY_EXE)
    message(FATAL_ERROR "unable to locate clang-tidy")
    macro(set_tidy_target_properties TARGET_NAME)
    endmacro()

  else ()

    list(APPEND CLANG_TIDY_BIN_ARGS
      -header-filter=.*
      -checks=*,-clang-analyzer-alpha.*
      )

    macro(tidy_target target_name)
      set_target_properties(
        ${target_name} PROPERTIES
        CXX_CLANG_TIDY ${CLANG_TIDY_EXE} ${RUN_CLANG_TIDY_BIN_ARGS})
    endmacro()

  endif ()

else()
  macro(tidy_target target_name)
  endmacro()
endif()
