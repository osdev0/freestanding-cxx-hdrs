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


/* The absolute value functions of both <cstdlib> and <cmath>, which are
 * freestanding from C++26 on. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ABS_H
#define __FREESTANDING_CXX_HDRS_FCXX_ABS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

namespace std {

constexpr int abs(int __j) { return __j < 0 ? -__j : __j; }
constexpr long abs(long __j) { return __j < 0 ? -__j : __j; }
constexpr long long abs(long long __j) { return __j < 0 ? -__j : __j; }

/* Those for the floating-point types are freestanding deleted functions,
 * which are deleted. */
float abs(float) = delete;
double abs(double) = delete;
long double abs(long double) = delete;
#ifdef __STDCPP_FLOAT16_T__
_Float16 abs(_Float16) = delete;
#endif
#ifdef __STDCPP_FLOAT32_T__
_Float32 abs(_Float32) = delete;
#endif
#ifdef __STDCPP_FLOAT64_T__
_Float64 abs(_Float64) = delete;
#endif
#ifdef __STDCPP_FLOAT128_T__
_Float128 abs(_Float128) = delete;
#endif
#ifdef __STDCPP_BFLOAT16_T__
__bf16 abs(__bf16) = delete;
#endif

} /* namespace std */

#endif
