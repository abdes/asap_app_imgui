# ~~~
# SPDX-License-Identifier: BSD-3-Clause

# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~


include(common/Valgrind)

function(asap_add_valgrind_memcheck target)
  swift_add_valgrind_memcheck(${target}, ${ARGN})
endfunction()

function(asap_add_valgrind_callgrind target)
  swift_add_valgrind_callgrind(${target}, ${ARGN})
endfunction()

function(asap_add_valgrind_massif target)
  swift_add_valgrind_massif(${target}, ${ARGN})
endfunction()
