<div align="center">

# Don't waste your time!

![Start Now!!](doc/_static/asap-banner.png "ASAP banner")

</div>

<h4 align="center">A complete starter project for C++ projects built with
<a href="https://cmake.org/" target="_blank">CMake</a>.</h4>

<div align="center">

[![Build status][build-status-badge]][build-matrix]
[![Latest release][release-badge]][latest-release]
[![Commits][last-commit-badge]][commits]
[![Linux][linux-badge]][latest-release]
[![Windows][windows-badge]][latest-release]
[![Mac OS][macos-badge]][latest-release]
[![License][license-badge]][license]

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

## Project Documentation

We have detailed guides for setting up an efficient development environment, the
development process, project structure, etc. Take a look at the project's github
pages [here](https://abdes.github.io/asap/asap_master/html/).

[![Project Documentation Demo][project-docs-thumb]][project-docs-video]

## Getting Started

It is strongly recommended that you take some time to browse the project
documentation to familiarize yourself with its structure and development
workflows.

For the impatient, or the already experts, below is a short tutorial video for
how to start from asap to make your own project.

**--> Video is still TODO <--**

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

[license-badge]: https://img.shields.io/github/license/abdes/asap
[build-status-badge]: https://github.com/abdes/asap/actions/workflows/cmake-build.yml/badge.svg?branch=develop
[release-badge]: https://img.shields.io/github/v/release/abdes/asap
[linux-badge]: https://img.shields.io/badge/OS-linux-blue
[windows-badge]: https://img.shields.io/badge/OS-windows-blue
[macos-badge]: https://img.shields.io/badge/OS-macOS-blue
[last-commit-badge]: https://img.shields.io/github/last-commit/abdes/asap
[build-matrix]: https://github.com/abdes/asap/actions/workflows/cmake-build.yml
[license]: https://opensource.org/licenses/BSD-3-Clause
[latest-release]: https://github.com/abdes/asap/releases/latest
[commits]: https://github.com/abdes/asap/commits
[project-docs]: https://abdes.github.io/asap/asap_master/html/index.html
[project-docs-video]: https://www.loom.com/embed/3a1f6c2b49544ba7877a8266ef790a8d
[project-docs-thumb]: https://cdn.loom.com/sessions/thumbnails/3a1f6c2b49544ba7877a8266ef790a8d-with-play.gif
