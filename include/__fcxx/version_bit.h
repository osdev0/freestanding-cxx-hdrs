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

/* <bit> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_BIT)) \
    && _FCXX_STD >= 2020 && _FCXX_HAS_BUILTIN_BIT_CAST
#  undef __cpp_lib_bit_cast
#  define __cpp_lib_bit_cast 201806L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_BIT)) \
    && _FCXX_STD >= 2020
#  undef __cpp_lib_bitops
#  define __cpp_lib_bitops 201907L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_BIT)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_byteswap
#  define __cpp_lib_byteswap 202110L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_BIT)) \
    && _FCXX_STD >= 2020
#  undef __cpp_lib_endian
#  define __cpp_lib_endian 201907L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_BIT)) \
    && _FCXX_STD >= 2020
#  undef __cpp_lib_int_pow2
#  define __cpp_lib_int_pow2 202002L
#endif

/* [version.syn] does not mark __cpp_lib_stdbit_h as freestanding, but all
 * of <stdbit.h> is freestanding, so the macro is defined where the header,
 * that of the C headers, whose part of C++ (c_stdbit.h) defines it too, is
 * there. */
#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_STDBIT_H)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_stdbit_h
#  define __cpp_lib_stdbit_h 202603L
#endif
