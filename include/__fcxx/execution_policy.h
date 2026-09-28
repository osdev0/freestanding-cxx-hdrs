/* SPDX-License-Identifier: 0BSD */

/* Copyright (C) 2026 Mintsuki and contributors.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


/* The execution policy type trait of <execution>, which the parallel
 * overloads of the algorithms of <algorithm>, <numeric> and <memory> are
 * constrained with. The execution policies themselves are not freestanding,
 * and the parallel overloads are deleted. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_EXECUTION_POLICY_H
#define __FREESTANDING_CXX_HDRS_FCXX_EXECUTION_POLICY_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <type_traits>

namespace std {

/* [execpol.type], execution policy type trait. No type is an execution
 * policy, as the implementation has none. */
template <class _Tp>
struct is_execution_policy : false_type {};

template <class _Tp>
inline constexpr bool is_execution_policy_v = is_execution_policy<_Tp>::value;

/* The condition of the parallel overloads of the algorithms of namespace
 * std, which do not participate in overload resolution unless their first
 * parameter is an execution policy, for their SFINAE. */
template <class _Ep>
using __fcxx_enable_if_execution_policy_t =
    __fcxx_enable_if_t<is_execution_policy<__fcxx_remove_cvref_t<_Ep> >::value, int>;

/* [algorithms.parallel.defns], execution-policy, of C++26, which constrains
 * the parallel overloads of the algorithms of namespace ranges. */
template <class _Ep>
concept __fcxx_execution_policy = is_execution_policy_v<remove_cvref_t<_Ep> >;

} /* namespace std */

#endif
