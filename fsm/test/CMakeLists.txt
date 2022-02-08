# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

# ==============================================================================
# Build instructions
# ==============================================================================

set(MAIN_TEST_TARGET_NAME ${MODULE_TARGET_NAME}_test)

asap_add_test(
  ${MAIN_TEST_TARGET_NAME}
  UNIT_TEST
  VALGRIND_MEMCHECK
  SRCS
  "state_machine_error_test.cpp"
  "fsm_test.cpp"
  "fsm_example_test.cpp"
  "main.cpp"
  LINK
  asap::common
  gtest_main
  gmock_main
  COMMENT
  "ASAP state machine module unit tests")

# Target specific compiler features
target_compile_features(${MAIN_TEST_TARGET_NAME} PRIVATE cxx_constexpr)

# Target specific libraries
target_link_libraries(${MAIN_TEST_TARGET_NAME} PRIVATE asap::fsm)

gtest_discover_tests(${MAIN_TEST_TARGET_NAME})

# Add support for (optional) code quality tools
asap_add_sanitizers(${MAIN_TEST_TARGET_NAME})
swift_add_valgrind_massif(${MAIN_TEST_TARGET_NAME})
swift_add_valgrind_callgrind(${MAIN_TEST_TARGET_NAME})
