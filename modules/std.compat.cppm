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


/* The named module std.compat ([std.modules]), from C++23 on: what the module
 * std exports, which it imports, and the names of the headers for C library
 * facilities in the global namespace, but for those [support.c.headers.other]
 * leaves out, and, from C++26 on, those of <stdbit.h>. Those that the C
 * headers do not declare in the global namespace are brought into it from
 * namespace std. Build it after std, with the same options. */

module;

#include <__fcxx/std_module.h>

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-module-identifier"
#endif

export module std.compat;

export import std;

export {
using ::int16_t;
using ::int32_t;
using ::int64_t;
using ::int8_t;
using ::int_fast16_t;
using ::int_fast32_t;
using ::int_fast64_t;
using ::int_fast8_t;
using ::int_least16_t;
using ::int_least32_t;
using ::int_least64_t;
using ::int_least8_t;
using ::intmax_t;
using ::intptr_t;
using ::max_align_t;
using ::nullptr_t;
using ::ptrdiff_t;
using ::size_t;
using ::uint16_t;
using ::uint32_t;
using ::uint64_t;
using ::uint8_t;
using ::uint_fast16_t;
using ::uint_fast32_t;
using ::uint_fast64_t;
using ::uint_fast8_t;
using ::uint_least16_t;
using ::uint_least32_t;
using ::uint_least64_t;
using ::uint_least8_t;
using ::uintmax_t;
using ::uintptr_t;
using ::va_list;
using std::abort;
using std::at_quick_exit;
using std::atexit;
using std::exit;
using std::quick_exit;
#if _FCXX_STD >= 2026
using std::abs;
using std::bsearch;
using std::div;
using std::div_t;
using std::labs;
using std::ldiv;
using std::ldiv_t;
using std::llabs;
using std::lldiv;
using std::lldiv_t;
using std::memalignment;
using std::memccpy;
using std::memchr;
using std::memcmp;
using std::memcpy;
using std::memmove;
using std::memset;
using std::memset_explicit;
using std::qsort;
using std::strcat;
using std::strchr;
using std::strcmp;
using std::strcpy;
using std::strcspn;
using std::strlen;
using std::strncat;
using std::strncmp;
using std::strncpy;
using std::strpbrk;
using std::strrchr;
using std::strspn;
using std::strstr;
using std::wcscat;
using std::wcschr;
using std::wcscmp;
using std::wcscpy;
using std::wcscspn;
using std::wcslen;
using std::wcsncat;
using std::wcsncmp;
using std::wcsncpy;
using std::wcspbrk;
using std::wcsrchr;
using std::wcsspn;
using std::wcsstr;
using std::wcstok;
using std::wint_t;
using std::wmemchr;
using std::wmemcmp;
using std::wmemcpy;
using std::wmemmove;
using std::wmemset;
#endif
#if _FCXX_STD >= 2026
using ::stdc_leading_zeros_uc;
using ::stdc_leading_zeros_us;
using ::stdc_leading_zeros_ui;
using ::stdc_leading_zeros_ul;
using ::stdc_leading_zeros_ull;
using ::stdc_leading_zeros;
using ::stdc_leading_ones_uc;
using ::stdc_leading_ones_us;
using ::stdc_leading_ones_ui;
using ::stdc_leading_ones_ul;
using ::stdc_leading_ones_ull;
using ::stdc_leading_ones;
using ::stdc_trailing_zeros_uc;
using ::stdc_trailing_zeros_us;
using ::stdc_trailing_zeros_ui;
using ::stdc_trailing_zeros_ul;
using ::stdc_trailing_zeros_ull;
using ::stdc_trailing_zeros;
using ::stdc_trailing_ones_uc;
using ::stdc_trailing_ones_us;
using ::stdc_trailing_ones_ui;
using ::stdc_trailing_ones_ul;
using ::stdc_trailing_ones_ull;
using ::stdc_trailing_ones;
using ::stdc_first_leading_zero_uc;
using ::stdc_first_leading_zero_us;
using ::stdc_first_leading_zero_ui;
using ::stdc_first_leading_zero_ul;
using ::stdc_first_leading_zero_ull;
using ::stdc_first_leading_zero;
using ::stdc_first_leading_one_uc;
using ::stdc_first_leading_one_us;
using ::stdc_first_leading_one_ui;
using ::stdc_first_leading_one_ul;
using ::stdc_first_leading_one_ull;
using ::stdc_first_leading_one;
using ::stdc_first_trailing_zero_uc;
using ::stdc_first_trailing_zero_us;
using ::stdc_first_trailing_zero_ui;
using ::stdc_first_trailing_zero_ul;
using ::stdc_first_trailing_zero_ull;
using ::stdc_first_trailing_zero;
using ::stdc_first_trailing_one_uc;
using ::stdc_first_trailing_one_us;
using ::stdc_first_trailing_one_ui;
using ::stdc_first_trailing_one_ul;
using ::stdc_first_trailing_one_ull;
using ::stdc_first_trailing_one;
using ::stdc_count_zeros_uc;
using ::stdc_count_zeros_us;
using ::stdc_count_zeros_ui;
using ::stdc_count_zeros_ul;
using ::stdc_count_zeros_ull;
using ::stdc_count_zeros;
using ::stdc_count_ones_uc;
using ::stdc_count_ones_us;
using ::stdc_count_ones_ui;
using ::stdc_count_ones_ul;
using ::stdc_count_ones_ull;
using ::stdc_count_ones;
using ::stdc_has_single_bit_uc;
using ::stdc_has_single_bit_us;
using ::stdc_has_single_bit_ui;
using ::stdc_has_single_bit_ul;
using ::stdc_has_single_bit_ull;
using ::stdc_has_single_bit;
using ::stdc_bit_width_uc;
using ::stdc_bit_width_us;
using ::stdc_bit_width_ui;
using ::stdc_bit_width_ul;
using ::stdc_bit_width_ull;
using ::stdc_bit_width;
using ::stdc_bit_floor_uc;
using ::stdc_bit_floor_us;
using ::stdc_bit_floor_ui;
using ::stdc_bit_floor_ul;
using ::stdc_bit_floor_ull;
using ::stdc_bit_floor;
using ::stdc_bit_ceil_uc;
using ::stdc_bit_ceil_us;
using ::stdc_bit_ceil_ui;
using ::stdc_bit_ceil_ul;
using ::stdc_bit_ceil_ull;
using ::stdc_bit_ceil;
#endif
}
