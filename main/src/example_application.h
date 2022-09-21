/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include "application_base.h"

#include <glad/gl.h>

class Shader;

class ExampleApplication final : public ApplicationBase {
public:
  ExampleApplication() = default;

  ExampleApplication(const ExampleApplication &) = delete;
  auto operator=(const ExampleApplication &) -> ExampleApplication & = delete;

  /// Not move constructible
  ExampleApplication(ExampleApplication &&) = delete;
  /// Not move assignable
  auto operator=(ExampleApplication &&) -> ExampleApplication & = delete;

  virtual ~ExampleApplication() = default;

  auto Draw() -> bool override;

protected:
  void AfterInit() override;
  void DrawInsideMainMenu() override {
  }
  void DrawInsideStatusBar(float /*width*/, float /*height*/) override {
  }
  void BeforeShutDown() override;

private:
  GLuint VBO = 0, VAO = 0;
  GLuint program = 0;
  GLint mvp_location = -1, vpos_location = -1, vcol_location = -1;
  GLuint frameBuffer_ = 0;
  GLuint texColorBuffer_ = 0;
};
