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

/* The declarations of namespace std that every header needs, apart from the
 * macros of <__fcxx/config.h>, which includes this. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_STD_DECLS_H
#define __FREESTANDING_CXX_HDRS_FCXX_STD_DECLS_H 1

#pragma GCC system_header

_FCXX_BEGIN_EXTERN_CXX

namespace std {

/* They are long long on some targets, which C++98 does not have. */
__extension__ typedef __SIZE_TYPE__ size_t;
__extension__ typedef __PTRDIFF_TYPE__ ptrdiff_t;
#if _FCXX_STD >= 2011
typedef decltype(nullptr) nullptr_t;
#endif

/* Provided by the C++ runtime, or by the program. Declared here so that
 * the headers can call it without declaring all of <exception>. */
_FCXX_NORETURN void terminate() _FCXX_NOEXCEPT;

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
