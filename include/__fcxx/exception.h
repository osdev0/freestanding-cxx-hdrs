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


/* The base class of the standard exceptions, which several headers derive
 * their exceptions from without having to provide all of <exception>. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_EXCEPTION_H
#define __FREESTANDING_CXX_HDRS_FCXX_EXCEPTION_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

namespace std {

class exception {
public:
#if _FCXX_CONSTEXPR_EXCEPTION_CLASSES
    constexpr exception() noexcept {}
    constexpr exception(const exception&) noexcept = default;
    constexpr exception& operator=(const exception&) noexcept = default;
    constexpr virtual ~exception() {}
    constexpr virtual const char* what() const noexcept { return "std::exception"; }
#else
    exception() _FCXX_NOEXCEPT {}
#  if _FCXX_STD >= 2011
    exception(const exception&) noexcept = default;
    exception& operator=(const exception&) noexcept = default;
#  endif
    virtual ~exception() _FCXX_NOEXCEPT;
    virtual const char* what() const _FCXX_NOEXCEPT;
#endif
};

} /* namespace std */

#endif
