# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

#
# Clang-format targets work only when clang-format executable can be found.
#
find_program(CLANG_FORMAT NAMES clang-format clang-format-14)
if("${CLANG_FORMAT}" STREQUAL "CLANG_FORMAT-NOTFOUND")

  message(STATUS "Could not find appropriate clang-format, targets disabled")

  function(asap_setup_clang_format)
    # empty
  endfunction()

else()

  include(common/ClangFormat)

  function(asap_setup_clang_format)
    swift_setup_clang_format(${ARGV})
  endfunction()

endif()
