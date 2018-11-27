//    Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <glad/gl.h>

#include <ui/application_base.h>

class Shader;

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
  void AfterInit() override;
  void DrawInsideMainMenu() override {}
  void DrawInsideStatusBar(float /*width*/, float /*height*/) override {}
  void BeforeShutDown() override;

  private:
   GLuint VBO = 0, VAO = 0;
   GLuint frameBuffer_ = 0;
   GLuint texColorBuffer_ = 0;
   Shader *ourShader_{nullptr};
 };

}  // namespace asap
