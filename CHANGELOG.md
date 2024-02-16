# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

## 2.0.0 (2022-09-21)

This is a major release resulting from the move to the
[`asap`](https://github.com/abdes/asap) project template for a much better and
richer CMake based project.

### ⚠ BREAKING CHANGES

- The project requires C++17 as it is widely available in compilers now. Logical
  traits such as conjunction, disjunction and negation are available from the
  <type_traits> standard include.
- major redesign of the cmake build system, many macros and functions have been
  changed and the build system overall has been simplified.

* several 3rd party dependencies that were integrated as submodules are now
  either removed or pulled on-demand using `CPM.
* `catch2` has been replaced by Google Test/Mock, which provide more features,
  less compiler earnings and are more popular. Catch2 or any other framework can
  still be easily added and used.

### Features

- compiler caching with `ccache`
- the C++ GSL library ready to use
- a robust and simple way to add modules to the project
- formatting, linting, code coverage, valgrind, sanitizers,... all within reach
  via a simple option or a few lines of cmake script
- better documentation and an integrated way to generate doxygen and sphinx docs
- CI with GitHub actions
