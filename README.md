<div align="center">

# Don't waste your time

![Start Now!!](doc/_static/asap-banner.png "ASAP banner")

</div>

<h4 align="center">A complete starter project for C++ projects built with
<a href="https://cmake.org/" target="_blank">CMake</a>.</h4>

<div align="center">

-+- Build Status -+-

_develop_

[![Build status - develop][build-status-develop-badge]][build-matrix]

_master_

[![Build status - master][build-status-master-badge]][build-matrix]

-+-

[![Latest release][release-badge]][latest-release]
[![Commits][last-commit-badge]][commits]
[![Linux][linux-badge]][latest-release]
[![Windows][windows-badge]][latest-release]
[![Mac OS][macos-badge]][latest-release]
[![License][license-badge]][license]
[![CII Best Practices][openssf-badge]][openssf-project]

</div>

<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#project-documentation">Project Documentation</a> •
  <a href="#getting-started">Getting Started</a> •
  <a href="#Contributing">Contributing</a> •
  <a href="#credits">Credits</a> •
</p>

## Key Features

Have you ever found yourself spending countless hours trying to figure out how
to start a professional C++ project with unit tests, a robust dev workflow,
change logs, code coverage, linting, formatting...? The C++ ecosystem is getting
better but is still very far away from other ecosystems such as JavaScript, or
even Rust. It's certainly easy to use CMake, a compiler and vscode to start a
hello world project, but as soon as you start having multiple modules, with
third party dependencies, testing, coverage, and other development things that a
professional quality project needs, you start wasting hours and hours
troubleshooting and configuring until you make it work.

Waste your time no more!

This starter project solves most of your problems, using the best practices for
C++ development and modern CMake and allows you to get started in minutes rather
than days.

- `CMake` as the build system with or without presets
- cross-platform portability on Linux, OS X and Windows
- multiple compilers: clang, g++ and MSVC
- modular structure with each module self-contained in a subdirectory within the
  project
- `CMake` build helpers to facilitate declaration of library, exe, test modules,
  for the end-to-end lifecycle including doc generation, test, packaging etc...
- unit testing targets, with by default Google Test as the framework (other
  frameworks are also possible)
- code coverage with clang or g++
- zero-touch valgrind, clang-tidy, clang-format, google sanitizers, etc
- development can be done locally or in a dev container with vscode
- faster rebuilds with [`ccache`](https://ccache.dev) empowered caching
- `CMake` package management with [`CPM`](https://github.com/cpm-cmake/CPM.cmake)

## Project Documentation

We have detailed guides for setting up an efficient development environment, the
development process, project structure, etc. Take a look at the project's github
pages [here](https://abdes.github.io/asap/asap_master/html/).

[![Project Documentation demo][project-docs-thumb]][project-docs-video]

## Getting Started

It is strongly recommended that you take some time to browse the project
documentation to familiarize yourself with its structure and development
workflows.

For the impatient, or the already experts, below is a short tutorial video for
how to start from asap to make your own project.

### 1. Create a new repo from the `asap` template

[![New repo from template demo][from-template-thumb]][from-template-video]

### 2. Make it your own - stage 1

In this step, we'll clean up the freshly created repo and prepare it for being
customized.

[![Clean-up][cleanup-thumb]][cleanup-video]

### 2. Make it your own - stage 2

In this step, we'll go into Visual Studio Code to do some heavier customizations
while tracking our changes in git.

[![Customize][customize-thumb]][customize-video]

### 3. Start coding

For this final tutorial, we will ad an executable module to implement the famous
"Hello World!". You can see how simple it is and how fast it is to hit the
ground running with asap and start focusing on what matters the most: coding.

[![Hello World][hello-world-thumb]][hello-world-video]

Take some time to explore the different build targets in the project, and the
documentation to add tests, documentation etc.

## Contributing

If you would like to contribute code you can do so through GitHub by forking the
repository and sending a pull request. When submitting code, please make every
effort to follow existing conventions and style in order to keep the code as
readable as possible.

By contributing your code, you agree to license your contribution under the
terms of the BSD-3-Clause or a more permissive license. All files are released
with the BSD-3-Clause license.

Read the [developer guides](https://abdes.github.io/asap/asap_master/html/).

### Submitting a PR

- For every PR there should be an accompanying issue which the PR solves
- The PR itself should only contain code which is the solution for the given
  issue
- If you are a first time contributor check if there is a suitable issue for you

## Credits

- Swift Navigation for their [cmake common
  tools](https://github.com/swift-nav/cmake) project, an excellent starting
  point for several of the features in this starter project,
- The multitude of other open-source projects used to implement this project or
  to get inspiration - credits in the source code or the documentation as
  appropriate

[build-matrix]: https://github.com/abdes/asap/actions/workflows/cmake-build.yml?branch=develop
[build-status-develop-badge]: https://github.com/abdes/asap/actions/workflows/cmake-build.yml/badge.svg?branch=develop
[build-status-master-badge]: https://github.com/abdes/asap/actions/workflows/cmake-build.yml/badge.svg?branch=master
[cleanup-thumb]: https://asciinema.org/a/JOXq0l9CLZMolNcGhOnc84tNO.svg
[cleanup-video]: https://asciinema.org/a/JOXq0l9CLZMolNcGhOnc84tNO?autoplay=1
[commits]: https://github.com/abdes/asap/commits
[customize-thumb]: https://cdn.loom.com/sessions/thumbnails/bedff4e1532441a6af6497653e52cede-with-play.gif
[customize-video]: https://www.loom.com/embed/bedff4e1532441a6af6497653e52cede
[from-template-thumb]: https://cdn.loom.com/sessions/thumbnails/087f217b73454728900baa8b1487f358-with-play.gif
[from-template-video]: https://www.loom.com/embed/087f217b73454728900baa8b1487f358
[hello-world-thumb]: https://cdn.loom.com/sessions/thumbnails/ff20f1771a6d448c8fd7b6e53e117c93-with-play.gif
[hello-world-video]: https://www.loom.com/embed/ff20f1771a6d448c8fd7b6e53e117c93
[last-commit-badge]: https://img.shields.io/github/last-commit/abdes/asap
[latest-release]: https://github.com/abdes/asap/releases/latest
[license-badge]: https://img.shields.io/github/license/abdes/asap
[license]: https://opensource.org/licenses/BSD-3-Clause
[linux-badge]: https://img.shields.io/badge/OS-linux-blue
[macos-badge]: https://img.shields.io/badge/OS-macOS-blue
[openssf-badge]: https://bestpractices.coreinfrastructure.org/projects/5917/badge
[openssf-project]: https://bestpractices.coreinfrastructure.org/projects/5917
[project-docs-thumb]: https://cdn.loom.com/sessions/thumbnails/131bd53f1a004387b09bbeeb80a41f3e-with-play.gif
[project-docs-video]: https://www.loom.com/embed/131bd53f1a004387b09bbeeb80a41f3e
[release-badge]: https://img.shields.io/github/v/release/abdes/asap
[windows-badge]: https://img.shields.io/badge/OS-windows-blue
