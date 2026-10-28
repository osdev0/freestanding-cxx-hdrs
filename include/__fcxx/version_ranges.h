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
 * of <__fcxx/version.h>, which includes this. <ranges> has its views where
 * the compiler has concepts and three-way comparisons, and those whose
 * elements are tuples of references where pair and tuple have what C++23
 * gives them for these. __cpp_lib_ranges itself is not freestanding, nor are
 * __cpp_lib_ranges_indices and __cpp_lib_ranges_reserve_hint. */

/* <ranges> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_ranges
#  define __cpp_lib_freestanding_ranges 202306L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_ranges_as_const
#  if _FCXX_STD >= 2026
#    define __cpp_lib_ranges_as_const 202311L
#  else
#    define __cpp_lib_ranges_as_const 202207L
#  endif
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_ranges_as_input
#  define __cpp_lib_ranges_as_input 202502L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_ranges_as_rvalue
#  define __cpp_lib_ranges_as_rvalue 202207L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_ranges_cache_latest
#  define __cpp_lib_ranges_cache_latest 202411L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023 && _FCXX_CONSTRAINED_TUPLES
#  undef __cpp_lib_ranges_cartesian_product
#  define __cpp_lib_ranges_cartesian_product 202207L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_ranges_chunk
#  define __cpp_lib_ranges_chunk 202202L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_ranges_chunk_by
#  define __cpp_lib_ranges_chunk_by 202202L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_ranges_concat
#  define __cpp_lib_ranges_concat 202403L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023 && _FCXX_CONSTRAINED_TUPLES
#  undef __cpp_lib_ranges_enumerate
#  define __cpp_lib_ranges_enumerate 202302L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_ranges_filter
#  define __cpp_lib_ranges_filter 202603L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_ranges_join_with
#  define __cpp_lib_ranges_join_with 202202L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_ranges_repeat
#  define __cpp_lib_ranges_repeat 202207L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_ranges_slide
#  define __cpp_lib_ranges_slide 202202L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_ranges_stride
#  define __cpp_lib_ranges_stride 202207L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_ranges_to_container
#  define __cpp_lib_ranges_to_container 202202L
#endif

/* <ranges>, <tuple> and <utility>, which give pair and tuple what zip_view
 * needs of them. */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_RANGES) || defined(_FCXX_VERSION_TUPLE) \
        || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2023 && _FCXX_CONSTRAINED_TUPLES
#  undef __cpp_lib_ranges_zip
#  define __cpp_lib_ranges_zip 202110L
#endif
