# ~~~
# SPDX-License-Identifier: BSD-3-Clause

# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~


# ==================================================================================================
# Build instructions
# ==================================================================================================

set(MAIN_TEST_TARGET_NAME ${MODULE_TARGET_NAME}_test)

asap_add_test(
  ${MAIN_TEST_TARGET_NAME}
  UNIT_TEST
  VALGRIND_MEMCHECK
  SRCS
  "logging_test.cpp"
  "main.cpp"
  LINK
  ${META_PROJECT_NAME}::logging
  gtest_main
  gmock_main
  COMMENT
  "ASAP common logging unit tests")

gtest_discover_tests(${MAIN_TEST_TARGET_NAME})

# Add support for (optional) code quality tools
asap_configure_sanitizers(${MAIN_TEST_TARGET_NAME})
swift_add_valgrind_massif(${MAIN_TEST_TARGET_NAME})
swift_add_valgrind_callgrind(${MAIN_TEST_TARGET_NAME})
