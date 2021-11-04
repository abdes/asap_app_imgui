# ~~~
#        Copyright The Authors 2018.
#    Distributed under the 3-Clause BSD License.
#    (See accompanying file LICENSE or copy at
#   https://opensource.org/licenses/BSD-3-Clause)
# ~~~

# ------------------------------------------------------------------------------
# Google Sanitizers
# ------------------------------------------------------------------------------

if(OPTION_GOOGLE_ASAN)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using Clang or GNU compilers
    set(ASAN_COMPILER_FLAGS "-fno-omit-frame-pointer -fsanitize=address -fsanitize=leak"
    )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_COMPILER_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_COMPILER_FLAGS}")
  else()
    message(FATAL_ERROR "Google Address Sanitizer can only be used with clang or gnu compilers")
  endif()
endif()

if(OPTION_GOOGLE_USAN)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(USAN_COMPILER_FLAGS "-fsanitize=memory -fPIE -pie -Qunused-arguments -fsanitize-memory-track-origins")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USAN_COMPILER_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USAN_COMPILER_FLAGS}")
  else()
    message(FATAL_ERROR "Google Memory Sanitizer can only be used with clang compiler")
  endif()
endif()

if(OPTION_GOOGLE_TSAN)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(TSAN_COMPILER_FLAGS "-fsanitize=thread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TSAN_COMPILER_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TSAN_COMPILER_FLAGS}")
  else()
    message(FATAL_ERROR "Google Address Sanitizer can only be used with clang or gnu compilers")
  endif()
endif()
