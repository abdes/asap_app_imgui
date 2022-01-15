# ===-----------------------------------------------------------------------===#
# Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
# copy at https://opensource.org/licenses/BSD-3-Clause).
# SPDX-License-Identifier: BSD-3-Clause
# ===-----------------------------------------------------------------------===#

# ------------------------------------------------------------------------------
# Build Helpers to simplify target creation.
# ------------------------------------------------------------------------------

function(asap_compile_definitions target)
  #
  # Compile definitions
  #
  # ones we use for every single target
  target_compile_definitions(
    ${target}
    PRIVATE $<$<STREQUAL:${OPTION_CONTRACT_MODE},DEFAULT>:
            ASAP_CONTRACT_DEFAULT
            >
            $<$<STREQUAL:${OPTION_CONTRACT_MODE},XXX>:
            ASAP_CONTRACT_OFF
            >
            $<$<STREQUAL:${OPTION_CONTRACT_MODE},AUDIT>:
            ASAP_CONTRACT_AUDIT
            >
            $<$<CXX_COMPILER_ID:MSVC>:
            NOMINMAX
            WIN32_LEAN_AND_MEAN=1
            _WIN32_WINNT=0x0600
            >)
endfunction()

include(GenerateTemplateExportHeader)
function(asap_generate_export_headers target include_dir)
  # Set API export file and macro
  string(MAKE_C_IDENTIFIER ${target} TEMPLATE_TARGET_ID)
  string(TOUPPER ${TEMPLATE_TARGET_ID} TEMPLATE_TARGET_ID)
  set(export_file "include/${include_dir}/${target}_export.h")
  set(template_export_file "include/${include_dir}/${target}_api.h")
  set(TEMPLATE_TARGET "${target}")
  set(TEMPLATE_INCLUDE_DIR "${include_dir}")

  # Create API export headers
  generate_export_header(${target} EXPORT_FILE_NAME ${export_file}
                         EXPORT_MACRO_NAME ${TEMPLATE_TARGET_ID}_API)
  generate_template_export_header(${target} ${TEMPLATE_TARGET_ID}
                                  ${template_export_file})
endfunction()
