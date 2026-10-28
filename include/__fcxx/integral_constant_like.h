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


/* The exposition-only concepts constexpr-wrapper-like ([expos.only.entity])
 * and integral-constant-like ([span.syn]) of C++26, for the types that carry
 * a constant in their type, such as integral_constant and constant_wrapper,
 * which <span> and <mdspan> deduce static extents from. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_INTEGRAL_CONSTANT_LIKE_H
#define __FREESTANDING_CXX_HDRS_FCXX_INTEGRAL_CONSTANT_LIKE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <concepts>
#include <type_traits>

namespace std {

template <class _Tp>
concept __fcxx_constexpr_wrapper_like =
    convertible_to<_Tp, decltype(_Tp::value)> && equality_comparable_with<_Tp, decltype(_Tp::value)>
    && bool_constant<_Tp() == _Tp::value>::value
    && bool_constant<static_cast<decltype(_Tp::value)>(_Tp()) == _Tp::value>::value;

template <class _Tp>
concept __fcxx_integral_constant_like = is_integral_v<remove_cvref_t<decltype(_Tp::value)> >
    && !is_same_v<bool, remove_cvref_t<decltype(_Tp::value)> > && __fcxx_constexpr_wrapper_like<_Tp>;

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
