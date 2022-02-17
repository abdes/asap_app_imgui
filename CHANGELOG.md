# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

### [4.0.2](http://github.com/abdes/asap/compare/v4.0.1...v4.0.2) (2022-02-17)

This is a patch update to fix minor issues in the documentation files and to 
refactor the docs for easier maintenance and use in forked projects.

### Bug Fixes

* contract api docs now in contract module ([d3366bd](http://github.com/abdes/asap/commit/d3366bd846d66486b2fcf98f36fd741452be8516))
* keep `src` even when empty for doxygen not to complain ([be28f42](http://github.com/abdes/asap/commit/be28f429eddde8f258a08e84722e4fcaf975b384))

## [4.0.1](http://github.com/abdes/asap/compare/v4.0.1...v4.0.0) (2022-02-17)

* update the GitHub action for windows build to 2022

## [4.0.0](http://github.com/abdes/asap/compare/v3.1.2...v4.0.0) (2022-02-17)

### Features

* overload pattern for variant visitation on the fly ([53ea7cc](http://github.com/abdes/asap/commit/53ea7cc74e89913cbf59eda2e30bd36c356e6acd))
* refactor: trim the project to the bare minimum([8d48a56](https://github.com/abdes/asap/commit/8d48a56ef0db2359aa80b5e5b2854802dc6a45f0))

  The intention from the asap base project is to be a template starter repo that
  can be cloned and used for new projects. The bare minimum is the cmake build
  infrastructure, documentation, the common submodule and the contract checking
  assertions submodule.

  Contract checking library is now in a separate submodule to keep the common
  module as minimal as possible. Projects that do not intend to use contract
  checking will not have to depend on it.

### Bug Fixes

* use separate sphinx cache dir for each module ([03f36fc](http://github.com/abdes/asap/commit/03f36fc96c78c51b441feb9a7b9c70d19a2125c7))

## [3.1.2](http://github.com/abdes/asap/compare/v3.1.1...v3.1.2) (2021-12-19)


### Features

* doxygen snippets from `test` or `examples` source directories ([85b8000](http://github.com/abdes/asap/commit/85b8000e91dd105c0f90a62b1824957b68ce9c03))


### Bug Fixes

* **doc:** wrong chapter title in logging module page ([5966912](http://github.com/abdes/asap/commit/59669122d3ea8565878667e6f09b124bc7da1b99))
* doxygen doc generation uses wrong module info ([8833474](http://github.com/abdes/asap/commit/8833474402c523a3f2eda10b8927da46989f4bba))
* need to hardcode `asap` when linking `common` ([e038a70](http://github.com/abdes/asap/commit/e038a702ce8727fa1a6c8b3a45cb4bb211281db5))

## [3.1.1](http://github.com/abdes/asap/compare/v3.1.0...v3.1.1) (2021-12-19)

### Documentation

* minor tweaks to the sphinx config

## [3.1.0](http://github.com/abdes/asap/compare/v3.0.0...v3.1.0) (2021-12-18)


### ⚠ BREAKING CHANGES

* The logging `Registry` is now implemented as a singleton class and therefore it needs to be
  accessed via its instance() method. All other methods in its interface are not static anymore.
* Prefix the build options with `ASAP_` to make them unique and avoid clashing with other projects
  that may use the generic `OPTION_xxx` names. Additionally, the build presets now always activate
  building of tests and examples except in release builds where examples are not built.
* The cmake option `OPTION_SELF_CONTAINED` is no longer relevant as we believe that 3rd party
  dependencies should be installed using their own projects. In the wrostcase scenario, they should
  be explicitly added as install instructions to the project in a visible and documented way.

### Features

* add support for .editorconfig ([5a7a689](http://github.com/abdes/asap/commit/5a7a6892f7c05798e79c6f81e9eef3b0a6256ca1))
* provide a way to distinguish between debug and release builds via preprocessor define ([bbd84a2](http://github.com/abdes/asap/commit/bbd84a2c32c9833f79b442e202b647f93946b105))


### Bug Fixes

* adjust doc target names to work in renamed projects ([60acc65](http://github.com/abdes/asap/commit/60acc65fcc8d529a69f3617d792376be3e54b6ce))
* cmake option was placed in the wrong command ([b634b15](http://github.com/abdes/asap/commit/b634b153db975a8bc8715e82f7fafdc214504cfa))
* clean the logging API implementation ([25535c7](http://github.com/abdes/asap/commit/25535c76c0586d53f7c6c55db250a251f0c5390b))


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
