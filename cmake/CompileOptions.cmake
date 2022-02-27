# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

# ------------------------------------------------------------------------------
# Set a common set of compiler options and warning flags
# ------------------------------------------------------------------------------

#
# Call swift_set_compile_options() for any target to set the Swift default set
# of compiler options. This includes
# ~~~
# - exceptions disabled
# - rtti disabled
# - strict aliasing disabled
# - Warnings as errors (-Werror)
# - Extensive set of enabled warnings
# ~~~
#
# Exceptions and/or RTTI can be selectively enabled for a target be passing
# EXCEPTIONS and/or RTTI as a parameter, eg
#
# ~~~
#    swift_set_compile_options(sample-target EXCEPTIONS RTTI)
# ~~~
#
# will enable exceptions and rtti for sample-target only
#
# Warning flags can be removed from the default set by passing REMOVE followed
# by a list of warning flags, eg
#
# ~~~
#    swift_set_compile_options(sample-target REMOVE -Wconversion)
# ~~~
#
# will prevent -Wconversion from being passed to the compiler for sample-target
# only
#
# Similarly extra options can be given by passing ADD followed by a list of
# warning flags (or other compiler options), eg
#
# ~~~
#    swift_set_compile_options(sample-target ADD -Wformat=2)
# ~~~
#
# will pass -Wformat=2 to the compiler for sample-target only
#
# By default -Werror is set, but this can be prevented by passing WARNING as a
# parameter, eg
#
# ~~~
#    swift_set_compile_options(sample-target WARNING)
# ~~~
#
# will disable warnings-as-errors for sample-target only
#
# All flags will be checked for suitability with the in-use compilers before
# being selected. This is important since Swift code tends to be compiled with a
# wide variety of compilers which may not support the same set of flags and
# options. Therefore, it should be preferred to use this function to set
# compiler flags and options rather than target_compile_options()
#
# NOTE: user's can call on EXTRA_FLAGS to augment the default list of flags
# before flags are removed with REMOVE and subsequently added with ADD.
#

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

