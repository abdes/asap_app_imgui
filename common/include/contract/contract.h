/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <common/asap_common_api.h>

#include <cstdlib>
#include <functional>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

namespace asap {
namespace contract {

struct ASAP_COMMON_API Violation {
  const char *file;
  size_t line;
  const char *function;
  const char *type;
  const char *condition;
};

class ASAP_COMMON_API ViolationHandler {
public:
  using FunctionType = void (*)(const Violation *);
  using WrapperType = std::function<void(const Violation *)>;

  ViolationHandler() = default;
  virtual ~ViolationHandler();
  /// Copy constructor (deleted)
  ViolationHandler(const ViolationHandler &) = delete;
  /// Move constructor (default)
  ViolationHandler(ViolationHandler &&) = default;
  /// Copy assignment operator (deleted)
  auto operator=(const ViolationHandler &) -> ViolationHandler & = delete;
  /// Move assignment operator (default)
  auto operator=(ViolationHandler &&) -> ViolationHandler & = default;

  virtual void HandleViolation(const Violation *violation) = 0;

  virtual void SwapHandler(WrapperType &other_handler) = 0;
};

ASAP_COMMON_API auto GetViolationHandler() -> ViolationHandler &;

} // namespace contract
} // namespace asap

/* Default shorthand macros */
#define ASAP_EXPECT(cond) ASAP_EXPECT_DEFAULT(cond)
#define ASAP_ENSURE(cond) ASAP_ENSURE_DEFAULT(cond)
#define ASAP_ASSERT(cond) ASAP_ASSERT_DEFAULT(cond)

/* Default macros */
#define ASAP_EXPECT_DEFAULT(cond)                                                                  \
  INTERNAL_ASAP_CONTRACT_CREATE_EXPECT_(INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_, cond)

#define ASAP_ENSURE_DEFAULT(cond)                                                                  \
  INTERNAL_ASAP_CONTRACT_CREATE_ENSURE_(INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_, cond)

#define ASAP_ASSERT_DEFAULT(cond)                                                                  \
  INTERNAL_ASAP_CONTRACT_CREATE_ASSERT_(INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_, cond)

/* Audit macros */
#define ASAP_EXPECT_AUDIT(cond)                                                                    \
  INTERNAL_ASAP_CONTRACT_CREATE_EXPECT_(INTERNAL_ASAP_CONTRACT_MODE_AUDIT_, cond)

#define ASAP_ENSURE_AUDIT(cond)                                                                    \
  INTERNAL_ASAP_CONTRACT_CREATE_ENSURE_(INTERNAL_ASAP_CONTRACT_MODE_AUDIT_, cond)

#define ASAP_ASSERT_AUDIT(cond)                                                                    \
  INTERNAL_ASAP_CONTRACT_CREATE_ASSERT_(INTERNAL_ASAP_CONTRACT_MODE_AUDIT_, cond)

/* Helper macros */
#define INTERNAL_ASAP_CONTRACT_CREATE_EXPECT_(check, cond)                                         \
  check(INTERNAL_ASAP_CONTRACT_TYPE_EXPECT_, cond)

#define INTERNAL_ASAP_CONTRACT_CREATE_ENSURE_(check, cond)                                         \
  check(INTERNAL_ASAP_CONTRACT_TYPE_ENSURE_, cond)

#define INTERNAL_ASAP_CONTRACT_CREATE_ASSERT_(check, cond)                                         \
  check(INTERNAL_ASAP_CONTRACT_TYPE_ASSERT_, cond)

/* Contract names */
#define INTERNAL_ASAP_CONTRACT_TYPE_EXPECT_ "precondition"
#define INTERNAL_ASAP_CONTRACT_TYPE_ENSURE_ "postcondition"
#define INTERNAL_ASAP_CONTRACT_TYPE_ASSERT_ "assertion"

#if defined(__clang__)
#if __has_builtin(__builtin_unreachable)
#define INTERNAL_ASAP_CONTRACT_HAVE_BUILTIN_UNREACHABLE 1
#endif
#elif defined(__GNUC__)
#if __GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#define INTERNAL_ASAP_CONTRACT_HAVE_BUILTIN_UNREACHABLE 1
#endif
#endif

#if defined(_MSC_VER)
#define INTERNAL_ASAP_CONTRACT_HAVE_BUILTIN_ASSUME 1
#endif

/* Contract modes */
#define INTERNAL_ASAP_CONTRACT_IGNORE_(type, cond) (void)sizeof((cond) ? 1 : 0)

#define INTERNAL_ASAP_CONTRACT_CHECK_NEVER_CONTINUE_(type, cond)                                   \
  do {                                                                                             \
    if (!(cond)) {                                                                                 \
      INTERNAL_ASAP_CONTRACT_HANDLE_VIOLATION_(type, cond);                                        \
      abort();                                                                                     \
    }                                                                                              \
  } while (0)

