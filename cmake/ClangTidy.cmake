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
#
# We do not set clang-tidy configuration here, instead we rely on an external
# .clang-tidy config file in the project root or somewhere else in the hierarchy
# of modules.
# ------------------------------------------------------------------------------

if(OPTION_CLANG_TIDY)
  find_program(
    CLANG_TIDY_COMMAND
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable")

  if(NOT CLANG_TIDY_COMMAND)
    message(FATAL_ERROR "Unable to locate clang-tidy")
    macro(tidy_target TARGET_NAME)
      # left empty on purpose
    endmacro()
  else()
    macro(tidy_target TARGET_NAME)
      set_target_properties(
        ${TARGET_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    endmacro()
  endif()
else()
  macro(tidy_target TARGET_NAME)
    # left empty on purpose
  endmacro()
endif()
