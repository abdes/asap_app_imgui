#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)

# Boost
set(BOOST_URL https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.bz2 CACHE STRING "Boost download URL" FORCE)
set(BOOST_URL_SHA256 2684c972994ee57fc5632e03bf044746f6eb45d4920c343937a465fd67a5adba CACHE STRING "Boost download URL SHA256 checksum" FORCE)
add_subdirectory(boost-cmake)

# Catch2
# EXCLUDE_FROM_ALL to not have it install its files during install
add_subdirectory(catch2 EXCLUDE_FROM_ALL)

# spdlog
SET(SPDLOG_BUILD_TESTING OFF CACHE BOOL "Build spdlog tests")
add_subdirectory(spdlog)
