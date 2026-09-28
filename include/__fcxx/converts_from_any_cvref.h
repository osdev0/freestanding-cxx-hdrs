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


/* converts-from-any-cvref, with which optional and expected keep their
 * converting constructors and assignment operators from competing with
 * those of the types they hold. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_CONVERTS_FROM_ANY_CVREF_H
#define __FREESTANDING_CXX_HDRS_FCXX_CONVERTS_FROM_ANY_CVREF_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <type_traits>

namespace std {

template <class _Tp, class _Wp>
struct __fcxx_converts_from_any_cvref
    : __fcxx_or<is_constructible<_Tp, _Wp&>, is_convertible<_Wp&, _Tp>, is_constructible<_Tp, _Wp>,
                is_convertible<_Wp, _Tp>, is_constructible<_Tp, const _Wp&>, is_convertible<const _Wp&, _Tp>,
                is_constructible<_Tp, const _Wp>, is_convertible<const _Wp, _Tp> > {};

} /* namespace std */

#endif
