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

/* <charconv> */

#if !defined(__cpp_lib_constexpr_charconv) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_CHARCONV)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_CONSTEXPR20
#  define __cpp_lib_constexpr_charconv 202207L
#endif

#if !defined(__cpp_lib_freestanding_charconv) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_CHARCONV)) \
    && _FCXX_STD >= 2026
#  define __cpp_lib_freestanding_charconv 202306L
#endif
