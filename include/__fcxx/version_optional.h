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

/* <optional>, which the C++1z modes without if constexpr lack, and whose
 * specialization for references needs concepts and conditional explicit,
 * and its range support, concepts. */

#if !defined(__cpp_lib_freestanding_optional) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_OPTIONAL)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_IF_CONSTEXPR && _FCXX_HAS_CONCEPTS && _FCXX_HAS_CONDITIONAL_EXPLICIT
#  define __cpp_lib_freestanding_optional 202506L
#endif

#if !defined(__cpp_lib_optional_range_support) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_OPTIONAL)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_IF_CONSTEXPR && _FCXX_HAS_CONCEPTS
#  define __cpp_lib_optional_range_support 202406L
#endif

/* <variant> */

#if !defined(__cpp_lib_freestanding_variant) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_VARIANT)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_IF_CONSTEXPR && _FCXX_HAS_FOLD_EXPRESSIONS
#  define __cpp_lib_freestanding_variant 202311L
#endif

/* <expected> */

#if !defined(__cpp_lib_freestanding_expected) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_EXPECTED)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_CONCEPTS && _FCXX_HAS_CONDITIONAL_EXPLICIT
#  define __cpp_lib_freestanding_expected 202311L
#endif
