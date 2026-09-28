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


/* The allocator argument tag and uses_allocator, of <memory>, which <tuple>
 * needs too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_USES_ALLOCATOR_H
#define __FREESTANDING_CXX_HDRS_FCXX_USES_ALLOCATOR_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <type_traits>

namespace std {

/* [allocator.tag], allocator argument tag */

struct allocator_arg_t {
    explicit allocator_arg_t() = default;
};

inline constexpr allocator_arg_t allocator_arg = allocator_arg_t();

/* [allocator.uses.trait], uses_allocator */

template <class _Tp, class _Alloc, class = void>
struct __fcxx_uses_allocator_impl : false_type {};

template <class _Tp, class _Alloc>
struct __fcxx_uses_allocator_impl<_Tp, _Alloc, __fcxx_void_t<typename _Tp::allocator_type> >
    : is_convertible<_Alloc, typename _Tp::allocator_type> {};

template <class _Tp, class _Alloc>
struct uses_allocator : __fcxx_uses_allocator_impl<_Tp, _Alloc> {};

template <class _Tp, class _Alloc>
inline constexpr bool uses_allocator_v = uses_allocator<_Tp, _Alloc>::value;

/* Defined by <__fcxx/uses_allocator_args.h>, which needs tuple. */
template <class _Tp, class _Alloc, class... _Args>
constexpr _Tp make_obj_using_allocator(const _Alloc& __alloc, _Args&&... __args);

} /* namespace std */

#endif
