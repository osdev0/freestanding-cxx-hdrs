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


/* What <stdckdint.h>, that of freestanding-c-hdrs, has in C++, from C++26
 * on, which it includes this for: the checked operations, as function
 * templates, which give whether the result of the mathematical operation
 * does not fit in the type of *result, where they store it wrapped. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_C_STDCKDINT_H
#define __FREESTANDING_CXX_HDRS_FCXX_C_STDCKDINT_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#if _FCXX_STD >= 2026

/* C++ has <stdckdint.h> itself define this, which <version> has not as it
 * is no freestanding header. */
#undef __cpp_lib_stdckdint_h
#define __cpp_lib_stdckdint_h 202603L

namespace std {

/* The types the operations take: the signed and unsigned integer types, but
 * neither bool nor the character types, nor their const or volatile
 * versions. */
template <class _Tp>
inline constexpr bool __fcxx_ckd_type = false;
template <>
inline constexpr bool __fcxx_ckd_type<signed char> = true;
template <>
inline constexpr bool __fcxx_ckd_type<short> = true;
template <>
inline constexpr bool __fcxx_ckd_type<int> = true;
template <>
inline constexpr bool __fcxx_ckd_type<long> = true;
template <>
inline constexpr bool __fcxx_ckd_type<long long> = true;
template <>
inline constexpr bool __fcxx_ckd_type<unsigned char> = true;
template <>
inline constexpr bool __fcxx_ckd_type<unsigned short> = true;
template <>
inline constexpr bool __fcxx_ckd_type<unsigned int> = true;
template <>
inline constexpr bool __fcxx_ckd_type<unsigned long> = true;
template <>
inline constexpr bool __fcxx_ckd_type<unsigned long long> = true;
#ifdef __SIZEOF_INT128__
__extension__ template <>
inline constexpr bool __fcxx_ckd_type<__int128> = true;
__extension__ template <>
inline constexpr bool __fcxx_ckd_type<unsigned __int128> = true;
#endif

} /* namespace std */

#define _FCXX_CKD(name)                                                                                     \
    template <class _Tp1, class _Tp2, class _Tp3>                                                           \
    bool ckd_##name(_Tp1* __result, _Tp2 __a, _Tp3 __b) {                                                   \
        static_assert(std::__fcxx_ckd_type<_Tp1>, "ckd_" #name " requires result to be a pointer to a signed " \
                                                  "or unsigned integer type that is neither const nor volatile"); \
        static_assert(std::__fcxx_ckd_type<_Tp2>,                                                           \
                      "ckd_" #name " requires operand a to have a signed or unsigned integer type");        \
        static_assert(std::__fcxx_ckd_type<_Tp3>,                                                           \
                      "ckd_" #name " requires operand b to have a signed or unsigned integer type");        \
        return __builtin_##name##_overflow(__a, __b, __result);                                             \
    }

_FCXX_CKD(add)
_FCXX_CKD(sub)
_FCXX_CKD(mul)

#undef _FCXX_CKD

#endif

#endif
