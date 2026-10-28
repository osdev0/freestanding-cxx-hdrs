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


/* Compile-time integer sequences, of <utility>, which other headers also
 * need. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_INTEGER_SEQUENCE_H
#define __FREESTANDING_CXX_HDRS_FCXX_INTEGER_SEQUENCE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

namespace std {

template <class _Tp, _Tp... _Ip>
struct integer_sequence {
    typedef _Tp value_type;
    static constexpr size_t size() noexcept { return sizeof...(_Ip); }
};

template <size_t... _Ip>
using index_sequence = integer_sequence<size_t, _Ip...>;

/* Both compilers make the sequences themselves, with different builtins,
 * which are used in a class template.
 * Workaround: GCC 11.1 and 11.2, all targets: they do not expand
 * __integer_pack in an alias template where the alias is used in the
 * expansion of another pack ("incomplete type ... used in nested name
 * specifier"). */
#if _FCXX_HAS_BUILTIN(__make_integer_seq)
template <class _Tp, _Tp _Np>
struct __fcxx_make_integer_sequence {
    typedef __make_integer_seq<integer_sequence, _Tp, _Np> type;
};
#else
template <class _Tp, _Tp _Np>
struct __fcxx_make_integer_sequence {
    typedef integer_sequence<_Tp, __integer_pack(_Np)...> type;
};
#endif

template <class _Tp, _Tp _Np>
using make_integer_sequence = typename __fcxx_make_integer_sequence<_Tp, _Np>::type;

template <size_t _Np>
using make_index_sequence = make_integer_sequence<size_t, _Np>;

template <class... _Tp>
using index_sequence_for = make_index_sequence<sizeof...(_Tp)>;

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
