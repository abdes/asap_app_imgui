/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

/*!
 * \file compilers.h
 *
 * \brief Compiler detection, diagnostics and feature helpers.
 */

#pragma once

// -------------------------------------------------------------------------------------------------
// Compiler detection
// -------------------------------------------------------------------------------------------------

#if !defined(DOXYGEN_DOCUMENTATION_BUILD)

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

#else // DOXYGEN_DOCUMENTATION_BUILD
/*!
 * \brief Clang compiler detection macro.
 *
 * This macro is only defined if the compiler in use is a Clang compiler (including Apple Clang).
 *
 * Example
 * ```
 * #if defined(ASAP_CLANG_VERSION)
 * // Do things for clang
 * #endif
 * ```
 *
 * \see ASAP_CLANG_VERSION_CHECK
 */
#define ASAP_CLANG_VERSION 0
#endif // DOXYGEN_DOCUMENTATION_BUILD

#if defined(ASAP_CLANG_VERSION_CHECK)
#undef ASAP_CLANG_VERSION_CHECK
#endif
#if defined(ASAP_CLANG_VERSION)
#define ASAP_CLANG_VERSION_CHECK(major, minor, patch)                                              \
  (ASAP_CLANG_VERSION >= ASAP_VERSION_ENCODE_INTERNAL_(major, minor, patch))
#else
#define ASAP_CLANG_VERSION_CHECK(major, minor, patch) (0)
#endif
/*!
 * \def ASAP_CLANG_VERSION_CHECK
 *
 * \brief Clang compiler version check macro.
 *
 * This macro is always defined, and provides a convenient way to check for features based on the
 * version number.
 *
 * \note In most cases for clang, you shouldn't test its version number, you should use the feature
 * checking macros (https://clang.llvm.org/docs/LanguageExtensions.html#feature-checking-macros).
 *
 * Example
 * ```
 * #if ASAP_CLANG_VERSION_CHECK(14,0,0)
 * // Do a thing that only clang-14+ supports
 * #endif
 * ```
 *
 * \return true (1) if the current compiler corresponds to the macro name, and the compiler version
 * is greater than or equal to the provided values.
 *
 * \see ASAP_CLANG_VERSION
 */

#if !defined(DOXYGEN_DOCUMENTATION_BUILD)

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

#else // DOXYGEN_DOCUMENTATION_BUILD
/*!
 * \brief MSVC compiler detection macro.
 *
 * This macro is only defined if the compiler in use is Microsoft Visual Studio C++ compiler.
 *
 * Example
 * ```
 * #if defined(ASAP_MSVC_VERSION)
 * // Do things for MSVC
 * #endif
 * ```
 *
 * \see ASAP_MSVC_VERSION_CHECK
 */
#define ASAP_MSVC_VERSION 0
#endif // DOXYGEN_DOCUMENTATION_BUILD

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
/*!
 * \def ASAP_MSVC_VERSION_CHECK
 * \brief MSVC compiler version check macro.
 *
 * This macro is always defined, and provides a convenient way to check for features based on the
 * version number.
 *
 * Example
 * ```
 * #if ASAP_MSVC_VERSION_CHECK(16,0,0)
 * // Do a thing that only MSVC 16+ supports
 * #endif
 * ```
 *
 * \return true (1) if the current compiler corresponds to the macro name, and the compiler version
 * is greater than or equal to the provided values.
 *
 * \see ASAP_MSVC_VERSION
 */

#if !defined(DOXYGEN_DOCUMENTATION_BUILD)

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

#else // DOXYGEN_DOCUMENTATION_BUILD
/*!
 * \brief GNU like compiler detection macro.
 *
 * This macro is only defined if the compiler in use defines `__GNUC__`, which may indicate that it
 * is the real GCC compiler or a compiler masquerading GCC.
 *
 * Example
 * ```
 * #if defined(ASAP_GNUC_VERSION)
 * // Do things that work for GNU like compilers
 * #endif
 * ```
 *
 * \see ASAP_GNUC_VERSION_CHECK
 */
#define ASAP_GNUC_VERSION 0
#endif // DOXYGEN_DOCUMENTATION_BUILD

#if defined(ASAP_GNUC_VERSION_CHECK)
#undef ASAP_GNUC_VERSION_CHECK
#endif
#if defined(ASAP_GNUC_VERSION)
#define ASAP_GNUC_VERSION_CHECK(major, minor, patch)                                               \
  (ASAP_GNUC_VERSION >= ASAP_VERSION_ENCODE_INTERNAL_(major, minor, patch))
