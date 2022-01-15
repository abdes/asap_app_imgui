# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

#
# Code coverage only works with clang/gcc.
#
if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang|GNU")

  include(common/CodeCoverage)

  function(asap_add_code_coverage)
    add_code_coverage(${ARGV})
  endfunction()

  function(asap_add_code_coverage_all_targets)
    add_code_coverage_all_targets(${ARGV})
  endfunction()

else()
  function(target_code_coverage)
    # empty
  endfunction()

  function(asap_add_code_coverage)
    # empty
  endfunction()

  function(asap_add_code_coverage_all_targets)
    # empty
  endfunction()

endif()
