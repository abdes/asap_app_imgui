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
if(NOT (WIN32 OR MINGW))
  SET(SPDLOG_BUILD_SHARED ${BUILD_SHARED_LIBS} CACHE BOOL "Build spdlog as a shared library")
endif()
add_subdirectory(spdlog)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
add_subdirectory(glfw)

# GLM
set(GLM_TEST_ENABLE OFF CACHE BOOL "Build GLM tests")
add_subdirectory(glm)

# cxxopts - used to parse command line arguments
SET(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "Build cxxopts examples")
add_subdirectory(cxxopts)

# date - used for time formatting in the logging widget
SET(USE_SYSTEM_TZ_DB ON CACHE BOOL "Use system TZ db for date library" FORCE)
SET(ENABLE_DATE_TESTING OFF CACHE BOOL "Build tests for the date library" FORCE)
set(CMAKE_CXX_STANDARD 14 CACHE STRING "The C++ standard whose features are requested.")
add_subdirectory(date)

# cpptoml - used to save/load the configuration files
set(CPPTOML_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set(ENABLE_LIBCXX OFF CACHE BOOL "Use libc++ for the C++ standard library" FORCE)
add_subdirectory(cpptoml)

