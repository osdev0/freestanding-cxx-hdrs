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


/* What <stdarg.h>, that of freestanding-c-hdrs, has differently in C++,
 * which it includes this for at its end.
 *
 * Workaround: Clang 17 up to at least 23, all targets: from C++26 on, where
 * va_start takes the list alone, as in C23, it has no builtin of C23 for
 * C++, and its -Wvarargs warns, in the code of the program, that the 0
 * passed in its stead is not the last named parameter. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_C_STDARG_H
#define __FREESTANDING_CXX_HDRS_FCXX_C_STDARG_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#if _FCXX_STD >= 2026 && _FCXX_CLANG && !_FCXX_HAS_BUILTIN(__builtin_c23_va_start)
#  undef va_start
#  define va_start(v, ...)                              \
      _Pragma("clang diagnostic push")                  \
      _Pragma("clang diagnostic ignored \"-Wvarargs\"") \
      __builtin_va_start(v, 0)                          \
      _Pragma("clang diagnostic pop")
#endif

_FCXX_END_EXTERN_CXX
#endif
