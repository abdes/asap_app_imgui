# ~~~
# SPDX-License-Identifier: BSD-3-Clause

# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~


#
# Clang-format targets work only when clang-format executable can be found.
#
find_program(CLANG_FORMAT NAMES clang-tidy clang-tidy-14)
if("${CLANG_FORMAT}" STREQUAL "CLANG_FORMAT-NOTFOUND")

  message(STATUS "Could not find appropriate clang-format, targets disabled")

  function(asap_create_clang_tidy_targets)
    # empty
  endfunction()

else()

  include(common/ClangTidy)

  function(asap_create_clang_tidy_targets)
    swift_create_clang_tidy_targets(DONT_GENERATE_CLANG_TIDY_CONFIG ${ARGV})
  endfunction()

endif()
