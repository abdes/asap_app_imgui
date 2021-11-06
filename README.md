# Starter project with minimum necessary functionality

![Start Now!!](doc/_static/logo.png "ASAP Logo")

[![Linux Build (Ubuntu latest)](https://github.com/abdes/asap/actions/workflows/linux-build.yml/badge.svg?branch=develop)](https://github.com/abdes/asap/actions/workflows/linux-build.yml)
[![Windows Build (latest)](https://github.com/abdes/asap/actions/workflows/windows-build.yml/badge.svg?branch=develop)](https://github.com/abdes/asap/actions/workflows/windows-build.yml)

## [Project Documentation](https://abdes.github.io/asap/)

## Overview

- cmake as the build system
- modular structure with each module self-contained in a subdirectory within
  the project
- build helpers in common/cmake to facilitate declaration of library, exe,
  test modules, for the end-to-end lifecycle including doc generation, test,
  packaging etc...
- common facilities (common module) for platform specifics, assertions
  support, logging
- unit testing with Catch2
- Optional Modules:
  - backported C++17 filesystem implementation (will be portable across Linux, Mac
    and Windows)

Any optional submodule that is not needed can be simply removed from the git submodules
and from the master CMakeLists.txt (`add_subdirectory()`).

## Getting the code

```bash
git clone --recurse-submodules -j4 https://github.com/abdes/asap.git
```

NOTES:

- -j4 requests git to parallelize cloning of repos. Needs a relatively recent
  version of git. If that is not available, simply do not use this option.

## Requirements

Make sure you have a C++ compiler with C++-14 capabilities at least. Gnu, Clang and MSVC
all can do that with a recent version.

## Enabling husky/commitlint/standard-version

Only one time after the project is cloned, do the following:

```bash
npx husky install
npm install -g @commitlint/cli @commitlint/config-conventional
npm install -g standard-version
```

## Building

```bash
mkdir _build && cd _build && cmake .. && cmake --build .
```

You can also use any of the cmake options, generators, etc...

By default the build will create shared libraries. If you want static libraries, pass
-DBUILD_SHARED_LIBS=OFF to cmake during configuration:

```bash
cmake -DBUILD_SHARED_LIBS=OFF ..
```

You can also use any of the cmake options, generators, etc...

```cmake
# Project options
option(BUILD_SHARED_LIBS     "Build shared instead of static libraries."              ON)
option(OPTION_SELF_CONTAINED "Create a self-contained install with all dependencies." OFF)
option(OPTION_BUILD_TESTS    "Build tests."                                           ON)
option(OPTION_BUILD_DOCS     "Build documentation."                                   OFF)
option(OPTION_BUILD_EXAMPLES "Build examples."                                        OFF)
option(OPTION_CLANG_TIDY     "Analyze code with clang-tidy."                          OFF)
option(OPTION_CPPCHECK       "Analyze code with CppCheck."                            OFF)
option(OPTION_GOOGLE_ASAN    "Instrument code with address sanitizer"                 OFF)
option(OPTION_GOOGLE_USAN    "Instrument code with memory sanitizer"                  OFF)
option(OPTION_GOOGLE_TSAN    "Instrument code with thread sanitizer"                  OFF)
```

The code is portable across Linux (g++ and clang), OS X and Visual Studio 2017.
