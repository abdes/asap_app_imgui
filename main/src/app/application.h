/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

namespace asap::app {

class ImGuiRunner;

class Application {
public:
  virtual void Init(ImGuiRunner *runner) = 0;
  virtual auto Draw() -> bool = 0;
  virtual void ShutDown() = 0;

  Application(const Application &) = delete;
  Application(Application &&) = delete;

  auto operator=(const Application &) -> Application & = delete;
  auto operator=(Application &&) -> Application & = delete;

protected:
  Application() = default;
  ~Application() = default;
};

} // namespace asap::app
