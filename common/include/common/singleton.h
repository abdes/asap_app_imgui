/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <common/asap_common_api.h>

#include <memory>

namespace asap {

template <typename T> class ASAP_COMMON_TEMPLATE_API Singleton {
public:
  static auto instance() -> T &;

  Singleton(const Singleton &) = delete;
  auto operator=(const Singleton) -> Singleton & = delete;

  Singleton(Singleton &&) = delete;
  auto operator=(Singleton &&) -> Singleton & = delete;

protected:
  struct token {};
  Singleton() = default;
  ~Singleton() = default;
};

template <typename T> inline auto Singleton<T>::instance() -> T & {
  // We do expect a clang compiler warning here about the exit time destructor required for this
  // statement. The risk present here is that some other singleton destructor would use contract
  // validation macros after the violation handler is destroyed.
  // We can't really fix this warning and just stroing a function pointer instead of a function
  // object would create way too many more warnings...
  //
  // ==> It is acceptable to not use contract checks in singleton destructors.
#if defined(__clang__)
#if __has_warning("-Wexit-time-destructors")
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
#endif
  static const std::unique_ptr<T> instance{new T{token{}}};
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

  return *instance;
}

} // namespace asap