#else
#define ASAP_GNUC_VERSION_CHECK(major, minor, patch) (0)
#endif
/*!
 * \def ASAP_GNUC_VERSION_CHECK
 * \brief GNU like compiler version check macro.
 *
 * This macro is always defined, and provides a convenient way to check for features based on the
 * version number.
 *
 * Example
 * ```
 * #if ASAP_GNUC_VERSION_CHECK(9,0,0)
 * // Do a thing that only GNU-like compiler 9+ supports
 * #endif
 * ```
 *
 * \return true (1) if the current compiler corresponds to the macro name, and the compiler version
 * is greater than or equal to the provided values.
 *
 * \see ASAP_GNUC_VERSION
 */

#if !defined(DOXYGEN_DOCUMENTATION_BUILD)

#if defined(ASAP_GCC_VERSION)
#undef ASAP_GCC_VERSION
#endif
#if defined(ASAP_GNUC_VERSION) && !defined(__clang__)
#define ASAP_GCC_VERSION ASAP_GNUC_VERSION
#endif

#else // DOXYGEN_DOCUMENTATION_BUILD
/*!
 * \brief GCC compiler detection macro.
 *
 * This macro is only defined if the compiler in use is GNU C/C++ compiler. Any other compilers that
 * masquerade as `__GNUC__` but define another known compiler symbol are excluded.
 *
 * Example
 * ```
 * #if defined(ASAP_GCC_VERSION)
 * // Do things for GCC/G++
 * #endif
 * ```
 *
 * \see ASAP_GCC_VERSION_CHECK
 */
#define ASAP_GCC_VERSION 0
#endif // DOXYGEN_DOCUMENTATION_BUILD

#if defined(ASAP_GCC_VERSION_CHECK)
#undef ASAP_GCC_VERSION_CHECK
#endif
#if defined(ASAP_GCC_VERSION)
#define ASAP_GCC_VERSION_CHECK(major, minor, patch)                                                \
  (ASAP_GCC_VERSION >= ASAP_VERSION_ENCODE_INTERNAL_(major, minor, patch))
#else
#define ASAP_GCC_VERSION_CHECK(major, minor, patch) (0)
#endif
/*!
 * \def ASAP_GCC_VERSION_CHECK
 * \brief GCC/G++ compiler version check macro.
 *
 * This macro is always defined, and provides a convenient way to check for features based on the
 * version number.
 *
 * Example
 * ```
 * #if ASAP_GCC_VERSION_CHECK(11,0,0)
 * // Do a thing that only GCC 11+ supports
 * #endif
 * ```
 *
 * \return true (1) if the current compiler corresponds to the macro name, and the compiler version
 * is greater than or equal to the provided values.
 *
 * \see ASAP_GCC_VERSION
 */

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
/*!
 * \def ASAP_HAS_ATTRIBUTE
 * \brief Checks for the presence of an attribute named by `attribute`.
 *
 * Example
 * ```
 * #if ASAP_HAS_ATTRIBUTE(deprecated) // Check for an attribute
 * #  define DEPRECATED(msg) [[deprecated(msg)]]
 * #else
 * #  define DEPRECATED(msg)
 * #endif
 *
 * DEPRECATED("foo() has been deprecated") void foo();
 * ```
 *
 * \return non-zero value if the attribute is supported by the compiler.
 */

#if defined(ASAP_HAS_CPP_ATTRIBUTE)
#undef ASAP_HAS_CPP_ATTRIBUTE
#endif
#if defined(__has_cpp_attribute) && defined(__cplusplus)
#define ASAP_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
#define ASAP_HAS_CPP_ATTRIBUTE(attribute) (0)
#endif
/*!
 * \def ASAP_HAS_CPP_ATTRIBUTE
 * \brief Checks for the presence of a C++ compiler attribute named by `attribute`.
 *
 * Example
 * ```
 * #if ASAP_HAS_CPP_ATTRIBUTE(nodiscard)
 * [[nodiscard]]
 * #endif
 * int foo(int i) { return i * i; }
 * ```
 *
 * \return non-zero value if the attribute is supported by the compiler.
 */

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
/*!
 * \def ASAP_HAS_BUILTIN
 * \brief Checks for the presence of a compiler builtin function named by `builtin`.
 *
 * Example
 * ```
 * #if ASAP_HAS_BUILTIN(__builtin_trap)
 *   __builtin_trap();
 * #else
 *   abort();
 * #endif
 * ```
 *
 * \return non-zero value if the builtin function is supported by the compiler.
 */

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
/*!
 * \def ASAP_HAS_FEATURE
 * \brief Checks for the presence of a compiler feature named by `feature`.
 *
 * Example
 * ```
 * #if ASAP_HAS_FEATURE(attribute_overloadable) || ASAP_HAS_FEATURE(blocks)
 * ...
 * #endif
 * ```
 *
 * \return non-zero value if the feature is supported by the compiler.
 */

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
/*!
 * \def ASAP_PRAGMA
 *
 * \brief Produce a `pragma` directive for the compiler.
 *
 * Pragma directives specify machine-specific or operating system-specific compiler features. There
 * are different ways compilers support the specification of pragmas and this macro will simply
 * abstract these ways in a single generic way.
 */

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
/*!
 * \def ASAP_DIAGNOSTIC_PUSH
 *
 * \brief Remember the current state of the compiler's diagnostics.
 *
 * Example
 * ```
 * ASAP_DIAGNOSTIC_PUSH
 * #if defined(__clang__) && ASAP_HAS_WARNING("-Wunused-const-variable")
 * #pragma clang diagnostic ignored "-Wunused-const-variable"
 * #endif
 * const char *const FOO = "foo";
 * ASAP_DIAGNOSTIC_POP
 * ```
 *
 * \see ASAP_DIAGNOSTIC_POP
 */

