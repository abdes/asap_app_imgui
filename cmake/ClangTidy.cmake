# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

#
# clang-tidy targets work only when clang-tidy executable can be found and the
# compiler being used is clang (otherwise gcc/others options may cause errors
# for clang-tidy)
#
find_program(CLANG_TIDY NAMES clang-tidy clang-tidy-14)
if(NOT "${CLANG_TIDY}" STREQUAL "CLANG_TIDY-NOTFOUND")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    include(common/ClangTidy)

    function(asap_create_clang_tidy_targets)
      swift_create_clang_tidy_targets(DONT_GENERATE_CLANG_TIDY_CONFIG ${ARGV})
    endfunction()
    return()

  else()
    message(
      STATUS "Not using clang as a compiler, disabling clang-tidy targets")
  endif()
else()
  message(STATUS "Could not find appropriate clang-tidy, targets disabled")
endif()

function(asap_create_clang_tidy_targets)
  # empty
endfunction()
