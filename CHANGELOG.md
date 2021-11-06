# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

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
