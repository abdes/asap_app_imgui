[![Build status](https://ci.appveyor.com/api/projects/status/fbqox338lae708fq?svg=true)](https://ci.appveyor.com/project/abdes/asap)
[![pipeline status](https://gitlab.com/absassi/asap/badges/master/pipeline.svg)](https://gitlab.com/absassi/asap/commits/master)

# Starter project with minimum necessary functionality
  - use cmake for the build system
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

TODO: More Documentation

## Getting the code
```
git clone --recurse-submodules -j4 https://gitlab.com/absassi/asap.git
```

## Building
```
mkdir _build && cd _build && cmake --build ..
```
You can also use any of the cmake options, generators, etc...

```
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

