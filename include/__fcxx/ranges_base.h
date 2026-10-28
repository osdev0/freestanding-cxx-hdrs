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


/* What of <ranges> the headers of the types that are views or borrowed
 * ranges need, to specialize enable_view and enable_borrowed_range. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_BASE_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_BASE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <concepts>
#include <type_traits>

namespace std {
namespace ranges {

/* [range.view], views */
struct view_base {};

/* Defined by <ranges>, with this same template head. */
template <class _Dp>
    requires is_class_v<_Dp> && same_as<_Dp, remove_cv_t<_Dp> >
class view_interface;

template <class _Up>
void __fcxx_view_interface_test(const view_interface<_Up>&);

/* Whether a type has exactly one base class view_interface<U>, which is
 * public, and no other view_interface base classes. */
template <class _Tp>
concept __fcxx_derived_from_view_interface = requires(const _Tp& __t) {
    ranges::__fcxx_view_interface_test(__t);
};

template <class _Tp>
inline constexpr bool enable_view = derived_from<_Tp, view_base> || __fcxx_derived_from_view_interface<_Tp>;

/* [range.range], borrowed ranges */
template <class _Tp>
inline constexpr bool enable_borrowed_range = false;

} /* namespace ranges */
} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
