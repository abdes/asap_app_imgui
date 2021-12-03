/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

// -------------------------------------------------------------------------------------------------
// Compiler detection
// -------------------------------------------------------------------------------------------------

#if defined(ASAP_VERSION_ENCODE_INTERNAL_)
#undef ASAP_VERSION_ENCODE_INTERNAL_
#endif
#define ASAP_VERSION_ENCODE_INTERNAL_(major, minor, revision)                                      \
  (((major)*1000000) + ((minor)*1000) + (revision))

#if defined(ASAP_CLANG_VERSION)
#undef ASAP_CLANG_VERSION
#endif
#if defined(__clang__)
#define ASAP_CLANG_VERSION                                                                         \
  ASAP_VERSION_ENCODE_INTERNAL_(__clang_major__, __clang_minor__, __clang_patchlevel__)
#endif

/*
 * For clang you shouldn't test its version number, you should use the feature checking macros
 * (https://clang.llvm.org/docs/LanguageExtensions.html#feature-checking-macros).
 */
#if defined(ASAP_CLANG_VERSION_CHECK)
#undef ASAP_CLANG_VERSION_CHECK
#endif
#if defined(ASAP_CLANG_VERSION)
#define ASAP_CLANG_VERSION_CHECK(major, minor, patch)                                              \
  (ASAP_CLANG_VERSION >= ASAP_VERSION_ENCODE_INTERNAL_(major, minor, patch))
#else
#define ASAP_CLANG_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(ASAP_MSVC_VERSION)
#undef ASAP_MSVC_VERSION
#endif
#if defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 140000000) && !defined(__ICL)
#define ASAP_MSVC_VERSION                                                                          \
  ASAP_VERSION_ENCODE_INTERNAL_(_MSC_FULL_VER / 10000000, (_MSC_FULL_VER % 10000000) / 100000,     \
      (_MSC_FULL_VER % 100000) / 100)
#elif defined(_MSC_FULL_VER) && !defined(__ICL)
#define ASAP_MSVC_VERSION                                                                          \
  ASAP_VERSION_ENCODE_INTERNAL_(                                                                   \
      _MSC_FULL_VER / 1000000, (_MSC_FULL_VER % 1000000) / 10000, (_MSC_FULL_VER % 10000) / 10)
#elif defined(_MSC_VER) && !defined(__ICL)
#define ASAP_MSVC_VERSION ASAP_VERSION_ENCODE_INTERNAL_(_MSC_VER / 100, _MSC_VER % 100, 0)
#endif

#if defined(ASAP_MSVC_VERSION_CHECK)
#undef ASAP_MSVC_VERSION_CHECK
#endif
#if !defined(ASAP_MSVC_VERSION)
#define ASAP_MSVC_VERSION_CHECK(major, minor, patch) (0)
#elif defined(_MSC_VER) && (_MSC_VER >= 1400)
#define ASAP_MSVC_VERSION_CHECK(major, minor, patch)                                               \
  (_MSC_FULL_VER >= ((major * 10000000) + (minor * 100000) + (patch)))
#elif defined(_MSC_VER) && (_MSC_VER >= 1200)
#define ASAP_MSVC_VERSION_CHECK(major, minor, patch)                                               \
  (_MSC_FULL_VER >= ((major * 1000000) + (minor * 10000) + (patch)))
#else
#define ASAP_MSVC_VERSION_CHECK(major, minor, patch) (_MSC_VER >= ((major * 100) + (minor)))
#endif

/*
 * Note that the GNUC and GCC macros are different. Many compilers masquerade as GCC (by defining
 * __GNUC__, __GNUC_MINOR__, and __GNUC_PATCHLEVEL__, but often do not implement all the features of
 * the version of GCC they pretend to support.
 *
 * To work around this, the ASAP_GCC_VERSION macro is only defined for GCC, whereas
 * ASAP_GNUC_VERSION will be defined whenever a compiler defines __GCC__.
 */

