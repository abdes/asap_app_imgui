//    Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

namespace asap {
class ImGuiRunner;

class AbstractApplication {
 public:
  virtual ~AbstractApplication() = default;

  virtual void Init(ImGuiRunner *runner) = 0;
  virtual bool Draw() = 0;
  virtual void ShutDown() = 0;
};

}  // namespace asap
