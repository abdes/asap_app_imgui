/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

/*!
 * \file singleton.h
 *
 * \brief C++ template based singleton interface declarations.
 */

#pragma once

#include <common/asap_common_api.h>

#include <memory>

/// Top level namespace.
namespace asap {

/*!
 * \brief Templated singleton design pattern implementation with thread-safe lazy initialization.
 *
 * This singleton implementation follows Scott Meyers' approach. This approach is founded on C++'s
 * guarantee that local static objects are initialized when the object's definition is first
 * encountered during a call to that function." ... "As a bonus, if you never call a function
 * emulating a non-local static object, you never incur the cost of constructing and destructing the
 * object.
 *
 * To avoid that the single instance is always initialized before main whether it is used or not, we
 * introduce the use of a smart pointer (`unique_ptr`) to wrap the single instance.
 *
 * The problem is that the make_unique_ptr/make_shared_ptr of the C++11 standard library cannot work
 * on private constructors, but the direct solution is not concise (and it is difficult to achieve
 * cross-compiler compatibility). Instead we use a token to deny users directly constructing a
 * class and we ask derived classes to implement a special constructor to implicitly convert a token
 * into the derived class.
 *
 * Example:
 * ```
 * class MyVars: public asap::singleton<MyVars> {
 * public:
 *   explicit MyVars(typename asap::singleton<MyVars>::token) {}
 *   long var1;
 * };
 * ```
 */
template <typename T> class ASAP_COMMON_TEMPLATE_API Singleton {
public:
  /*!
   * Get the single instance of the singleton.
   *
   * \return the single instance of the singleton.
   */
  static auto instance() -> T &;

  /// Copy constructor (deleted).
  Singleton(const Singleton &) = delete;

  /// Assignment operator (deleted).
  auto operator=(const Singleton) -> Singleton & = delete;

  /// Move constructor (deleted).
  Singleton(Singleton &&) = delete;

  /// Move assignment operator (deleted).
  auto operator=(Singleton &&) -> Singleton & = delete;

protected:
  /// A token type used to only allow derived classes to be singletons.
  struct token {};

  /// Default constructor.
  Singleton() = default;

  /// Destructor.
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