function(swift_set_compile_options)
  set(argOption "WARNING" "NO_EXCEPTIONS" "EXCEPTIONS" "NO_RTTI" "RTTI")
  set(argSingle "")
  set(argMulti "ADD" "REMOVE" "EXTRA_FLAGS")

  unset(x_WARNING)
  unset(x_ADD)
  unset(x_REMOVE)
  unset(x_EXTRA_FLAGS)

  cmake_parse_arguments(x "${argOption}" "${argSingle}" "${argMulti}" ${ARGN})
  set(targets ${x_UNPARSED_ARGUMENTS})

  if(x_RTTI AND x_NO_RTTI)
    message(FATAL_ERROR "RTTI and NO_RTTI can't be used together")
  endif()

  if(x_EXCEPTIONS AND x_NO_EXCEPTIONS)
    message(FATAL_ERROR "EXCEPTIONS and NO_EXCEPTIONS can't be used together")
  endif()

  foreach(flag ${x_ADD} ${x_REMOVE})
    if(${flag} STREQUAL "-Werror")
      message(
        FATAL_ERROR
          "Do not specify -Werror directly, use WARNING to disable -Werror")
    endif()
    if(${flag} STREQUAL "-Wno-error")
      message(
        FATAL_ERROR
          "Do not specify -Wno-error directly, use WARNING to disable -Werror")
    endif()
  endforeach()

  if(DEFINED SWIFT_COMPILER_WARNING_ARE_ERROR)
    if(SWIFT_COMPILER_WARNING_ARE_ERROR)
      if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(all_flags /WX)
      else()
        set(all_flags -Werror -Wno-error=deprecated-declarations)
      endif()
    else()
      set(all_flags -Wno-error)
    endif()
  else()
    if(NOT x_WARNING)
      if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(all_flags /WX)
      else()
        set(all_flags -Werror -Wno-error=deprecated-declarations)
      endif()
    endif()
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # using Clang
    list(
      APPEND
      all_flags
      -Weverything
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-c++98-c++11-compat-pedantic
      -Wno-padded
      -Wno-documentation-unknown-command
      -Wno-switch-enum
      -Wno-unused-macros
      -Wno-disabled-macro-expansion)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    list(
      APPEND
      all_flags
      -Wall
      -Wextra
      -Wcast-align
      -Wcast-qual
      -Wctor-dtor-privacy
      -Wdisabled-optimization
      -Wformat=2
      -Winit-self
      -Wmissing-declarations
      -Wmissing-include-dirs
      -Wold-style-cast
      -Woverloaded-virtual
      -Wredundant-decls
      -Wshadow
      -Wsign-conversion
      -Wsign-promo
      -Wundef
      -Werror
      -Wno-unused)
    if(NOT DEFINED CMAKE_CXX_CLANG_TIDY)
      list(APPEND all_flags -Wlogical-op -Wstrict-null-sentinel)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    list(APPEND all_flags /EHsc /MP /W4)
  endif()

  if(x_REMOVE)
    foreach(flag ${x_REMOVE})
      list(FIND all_flags ${flag} found)
      if(found EQUAL -1)
        message(
          FATAL_ERROR
            "Compiler flag '${flag}' specified for removal is not part of the set of common compiler flags"
        )
      endif()
    endforeach()
    list(REMOVE_ITEM all_flags ${x_REMOVE})
  endif()

  list(APPEND all_flags ${x_ADD})

  unset(final_flags)

  get_property(enabled_languages GLOBAL PROPERTY ENABLED_LANGUAGES)
  list(FIND enabled_languages "C" c_enabled)
  list(FIND enabled_languages "CXX" cxx_enabled)

  foreach(flag ${all_flags})
    string(TOUPPER ${flag} sanitised_flag)
    string(REPLACE "+" "X" sanitised_flag ${sanitised_flag})
    string(REGEX REPLACE "[^A-Za-z_0-9]" "_" sanitised_flag ${sanitised_flag})

    set(c_supported HAVE_C_FLAG_${sanitised_flag})
    string(REGEX REPLACE "_+" "_" c_supported ${c_supported})
    set(cxx_supported HAVE_CXX_FLAG_${sanitised_flag})
    string(REGEX REPLACE "_+" "_" cxx_supported ${cxx_supported})

    if(${c_enabled} GREATER -1)
      if(MSVC)
        check_c_compiler_flag("/WX ${flag}" ${c_supported})
      else()
        check_c_compiler_flag("-Werror ${flag}" ${c_supported})
      endif()
      if(${${c_supported}})
        list(APPEND final_flags $<$<COMPILE_LANGUAGE:C>:${flag}>)
      endif()
    endif()

    if(${cxx_enabled} GREATER -1)
      if(MSVC)
        check_cxx_compiler_flag("/WX ${flag}" ${cxx_supported})
      else()
        check_cxx_compiler_flag("-Werror ${flag}" ${cxx_supported})
      endif()
      if(${${cxx_supported}})
        list(APPEND final_flags $<$<COMPILE_LANGUAGE:CXX>:${flag}>)
      endif()
    endif()
  endforeach()

  foreach(target ${targets})
    target_compile_options(${target} PRIVATE ${final_flags})
  endforeach()

endfunction()

function(asap_set_compile_options)
  swift_set_compile_options(${ARGV})
endfunction()

# ------------------------------------------------------------------------------
# Clean compiler settings and options
# ------------------------------------------------------------------------------

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  # remove default warning level from CMAKE_CXX_FLAGS_INIT CMake adds compiler
  # warning levels by default and for MSVC, it uses /W3 which we want to
  # override with /W4. The override does make MSVC complain though, so we just
  # strip any argument already added by cmake before we set ours.
  string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()
