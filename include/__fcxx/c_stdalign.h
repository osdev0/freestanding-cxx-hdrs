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


/* What <stdalign.h>, that of freestanding-c-hdrs, has in C++, which it
 * includes this for at its end: alignas and alignof are keywords, as in C23,
 * whose header has nothing then, but C++ has __alignas_is_defined up to
 * C++23, and __alignof_is_defined from C++17 to C++23 (LWG 4036). */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_C_STDALIGN_H
#define __FREESTANDING_CXX_HDRS_FCXX_C_STDALIGN_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#if _FCXX_STD <= 2023
#  undef __alignas_is_defined
#  define __alignas_is_defined 1
#endif
#if _FCXX_STD >= 2017 && _FCXX_STD <= 2023
#  undef __alignof_is_defined
#  define __alignof_is_defined 1
#endif

_FCXX_END_EXTERN_CXX
#endif
