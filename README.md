[![repo on GitHub](https://img.shields.io/badge/repo-GitHub-brightgreen.svg)](https://github.com/abdes/asap_app_imgui)
[![repo on GitLab](https://img.shields.io/badge/repo-GitLab-brightgreen.svg)](https://gitlab.com/absassi/asap_app_imgui)

[![Linux Build (Ubuntu latest)](https://github.com/abdes/asap_app_imgui/actions/workflows/linux-build.yml/badge.svg?branch=develop)](https://github.com/abdes/asap_app_imgui/actions/workflows/linux-build.yml)
[![Windows Build (latest)](https://github.com/abdes/asap_app_imgui/actions/workflows/windows-build.yml/badge.svg?branch=develop)](https://github.com/abdes/asap_app_imgui/actions/workflows/windows-build.yml)

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

We have detailed guides for setting up an efficient development environment, the
development process, project structure, etc. Take a look at the project's github
pages [here](https://abdes.github.io/asap/asap_master/html/).

```shell
# Use the one you prefer :-)
# git clone --recurse-submodules -j4 https://gitlab.com/absassi/asap_app_imgui.git
# git clone --recurse-submodules -j4 https://github.com/abdes/asap_app_imgui.git

# The develop branch carries the ongoing feature development
# the master branch carries stable releases only
git checkout develop

git submodule update --recursive
```

## Getting Started

- most of the cutting edge stuff will be in the 'develop' branch. The master branch
  carries the stable releases only. As many changes are currently being made to ImGui
  for docking and viewports, it is recommended you checkout the develop branch for
  the latest features.
- -j4 requests git to parallelize cloning of repos. Needs a relatively recent version
  of git. If that is not available, simply do not use this option.

For the impatient, or the already experts, below is a short tutorial video for
how to start from asap to make your own project.

Make sure you have a C++ compiler with C++-14 capabilities at least. Gnu, Clang and MSVC
all can do that with a recent version.

[![Clean-up][cleanup-thumb]][cleanup-video]

```shell
mkdir _build && cd _build && cmake .. && cmake --build .
```

In this step, we'll go into Visual Studio Code to do some heavier customizations
while tracking our changes in git.

[![Customize][customize-thumb]][customize-video]

```shell
cmake -DBUILD_SHARED_LIBS=OFF ..
```

For this final tutorial, we will ad an executable module to implement the famous
"Hello World!". You can see how simple it is and how fast it is to hit the
ground running with asap and start focusing on what matters the most: coding.

By default the build will create shared libraries. If you want static libraries, pass
-DBUILD_SHARED_LIBS=OFF to cmake during configuration:

```shell
cmake -DBUILD_SHARED_LIBS=OFF ..
```

```cmake
# Project options
option(BUILD_SHARED_LIBS        "Build shared instead of static libraries."              ON)
option(ASAP_BUILD_TESTS         "Build tests."                                           OFF)
option(ASAP_BUILD_EXAMPLES      "Build examples."                                        OFF)
option(ASAP_WITH_GOOGLE_ASAN    "Instrument code with address sanitizer"                 OFF)
option(ASAP_WITH_GOOGLE_UBSAN   "Instrument code with undefined behavior sanitizer"      OFF)
option(ASAP_WITH_GOOGLE_TSAN    "Instrument code with thread sanitizer"                  OFF)
option(ASAP_WITH_VALGRIND       "Builds targets with valgrind profilers added"           OFF)
```

The code is portable across Linux (g++ and clang), OS X and Visual Studio 2017.

## Running the demo

1. Copy the shaders from the source directory (main/src/shaders) to the build
   directory.
2. Run the app from within the build directory (./asap_app)
3. Drag and drop the frames to dock them to whichever side you want.
4. Edit the settings as you wish
5. When the app is closed, the settings, docks, loggers, etc are saved into
   the .asap directory under the build directory. Some files saved by imgui
   are still under the build directory directly (work in progress)

You need to have a graphics system capable of providing OpenGL 3.2 to properly run the
demo.
