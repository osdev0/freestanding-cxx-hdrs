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


/* The header of C23, which C++ has its own version of from C++26 on, with
 * function templates for the type-generic functions of C. */

#ifndef __FREESTANDING_CXX_HDRS_STDBIT_H
#define __FREESTANDING_CXX_HDRS_STDBIT_H 1

#pragma GCC system_header

#if !defined(__cplusplus) || __cplusplus <= 202302L
#  error "<stdbit.h> requires C++26 or later"
#else

/* The C headers are to include no other header of the library but their
 * corresponding one, which this has none of, so the declarations of
 * namespace std that the headers of the library have in common are left out
 * here, and the feature-test macro of the header is not freestanding. */
#define _FCXX_NO_STD_DECLARATIONS
#include <__fcxx/config.h>
#include <__fcxx/bit.h>
#undef _FCXX_NO_STD_DECLARATIONS

/* The type names the header makes available, as the C header does: size_t,
 * and those of the exact-width and minimum-width integer types that the
 * target has, which <stddef.h> and <stdint.h> declare too. */
typedef __SIZE_TYPE__ size_t;
#ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__ uint8_t;
#endif
#ifdef __UINT16_TYPE__
typedef __UINT16_TYPE__ uint16_t;
#endif
#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
#endif
#ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__ uint64_t;
#endif
#ifdef __INT8_TYPE__
typedef __INT8_TYPE__ int8_t;
#endif
#ifdef __INT16_TYPE__
typedef __INT16_TYPE__ int16_t;
#endif
#ifdef __INT32_TYPE__
typedef __INT32_TYPE__ int32_t;
#endif
#ifdef __INT64_TYPE__
typedef __INT64_TYPE__ int64_t;
#endif
typedef __UINT_LEAST8_TYPE__ uint_least8_t;
typedef __UINT_LEAST16_TYPE__ uint_least16_t;
typedef __UINT_LEAST32_TYPE__ uint_least32_t;
typedef __UINT_LEAST64_TYPE__ uint_least64_t;
typedef __INT_LEAST8_TYPE__ int_least8_t;
typedef __INT_LEAST16_TYPE__ int_least16_t;
typedef __INT_LEAST32_TYPE__ int_least32_t;
typedef __INT_LEAST64_TYPE__ int_least64_t;

#define __STDC_VERSION_STDBIT_H__ 202311L

/* The byte order of the integer types, in the values the compilers use for
 * the byte orders. */
#define __STDC_ENDIAN_LITTLE__ __ORDER_LITTLE_ENDIAN__
#define __STDC_ENDIAN_BIG__ __ORDER_BIG_ENDIAN__
#define __STDC_ENDIAN_NATIVE__ __BYTE_ORDER__

