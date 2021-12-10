/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <functional> // for std::function

#include <abstract_application.h>
#include <logging/logging.h>

namespace asap {

class RunnerBase : public asap::logging::Loggable<RunnerBase> {
public:
  using shutdown_function_type = std::function<void()>;

  RunnerBase(AbstractApplication &app, shutdown_function_type func)
      : app_(app), shutdown_function_(std::move(func)) {
  }

  virtual ~RunnerBase() = default;

  virtual void Run() = 0;

  static const char *LOGGER_NAME;

protected:
  AbstractApplication &app_;
  shutdown_function_type shutdown_function_;
};

} // namespace asap
