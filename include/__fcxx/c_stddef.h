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


/* What <stddef.h>, that of freestanding-c-hdrs, has differently in C++,
 * which it includes this for at its end: NULL is __null, which converts to
 * the pointers and the pointers to members, and nullptr_t is from C++11 on
 * (C23 has its own, of the C part of the header). */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_C_STDDEF_H
#define __FREESTANDING_CXX_HDRS_FCXX_C_STDDEF_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#undef NULL
#define NULL __null

#if _FCXX_STD >= 2011
typedef decltype(nullptr) nullptr_t;
#endif

#endif
