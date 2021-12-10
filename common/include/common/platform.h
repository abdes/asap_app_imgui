/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

/*!
 * \file platform.h
 *
 * \brief Contains compiler definitions and macros for platform detection.
 */

#pragma once

// -------------------------------------------------------------------------------------------------
//   Operating System detection
// -------------------------------------------------------------------------------------------------

// WINDOWS
#if defined(_WIN32) // defined for 32-bit and 64-bit environments
#define ASAP_WINDOWS
#if defined(__CYGWIN__) // non-POSIX CygWin
#define ASAP_WINDOWS_CYGWIN
#endif
#if defined(__MINGW32__) || defined(__MINGW64__)
#define ASAP_WINDOWS_MINGW
#endif
// Not a Windows
// UNIX-style OS
// All UNIX-style OSes define some form of the unix symbol, except for Apple.
// GCC with CygWin also defines unix symbols even when building WIN32 apps and
// this is why UNIX detection is within the #elif of _WIN32
#elif (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#define ASAP_UNIX // UNIX-style OS.
// Apple OSX, iOS, Darwin
#if defined(__APPLE__) && defined(__MACH__)
#define ASAP_APPLE // Apple OSX and iOS (Darwin)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#define ASAP_APPLE_IOS_SIMULATOR // iOS in Xcode simulator
#elif TARGET_OS_IPHONE == 1
#define ASAP_APPLE_IOS // iOS on iPhone, iPad, etc.
#elif TARGET_OS_MAC == 1
#define ASAP_APPLE_OSX // OSX
#endif
#endif
// CygWin (not WIN32)
#if defined(__CYGWIN__)
#define ASAP_CYGWIN
#endif
// Any Linux based OS, including Gnu/Linux and Android
#if defined(__linux__)
#define ASAP_LINUX
#if defined(__gnu_linux__) // Specificaly Gnu/Linux
#define ASAP_GNU_LINUX
#endif
#if defined(__ANDROID__) // Android (which also defines __linux__)
#define ASAP_ANDROID
#endif
#endif
// Solaris and SunOS
#if defined(sun) || defined(__sun)
#define ASAP_SUN
#if defined(__SVR4) || defined(__svr4__)
#define ASAP_SUN_SOLARIS // Solaris
#else
#define ASAP_SUN_SUNOS // SunOS
#endif
#endif
// HP-UX
#if defined(__hpux)
#define ASAP_HPUX
#endif
// IBM AIX
#if defined(_AIX)
#define ASAP_AIX
#endif
// BSD (DragonFly BSD, FreeBSD, OpenBSD, NetBSD)
#include <sys/param.h>
#if defined(BSD)
#define ASAP_BSD
#endif
#endif
