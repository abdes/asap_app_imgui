# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

include_guard(GLOBAL)

# ------------------------------------------------------------------------------
# Set a common set of compile definitions
# ------------------------------------------------------------------------------

function(asap_set_compile_definitions target)
  set(argOption "NO_CONTRACT")
  set(argSingle "")
  set(argMulti "ADD" "REMOVE")

  unset(x_WARNING)
  unset(x_ADD)
  unset(x_REMOVE)

  cmake_parse_arguments(x "${argOption}" "${argSingle}" "${argMulti}" ${ARGN})

  set(all_flags)

  # Provide a way to distinguish between debug and release builds via
  # preprocessor define
  list(APPEND all_flags "$<$<CONFIG:Debug>:ASAP_IS_DEBUG_BUILD>")

  if(MSVC)
    list(APPEND all_flags "NOMINMAX" "WIN32_LEAN_AND_MEAN=1"
         "_WIN32_WINNT=0x0600")
    # Disabling warning for not using "secure-but-not-standard" STL algos
    list(APPEND all_flags "_CRT_SECURE_NO_WARNINGS" "_SCL_SECURE_NO_WARNINGS")
  endif()

  if(x_REMOVE)
    foreach(flag ${x_REMOVE})
      list(FIND all_flags ${flag} found)
      if(found EQUAL -1)
        message(
          FATAL_ERROR
            "Compiler flag '${flag}' specified for removal is not part of the set of common
     compiler flags")
      endif()
    endforeach()
    list(REMOVE_ITEM all_flags ${x_REMOVE})
  endif()

  list(APPEND all_flags ${x_ADD})
  target_compile_definitions(${target} PRIVATE ${all_flags})

  # If linking against asap_contract, set the contract mode based on the build
  # type. Use generator expressions only, do not check for CMAKE_BUILD_TYPE
  # which is not friendly with multi-config generators.
  #
  # Do not add this definition if we are testing asap-_contract
  if(TARGET asap_contract AND NOT ASAP_CONTRACT_TESTING)
    if(NOT DEFINED OPTION_CONTRACT_MODE)
      target_compile_definitions(
        ${target}
        PRIVATE $<$<CONFIG:Debug>:ASAP_CONTRACT_AUDIT>
                $<$<CONFIG:RelWithDebInfo>:ASAP_CONTRACT_DEFAULT>
                $<$<CONFIG:Release,RelMinSize>:ASAP_CONTRACT_OFF>)
    else()
      target_compile_definitions(
        ${target} PRIVATE "ASAP_CONTRACT_${OPTION_CONTRACT_MODE}")
    endif()
  endif()
endfunction()
