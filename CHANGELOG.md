# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

## [3.0.0](http://github.com/abdes/asap/compare/v2.0.1...v3.0.0) (2021-12-08)


### ⚠ BREAKING CHANGES

* The project requires C++17 as it is widely available in compilers now. Logical traits such as
  conjunction, disjunction and negation are available from the <type_traits> standard include.
* major redesign of the cmake build system, many macros and functions have been changed and the
  build system overall has been simplified.
* `catch2` has been replaced by Google Test/Mock, which provides more features, less compiler
  warnings and is more popular. Catch2 or any other framework can still be easily added to an `asap`
  based project.
* `hedely` was removed from `common` and was replaced by a much lighter new file `compilers.h`.
* `nowide` was removed from `common` and will be replaced by the standalone boost nowide library
  when needed.
* `filesystem` footprint is too large to be included by default in `asap` starter project. It will
  be provided separately and a mechanism to easily add it into an `asap` based project will be
  implemented in a future update.

### Features

* add gsl library
  ([9b982f1](http://github.com/abdes/asap/commit/9b982f17aeb9362619cf871ed9bbedb65344ab43))
* add support for CMake presets
  ([bdcfa4d](http://github.com/abdes/asap/commit/bdcfa4d6a0cfffe0026af39b635c6dcd4e61e3a8))
* add contract checking api (assertions)
  ([c691446](http://github.com/abdes/asap/commit/c691446fd18242840e62529f1aabdaf8480b7ec6))
* convert git submodule to local module
  ([4e32e8a](http://github.com/abdes/asap/commit/4e32e8a6d1152413af12c7013ee759eca5e2d51d))
* redesign cmake build system
  ([31ce14a](http://github.com/abdes/asap/commit/31ce14a301ac2e725b7760581e696f089225b161))
* remove filesystem submodule
  ([e2089ae](http://github.com/abdes/asap/commit/e2089ae7707c1448c78db23db2f53cbfab461599))
* remove logical traits backport
  ([5ed0fe6](http://github.com/abdes/asap/commit/5ed0fe6e9e03399e640221a285f87a1a8a015cd1))

* significantly enhance the documentation, both doxygen based for APIs and sphinx based for the
  project docs

### Bug Fixes

* configure compilation database for clangd when using cmake presets
  ([7493c24](http://github.com/abdes/asap/commit/7493c240ce97396040d9a7bd09cb32ed56ea2340))
* hardcode `asap` in places where we should not use the custom project name
  ([7f81298](http://github.com/abdes/asap/commit/7f81298b31e5a235bf4bce4778525a2e4594befa))
* match Clang and Apple Clang for compiler options
  ([7e1e123](http://github.com/abdes/asap/commit/7e1e12317b62099bb58ce5dd4adfdbf1a759ad03))
* convert unit testing to gtest/gmock
  ([b99c8d0](http://github.com/abdes/asap/commit/b99c8d0a315bb8200ca9cec93c9d5f64880c9271))

## [2.0.1](http://github.com/abdes/asap/compare/v2.0.0...v2.0.1) (2021-11-06)


### Bug Fixes

* "-Wreserved-identifier" only if not APPLE ([05fac12](http://github.com/abdes/asap/commit/05fac12d37978c6651c299e6ad1cc64dfed88fa0))
* cmake require c++ 14 in all modules ([f05c9a6](http://github.com/abdes/asap/commit/f05c9a63e97dbcbef3e7d8188a04e719e2247d62))
* use lower-case name for sphinx ([f3005b5](http://github.com/abdes/asap/commit/f3005b5bd4055b16a57cdcb3af70a2f32b1c5809))

## [2.0.0](http://github.com/abdes/asap/compare/v1.0.0...v2.0.0) (2021-11-06)

### Features

* Cleanup the code to remove most compiler and linter warnings.
* Refactor cmake build files to have robust support for sanitizers and linters.
* Rationalize the compiler options to strictly stick to those options that won't
 conflict with application decisions.
* Enhanced the documentation

### To be continued

* Refactoring of cmake build files.
* Remove additional warnings from code.
* Documenattion.

### ⚠ BREAKING CHANGES

* logging is in a new module: asap::logging.

Many modules would want to only get the basic common functionality
without pulling extra 3rd party dependencies such as spdlog, fmt etc...
For this reason, the logging functionality is taken out of the common
submodule and moved to logging submodule.

* move logging functionality to separate module ([46f4dd9](http://github.com/abdes/asap/commit/46f4dd96edb1148e0772a1539b989fd80f3821e4))

## 1.0.0 (2021-11-03)


### Features

* add support for Conventional Commits and auto Changelog ([7b827fa](http://github.com/abdes/asap/commit/7b827fab2ae28ba903c69ab5acdf249cb3e55d85))
* replace cmake compiler detection with hedley ([8bf0a3d](http://github.com/abdes/asap/commit/8bf0a3d53fcdba65497491cbd63c1dcffcba8467))
