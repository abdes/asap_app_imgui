//    Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <stdint.h> // for unitptr_t

#include <application.h>
#include <shader.h>

#include <imgui/imgui.h>

namespace asap {

bool Application::Draw() {
  bool sleep_when_inactive = ApplicationBase::Draw();

  if (ImGui::GetIO().DisplaySize.y > 0) {
    if (ImGui::Begin("OpenGL Render")) {
      auto wsize = ImGui::GetWindowSize();

      glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_);
      // Define the viewport dimensions
      glBindTexture(GL_TEXTURE_2D, texColorBuffer_);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(wsize.x),
                   static_cast<GLsizei>(wsize.y), 0, GL_RGB, GL_UNSIGNED_BYTE,
                   NULL);
      glBindTexture(GL_TEXTURE_2D, 0);
      glViewport(0, 0, static_cast<GLsizei>(wsize.x),
                 static_cast<GLsizei>(wsize.y));

      // now that we actually created the framebuffer and added all attachments
      // we want to check if it is actually complete now
      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        ASLOG(error, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

      // Render
      // Clear the colorbuffer
      glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);  // we're not using the stencil buffer now

      // Draw the triangle
      ourShader_->use();
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);  // back to default
      // get the mouse position
      ImVec2 pos = ImGui::GetCursorScreenPos();

      // pass the texture of the FBO
      // window.getRenderTexture() is the texture of the FBO
      // the next parameter is the upper left corner for the uvs to be applied
      // at the third parameter is the lower right corner the last two
      // parameters are the UVs they have to be flipped (normally they would be
      // (0,0);(1,1)
      ImGui::GetWindowDrawList()->AddImage(
          (ImTextureID)(uintptr_t)texColorBuffer_, pos,
          ImVec2(pos.x + wsize.x, pos.y + wsize.y), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
  }

  return sleep_when_inactive;
}

void Application::AfterInit() {
  // Build and compile our shader program
  ourShader_ = new Shader("core.vs", "core.frag");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      // positions         // colors
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f   // top
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

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
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texColorBuffer_, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Application::BeforeShutDown() {
  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteTextures(1, &texColorBuffer_);
  glDeleteFramebuffers(1, &frameBuffer_);

  delete ourShader_;
}

}  // namespace asap
