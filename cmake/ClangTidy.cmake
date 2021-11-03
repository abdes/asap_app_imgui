# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

# ------------------------------------------------------------------------------
# Helper module providing a macro that can be used to declare that a target is
# interested in participating in a build that uses clang-tidy instead of the
# regular C++ compiler.
# ------------------------------------------------------------------------------

if(OPTION_CLANG_TIDY)
  find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy" "clang-tidy-4.0"
    DOC "Path to clang-tidy executable")

  mark_as_advanced(CLANG_TIDY_EXE)

  if(NOT CLANG_TIDY_EXE)
    message(FATAL_ERROR "unable to locate clang-tidy")
    macro(tidy_target TARGET_NAME)
      # left empty on purpose
    endmacro()
  else()
    list(APPEND CLANG_TIDY_BIN_ARGS -header-filter=.*
         -checks=*,-clang-analyzer-alpha.*)
    macro(tidy_target TARGET_NAME)
      set_target_properties(
        ${target_name} PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_EXE}
                                  ${RUN_CLANG_TIDY_BIN_ARGS})
    endmacro()
  endif()
else()
  macro(tidy_target TARGET_NAME)
    # left empty on purpose
  endmacro()
endif()
