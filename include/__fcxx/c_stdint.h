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


/* What <stdint.h>, that of freestanding-c-hdrs, has differently in C++,
 * which it includes this for at its end: wchar_t is a type of its own, which
 * is promoted to int or unsigned int where it is as wide as them, so that
 * WCHAR_MIN and WCHAR_MAX are of that type there, while in C they are of
 * the type that wchar_t is a name of, which GCC makes long on i386 and 32-bit
 * PowerPC. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_C_STDINT_H
#define __FREESTANDING_CXX_HDRS_FCXX_C_STDINT_H 1

#pragma GCC system_header

#if __SIZEOF_WCHAR_T__ == __SIZEOF_INT__
#  undef WCHAR_MAX
#  undef WCHAR_MIN
#  ifdef __WCHAR_UNSIGNED__
#    define WCHAR_MAX (__INT_MAX__ * 2U + 1U)
#    define WCHAR_MIN 0U
#  else
#    define WCHAR_MAX __INT_MAX__
#    define WCHAR_MIN (-__INT_MAX__ - 1)
#  endif
#endif

#endif
