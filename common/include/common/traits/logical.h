/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2021.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#pragma once

#include <type_traits>

namespace asap {

/// Forms the logical conjunction of the type traits ..., effectively performing a logical AND on
/// the sequence of traits.
template <class...> struct conjunction : std::true_type {};

template <class B> struct conjunction<B> : B {};

template <class B1, class... Bi>
struct conjunction<B1, Bi...> : std::conditional<B1::value, conjunction<Bi...>, B1>::type {};

/// Forms the logical disjunction of the type traits ..., effectively performing a logical OR on the
/// sequence of traits.
template <class...> struct disjunction : std::false_type {};

template <class B> struct disjunction<B> : B {};

template <class B1, class... Bi>
struct disjunction<B1, Bi...> : std::conditional<B1::value, B1, disjunction<Bi...>>::type {};

#if (__cplusplus >= 201402L)
template <class... Bi> constexpr bool conjunction_v = conjunction<Bi...>::value;

template <class... Bi> constexpr bool disjunction_v = disjunction<Bi...>::value;
#endif

template <bool B> struct bool_constant : std::integral_constant<bool, B> {};

/// Forms the logical negation of the type trait B.
template <class B> struct negation : bool_constant<!bool(B::value)> {};

#if (__cplusplus >= 201402L)
template <class B> constexpr bool negation_v = negation<B>::value;
#endif

} // namespace asap
