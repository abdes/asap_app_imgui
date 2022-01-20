//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License.See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

/*!
 * \file
 *
 * \brief C++ lightweight mixin support library.
 *
 * The idea with mix-ins is to provide a bunch of primitive classes, where each
 * of them models a basic orthogonal concept, and be able to stick them together
 * to compose more complex classes with just the functionality you want -- sort
 * of like Lego. The primitive classes themselves are meant to be used as
 * building blocks. This is extensible since later on you can add other
 * primitive classes to the collection without affecting the existing ones.
 *
 * A technique for doing this in C++ is using templates and inheritance. The
 * basic idea here is you connect these building blocks together by providing
 * them via the template parameter. You then chain them together, eg. via
 * typedef, to form a new type containing the functionality you want.
 *
 * **What's a mixin**
 *
 * A mixin is a template struct/class that:
 *  - inherits publicly from its only template parameter (the CRTP pattern). It
 *    is allowed to also inherit from other things,
 *  - has a constructor that forwards arguments its does not consume to its
 *    base.
 *
 * \code
 * template<Base>
 * struct my_mixin : Base { // and any other additional bases
 *
 *   // A mixin must forward the constructor parameters it does not consume
 *   // add the parameters to your mixin at the front, before `rest`
 *   template <typename...Args>
 *   my_mixin(Args&&... rest) : Base(std::forward<Args>(rest)...) {}
 *
 *   // public interface methods
 *
 *   // private state
 * }
 * \endcode
 *
 * Mixins can have state, which should be implemented with private data members.
 * Access to the state information should be provided via the public interface.
 * That way, mixins are self-contained, their interface is clear and conflicts
 * with other mixins are minimized.
 *
 * *Composing mixins*
 * Mixins are added to a class using chain inheritance, where each layer
 * inherits from the layer before it. At the end, the resulting class (which is
 * the one we'll be using) is the most derived class and provides the interfaces
 * of all mixins used to build it.
 *
 * One problem remains though: just like that, mixins are restricted on which
 * functionality they can use from other mixins. Interoperability between mixins
 * works only in the direction of the inheritance chain. For example, if we are
 * using a logging mixin with a persistence mixin, then the persistence mixin
 * can use the logging interface as long as it is mixed-in after the logging
 * mixin. But this is not really what we want as conceptually, mixins are a set
 * and we want to be able to compose them in any order we want.
 *
 * In order to deal with this elegantly, we automatically add a top layer in the
 * chain (not a mixin), that will track the 'most derived class' (the one with
 * all the mixins interfaces) and offer access to a `self` that is the result of
 * casting `this` to that **most derived class**. This layer is the initial base
 * in the inheritance chain and is therefore visible from any other mixin.
 *
 * When defining a concrete class using mixins, you don't need to do anything to
 * benefit from the above quality of life improvement. Simply declare mixins as
 * usual and access their interfaces via `this->self()`. We need to have
 * `this->` because `self()` is a *dependent* name (i.e. depends on the template
 * parameter but is not declared within the template).
 *
 * \snippet mixin_top_test.cpp Mixin calls other mixin
 *
 * The order in which you use compose the concrete class using the above mixins
 * is not important, thanks to the top layer and to the trick of providing
 * access to the `most derived class` via `self()`. In the example below, the
 * two classes, `Concrete` and `Concrete2`, are equivalent:
 *
 * \snippet mixin_top_test.cpp Composition order does not matter
 */

#pragma once

#include <type_traits>

