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


/* addressof, of <memory>, which many other headers need, under a name of
 * the library's own, as <memory> is only freestanding from C++23 on, where
 * the name of the standard is declared along with it. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ADDRESSOF_H
#define __FREESTANDING_CXX_HDRS_FCXX_ADDRESSOF_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

namespace std {

/* It can only be constexpr with the builtin, which also sees through any
 * operator& of the type, for functions as well as for objects. */
#if _FCXX_HAS_BUILTIN_ADDRESSOF
template <class _Tp>
_FCXX_NODISCARD _FCXX_CONSTEXPR17 _Tp* __fcxx_addressof(_Tp& __r) noexcept {
    return __builtin_addressof(__r);
}
#else
template <class _Tp>
inline _Tp* __fcxx_addressof(_Tp& __r) noexcept {
    return reinterpret_cast<_Tp*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(__r)));
}

template <class _Rp, class... _Args>
inline _Rp (*__fcxx_addressof(_Rp (&__f)(_Args...)) noexcept)(_Args...) {
    return &__f;
}

template <class _Rp, class... _Args>
inline _Rp (*__fcxx_addressof(_Rp (&__f)(_Args..., ...)) noexcept)(_Args..., ...) {
    return &__f;
}
#endif

/* [specialized.addressof], which every compiler with a mode for C++23 has
 * the builtin for. */
#if _FCXX_STD >= 2023
template <class _Tp>
_FCXX_NODISCARD constexpr _Tp* addressof(_Tp& __r) noexcept {
    return __builtin_addressof(__r);
}

template <class _Tp>
const _Tp* addressof(const _Tp&&) = delete;
#endif

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
