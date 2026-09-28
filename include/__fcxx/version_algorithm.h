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

/* <algorithm> */

#if !defined(__cpp_lib_robust_nonmodifying_seq_ops) \
    && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ALGORITHM)) && _FCXX_STD >= 2026
#  define __cpp_lib_robust_nonmodifying_seq_ops 201304L
#endif

#if !defined(__cpp_lib_clamp) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ALGORITHM)) \
    && _FCXX_STD >= 2026
#  define __cpp_lib_clamp 201603L
#endif

#if !defined(__cpp_lib_sample) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ALGORITHM)) \
    && _FCXX_STD >= 2026
#  define __cpp_lib_sample 201603L
#endif

#if !defined(__cpp_lib_ranges_starts_ends_with) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ALGORITHM)) \
    && _FCXX_STD >= 2026 && _FCXX_CONCEPT_ITERATORS
#  define __cpp_lib_ranges_starts_ends_with 202106L
#endif

#if !defined(__cpp_lib_ranges_contains) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ALGORITHM)) \
    && _FCXX_STD >= 2026 && _FCXX_CONCEPT_ITERATORS
#  define __cpp_lib_ranges_contains 202207L
#endif

#if !defined(__cpp_lib_ranges_find_last) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ALGORITHM)) \
    && _FCXX_STD >= 2026 && _FCXX_CONCEPT_ITERATORS
#  define __cpp_lib_ranges_find_last 202207L
#endif

#if !defined(__cpp_lib_ranges_fold) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ALGORITHM)) \
    && _FCXX_STD >= 2023 && _FCXX_CONCEPT_ITERATORS && _FCXX_STD >= 2026
#  define __cpp_lib_ranges_fold 202207L
#endif

#if !defined(__cpp_lib_freestanding_algorithm) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ALGORITHM)) \
    && _FCXX_STD >= 2026 && _FCXX_CONCEPT_ITERATORS
#  define __cpp_lib_freestanding_algorithm 202502L
#endif

/* <numeric> */

#if !defined(__cpp_lib_gcd_lcm) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NUMERIC)) \
    && _FCXX_STD >= 2026
#  define __cpp_lib_gcd_lcm 201606L
#endif

#if !defined(__cpp_lib_constexpr_numeric) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NUMERIC)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_CONSTEXPR20
#  define __cpp_lib_constexpr_numeric 201911L
#endif

#if !defined(__cpp_lib_ranges_iota) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NUMERIC)) \
    && _FCXX_STD >= 2026 && _FCXX_CONCEPT_ITERATORS
#  define __cpp_lib_ranges_iota 202202L
#endif

#if !defined(__cpp_lib_saturation_arithmetic) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NUMERIC)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_CONCEPTS
#  define __cpp_lib_saturation_arithmetic 202603L
#endif

#if !defined(__cpp_lib_freestanding_numeric) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NUMERIC)) \
    && _FCXX_STD >= 2026 && _FCXX_CONCEPT_ITERATORS
#  define __cpp_lib_freestanding_numeric 202502L
#endif

/* <execution> */

#if !defined(__cpp_lib_freestanding_execution) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_EXECUTION)) \
    && _FCXX_STD >= 2026
#  define __cpp_lib_freestanding_execution 202502L
#endif