/// C++ mixin support library
namespace asap::mixin {

/*!
 * \brief Top layer in the mixin chain, providing access to the `most derived
 * class`, thus giving visibility to all public interfaces of all mixins in the
 * chain.
 *
 * This is not a mixin. It's simply the top layer in the mixin chain, with the
 * bottom layer being the `most derived class`. This layer is automatically
 * added to the chain in order to provide access to the `most derived class`
 * from within any mixin in the chain. This trick is extremely powerful and
 * allows any mixin to access any public interface in another mixin or in the
 * base class. Simply use `this->self()` (note that `this->` here is required
 * due to the fact that `self()` is a dependent name).
 *
 * **Example**
 *
 * \snippet mixin_top_test.cpp Mixin calls other mixin
 *
 * The above mixins can be put together, independently of their order, as
 * following:
 *
 * \snippet mixin_top_test.cpp Composition order does not matter
 */
template <typename MostDerived> struct MixinTop { /* not a mixin */
  using self_type = MostDerived;
  // clang-format off
  auto self() & -> decltype(auto) { return static_cast<self_type &>(*this); }
  auto self() && -> decltype(auto) { return static_cast<self_type &&>(*this); }
  auto self() const & -> decltype(auto) { return static_cast<self_type const &>(*this); }
  auto self() const && -> decltype(auto) { return static_cast<self_type const &&>(*this); }
  // clang-format on
};

#if !defined(DOXYGEN_DOCUMENTATION_BUILD)
namespace detail {
/*
 * Mixing multiple mixins with multiple inheritance (the traditional CRTP
 * pattern), would require complicated casting from within a mixin to access
 * another mixin and sometimes would create very large size classes. Chain
 * inheritance, combined with the `MixinTop` trick solves these problems.
 *
 * The helpers in this implementation detail block are here to simplify the
 * right-folding of the mixins template parameters and the automatic chaining of
 * the `MixinTop` layer. This concretely results in an inheritance chain that
 * starts with the `MixinTop` and ends with the Concrete class, adding an
 * inheritance level for each template parameter (a Mixin) because every Mixin
 * extends its only template parameter.
 */

template <typename Concrete, template <class> class H,
    template <class> class... Tail>
struct ChainInherit {
  using result = typename ChainInherit<Concrete, Tail...>::type;
  using type = H<result>;
};
// base-case
template <typename Concrete, template <class> class H>
struct ChainInherit<Concrete, H> {
  using type = H<Concrete>;
};

template <typename Concrete, template <class> class... Mixins>
using MixinImpl = typename ChainInherit<MixinTop<Concrete>, Mixins...>::type;

} // namespace detail
#endif // !DOXYGEN_DOCUMENTATION_BUILD

/*!
 * \brief Mixes multiple mixins in an inheritance chain starting with a
 * `MixinTop` and ending with the `Concrete` class, where each layer inherits
 * from the previous one.
 *
 * Similar to the rule we set for mixins, where a mixin's constructor must
 * forward arguments it does not use to its base class, we also require the
 * mixed in class to do the same.
 *
 * @tparam Concrete The base class to which mixins will be added.
 *
 * @tparam Mixins The list of mixins to be added to the Concrete base class.
 * Each mixin must respect the following requirements: inherits publicly from
 * its **only template parameter** and has a constructor that forwards arguments
 * its does not consume to its base.
 */
template <typename Concrete, template <class> class... Mixins>
struct Mixin : detail::MixinImpl<Concrete, Mixins...> {
  /*!
   * \brief Constructor that forwards all its arguments to the base class.
   */
  template <typename... Rest>
  constexpr explicit Mixin(Rest &&...rest)
      : detail::MixinImpl<Concrete, Mixins...>(
            static_cast<decltype(rest)>(rest)...) {
  }

  // Explicitly implement the other builtins because we have defined one
  // constructor above.

  Mixin() = default;
  Mixin(Mixin const &) = default;
  Mixin(Mixin &&) noexcept = default;
  auto operator=(Mixin const &) -> Mixin & = default;
  auto operator=(Mixin &&) noexcept -> Mixin & = default;

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsuggest-destructor-override"
#endif
  ~Mixin() = default;
#if defined(__clang__)
#pragma clang diagnostic push
#endif
};

/*!
 * \brief Allows currying of mixin classes with multiple template parameters.
 *
 *
 * **Example**
 *
 * \snippet mixin_curry_test.cpp Curry mixin with multiple parameters
 */
template <template <class...> class Mixin, typename... Args> struct Curry {
  template <typename Base> using mixin = Mixin<Args..., Base>;
};

} // namespace asap::mixin

/// Utility mixins
namespace asap::mixins {

/// Implementation detail used in the `Provides` mixin.
struct Deferred {
  Deferred() = delete;
};

/**
 * \brief A mixin that provides an interface to be mixed into the composite
 * class.
 *
 * @tparam Interface - the abstract base class to add to the interface. The rest
 * are implementation details.
 *
 * **Example**
 *
 * \snippet mixin_interfaces_test.cpp Virtual interface
 *
 * \snippet mixin_interfaces_test.cpp Mixin provides interface
 */
template <typename Interface, typename Base = Deferred>
struct Provides : Base, Interface {
  template <typename Base_>
  using mixin =
      typename mixin::Curry<Provides, Interface>::template mixin<Base_>;

  template <typename... Args>
  constexpr explicit Provides(Args &&...args)
      : Base(static_cast<decltype(args)>(args)...) {
  }
};

} // namespace asap::mixins
