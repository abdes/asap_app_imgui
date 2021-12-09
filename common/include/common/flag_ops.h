/*     SPDX-License-Identifier: BSD-3-Clause     */

//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

/*!
 * \file flag_ops.h
 *
 * \brief C++ template API for bit flags manipulation.
 */

#pragma once

#include <type_traits>

namespace asap {

/*!
 * @brief Set bits in a mask based on the bits set in flags.
 *
 * @param[in,out] mask   the bitset mask to be changed.
 * @param[in]     flags  the flags to set in the mask. May contain one or more bits to set.
 */
template <typename T,
    typename std::enable_if<std::disjunction<std::is_arithmetic<T>, std::is_enum<T>>::value>::type
        * = nullptr>
void FlagSet(T &mask, T flags) {
  mask |= flags;
}

/*!
 * @brief Clear bits in a mask based on the bits set in flags.
 *
 * @param[in,out] mask   the bitset mask to be changed.
 * @param[in]     flags  the flags to clear in the mask. May contain one or more bits to clear.
 */
template <typename T,
    typename std::enable_if<std::disjunction<std::is_arithmetic<T>, std::is_enum<T>>::value>::type
        * = nullptr>
void FlagClear(T &mask, T flags) {
  mask &= ~flags;
}

/*!
 * @brief Flip bits ('0' to '1' and '1' to '0') in a mask based on the bits set in flags.
 *
 * @param[in,out] mask   the bitset mask to be changed.
 * @param[in]     flags  the flags to flip in the mask. May contain one or more bits to set.
 */
template <typename T,
    typename std::enable_if<std::disjunction<std::is_arithmetic<T>, std::is_enum<T>>::value>::type
        * = nullptr>
void FlagFlip(T &mask, T flags) {
  mask ^= flags;
}

/*!
 * @brief Check if the bits set in `flags` are also set in `mask`.
 *
 * @param[in] mask   the bitset mask to be tested.
 * @param[in] flags  the flags to check in the mask. May contain one or more bits to set.
 *
 * @return \b true if the flags are set in mask; otherwise \b false;
 */
template <typename T,
    typename std::enable_if<std::disjunction<std::is_arithmetic<T>, std::is_enum<T>>::value>::type
        * = nullptr>
auto FlagTest(T mask, T flags) -> bool {
  return (mask & flags) == flags;
}

} // namespace asap
