# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

# ==============================================================================
# Build instructions
# ==============================================================================

# If running with google sanitizers, skill all these targets as they are abort
# and segmentation fault tests that do not make sense to run under sanitizers
if(ASAP_WITH_GOOGLE_ASAN
   OR ASAP_WITH_GOOGLE_UBSAN
   OR ASAP_WITH_GOOGLE_TSAN)
  return()
endif()

function(_remove_contract_mode target)
  get_target_property(defs ${target} COMPILE_DEFINITIONS)
  list(REMOVE_ITEM defs ASAP_CONTRACT_DEFAULT ASAP_CONTRACT_AUDIT
       ASAP_CONTRACT_OFF)
  set_property(TARGET ${target} PROPERTY COMPILE_DEFINITIONS ${defs})
endfunction()

# ------------------------------------------------------------------------------
# Unit Test Macros
# ------------------------------------------------------------------------------

set(target_name ${MODULE_TARGET_NAME}_contract_ut_test)

asap_add_test(
  ${target_name}
  UNIT_TEST
  VALGRIND_MEMCHECK
  SRCS
  "contract_ut_gtest_test.cpp"
  LINK
  asap::common
  asap::contract
  gtest
  gmock
  COMMENT
  "Contract unit test macros tests")

# Remove any existing contract mode compiler definition and force the mode to
# DEFAULT for this target.
_remove_contract_mode(${target_name})
target_compile_definitions(${target_name} PRIVATE ASAP_CONTRACT_DEFAULT)

gtest_discover_tests(${target_name})