namespace std {

/* The operations, on any unsigned integer type. The first leading or trailing
 * zero or one bit is at 1 plus the number of the bits before it, or at 0 when
 * there is no such bit, and the ceiling is 0 when it cannot be represented. */

template <class _Tp>
constexpr unsigned int __fcxx_stdc_leading_zeros(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_countl_zero(__x));
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_leading_ones(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_countl_one(__x));
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_trailing_zeros(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_countr_zero(__x));
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_trailing_ones(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_countr_one(__x));
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_first_leading_zero(_Tp __x) noexcept {
    return std::__fcxx_countl_one(__x) == __fcxx_bit_digits<_Tp>::value
             ? 0u
             : static_cast<unsigned int>(std::__fcxx_countl_one(__x)) + 1u;
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_first_leading_one(_Tp __x) noexcept {
    return __x == 0 ? 0u : static_cast<unsigned int>(std::__fcxx_countl_zero(__x)) + 1u;
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_first_trailing_zero(_Tp __x) noexcept {
    return std::__fcxx_countr_one(__x) == __fcxx_bit_digits<_Tp>::value
             ? 0u
             : static_cast<unsigned int>(std::__fcxx_countr_one(__x)) + 1u;
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_first_trailing_one(_Tp __x) noexcept {
    return __x == 0 ? 0u : static_cast<unsigned int>(std::__fcxx_countr_zero(__x)) + 1u;
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_count_zeros(_Tp __x) noexcept {
    return static_cast<unsigned int>(__fcxx_bit_digits<_Tp>::value - std::__fcxx_popcount(__x));
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_count_ones(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_popcount(__x));
}

template <class _Tp>
constexpr bool __fcxx_stdc_has_single_bit(_Tp __x) noexcept {
    return std::__fcxx_has_single_bit(__x);
}

template <class _Tp>
constexpr unsigned int __fcxx_stdc_bit_width(_Tp __x) noexcept {
    return static_cast<unsigned int>(std::__fcxx_bit_width(__x));
}

template <class _Tp>
constexpr _Tp __fcxx_stdc_bit_floor(_Tp __x) noexcept {
    return std::__fcxx_bit_floor(__x);
}

template <class _Tp>
constexpr _Tp __fcxx_stdc_bit_ceil(_Tp __x) noexcept {
    return std::__fcxx_bit_ceil(__x);
}

} /* namespace std */

/* The functions for each of the standard unsigned integer types, which are
 * not constexpr, as in C, and the function template for all of the unsigned
 * integer types, whose result is of the type given last. C leaves that of the
 * counts to the implementation, and it is unsigned int, as for the others. */
#define _FCXX_STDBIT_FUNCTIONS(op, ruc, rus, rui, rul, rull, rgen) \
    inline ruc stdc_##op##_uc(unsigned char __value) noexcept { return std::__fcxx_stdc_##op(__value); } \
    inline rus stdc_##op##_us(unsigned short __value) noexcept { return std::__fcxx_stdc_##op(__value); } \
    inline rui stdc_##op##_ui(unsigned int __value) noexcept { return std::__fcxx_stdc_##op(__value); } \
    inline rul stdc_##op##_ul(unsigned long __value) noexcept { return std::__fcxx_stdc_##op(__value); } \
    inline rull stdc_##op##_ull(unsigned long long __value) noexcept { return std::__fcxx_stdc_##op(__value); } \
    template <class _Tp> \
    rgen stdc_##op(_Tp __value) noexcept { \
        static_assert(std::__fcxx_bit_is_unsigned_integer<_Tp>::value, \
                      "stdc_" #op " of a type that is not an unsigned integer type"); \
        return std::__fcxx_stdc_##op(__value); \
    }

#define _FCXX_STDBIT_COUNTING(op) \
    _FCXX_STDBIT_FUNCTIONS(op, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int)

#define _FCXX_STDBIT_SAME_TYPE(op) \
    _FCXX_STDBIT_FUNCTIONS(op, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long, _Tp)

_FCXX_STDBIT_COUNTING(leading_zeros)
_FCXX_STDBIT_COUNTING(leading_ones)
_FCXX_STDBIT_COUNTING(trailing_zeros)
_FCXX_STDBIT_COUNTING(trailing_ones)
_FCXX_STDBIT_COUNTING(first_leading_zero)
_FCXX_STDBIT_COUNTING(first_leading_one)
_FCXX_STDBIT_COUNTING(first_trailing_zero)
_FCXX_STDBIT_COUNTING(first_trailing_one)
_FCXX_STDBIT_COUNTING(count_zeros)
_FCXX_STDBIT_COUNTING(count_ones)
_FCXX_STDBIT_FUNCTIONS(has_single_bit, bool, bool, bool, bool, bool, bool)
_FCXX_STDBIT_COUNTING(bit_width)
_FCXX_STDBIT_SAME_TYPE(bit_floor)
_FCXX_STDBIT_SAME_TYPE(bit_ceil)

#undef _FCXX_STDBIT_SAME_TYPE
#undef _FCXX_STDBIT_COUNTING
#undef _FCXX_STDBIT_FUNCTIONS

#endif

#endif
