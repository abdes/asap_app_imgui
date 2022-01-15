# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

include(common/Valgrind)

mark_as_advanced(${PROJECT_NAME}_ENABLE_PROFILING)
set(${PROJECT_NAME}_ENABLE_PROFILING ${ASAP_WITH_VALGRIND})

function(asap_add_valgrind_memcheck target)
  swift_add_valgrind_memcheck(${target}, ${ARGN})
endfunction()

function(asap_add_valgrind_callgrind target)
  swift_add_valgrind_callgrind(${target}, ${ARGN})
endfunction()

function(asap_add_valgrind_massif target)
  swift_add_valgrind_massif(${target}, ${ARGN})
endfunction()