/* Create violation handler call */
#define INTERNAL_ASAP_CONTRACT_HANDLE_VIOLATION_(type, cond)                                       \
  do {                                                                                             \
    const struct asap::contract::Violation violation = {                                           \
        __FILE__, __LINE__, static_cast<const char *>(__func__), type, #cond};                     \
    asap::contract::GetViolationHandler().HandleViolation(&violation);                             \
  } while (0)

/* Build configuration options */
#if defined ASAP_CONTRACT_OFF
#if defined ASAP_CONTRACT_DEFAULT || defined ASAP_CONTRACT_AUDIT || defined ASAP_CONTRACT_TEST
#error At most one of ASAP_CONTRACT_OFF, ASAP_CONTRACT_DEFAULT, ASAP_CONTRACT_AUDIT and ASAP_CONTRACT_TEST must be specified
#endif
#define INTERNAL_ASAP_CONTRACT_BUILD_ INTERNAL_ASAP_CONTRACT_BUILD_MODE_OFF_
#elif defined ASAP_CONTRACT_DEFAULT
#if defined ASAP_CONTRACT_AUDIT || defined ASAP_CONTRACT_TEST
#error At most one of ASAP_CONTRACT_OFF, ASAP_CONTRACT_DEFAULT, ASAP_CONTRACT_AUDIT and ASAP_CONTRACT_TEST must be specified
#endif
#define INTERNAL_ASAP_CONTRACT_BUILD_ INTERNAL_ASAP_CONTRACT_BUILD_MODE_DEFAULT_
#elif defined ASAP_CONTRACT_AUDIT
#if defined ASAP_CONTRACT_TEST
#error At most one of ASAP_CONTRACT_OFF, ASAP_CONTRACT_DEFAULT, ASAP_CONTRACT_AUDIT and ASAP_CONTRACT_TEST must be specified
#endif
#define INTERNAL_ASAP_CONTRACT_BUILD_ INTERNAL_ASAP_CONTRACT_BUILD_MODE_AUDIT_
#elif defined ASAP_CONTRACT_TEST
#define INTERNAL_ASAP_CONTRACT_BUILD_ INTERNAL_ASAP_CONTRACT_BUILD_MODE_AUDIT_
#else /* Default build */
#define INTERNAL_ASAP_CONTRACT_BUILD_ INTERNAL_ASAP_CONTRACT_BUILD_MODE_DEFAULT_
#endif

#define INTERNAL_ASAP_CONTRACT_BUILD_MODE_OFF_ 0
#define INTERNAL_ASAP_CONTRACT_BUILD_MODE_DEFAULT_ 1
#define INTERNAL_ASAP_CONTRACT_BUILD_MODE_AUDIT_ 2

/* Standard build configurations */
#if INTERNAL_ASAP_CONTRACT_BUILD_ == INTERNAL_ASAP_CONTRACT_BUILD_MODE_OFF_
#define INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_ INTERNAL_ASAP_CONTRACT_IGNORE_
#define INTERNAL_ASAP_CONTRACT_MODE_AUDIT_ INTERNAL_ASAP_CONTRACT_IGNORE_
#elif INTERNAL_ASAP_CONTRACT_BUILD_ == INTERNAL_ASAP_CONTRACT_BUILD_MODE_DEFAULT_
#define INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_ INTERNAL_ASAP_CONTRACT_CHECK_NEVER_CONTINUE_
#define INTERNAL_ASAP_CONTRACT_MODE_AUDIT_ INTERNAL_ASAP_CONTRACT_IGNORE_
#elif INTERNAL_ASAP_CONTRACT_BUILD_ == INTERNAL_ASAP_CONTRACT_BUILD_MODE_AUDIT_
#define INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_ INTERNAL_ASAP_CONTRACT_CHECK_NEVER_CONTINUE_
#define INTERNAL_ASAP_CONTRACT_MODE_AUDIT_ INTERNAL_ASAP_CONTRACT_CHECK_NEVER_CONTINUE_
#endif

/* Options to override standard build configurations */
#if defined ASAP_CONTRACT_MODE_DEFAULT
#undef INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_
#define INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_ ASAP_CONTRACT_MODE_DEFAULT
#endif

#if defined ASAP_CONTRACT_MODE_AUDIT
#undef INTERNAL_ASAP_CONTRACT_MODE_AUDIT_
#define INTERNAL_ASAP_CONTRACT_MODE_AUDIT_ ASAP_CONTRACT_MODE_AUDIT
#endif

// NOLINTEND(cppcoreguidelines-macro-usage)
