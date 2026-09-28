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


/* The feature-test macros of the headers of this area, in the form of those
 * of <__fcxx/version.h>, which includes this. */

/* <iterator> */

#if !defined(__cpp_lib_array_constexpr) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ITERATOR) \
    || defined(_FCXX_VERSION_ARRAY)) && _FCXX_STD >= 2023
#  define __cpp_lib_array_constexpr 201811L
#endif

#if !defined(__cpp_lib_constexpr_iterator) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ITERATOR)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_CONSTEXPR20
#  define __cpp_lib_constexpr_iterator 201811L
#endif

#if !defined(__cpp_lib_freestanding_iterator) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ITERATOR)) \
    && _FCXX_STD >= 2026
#  define __cpp_lib_freestanding_iterator 202306L
#endif

#if !defined(__cpp_lib_make_reverse_iterator) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ITERATOR)) \
    && _FCXX_STD >= 2023
#  define __cpp_lib_make_reverse_iterator 201402L
#endif

#if !defined(__cpp_lib_move_iterator_concept) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ITERATOR)) \
    && _FCXX_STD >= 2023 && _FCXX_CONCEPT_ITERATORS
#  define __cpp_lib_move_iterator_concept 202207L
#endif

#if !defined(__cpp_lib_nonmember_container_access) \
    && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ITERATOR) || defined(_FCXX_VERSION_ARRAY) \
        || defined(_FCXX_VERSION_STRING)) \
    && _FCXX_STD >= 2023
#  define __cpp_lib_nonmember_container_access 201411L
#endif

#if !defined(__cpp_lib_null_iterators) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ITERATOR)) \
    && _FCXX_STD >= 2023
#  define __cpp_lib_null_iterators 201304L
#endif

#if !defined(__cpp_lib_ssize) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ITERATOR)) \
    && _FCXX_STD >= 2023
#  define __cpp_lib_ssize 201902L
#endif
