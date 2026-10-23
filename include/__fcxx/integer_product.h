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



/* The products of the integers that the library makes at run time, with
 * _FCXX_MUL(a, b), which is (a) * (b) but for the workaround below.
 *
 * Workaround: GCC 10 and later, up to 16 at least (9 is right), 64-bit MIPS
 * release 6: it makes a product of 128 bits, which it also makes of the
 * operands of 64 bits to divide by a constant, of a dmul and a dmuhu of the
 * same operands, the dmul first, and lets the dmul write its half of the
 * product over an operand that the dmuhu then reads, where the operand is not
 * used after the product (as in x = x * y), so that the high half of the
 * product is wrong. The products of the integers of 8 bytes and more keep
 * their operands alive past them there, so that their registers cannot be
 * written over. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_INTEGER_PRODUCT_H
#define __FREESTANDING_CXX_HDRS_FCXX_INTEGER_PRODUCT_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#if _FCXX_GCC >= 1000 && defined(__mips64) && defined(__mips_isa_rev) && __mips_isa_rev >= 6 && _FCXX_STD >= 2020 \
    && _FCXX_HAS_IS_CONSTANT_EVALUATED

#  define _FCXX_MUL(a, b) ::std::__fcxx_mul(a, b)

namespace std {

template <class _Tp>
struct __fcxx_mul_wide {
    static constexpr bool value = false;
};

template <> struct __fcxx_mul_wide<long> { static constexpr bool value = sizeof(long) >= 8; };
template <> struct __fcxx_mul_wide<unsigned long> { static constexpr bool value = sizeof(long) >= 8; };
template <> struct __fcxx_mul_wide<long long> { static constexpr bool value = true; };
template <> struct __fcxx_mul_wide<unsigned long long> { static constexpr bool value = true; };
#  ifdef __SIZEOF_INT128__
__extension__ template <> struct __fcxx_mul_wide<__int128> { static constexpr bool value = true; };
__extension__ template <> struct __fcxx_mul_wide<unsigned __int128> { static constexpr bool value = true; };
#  endif

template <class _Tp, class _Up>
_FCXX_ALWAYS_INLINE constexpr auto __fcxx_mul(_Tp&& __a, _Up&& __b)
    -> decltype(static_cast<_Tp&&>(__a) * static_cast<_Up&&>(__b)) {
    typedef decltype(static_cast<_Tp&&>(__a) * static_cast<_Up&&>(__b)) _Rp;
    if constexpr (__fcxx_mul_wide<_Rp>::value) {
        const _Rp __x = __a;
        const _Rp __y = __b;
        _Rp __p = __x * __y;
        if (!__builtin_is_constant_evaluated()) {
            __asm__("" : "+r"(__p) : "r"(__x), "r"(__y));
        }
        return __p;
    } else {
        return static_cast<_Tp&&>(__a) * static_cast<_Up&&>(__b);
    }
}

} /* namespace std */

#else
#  define _FCXX_MUL(a, b) ((a) * (b))
#endif

#endif
