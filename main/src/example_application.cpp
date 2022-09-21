/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include "example_application.h"
#include "logging/logging.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp>   // glm::vec3
#include <imgui/imgui.h>

#include <cstdint> // for unitptr_t

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

namespace {
const struct {
  float x, y;
  float r, g, b;
} VERTICES[3] = {
    // clang-format off
    { -0.6F, -0.4F, 1.0F, 0.0F, 0.0F },
    {  0.6F, -0.4F, 0.0F, 1.0F, 0.0F },
    {  0.0F,  0.6F, 0.0F, 0.0F, 1.0F }
    // clang-format on
};

// clang-format off
const char *const VERTEX_SHADER_TEXT =
"#version 150\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
// clang-format on

// clang-format off
const char *const FRAGMENT_SHADER_TEXT =
"#version 150\n"
"in vec3 color;\n"
"out vec4 colorOut;\n"
"void main()\n"
"{\n"
"    colorOut = vec4(color, 1.0);\n"
"}\n";
// clang-format on

auto OpenGLErrorCheck() -> GLenum {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static auto &logger = asap::logging::Registry::GetLogger("main");
  auto code = glGetError();
  if (code != GL_NO_ERROR) {
    ASLOG_TO_LOGGER(logger, error, "OpenGL error: {}", code);
  }
  return code;
}

} // namespace

auto ExampleApplication::Draw() -> bool {
  bool sleep_when_inactive = DrawCommonElements();

  if (ImGui::GetIO().DisplaySize.y > 0) {
    if (ImGui::Begin("OpenGL Render")) {
      auto wsize = ImGui::GetWindowSize();

      glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_);
      // Define the viewport dimensions
      glBindTexture(GL_TEXTURE_2D, texColorBuffer_);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(wsize.x),
          static_cast<GLsizei>(wsize.y), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
      glBindTexture(GL_TEXTURE_2D, 0);
      glViewport(
          0, 0, static_cast<GLsizei>(wsize.x), static_cast<GLsizei>(wsize.y));

      // now that we actually created the framebuffer and added all attachments
      // we want to check if it is actually complete now
      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ASLOG(error, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
      }
      // Render
      // Clear the colorbuffer
      glClearColor(0.2F, 0.2F, 0.3F, 1.0F);
      glClear(GL_COLOR_BUFFER_BIT); // we're not using the stencil buffer now

      glm::mat4 Projection =
          glm::perspective(glm::radians(45.0F), 4.0F / 3.0F, 0.1F, 100.F);
      glm::mat4 View =
          glm::translate(glm::mat4(1.0F), glm::vec3(0.0F, 0.0F, -1.0F));
      View = glm::rotate(View, static_cast<float>(glfwGetTime()),
          glm::vec3(-1.0F, 0.0F, 0.0F));
      View = glm::rotate(
          View, static_cast<float>(glfwGetTime()), glm::vec3(0.0F, 1.0F, 0.0F));
      View = glm::rotate(
          View, static_cast<float>(glfwGetTime()), glm::vec3(0.0F, 0.0F, 1.0F));
      glm::mat4 Model = glm::scale(glm::mat4(1.0F), glm::vec3(0.5F));
      glm::mat4 mvp = Projection * View * Model;

      glUseProgram(program);
      glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glBindVertexArray(0);
      glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

      ImVec2 pos = ImGui::GetCursorScreenPos();
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,
      // performance-no-int-to-ptr)
      ImGui::GetWindowDrawList()->AddImage(
          reinterpret_cast<ImTextureID>(texColorBuffer_), pos,
          ImVec2(pos.x + wsize.x, pos.y + wsize.y), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
  }

  return sleep_when_inactive;
}

void ExampleApplication::AfterInit() {

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES),
      static_cast<const void *>(VERTICES), GL_STATIC_DRAW);

  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &VERTEX_SHADER_TEXT, nullptr);
  glCompileShader(vertex_shader);
  GLint isCompiled = 0;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(vertex_shader, maxLength, &maxLength, &errorLog[0]);

    // Provide the infolog in whatever manor you deem best.
    // Exit with failure.
    ASLOG(error, "GL COMPILE ERROR: {}", errorLog.data());
  }

  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_TEXT, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, &errorLog[0]);

    // Provide the infolog in whatever manor you deem best.
    // Exit with failure.
    ASLOG(error, "GL COMPILE ERROR: {}", errorLog.data());
  }

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    // The program is useless now. So delete it.
    glDeleteProgram(program);

    // Provide the infolog in whatever manner you deem best.
    // Exit with failure.
    ASLOG(error, "GL LINK ERROR: {}", infoLog.data());
  }

  mvp_location = glGetUniformLocation(program, "MVP");
  vpos_location = glGetAttribLocation(program, "vPos");
  vcol_location = glGetAttribLocation(program, "vCol");

  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(
      vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), nullptr);
  glEnableVertexAttribArray(vcol_location);
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
      sizeof(VERTICES[0]),
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,
      // performance-no-int-to-ptr)
      reinterpret_cast<const void *>(sizeof(float) * 2));

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  glGenFramebuffers(1, &frameBuffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_);

  // generate texture
  glGenTextures(1, &texColorBuffer_);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer_);
  // We'll set the texture size just before the draw happens based on the window
  // size.
  //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 760, 0, GL_RGB,
  //  GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // attach it to currently bound framebuffer object
  glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer_, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ExampleApplication::BeforeShutDown() {
  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteTextures(1, &texColorBuffer_);
  glDeleteFramebuffers(1, &frameBuffer_);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
