//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

// clang-format off
/*!
 * \file
 *
 * \brief Types and and macros used for contract checking.
 *
 * Behavior of the contract checking macros can be changed by setting the
 * contract checking build mode to one of the values: `OFF`, `DEFAULT` or
 * `AUDIT` by defining the corresponding symbol as per the table below:
 *
 * Build Mode | Symbol to be defined  | Behavior
 * ---------- | --------------------- | --------
 *  OFF       | ASAP_CONTRACT_OFF     | None of the conditions are enforced.
 *  DEFAULT   | ASAP_CONTRACT_DEFAULT | Default mode conditions are enforced and execution will abort if the contract is not honored. Audit mode conditions are ignored.
 *  AUDIT     | ASAP_CONTRACT_AUDIT   | All conditions are enforced and execution will abort if the contract is not honored.
 */
// clang-format on

#pragma once

#include <contract/asap_contract_api.h>

#include <cstdlib>
#include <functional>

// -----------------------------------------------------------------------------
// Types used to implement the contract checking macros/apis
// -----------------------------------------------------------------------------

/// Contract checking namespace.
namespace asap::contract {

/// Encapsulates the information related to a contract violation.
struct ASAP_CONTRACT_API Violation {
  /// The name of the source file in which the violation occurred.
  const char *file;
  /// The line number at which the violation occurred.
  size_t line;
  /// The function name inside which the contract violation occurred.
  const char *function;
  /// The type of the violation (`precondition`, `postcondition` or
  /// `assertion`).
  const char *type;
  /// The expression that specifies the predicate of the contract.
  const char *condition;
};

/*!
 * \brief Interface for a violation handler which can switch its implementation
 * at runtime.
 *
 * We expect to have a single violation handler instance in the system, although
 * its implementation can be changed at runtime. Therefore, the interface
 * clearly deletes the copy constructors and assignment operators.
 *
 * \see GetViolationHandler
 */
class ASAP_CONTRACT_API ViolationHandler {
public:
  /// A wrapper using std::function around violation handler implementation
  /// functions.
  using WrapperType = std::function<void(const Violation *)>;

  /// Constructor.
  ViolationHandler() = default;

  /// Destructor.
  virtual ~ViolationHandler();

  /// Copy constructor (deleted).
  ViolationHandler(const ViolationHandler &) = delete;

  /// Move constructor (default)
  ViolationHandler(ViolationHandler &&) = default;

  /// Copy assignment operator (deleted)
  auto operator=(const ViolationHandler &) -> ViolationHandler & = delete;

  /// Move assignment operator (default)
  auto operator=(ViolationHandler &&) -> ViolationHandler & = default;

  /*!
   * \brief Handle a contract violation.
   *
   * This method is called when a contract violation occurs. The information
   * related to the violation is provided by the `violation` parameter.
   *
   * \note This function *may* never return.
   *
   * \param violation violation details. Will never be null and is allocated on
   * the stack.
   */
  virtual void HandleViolation(const Violation *violation) = 0;

  /*!
   * \brief Swap the existing violation handler implementation with the give
   * one.
   *
   * \param other_handler at the call time this holds the new violation handler
   * implementation function. Upon return, it will hold the old violation
   * handler function.
   */
  virtual void SwapHandler(WrapperType &other_handler) = 0;
};

/*!
 * \brief Obtain the single instance of the violation handler.
 *
 * \note This function is very lightweight and can be called as often as
 * required. It is preferred to exclusively call it every time the violation
 * handler is needed instead of storing the returned reference.
 *
 * \return a reference to the violation handler.
 */
ASAP_CONTRACT_API auto GetViolationHandler() -> ViolationHandler &;

} // namespace asap::contract

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

// -----------------------------------------------------------------------------
// Public contract checking macros
// -----------------------------------------------------------------------------

// --- Default mode macros
// -----------------------------------------------------------------------------

/*!
 * \brief Defines a precondition.
 *
 * A precondition describes the function's expectation of its arguments and/or
 * the state of other objects upon entry into the function and is usually placed
 * at the start of a function body.
 *
 * \note The prediction is ignored if the contract checking build mode is `OFF`.
 *
 * \param cond an expression, that specifies the predicate of the contract.
 */
#define ASAP_EXPECT(cond)                                                      \
  INTERNAL_ASAP_CONTRACT_CREATE_EXPECT_(                                       \
      INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_, cond)

/*!
 * Defines a postcondition.
 *
 * A postcondition is a condition that a function should ensure for the return
 * value and/or the state of objects upon exit from the function and is usually
 * placed immediately before returning control to the caller of the function.
 *
 * \note The postcondition is ignored if the contract checking build mode is
 * `OFF`.
 *
 * \param cond an expression, that specifies the predicate of the contract.
 */
#define ASAP_ENSURE(cond)                                                      \
  INTERNAL_ASAP_CONTRACT_CREATE_ENSURE_(                                       \
      INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_, cond)

/*!
 * \brief Defines an assertion.
 *
 * An assertion is a condition that should be satisfied where it appears in a
 * function body and can be placed anywhere within the body of a function.
 *
 * \note The assertion is ignored if the contract checking build mode is `OFF`.
 *
 * \param cond an expression, that specifies the predicate of the contract.
 */
#define ASAP_ASSERT(cond)                                                      \
  INTERNAL_ASAP_CONTRACT_CREATE_ASSERT_(                                       \
      INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_, cond)

// --- Audit mode macros
// -----------------------------------------------------------------------------

