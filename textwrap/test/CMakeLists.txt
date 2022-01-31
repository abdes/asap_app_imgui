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
  "tokenizer_test.cpp"
  "textwrapper_example.cpp"
  "textwrapper_test.cpp"
  "main.cpp"
  LINK
  asap::common
  asap::logging
  asap::textwrap
  gtest_main
  gmock_main
  COMMENT
  "ASAP textwrap unit tests")

target_include_directories(${MAIN_TEST_TARGET_NAME} PRIVATE "../src")

gtest_discover_tests(${MAIN_TEST_TARGET_NAME})

# Add support for (optional) code quality tools
asap_add_sanitizers(${MAIN_TEST_TARGET_NAME})
swift_add_valgrind_massif(${MAIN_TEST_TARGET_NAME})
swift_add_valgrind_callgrind(${MAIN_TEST_TARGET_NAME})

# ------------------------------------------------------------------------------
# Long-running random tests
# ------------------------------------------------------------------------------

set(target_name ${MODULE_TARGET_NAME}_random_test)

asap_add_test(
  ${target_name}
  UNIT_TEST
  VALGRIND_MEMCHECK
  SRCS
  "textwrapper_random_test.cpp"
  "main.cpp"
  LINK
  asap::common
  asap::logging
  asap::textwrap
  gtest_main
  gmock_main
  COMMENT
  "ASAP textwrap long-running random test")

target_include_directories(${target_name} PRIVATE "../src")

gtest_discover_tests(${target_name})

# Add support for (optional) code quality tools
asap_add_sanitizers(${target_name})
swift_add_valgrind_massif(${target_name})
swift_add_valgrind_callgrind(${target_name})
