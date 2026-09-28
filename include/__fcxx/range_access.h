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


/* The range access functions of <iterator>, std::begin, std::end and the
 * others, which the headers of the containers and views provide too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGE_ACCESS_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGE_ACCESS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <initializer_list>
#include <type_traits>
#include <__fcxx/reverse_iterator.h>

namespace std {

/* [iterator.range], range access. The functions are noexcept when what they
 * call is, which C++26 has, and which the functions of the earlier
 * revisions can have too. */
template <class _Cp>
constexpr auto begin(_Cp& __c) noexcept(noexcept(__c.begin())) -> decltype(__c.begin()) {
    return __c.begin();
}

template <class _Cp>
constexpr auto begin(const _Cp& __c) noexcept(noexcept(__c.begin())) -> decltype(__c.begin()) {
    return __c.begin();
}

template <class _Cp>
constexpr auto end(_Cp& __c) noexcept(noexcept(__c.end())) -> decltype(__c.end()) {
    return __c.end();
}

template <class _Cp>
constexpr auto end(const _Cp& __c) noexcept(noexcept(__c.end())) -> decltype(__c.end()) {
    return __c.end();
}

template <class _Tp, size_t _Np>
constexpr _Tp* begin(_Tp (&__array)[_Np]) noexcept {
    return __array;
}

template <class _Tp, size_t _Np>
constexpr _Tp* end(_Tp (&__array)[_Np]) noexcept {
    return __array + _Np;
}

template <class _Cp>
constexpr auto cbegin(const _Cp& __c) noexcept(noexcept(std::begin(__c))) -> decltype(std::begin(__c)) {
    return std::begin(__c);
}

template <class _Cp>
constexpr auto cend(const _Cp& __c) noexcept(noexcept(std::end(__c))) -> decltype(std::end(__c)) {
    return std::end(__c);
}

template <class _Cp>
constexpr auto rbegin(_Cp& __c) noexcept(noexcept(__c.rbegin())) -> decltype(__c.rbegin()) {
    return __c.rbegin();
}

template <class _Cp>
constexpr auto rbegin(const _Cp& __c) noexcept(noexcept(__c.rbegin())) -> decltype(__c.rbegin()) {
    return __c.rbegin();
}

template <class _Cp>
constexpr auto rend(_Cp& __c) noexcept(noexcept(__c.rend())) -> decltype(__c.rend()) {
    return __c.rend();
}

template <class _Cp>
constexpr auto rend(const _Cp& __c) noexcept(noexcept(__c.rend())) -> decltype(__c.rend()) {
    return __c.rend();
}

template <class _Tp, size_t _Np>
constexpr reverse_iterator<_Tp*> rbegin(_Tp (&__array)[_Np]) noexcept {
    return reverse_iterator<_Tp*>(__array + _Np);
}

template <class _Tp, size_t _Np>
constexpr reverse_iterator<_Tp*> rend(_Tp (&__array)[_Np]) noexcept {
    return reverse_iterator<_Tp*>(__array);
}

template <class _Ep>
constexpr reverse_iterator<const _Ep*> rbegin(initializer_list<_Ep> __il) noexcept {
    return reverse_iterator<const _Ep*>(__il.end());
}

template <class _Ep>
constexpr reverse_iterator<const _Ep*> rend(initializer_list<_Ep> __il) noexcept {
    return reverse_iterator<const _Ep*>(__il.begin());
}

template <class _Cp>
constexpr auto crbegin(const _Cp& __c) noexcept(noexcept(std::rbegin(__c))) -> decltype(std::rbegin(__c)) {
    return std::rbegin(__c);
}

template <class _Cp>
constexpr auto crend(const _Cp& __c) noexcept(noexcept(std::rend(__c))) -> decltype(std::rend(__c)) {
    return std::rend(__c);
}

template <class _Cp>
constexpr auto size(const _Cp& __c) noexcept(noexcept(__c.size())) -> decltype(__c.size()) {
    return __c.size();
}

template <class _Tp, size_t _Np>
constexpr size_t size(const _Tp (&)[_Np]) noexcept {
    return _Np;
}

template <class _Cp>
constexpr auto ssize(const _Cp& __c) noexcept(noexcept(__c.size()))
    -> common_type_t<ptrdiff_t, make_signed_t<decltype(__c.size())> > {
    return static_cast<common_type_t<ptrdiff_t, make_signed_t<decltype(__c.size())> > >(__c.size());
}

template <class _Tp, ptrdiff_t _Np>
constexpr ptrdiff_t ssize(const _Tp (&)[_Np]) noexcept {
    return _Np;
}

template <class _Cp>
[[nodiscard]] constexpr auto empty(const _Cp& __c) noexcept(noexcept(__c.empty())) -> decltype(__c.empty()) {
    return __c.empty();
}

template <class _Tp, size_t _Np>
[[nodiscard]] constexpr bool empty(const _Tp (&)[_Np]) noexcept {
    return false;
}

template <class _Cp>
constexpr auto data(_Cp& __c) noexcept(noexcept(__c.data())) -> decltype(__c.data()) {
    return __c.data();
}

template <class _Cp>
constexpr auto data(const _Cp& __c) noexcept(noexcept(__c.data())) -> decltype(__c.data()) {
    return __c.data();
}

template <class _Tp, size_t _Np>
constexpr _Tp* data(_Tp (&__array)[_Np]) noexcept {
    return __array;
}

/* initializer_list has had members for these since C++26. */
#  if _FCXX_STD < 2026
template <class _Ep>
[[nodiscard]] constexpr bool empty(initializer_list<_Ep> __il) noexcept {
    return __il.size() == 0;
}

template <class _Ep>
constexpr const _Ep* data(initializer_list<_Ep> __il) noexcept {
    return __il.begin();
}
#  endif

} /* namespace std */

#endif