/*!
 * \def ASAP_DIAGNOSTIC_POP
 *
 * \brief Return the state of the compiler's diagnostics to the value from the last push.
 *
 * Example
 * ```
 * ASAP_DIAGNOSTIC_PUSH
 * #if defined(__clang__) && ASAP_HAS_WARNING("-Wunused-const-variable")
 * #pragma clang diagnostic ignored "-Wunused-const-variable"
 * #endif
 * const char *const FOO = "foo";
 * ASAP_DIAGNOSTIC_POP
 * ```
 *
 * \see ASAP_DIAGNOSTIC_PUSH
 */

#if defined(ASAP_HAS_WARNING)
#undef ASAP_HAS_WARNING
#endif
#if defined(__has_warning)
#define ASAP_HAS_WARNING(warning) __has_warning(warning)
#else
#define ASAP_HAS_WARNING(warning) (0)
#endif
/*!
 * \def ASAP_HAS_WARNING
 * \brief Checks for the presence of a compiler warning named by `warning`.
 *
 * Example
 * ```
 * ASAP_DIAGNOSTIC_PUSH
 * #if defined(__clang__) && ASAP_HAS_WARNING("-Wunused-const-variable")
 * #pragma clang diagnostic ignored "-Wunused-const-variable"
 * #endif
 * const char *const FOO = "foo";
 * ASAP_DIAGNOSTIC_POP
 * ```
 *
 * \return non-zero value if the feature is supported by the compiler.
 */

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

/*!
 * \def ASAP_UNREACHABLE
 *
 * \brief Inform the compiler/analyzer that the code should never be reached (even with invalid
 * input).
 *
 * Example
 * ```
 * switch (foo) {
 * 	case BAR:
 * 		do_something();
 * 		break;
 * 	case BAZ:
 * 		do_something_else();
 * 		break;
 * 	default:
 * 		ASAP_UNREACHABLE();
 * 		break;
 * }
 * ```
 * \see ASAP_UNREACHABLE_RETURN
 */

/*!
 * \def ASAP_UNREACHABLE_RETURN
 *
 * \brief Inform the compiler/analyzer that the code should never be reached or, for compilers which
 * don't provide a way to provide such information, return a value.
 *
 * Example
 * ```
 * static int handle_code(enum Foo code) {
 *   switch (code) {
 *   case FOO_BAR:
 *   case FOO_BAZ:
 *   case FOO_QUX:
 *     return 0;
 *   }
 *
 * ASAP_UNREACHABLE_RETURN(0);
 * }
 * ```
 * \see ASAP_UNREACHABLE
 */

/*!
 * \def ASAP_ASSUME
 *
 * \brief Inform the compiler/analyzer that the provided expression should always evaluate to a
 * non-false value.
 *
 * Note that the compiler is free to assume that the expression never evaluates to true and
 * therefore can elide code paths where it does evaluate to true.
 *
 * Example
 * ```
 * unsigned sum_small(unsigned data[], size_t count) {
 *   __builtin_assume(count <= 4);
 *   unsigned sum = 0;
 *   for (size_t i = 0; i < count; ++i) {
 *     sum += data[i];
 *   }
 *   return sum;
 * }
 * ```
 */

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

/*!
 * \def ASAP_FALL_THROUGH
 *
 * \brief Explicitly tell the compiler to fall through a case in the switch statement. Without this,
 * some compilers may think you accidentally omitted a "break;" and emit a diagnostic.
 *
 * Example
 * ```
 * switch (foo) {
 * 	case FOO:
 * 		handle_foo();
 * 		ASAP_FALL_THROUGH;
 * 	case BAR:
 * 		handle_bar();
 * 		break;
 * }
 * ```
 */