#if defined(ASAP_GNUC_VERSION)
#undef ASAP_GNUC_VERSION
#endif
#if defined(__GNUC__) && defined(__GNUC_PATCHLEVEL__)
#define ASAP_GNUC_VERSION                                                                          \
  ASAP_VERSION_ENCODE_INTERNAL_(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined(__GNUC__)
#define ASAP_GNUC_VERSION ASAP_VERSION_ENCODE_INTERNAL_(__GNUC__, __GNUC_MINOR__, 0)
#endif

#if defined(ASAP_GNUC_VERSION_CHECK)
#undef ASAP_GNUC_VERSION_CHECK
#endif
#if defined(ASAP_GNUC_VERSION)
#define ASAP_GNUC_VERSION_CHECK(major, minor, patch)                                               \
  (ASAP_GNUC_VERSION >= ASAP_VERSION_ENCODE_INTERNAL_(major, minor, patch))
#else
#define ASAP_GNUC_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(ASAP_GCC_VERSION)
#undef ASAP_GCC_VERSION
#endif
#if defined(ASAP_GNUC_VERSION) && !defined(__clang__)
#define ASAP_GCC_VERSION ASAP_GNUC_VERSION
#endif

#if defined(ASAP_GCC_VERSION_CHECK)
#undef ASAP_GCC_VERSION_CHECK
#endif
#if defined(ASAP_GCC_VERSION)
#define ASAP_GCC_VERSION_CHECK(major, minor, patch)                                                \
  (ASAP_GCC_VERSION >= ASAP_VERSION_ENCODE_INTERNAL_(major, minor, patch))
#else
#define ASAP_GCC_VERSION_CHECK(major, minor, patch) (0)
#endif

// -------------------------------------------------------------------------------------------------
// Compiler attribute check
// -------------------------------------------------------------------------------------------------

#if defined(ASAP_HAS_ATTRIBUTE)
#undef ASAP_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
#define ASAP_HAS_ATTRIBUTE(attribute) __has_attribute(attribute)
#else
#define ASAP_HAS_ATTRIBUTE(attribute) (0)
#endif

#if defined(ASAP_GCC_HAS_ATTRIBUTE)
#undef ASAP_GCC_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
#define ASAP_GCC_HAS_ATTRIBUTE(attribute, major, minor, patch) ASAP_HAS_ATTRIBUTE(attribute)
#else
#define ASAP_GCC_HAS_ATTRIBUTE(attribute, major, minor, patch)                                     \
  ASAP_GCC_VERSION_CHECK(major, minor, patch)
#endif

#if defined(ASAP_HAS_CPP_ATTRIBUTE)
#undef ASAP_HAS_CPP_ATTRIBUTE
#endif
#if defined(__has_cpp_attribute) && defined(__cplusplus)
#define ASAP_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
#define ASAP_HAS_CPP_ATTRIBUTE(attribute) (0)
#endif

#if defined(ASAP_GCC_HAS_CPP_ATTRIBUTE)
#undef ASAP_GCC_HAS_CPP_ATTRIBUTE
#endif
#if defined(__has_cpp_attribute) && defined(__cplusplus)
#define ASAP_GCC_HAS_CPP_ATTRIBUTE(attribute, major, minor, patch) __has_cpp_attribute(attribute)
#else
#define ASAP_GCC_HAS_CPP_ATTRIBUTE(attribute, major, minor, patch)                                 \
  ASAP_GCC_VERSION_CHECK(major, minor, patch)
#endif

// -------------------------------------------------------------------------------------------------
// Compiler builtin check
// -------------------------------------------------------------------------------------------------

#if defined(ASAP_HAS_BUILTIN)
#undef ASAP_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#define ASAP_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
#define ASAP_HAS_BUILTIN(builtin) (0)
#endif

#if defined(ASAP_GCC_HAS_BUILTIN)
#undef ASAP_GCC_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#define ASAP_GCC_HAS_BUILTIN(builtin, major, minor, patch) __has_builtin(builtin)
#else
#define ASAP_GCC_HAS_BUILTIN(builtin, major, minor, patch)                                         \
  ASAP_GCC_VERSION_CHECK(major, minor, patch)
#endif

// -------------------------------------------------------------------------------------------------
// Compiler feature check
// -------------------------------------------------------------------------------------------------

#if defined(ASAP_HAS_FEATURE)
#undef ASAP_HAS_FEATURE
#endif
#if defined(__has_feature)
#define ASAP_HAS_FEATURE(feature) __has_feature(feature)
#else
#define ASAP_HAS_FEATURE(feature) (0)
#endif

#if defined(ASAP_GCC_HAS_FEATURE)
#undef ASAP_GCC_HAS_FEATURE
#endif
#if defined(__has_feature)
#define ASAP_GCC_HAS_FEATURE(feature, major, minor, patch) __has_feature(feature)
#else
#define ASAP_GCC_HAS_FEATURE(feature, major, minor, patch)                                         \
  ASAP_GCC_VERSION_CHECK(major, minor, patch)
#endif

// -------------------------------------------------------------------------------------------------
// Compiler extension check
// -------------------------------------------------------------------------------------------------

#if defined(ASAP_HAS_EXTENSION)
#undef ASAP_HAS_EXTENSION
#endif
#if defined(__has_extension)
#define ASAP_HAS_EXTENSION(extension) __has_extension(extension)
#else
#define ASAP_HAS_EXTENSION(extension) (0)
#endif

#if defined(ASAP_GCC_HAS_EXTENSION)
#undef ASAP_GCC_HAS_EXTENSION
#endif
#if defined(__has_extension)
#define ASAP_GCC_HAS_EXTENSION(extension, major, minor, patch) __has_extension(extension)
#else
#define ASAP_GCC_HAS_EXTENSION(extension, major, minor, patch)                                     \
  ASAP_GCC_VERSION_CHECK(major, minor, patch)
#endif

// -------------------------------------------------------------------------------------------------
// Compiler declspec attribute check
// -------------------------------------------------------------------------------------------------

#if defined(ASAP_HAS_DECLSPEC_ATTRIBUTE)
#undef ASAP_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
#define ASAP_HAS_DECLSPEC_ATTRIBUTE(attribute) __has_declspec_attribute(attribute)
#else
#define ASAP_HAS_DECLSPEC_ATTRIBUTE(attribute) (0)
#endif

#if defined(ASAP_GCC_HAS_DECLSPEC_ATTRIBUTE)
#undef ASAP_GCC_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
#define ASAP_GCC_HAS_DECLSPEC_ATTRIBUTE(attribute, major, minor, patch)                            \
  __has_declspec_attribute(attribute)
#else
#define ASAP_GCC_HAS_DECLSPEC_ATTRIBUTE(attribute, major, minor, patch)                            \
  ASAP_GCC_VERSION_CHECK(major, minor, patch)
#endif

// -------------------------------------------------------------------------------------------------
// Pragma
// -------------------------------------------------------------------------------------------------

#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || defined(__clang__) ||          \
    ASAP_GCC_VERSION_CHECK(3, 0, 0)
#define ASAP_PRAGMA(value) _Pragma(#value)
#elif ASAP_MSVC_VERSION_CHECK(15, 0, 0)
#define ASAP_PRAGMA(value) __pragma(value)
#else
#define ASAP_PRAGMA(value)
#endif

// -------------------------------------------------------------------------------------------------
// Compiler diagnostics
// -------------------------------------------------------------------------------------------------

#if defined(ASAP_DIAGNOSTIC_PUSH)
#undef ASAP_DIAGNOSTIC_PUSH
#endif
#if defined(ASAP_DIAGNOSTIC_POP)
#undef ASAP_DIAGNOSTIC_POP
#endif
#if defined(__clang__)
#define ASAP_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
#define ASAP_DIAGNOSTIC_POP _Pragma("clang diagnostic pop")
#elif ASAP_GCC_VERSION_CHECK(4, 6, 0)
#define ASAP_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#define ASAP_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
#elif ASAP_MSVC_VERSION_CHECK(15, 0, 0)
#define ASAP_DIAGNOSTIC_PUSH __pragma(warning(push))
#define ASAP_DIAGNOSTIC_POP __pragma(warning(pop))
#else
#define ASAP_DIAGNOSTIC_PUSH
#define ASAP_DIAGNOSTIC_POP
#endif

#if defined(ASAP_HAS_WARNING)
#undef ASAP_HAS_WARNING
#endif
#if defined(__has_warning)
#define ASAP_HAS_WARNING(warning) __has_warning(warning)
#else
#define ASAP_HAS_WARNING(warning) (0)
#endif

#if defined(ASAP_GCC_HAS_WARNING)
#undef ASAP_GCC_HAS_WARNING
#endif
#if defined(__has_warning)
#define ASAP_GCC_HAS_WARNING(warning, major, minor, patch) __has_warning(warning)
#else
#define ASAP_GCC_HAS_WARNING(warning, major, minor, patch)                                         \
  ASAP_GCC_VERSION_CHECK(major, minor, patch)
#endif

// -------------------------------------------------------------------------------------------------
// assume, unreachable, unreachable return
// -------------------------------------------------------------------------------------------------

#if defined(ASAP_UNREACHABLE)
#undef ASAP_UNREACHABLE
#endif
#if defined(ASAP_UNREACHABLE_RETURN)
#undef ASAP_UNREACHABLE_RETURN
#endif
#if defined(ASAP_ASSUME)
#undef ASAP_ASSUME
#endif
#if ASAP_MSVC_VERSION_CHECK(13, 10, 0)
#define ASAP_ASSUME(expr) __assume(expr)
#elif ASAP_HAS_BUILTIN(__builtin_assume)
#define ASAP_ASSUME(expr) __builtin_assume(expr)
#endif
#if ASAP_HAS_BUILTIN(__builtin_unreachable) || ASAP_GCC_VERSION_CHECK(4, 5, 0)
#define ASAP_UNREACHABLE() __builtin_unreachable()
#elif defined(ASAP_ASSUME)
#define ASAP_UNREACHABLE() ASAP_ASSUME(0)
#endif
#if !defined(ASAP_ASSUME)
#if defined(ASAP_UNREACHABLE)
#define ASAP_ASSUME(expr) ASAP_STATIC_CAST(void, ((expr) ? 1 : (ASAP_UNREACHABLE(), 1)))
#else
#define ASAP_ASSUME(expr) ASAP_STATIC_CAST(void, expr)
#endif
#endif
#if defined(ASAP_UNREACHABLE)
#define ASAP_UNREACHABLE_RETURN(value) ASAP_UNREACHABLE()
#else
#define ASAP_UNREACHABLE_RETURN(value) return (value)
#endif
#if !defined(ASAP_UNREACHABLE)
#define ASAP_UNREACHABLE() ASAP_ASSUME(0)
#endif

// -------------------------------------------------------------------------------------------------
// fallthrough
// -------------------------------------------------------------------------------------------------

#if defined(ASAP_FALL_THROUGH)
#undef ASAP_FALL_THROUGH
#endif
#if ASAP_HAS_ATTRIBUTE(fallthrough) || ASAP_GCC_VERSION_CHECK(7, 0, 0)
#define ASAP_FALL_THROUGH __attribute__((__fallthrough__))
#else
#define ASAP_FALL_THROUGH
#endif
