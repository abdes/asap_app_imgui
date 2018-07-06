//    Copyright The Blocxxi Project Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <ui/application_base.h>

namespace asap {

 class Application final : public asap::ui::ApplicationBase {
 public:
  Application() : asap::ui::ApplicationBase() {}

  /// Not move constructible
  Application(Application &&) = delete;
  /// Not move assignable
  Application &operator=(Application &&) = delete;

  ~Application() override = default;

  bool Draw() final;

 protected:
  void AfterInit() override {}
  void DrawInsideMainMenu() override {}
  void DrawInsideStatusBar(float /*width*/, float /*height*/) override {}
  void BeforeShutDown() override {}
};

}  // namespace asap
