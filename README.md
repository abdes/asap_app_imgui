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

- `CMake` as the build system with or without presets
- cross-platform portability on Linux, OS X and Windows
- multiple compilers: clang, g++ and MSVC
- modular structure with each module self-contained in a subdirectory within the project
- `CMake` build helpers to facilitate declaration of library, exe, test modules, for the
  end-to-end lifecycle including doc generation, test, packaging etc...
- common facilities (common module) for platform specifics, assertions support, logging
- unit testing with Google Test
- code coverage with clang or g++
- zero-touch valgrind, clang-tidy, clang-format, google sanitizers, etc.
- development can be done locally or in a dev container with vscode.

## Project Documentation

We have detailed guides for setting up an efficient development environment, the
development process, project structure, etc. Take a look at the project's github
pages [here](https://abdes.github.io/asap/asap_master/html/).

<div style="position: relative; padding-bottom: 69.39611054247696%; height: 0;">
  <iframe
    src="https://www.loom.com/embed/3a1f6c2b49544ba7877a8266ef790a8d"
    frameborder="0"
    webkitallowfullscreen mozallowfullscreen allowfullscreen
    style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;">
  </iframe>
</div>

## Getting Started

It is strongly recommended that you take some time to browse the project
documentation to familiarize yourself with its structure and development
workflows.

For the impatient, or the already experts, below is a short tutorial video for
how to start from asap to make your own project.

**--> Video is still TODO <--**

## Contributing

Read the [developer guides](https://abdes.github.io/asap/master/html/).
If you're in a hurry, at least do the following:

Only one time after the project is cloned, do the following:

```bash
npx husky install
npm install -g @commitlint/cli @commitlint/config-conventional
npm install -g standard-version
```

## Credits

TODO

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
[project-docs-video]: https://www.loom.com/share/3a1f6c2b49544ba7877a8266ef790a8d
