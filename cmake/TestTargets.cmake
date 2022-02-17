# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

include(common/TestTargets)

macro(asap_add_test target)
  swift_add_test("${target}" ${ARGN})

  asap_set_compile_options(${target} WARNING)
  asap_set_compile_definitions(${target})
  if(TARGET gtest AND BUILD_SHARED_LIBS)
    target_compile_definitions(${target} PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY)
    if(MSVC)
      target_compile_options(${target} PRIVATE /wd4251)
    endif()
  endif()
  set_target_properties(${target} PROPERTIES FOLDER "Unit Tests")
endmacro()

macro(asap_add_test_runner target)
  swift_add_test_runner("${target}" ${ARGN})
endmacro()
