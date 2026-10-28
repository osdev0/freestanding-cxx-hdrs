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

/* The limits of wchar_t, which <cstdint> and <cwchar> both define. They
 * have the type wchar_t is promoted to, which in C++ is int, or unsigned
 * int, where wchar_t is as wide as int, whatever the type it has in C. The
 * C headers give them the type of wchar_t in C, which is long on some of
 * the 32-bit targets, such as i686 and 32-bit PowerPC, so they are defined
 * again here. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_WCHAR_LIMITS_H
#define __FREESTANDING_CXX_HDRS_FCXX_WCHAR_LIMITS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#if __SIZEOF_WCHAR_T__ == __SIZEOF_INT__
#  undef WCHAR_MIN
#  undef WCHAR_MAX
#  ifdef __WCHAR_UNSIGNED__
#    define WCHAR_MIN 0U
#    define WCHAR_MAX (__INT_MAX__ * 2U + 1U)
#  else
#    define WCHAR_MIN (-__INT_MAX__ - 1)
#    define WCHAR_MAX __INT_MAX__
#  endif
#else
#  undef WCHAR_MAX
#  define WCHAR_MAX __WCHAR_MAX__
/* Workaround: Clang 6 to 22, all targets: it does not tell the minimum of
 * wchar_t, only whether it is unsigned, which GCC does not tell, and that
 * of an unsigned type is 0 of its promoted type. */
#  undef WCHAR_MIN
#  if defined(__WCHAR_MIN__)
#    define WCHAR_MIN __WCHAR_MIN__
#  elif defined(__WCHAR_UNSIGNED__)
#    define WCHAR_MIN (WCHAR_MAX - WCHAR_MAX)
#  else
#    define WCHAR_MIN (-WCHAR_MAX - 1)
#  endif
#endif

_FCXX_END_EXTERN_CXX
#endif