/*!
 * \brief Defines a precondition for the `AUDIT` contract checking mode.
 *
 * A precondition describes the function's expectation of its arguments and/or
 * the state of other objects upon entry into the function and is usually placed
 * at the start of a function body.
 *
 * \note The precondition is enforced only if the contract checking build mode
 * is `AUDIT`.
 *
 * \param cond an expression, that specifies the predicate of the contract.
 */
#define ASAP_EXPECT_AUDIT(cond)                                                \
  INTERNAL_ASAP_CONTRACT_CREATE_EXPECT_(                                       \
      INTERNAL_ASAP_CONTRACT_MODE_AUDIT_, cond)

/*!
 * \brief Defines a postcondition for the `AUDIT` contract checking mode.
 *
 * A postcondition is a condition that a function should ensure for the return
 * value and/or the state of objects upon exit from the function and is usually
 * placed immediately before returning control to the caller of the function.
 *
 * \note The postcondition is enforced only if the contract checking build mode
 * is `AUDIT`.
 *
 * \param cond an expression, that specifies the predicate of the contract.
 */
#define ASAP_ENSURE_AUDIT(cond)                                                \
  INTERNAL_ASAP_CONTRACT_CREATE_ENSURE_(                                       \
      INTERNAL_ASAP_CONTRACT_MODE_AUDIT_, cond)

/*!
 * \brief Defines an assertion for the `AUDIT` contract checking mode.
 *
 * An assertion is a condition that should be satisfied where it appears in a
 * function body and can be placed anywhere within the body of a function.
 *
 * \note The assertion is enforced only if the contract checking build mode is
 * `AUDIT`.
 *
 * \param cond an expression, that specifies the predicate of the contract.
 */
#define ASAP_ASSERT_AUDIT(cond)                                                \
  INTERNAL_ASAP_CONTRACT_CREATE_ASSERT_(                                       \
      INTERNAL_ASAP_CONTRACT_MODE_AUDIT_, cond)

// -----------------------------------------------------------------------------
// Internal macros
// -----------------------------------------------------------------------------

// Exclude internal macros from doxygen documentation
#if !defined(DOXYGEN_DOCUMENTATION_BUILD)

/* Helper macros */
#define INTERNAL_ASAP_CONTRACT_CREATE_EXPECT_(check, cond)                     \
  check(INTERNAL_ASAP_CONTRACT_TYPE_EXPECT_, cond)

#define INTERNAL_ASAP_CONTRACT_CREATE_ENSURE_(check, cond)                     \
  check(INTERNAL_ASAP_CONTRACT_TYPE_ENSURE_, cond)

#define INTERNAL_ASAP_CONTRACT_CREATE_ASSERT_(check, cond)                     \
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

#if defined(_MSVC_VER)
#define INTERNAL_ASAP_CONTRACT_HAVE_BUILTIN_ASSUME 1
#endif

/* Contract modes */
#define INTERNAL_ASAP_CONTRACT_IGNORE_(type, cond) (void)sizeof((cond) ? 1 : 0)

#define INTERNAL_ASAP_CONTRACT_CHECK_NEVER_CONTINUE_(type, cond)               \
  do {                                                                         \
    if (!(cond)) {                                                             \
      INTERNAL_ASAP_CONTRACT_HANDLE_VIOLATION_(type, cond);                    \
      abort();                                                                 \
    }                                                                          \
  } while (0)

/* Create violation handler call */
#define INTERNAL_ASAP_CONTRACT_HANDLE_VIOLATION_(type, cond)                   \
  do {                                                                         \
    const struct asap::contract::Violation violation = {                       \
        __FILE__, __LINE__, static_cast<const char *>(__func__), type, #cond}; \
    asap::contract::GetViolationHandler().HandleViolation(&violation);         \
  } while (0)

/* Build configuration options */
#if defined ASAP_CONTRACT_OFF
#if defined ASAP_CONTRACT_DEFAULT
#error At most one of ASAP_CONTRACT_OFF, ASAP_CONTRACT_DEFAULT, ASAP_CONTRACT_AUDIT must be specified
#endif
#define INTERNAL_ASAP_CONTRACT_BUILD_ INTERNAL_ASAP_CONTRACT_BUILD_MODE_OFF_
#elif defined ASAP_CONTRACT_DEFAULT
#if defined ASAP_CONTRACT_AUDIT
#error At most one of ASAP_CONTRACT_OFF, ASAP_CONTRACT_DEFAULT, ASAP_CONTRACT_AUDIT must be specified
#endif
#define INTERNAL_ASAP_CONTRACT_BUILD_ INTERNAL_ASAP_CONTRACT_BUILD_MODE_DEFAULT_
#elif defined ASAP_CONTRACT_AUDIT
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
#elif INTERNAL_ASAP_CONTRACT_BUILD_ ==                                         \
    INTERNAL_ASAP_CONTRACT_BUILD_MODE_DEFAULT_
#define INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_                                   \
  INTERNAL_ASAP_CONTRACT_CHECK_NEVER_CONTINUE_
#define INTERNAL_ASAP_CONTRACT_MODE_AUDIT_ INTERNAL_ASAP_CONTRACT_IGNORE_
#elif INTERNAL_ASAP_CONTRACT_BUILD_ == INTERNAL_ASAP_CONTRACT_BUILD_MODE_AUDIT_
#define INTERNAL_ASAP_CONTRACT_MODE_DEFAULT_                                   \
  INTERNAL_ASAP_CONTRACT_CHECK_NEVER_CONTINUE_
#define INTERNAL_ASAP_CONTRACT_MODE_AUDIT_                                     \
  INTERNAL_ASAP_CONTRACT_CHECK_NEVER_CONTINUE_
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

#endif // DOXYGEN_DOCUMENTATION_BUILD

// NOLINTEND(cppcoreguidelines-macro-usage)
