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
  "traits_logical_test.cpp"
  "flag_ops_test.cpp"
  "main.cpp"
  LINK
  ${META_PROJECT_NAME}::common
  gtest_main
  gmock_main
  COMMENT
  "ASAP common unit tests")

  # Target specific compiler features
target_compile_features(${MAIN_TEST_TARGET_NAME} PRIVATE cxx_constexpr)

gtest_discover_tests(${MAIN_TEST_TARGET_NAME})

# Add support for (optional) code quality tools
asap_configure_sanitizers(${MAIN_TEST_TARGET_NAME})
swift_add_valgrind_massif(${MAIN_TEST_TARGET_NAME})
swift_add_valgrind_callgrind(${MAIN_TEST_TARGET_NAME})
