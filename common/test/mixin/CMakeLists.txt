# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

# ==============================================================================
# Build instructions
# ==============================================================================

set(target_name ${MODULE_TARGET_NAME}_mixin_test)

asap_add_test(
  ${target_name}
  UNIT_TEST
  VALGRIND_MEMCHECK
  SRCS
  # sources
  "mixin_chained_init_test.cpp"
  "mixin_curry_test.cpp"
  "mixin_interfaces_test.cpp"
  "mixin_top_test.cpp"
  "../main.cpp"
  LINK
  asap::common
  gtest
  gmock
  COMMENT
  "Mixin support library unit tests")

gtest_discover_tests(${target_name})

# Add support for (optional) code quality tools
asap_add_sanitizers(${target_name})
swift_add_valgrind_massif(${target_name})
swift_add_valgrind_callgrind(${target_name})
