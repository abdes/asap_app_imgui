# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

message("=> [third-party modules]")

include(FetchContent)
# We make sure that we have 'third_party' in the name so that the targets get
# excluded from the generated target lists for the various tools.
set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/third_party_deps)

include(FetchGoogleTest)
