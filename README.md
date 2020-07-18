[![repo on GitHub](https://img.shields.io/badge/repo-GitHub-brightgreen.svg)](https://github.com/abdes/asap_app_imgui)
[![repo on GitLab](https://img.shields.io/badge/repo-GitLab-brightgreen.svg)](https://gitlab.com/absassi/asap_app_imgui)

| Configuration | Build Status |
| ------------- | ------------- |
| Linux G++ 7 | [![Linux G++ 7][9]][0] |
| Linux Clang 4 | [![Linux Clang 4][10]][0] |
| Linux Clang 5 | [![Linux Clang 5][11]][0] |
| XCode 8.3 - OS X 10.12 | [![XCode 8.3 - OS X 10.12][12]][0] |
| XCode 9.4 - OS X 10.13 | [![XCode 9.4 - OS X 10.13][13]][0] |
| Windows | [![Windows][21]][20] |

[0]: https://travis-ci.org/abdes/asap_app_imgui
[9]: https://travis-matrix-badges.herokuapp.com/repos/abdes/asap_app_imgui/branches/master/9
[10]: https://travis-matrix-badges.herokuapp.com/repos/abdes/asap_app_imgui/branches/master/10
[11]: https://travis-matrix-badges.herokuapp.com/repos/abdes/asap_app_imgui/branches/master/11
[12]: https://travis-matrix-badges.herokuapp.com/repos/abdes/asap_app_imgui/branches/master/12
[13]: https://travis-matrix-badges.herokuapp.com/repos/abdes/asap_app_imgui/branches/master/13
[20]: https://ci.appveyor.com/project/abdes/asap-app-imgui
[21]: https://ci.appveyor.com/api/projects/status/qoaae14rw3cyivgq/branch/master?svg=true

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
  - backported C++17 filesystem implementation (will be portable across Linux, Mac
    and Windows)
  - imgui with docking
  - persistence of imgui and docks configuration


## Getting the code
```
# Use the one you prefer :-)
# git clone --recurse-submodules -j4 https://gitlab.com/absassi/asap_app_imgui.git
# git clone --recurse-submodules -j4 https://github.com/abdes/asap_app_imgui.git

# The develop branch carries the ongoing feature development
# the master branch carries stable releases only
git checkout develop

git submodule update --recursive
```

NOTES:
  - most of the cutting edge stuff will be in the 'develop' branch. The master branch 
    carries the stable releases only. As many changes are currently being made to ImGui
    for docking and viewports, it is recommended you checkout the develop branch for 
    the latest features.
  - -j4 requests git to parallelize cloning of repos. Needs a relatively recent version
    of git. If that is not available, simply do not use this option.

## Requirements
Make sure you have a C++ compiler with C++-14 capabilities at least. Gnu, Clang and MSVC
all can do that with a recent version.

## Building
```
mkdir _build && cd _build && cmake .. && cmake --build .
```
You can also use any of the cmake options, generators, etc...

By default the build will create shared libraries. If you want static libraries, pass
-DBUILD_SHARED_LIBS=OFF to cmake during configuration:
```
cmake -DBUILD_SHARED_LIBS=OFF ..
```

You can also use any of the cmake options, generators, etc...

By default the build will create shared libraries. If you want static libraries, pass
-DBUILD_SHARED_LIBS=OFF to cmake during configuration:
```
cmake -DBUILD_SHARED_LIBS=OFF ..
```


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

## Running the demo

1. Copy the shaders from the source directory (main/src/shaders) to the build
   directory.
2. Run the app from within the build directory (./asap_app)
3. Drag and drop the frames to dock them to whichever side you want.
4. Edit the settings as you wish
4. When the app is closed, the settings, docks, loggers, etc are saved into
   the .asap directory under the build directory. Some files saved by imgui
   are still under the build directory directly (work in progress)

You need to have a graphics system capable of providing OpenGL 3.2 to properly run the
demo.
