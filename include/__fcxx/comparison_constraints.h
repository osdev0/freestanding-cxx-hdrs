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


/* The constraints of the comparison operators of optional, variant and
 * expected from C++26 on: that the comparisons of what they hold are
 * well-formed, with results convertible to bool. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_COMPARISON_CONSTRAINTS_H
#define __FREESTANDING_CXX_HDRS_FCXX_COMPARISON_CONSTRAINTS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <type_traits>

namespace std {

template <class _Tp>
concept __fcxx_bool_convertible = is_convertible<_Tp, bool>::value;

template <class _Tp, class _Up>
concept __fcxx_eq_comparable = requires(const _Tp& __t, const _Up& __u) {
    { __t == __u } -> __fcxx_bool_convertible;
};

template <class _Tp, class _Up>
concept __fcxx_ne_comparable = requires(const _Tp& __t, const _Up& __u) {
    { __t != __u } -> __fcxx_bool_convertible;
};

template <class _Tp, class _Up>
concept __fcxx_lt_comparable = requires(const _Tp& __t, const _Up& __u) {
    { __t < __u } -> __fcxx_bool_convertible;
};

template <class _Tp, class _Up>
concept __fcxx_gt_comparable = requires(const _Tp& __t, const _Up& __u) {
    { __t > __u } -> __fcxx_bool_convertible;
};

template <class _Tp, class _Up>
concept __fcxx_le_comparable = requires(const _Tp& __t, const _Up& __u) {
    { __t <= __u } -> __fcxx_bool_convertible;
};

template <class _Tp, class _Up>
concept __fcxx_ge_comparable = requires(const _Tp& __t, const _Up& __u) {
    { __t >= __u } -> __fcxx_bool_convertible;
};

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
