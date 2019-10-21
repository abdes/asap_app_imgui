#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)

message("=> [third-party modules]")

# Catch2
# EXCLUDE_FROM_ALL to not have it install its files during install
add_subdirectory(catch2 EXCLUDE_FROM_ALL)

# spdlog
SET(SPDLOG_BUILD_TESTING OFF CACHE BOOL "Build spdlog tests")
SET(SPDLOG_INSTALL ON CACHE BOOL "Generate the spdlog install target")
add_subdirectory(spdlog)
