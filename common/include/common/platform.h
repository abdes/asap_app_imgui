//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <asap/asap-features.h>

// ==== Darwin/BSD ===
#if (defined __APPLE__ && defined __MACH__) || defined __FreeBSD__ || \
    defined __NetBSD__ || defined __OpenBSD__ || defined __bsdi__ ||  \
    defined __DragonFly__ || defined __FreeBSD_kernel__
#define ASAP_BSD
#if defined __APPLE__

// execinfo.h is available in the MacOS X 10.5 SDK.
#define ASAP_USE_EXECINFO 1

#endif  // __APPLE__

// ==== LINUX ===
#elif defined __linux__
#define ASAP_LINUX

#if defined __GLIBC__ && (defined __x86_64__ || defined __i386 || \
                          defined _M_X64 || defined _M_IX86)
#define ASAP_USE_EXECINFO 1
#endif

// ==== MINGW ===
#elif defined __MINGW32__ || defined __MINGW64__
#define ASAP_MINGW
#define ASAP_WINDOWS

// ==== WINDOWS ===
#elif defined _WIN32
#define ASAP_WINDOWS

#endif  // Platform checks

// SSE is x86 / amd64 specific. On top of that, we only
// know how to access it on msvc and gcc (and gcc compatibles).
// GCC requires the user to enable SSE support in order for
// the program to have access to the intrinsics, this is
// indicated by the __SSE4_1__ macro
#ifndef ASAP_HAS_SSE

#if (defined _M_AMD64 || defined _M_IX86 || defined _M_X64 ||   \
     defined __amd64__ || defined __i386 || defined __i386__ || \
     defined __x86_64__ || defined __x86_64) &&                 \
    (defined __GNUC__ || (defined _MSC_VER && _MSC_VER >= 1600))
#define ASAP_HAS_SSE 1
#else
#define ASAP_HAS_SSE 0
#endif

#endif  // ASAP_HAS_SSE

#if (defined __arm__ || defined __aarch64__ || defined _M_ARM || \
     defined _M_ARM64)
#define ASAP_HAS_ARM 1
#else
#define ASAP_HAS_ARM 0
#endif  // ASAP_HAS_ARM

#ifndef __has_builtin
#define __has_builtin(x) 0  // for non-clang compilers
#endif

#if (ASAP_HAS_SSE && defined __GNUC__)
#define ASAP_HAS_BUILTIN_CLZ 1
#elif (ASAP_HAS_ARM && defined __GNUC__ && !defined __clang__)
#define ASAP_HAS_BUILTIN_CLZ 1
#elif (defined __clang__ && __has_builtin(__builtin_clz))
#define ASAP_HAS_BUILTIN_CLZ 1
#else
#define ASAP_HAS_BUILTIN_CLZ 0
#endif  // ASAP_HAS_BUILTIN_CLZ

#if (ASAP_HAS_SSE && defined __GNUC__)
#define ASAP_HAS_BUILTIN_CTZ 1
#elif (ASAP_HAS_ARM && defined __GNUC__ && !defined __clang__)
#define ASAP_HAS_BUILTIN_CTZ 1
#elif (defined __clang__ && __has_builtin(__builtin_ctz))
#define ASAP_HAS_BUILTIN_CTZ 1
#else
#define ASAP_HAS_BUILTIN_CTZ 0
#endif  // ASAP_HAS_BUILTIN_CTZ

#if ASAP_HAS_ARM && defined __ARM_NEON
#define ASAP_HAS_ARM_NEON 1
#else
#define ASAP_HAS_ARM_NEON 0
#endif  // ASAP_HAS_ARM_NEON

#if ASAP_HAS_ARM && defined __ARM_FEATURE_CRC32
#define ASAP_HAS_ARM_CRC32 1
#else
#if defined ASAP_FORCE_ARM_CRC32
#define ASAP_HAS_ARM_CRC32 1
#else
#define ASAP_HAS_ARM_CRC32 0
#endif
#endif  // ASAP_HAS_ARM_CRC32
