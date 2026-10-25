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


/* What the C headers, those of freestanding-c-hdrs, have in C++, which they
 * include this for (through their __freestanding_c_hdrs/revision.h): the
 * revision of C whose contents each of them has, in the revision of C++ in
 * use, as __FREESTANDING_C_HDRS_<NAME>_REVISION, which is 0 where the header
 * is not one of C++. C++ takes the headers of C90 with its first amendment
 * (C95) in C++98, of C99 in C++11 and C++14, of C11 in C++17, of C17 in
 * C++20 and C++23, and of C23 in C++26, but for what it changes:
 * <stddef.h> has max_align_t, of C11, from C++11 on, <stdalign.h> and
 * <stdbool.h> have no macros for their names, which are keywords, as in C23,
 * and there is no <stdnoreturn.h>. What else C++ has differently is in
 * c_<name>.h, which the headers include in C++ too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_C_REVISION_H
#define __FREESTANDING_CXX_HDRS_FCXX_C_REVISION_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#if _FCXX_STD >= 2026
#  define __FREESTANDING_C_HDRS_C 202311L
#elif _FCXX_STD >= 2020
#  define __FREESTANDING_C_HDRS_C 201710L
#elif _FCXX_STD >= 2017
#  define __FREESTANDING_C_HDRS_C 201112L
#elif _FCXX_STD >= 2011
#  define __FREESTANDING_C_HDRS_C 199901L
#else
#  define __FREESTANDING_C_HDRS_C 199409L
#endif

#define __FREESTANDING_C_HDRS_FLOAT_REVISION __FREESTANDING_C_HDRS_C
#define __FREESTANDING_C_HDRS_ISO646_REVISION __FREESTANDING_C_HDRS_C
#define __FREESTANDING_C_HDRS_LIMITS_REVISION __FREESTANDING_C_HDRS_C
#define __FREESTANDING_C_HDRS_STDARG_REVISION __FREESTANDING_C_HDRS_C
#define __FREESTANDING_C_HDRS_STDNORETURN_REVISION 0

#if _FCXX_STD >= 2017
#  define __FREESTANDING_C_HDRS_STDDEF_REVISION __FREESTANDING_C_HDRS_C
#elif _FCXX_STD >= 2011
#  define __FREESTANDING_C_HDRS_STDDEF_REVISION 201112L
#else
#  define __FREESTANDING_C_HDRS_STDDEF_REVISION __FREESTANDING_C_HDRS_C
#endif

#if _FCXX_STD >= 2011
#  define __FREESTANDING_C_HDRS_STDINT_REVISION __FREESTANDING_C_HDRS_C
#  define __FREESTANDING_C_HDRS_STDALIGN_REVISION 202311L
#  define __FREESTANDING_C_HDRS_STDBOOL_REVISION 202311L
#else
#  define __FREESTANDING_C_HDRS_STDINT_REVISION 0
#  define __FREESTANDING_C_HDRS_STDALIGN_REVISION 0
#  define __FREESTANDING_C_HDRS_STDBOOL_REVISION 0
#endif

#if _FCXX_STD >= 2023
#  define __FREESTANDING_C_HDRS_STDATOMIC_REVISION __FREESTANDING_C_HDRS_C
#else
#  define __FREESTANDING_C_HDRS_STDATOMIC_REVISION 0
#endif

#if _FCXX_STD >= 2026
#  define __FREESTANDING_C_HDRS_STDBIT_REVISION __FREESTANDING_C_HDRS_C
#  define __FREESTANDING_C_HDRS_STDCKDINT_REVISION __FREESTANDING_C_HDRS_C
#else
#  define __FREESTANDING_C_HDRS_STDBIT_REVISION 0
#  define __FREESTANDING_C_HDRS_STDCKDINT_REVISION 0
#endif

#endif
