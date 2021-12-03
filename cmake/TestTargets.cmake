# ~~~
# SPDX-License-Identifier: BSD-3-Clause

# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~


include(common/TestTargets)

macro(asap_add_test target)
  swift_add_test("${target}" ${ARGN})

  asap_set_compile_options(${target})
  asap_set_compile_definitions(${target})
  set_target_properties(${target} PROPERTIES FOLDER "Unit Tests")
endmacro()

macro(asap_add_test_runner target)
  swift_add_test_runner("${target}" ${ARGN})
endmacro()
