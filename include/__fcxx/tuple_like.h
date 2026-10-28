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


/* The tuple protocol, of <tuple>, which <utility>, <array>, <ranges> and
 * <span> also provide, and what pair and tuple share. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_TUPLE_LIKE_H
#define __FREESTANDING_CXX_HDRS_FCXX_TUPLE_LIKE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <type_traits>

/* The constructors of pair and tuple that would bind a reference member to
 * a temporary are deleted from C++23 on, where the compiler can tell. */
#if _FCXX_HAS_BUILTIN(__reference_constructs_from_temporary)
#  define _FCXX_HAS_DANGLING_CHECK 1
#else
#  define _FCXX_HAS_DANGLING_CHECK 0
#endif

namespace std {

template <class _Tp>
struct tuple_size;

template <size_t _Ip, class _Tp>
struct tuple_element;

/* The size of a const tuple-like type, which is only there when that of
 * the type is. */
template <class _Tp, class = void>
struct __fcxx_tuple_size_const {};

template <class _Tp>
struct __fcxx_tuple_size_const<_Tp, __fcxx_void_t<decltype(tuple_size<_Tp>::value)> >
    : integral_constant<size_t, tuple_size<_Tp>::value> {};

template <class _Tp>
struct tuple_size<const _Tp> : __fcxx_tuple_size_const<_Tp> {};

template <size_t _Ip, class _Tp>
struct tuple_element<_Ip, const _Tp> {
    typedef typename add_const<typename tuple_element<_Ip, _Tp>::type>::type type;
};

template <size_t _Ip, class _Tp>
using tuple_element_t = typename tuple_element<_Ip, _Tp>::type;

template <class _Tp>
inline constexpr size_t tuple_size_v = tuple_size<_Tp>::value;

/* The specializations of ranges::subrange, which <ranges> registers. */
template <class _Tp>
struct __fcxx_is_subrange : false_type {};

/* Whether a type is copy-list-initializable from empty braces, which
 * decides whether the default constructors of pair and tuple are explicit:
 * whether a parameter of the type can be, which an argument of empty braces
 * copy-list-initializes, without the cv-qualifiers of the type, which do not
 * change that, and which C++20 deprecates for parameters. */
template <class _Tp>
void __fcxx_implicit_default_test(_Tp);

template <class _Tp, class = void>
struct __fcxx_is_implicitly_default_constructible : false_type {};

template <class _Tp>
struct __fcxx_is_implicitly_default_constructible<
    _Tp, decltype(std::__fcxx_implicit_default_test<typename remove_cv<_Tp>::type>({}))> : true_type {};

/* The parameter type of the assignment operators of pair and tuple that
 * the element types do not allow, so that the operators are not copy or
 * move assignment operators then. Nothing converts to it. */
struct __fcxx_nonesuch {
private:
    __fcxx_nonesuch(const __fcxx_nonesuch&);
};

/* Unwrapping a reference_wrapper, for make_pair and make_tuple. */
template <class _Tp>
struct __fcxx_unwrap_ref {
    typedef _Tp type;
};

template <class _Tp>
struct __fcxx_unwrap_ref<reference_wrapper<_Tp> > {
    typedef _Tp& type;
};

template <class _Tp>
using __fcxx_unwrap_ref_decay_t = typename __fcxx_unwrap_ref<__fcxx_decay_t<_Tp> >::type;

/* The exposition-only concepts tuple-like and pair-like, and what get
 * returns for an element of a tuple-like type. */
template <class _Tp>
concept __fcxx_tuple_like = __fcxx_is_tuple_like<_Tp>::value;

template <class _Tp>
concept __fcxx_pair_like = __fcxx_tuple_like<_Tp> && tuple_size<__fcxx_remove_cvref_t<_Tp> >::value == 2;

template <size_t _Ip, class _Tp>
using __fcxx_get_t = decltype(get<_Ip>(std::declval<_Tp>()));

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
