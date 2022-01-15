//===----------------------------------------------------------------------===//
// Distributed under the 3-Clause BSD License. See accompanying file LICENSE or
// copy at https://opensource.org/licenses/BSD-3-Clause).
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#pragma once

namespace asap::contract::testing {

/*
 * The test functions are placed in their own translation unit so that the
 * compiler cannot optimize away the contract checks. Furthermore, the compiler
 * cannot remove code that would be unreachable because off CCS's that are
 * violated.
 */

auto TestExpectDefault(const int *ptr) -> int;
auto TestEnsureDefault(const int *ptr) -> int;
auto TestAssertDefault(const int *ptr) -> int;

auto TestExpectAudit(const int *ptr) -> int;
auto TestEnsureAudit(const int *ptr) -> int;
auto TestAssertAudit(const int *ptr) -> int;

} // namespace asap::contract::testing
