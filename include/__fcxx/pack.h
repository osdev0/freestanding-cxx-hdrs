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


/* Things done with packs of types, which tuple and variant need. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_PACK_H
#define __FREESTANDING_CXX_HDRS_FCXX_PACK_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <type_traits>
#include <__fcxx/integer_sequence.h>

namespace std {

/* The type at an index of a pack. */
#if _FCXX_HAS_PACK_INDEXING
template <size_t _Ip, class... _Types>
using __fcxx_type_at_t = _Types...[_Ip];
#elif _FCXX_HAS_BUILTIN(__type_pack_element)
template <size_t _Ip, class... _Types>
using __fcxx_type_at_t = __type_pack_element<_Ip, _Types...>;
#else
template <size_t _Ip, class _Tp>
struct __fcxx_indexed_type {
    typedef _Tp type;
};

template <class _Seq, class... _Types>
struct __fcxx_type_indexer;

template <size_t... _Is, class... _Types>
struct __fcxx_type_indexer<index_sequence<_Is...>, _Types...> : __fcxx_indexed_type<_Is, _Types>... {};

template <size_t _Ip, class _Tp>
__fcxx_indexed_type<_Ip, _Tp> __fcxx_select_indexed(const __fcxx_indexed_type<_Ip, _Tp>&);

template <size_t _Ip, class... _Types>
using __fcxx_type_at_t = typename decltype(std::__fcxx_select_indexed<_Ip>(
    __fcxx_type_indexer<index_sequence_for<_Types...>, _Types...>()))::type;
#endif

/* The index of a type in a pack, and how often it occurs there. */
template <size_t _Ip, class _Tp, class... _Types>
struct __fcxx_find_type {
    static constexpr size_t __count = 0;
    static constexpr size_t __index = 0;
};

template <size_t _Ip, class _Tp, class _Up, class... _Rest>
struct __fcxx_find_type<_Ip, _Tp, _Up, _Rest...> {
    typedef __fcxx_find_type<_Ip + 1, _Tp, _Rest...> _Next;
    static constexpr size_t __count = _Next::__count + (is_same<_Tp, _Up>::value ? 1 : 0);
    static constexpr size_t __index = is_same<_Tp, _Up>::value ? _Ip : _Next::__index;
};

/* Lists of types, and whether a predicate holds for each pair of types of
 * two lists of the same length. */
template <class... _Types>
struct __fcxx_type_list {};

template <class _List>
struct __fcxx_list_size;

template <class... _Types>
struct __fcxx_list_size<__fcxx_type_list<_Types...> > : integral_constant<size_t, sizeof...(_Types)> {};

template <class _TList, class _UList>
struct __fcxx_same_size : __fcxx_bool_constant<__fcxx_list_size<_TList>::value == __fcxx_list_size<_UList>::value> {};

template <template <class, class> class _Pred, class _TList, class _UList,
          bool = __fcxx_same_size<_TList, _UList>::value>
struct __fcxx_zip_all : false_type {};

template <template <class, class> class _Pred, class... _Ts, class... _Us>
struct __fcxx_zip_all<_Pred, __fcxx_type_list<_Ts...>, __fcxx_type_list<_Us...>, true>
    : __fcxx_and<_Pred<_Ts, _Us>...> {};

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
