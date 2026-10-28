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


/* What <stdbit.h>, that of freestanding-c-hdrs, has in C++, from C++26 on,
 * which it includes this for: its functions, and the function templates of
 * its type-generic functions, made of the operations of <bit>. None of them
 * is constexpr, as in C. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_C_STDBIT_H
#define __FREESTANDING_CXX_HDRS_FCXX_C_STDBIT_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#define _FCXX_VERSION_STDBIT_H
#include <__fcxx/version.h>
#undef _FCXX_VERSION_STDBIT_H

#if _FCXX_STD >= 2026

#include <__fcxx/bit.h>

namespace std {

/* The operations, for x of an unsigned integer type: the first bits are at
 * their index plus 1, from the side they are counted from, or at 0 where
 * there is none, and the ceiling is 0 where it cannot be represented. */
template <class _Tp>
inline unsigned int __fcxx_stdc_leading_zeros(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_countl_zero(__x));
}

template <class _Tp>
inline unsigned int __fcxx_stdc_leading_ones(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_countl_one(__x));
}

template <class _Tp>
inline unsigned int __fcxx_stdc_trailing_zeros(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_countr_zero(__x));
}

template <class _Tp>
inline unsigned int __fcxx_stdc_trailing_ones(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_countr_one(__x));
}

template <class _Tp>
inline unsigned int __fcxx_stdc_first_leading_zero(_Tp __x) noexcept {
    const int __n = std::__fcxx_countl_one(__x);
    return __n == __fcxx_bit_digits<_Tp>::value ? 0u : static_cast<unsigned int>(__n + 1);
}

template <class _Tp>
inline unsigned int __fcxx_stdc_first_leading_one(_Tp __x) noexcept {
    return __x == 0 ? 0u : static_cast<unsigned int>(std::__fcxx_countl_zero(__x) + 1);
}

template <class _Tp>
inline unsigned int __fcxx_stdc_first_trailing_zero(_Tp __x) noexcept {
    const int __n = std::__fcxx_countr_one(__x);
    return __n == __fcxx_bit_digits<_Tp>::value ? 0u : static_cast<unsigned int>(__n + 1);
}

template <class _Tp>
inline unsigned int __fcxx_stdc_first_trailing_one(_Tp __x) noexcept {
    return __x == 0 ? 0u : static_cast<unsigned int>(std::__fcxx_countr_zero(__x) + 1);
}

template <class _Tp>
inline unsigned int __fcxx_stdc_count_zeros(_Tp __x) noexcept {
    return static_cast<unsigned int>(__fcxx_bit_digits<_Tp>::value - std::__fcxx_popcount(__x));
}

template <class _Tp>
inline unsigned int __fcxx_stdc_count_ones(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_popcount(__x));
}

template <class _Tp>
inline bool __fcxx_stdc_has_single_bit(_Tp __x) noexcept {
    return std::__fcxx_has_single_bit(__x);
}

template <class _Tp>
inline unsigned int __fcxx_stdc_bit_width(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_bit_width(__x));
}

template <class _Tp>
inline _Tp __fcxx_stdc_bit_floor(_Tp __x) noexcept {
    return std::__fcxx_bit_floor(__x);
}

template <class _Tp>
inline _Tp __fcxx_stdc_bit_ceil(_Tp __x) noexcept {
    return std::__fcxx_bit_ceil(__x);
}

} /* namespace std */

/* The functions of each operation, for each of the standard unsigned types,
 * and the function template, which takes any unsigned integer type, and
 * neither bool nor a character type, nor a const or volatile one, which
 * the template argument could be given as. */
#define _FCXX_STDBIT_FUNCTIONS(name, ruc, rus, rui, rul, rull)                                             \
    inline ruc stdc_##name##_uc(unsigned char __x) noexcept { return std::__fcxx_stdc_##name(__x); }      \
    inline rus stdc_##name##_us(unsigned short __x) noexcept { return std::__fcxx_stdc_##name(__x); }     \
    inline rui stdc_##name##_ui(unsigned int __x) noexcept { return std::__fcxx_stdc_##name(__x); }       \
    inline rul stdc_##name##_ul(unsigned long __x) noexcept { return std::__fcxx_stdc_##name(__x); }      \
    inline rull stdc_##name##_ull(unsigned long long __x) noexcept { return std::__fcxx_stdc_##name(__x); } \
    template <class _Tp>                                                                                    \
    inline decltype(std::__fcxx_stdc_##name(_Tp())) stdc_##name(_Tp __x) noexcept {                        \
        static_assert(std::__fcxx_bit_is_unsigned_integer<_Tp>::value,                                      \
                      "stdc_" #name " requires value to have an unsigned integer type");                    \
        return std::__fcxx_stdc_##name(__x);                                                                \
    }

#define _FCXX_STDBIT_COUNT(name) \
    _FCXX_STDBIT_FUNCTIONS(name, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int)

_FCXX_STDBIT_COUNT(leading_zeros)
_FCXX_STDBIT_COUNT(leading_ones)
_FCXX_STDBIT_COUNT(trailing_zeros)
_FCXX_STDBIT_COUNT(trailing_ones)
_FCXX_STDBIT_COUNT(first_leading_zero)
_FCXX_STDBIT_COUNT(first_leading_one)
_FCXX_STDBIT_COUNT(first_trailing_zero)
_FCXX_STDBIT_COUNT(first_trailing_one)
_FCXX_STDBIT_COUNT(count_zeros)
_FCXX_STDBIT_COUNT(count_ones)
_FCXX_STDBIT_FUNCTIONS(has_single_bit, bool, bool, bool, bool, bool)
_FCXX_STDBIT_COUNT(bit_width)
_FCXX_STDBIT_FUNCTIONS(bit_floor, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long)
_FCXX_STDBIT_FUNCTIONS(bit_ceil, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long)

#undef _FCXX_STDBIT_COUNT
#undef _FCXX_STDBIT_FUNCTIONS

#endif

_FCXX_END_EXTERN_CXX
#endif
