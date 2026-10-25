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


/* The bit manipulation of the unsigned integer types, which <bit> and the
 * functions of <stdbit.h> in C++ (c_stdbit.h) are made of. It includes no
 * public header, as <stdbit.h> may include none. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_BIT_H
#define __FREESTANDING_CXX_HDRS_FCXX_BIT_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

namespace std {

/* The traits the operations need that <type_traits> has no equivalent of. */

#ifdef __SIZEOF_INT128__
__extension__ typedef unsigned __int128 __fcxx_uint128;
#endif

/* The unsigned integer types, which are the standard ones and the extended
 * one, unsigned __int128, but neither bool nor the character types. */
template <class _Tp> struct __fcxx_bit_is_unsigned_integer { static constexpr bool value = false; };
template <> struct __fcxx_bit_is_unsigned_integer<unsigned char> { static constexpr bool value = true; };
template <> struct __fcxx_bit_is_unsigned_integer<unsigned short> { static constexpr bool value = true; };
template <> struct __fcxx_bit_is_unsigned_integer<unsigned int> { static constexpr bool value = true; };
template <> struct __fcxx_bit_is_unsigned_integer<unsigned long> { static constexpr bool value = true; };
template <> struct __fcxx_bit_is_unsigned_integer<unsigned long long> { static constexpr bool value = true; };
#ifdef __SIZEOF_INT128__
template <> struct __fcxx_bit_is_unsigned_integer<__fcxx_uint128> { static constexpr bool value = true; };
#endif

/* The number of bits of an integer type, none of which are padding bits with
 * GCC and Clang, but for bool. */
template <class _Tp>
struct __fcxx_bit_digits {
    static constexpr int value = static_cast<int>(sizeof(_Tp) * __CHAR_BIT__);
};

/* The type to compute in for an unsigned integer type, which is unsigned int
 * for the types that would otherwise be promoted to int. */
template <class _Tp, bool = (sizeof(_Tp) < sizeof(unsigned int))>
struct __fcxx_bit_uint {
    typedef _Tp type;
};

template <class _Tp>
struct __fcxx_bit_uint<_Tp, true> {
    typedef unsigned int type;
};

/* The builtins, for unsigned int, unsigned long and unsigned long long, which
 * the compilers turn into instructions, or into calls of the functions of
 * their runtime library where the target has none. The types narrower than
 * unsigned int are counted in it. The counting builtins are undefined for 0,
 * which the callers handle. */
constexpr int __fcxx_bit_builtin_clz(unsigned int __x) noexcept { return __builtin_clz(__x); }
constexpr int __fcxx_bit_builtin_clz(unsigned long __x) noexcept { return __builtin_clzl(__x); }
constexpr int __fcxx_bit_builtin_clz(unsigned long long __x) noexcept { return __builtin_clzll(__x); }
constexpr int __fcxx_bit_builtin_ctz(unsigned int __x) noexcept { return __builtin_ctz(__x); }
constexpr int __fcxx_bit_builtin_ctz(unsigned long __x) noexcept { return __builtin_ctzl(__x); }
constexpr int __fcxx_bit_builtin_ctz(unsigned long long __x) noexcept { return __builtin_ctzll(__x); }
constexpr int __fcxx_bit_builtin_popcount(unsigned int __x) noexcept { return __builtin_popcount(__x); }
constexpr int __fcxx_bit_builtin_popcount(unsigned long __x) noexcept { return __builtin_popcountl(__x); }
constexpr int __fcxx_bit_builtin_popcount(unsigned long long __x) noexcept { return __builtin_popcountll(__x); }

template <class _Tp, bool = (sizeof(_Tp) > sizeof(unsigned long long))>
struct __fcxx_bit_builtin {
    typedef typename __fcxx_bit_uint<_Tp>::type _Up;

    static constexpr int __clz(_Tp __x) noexcept {
        return __fcxx_bit_builtin_clz(static_cast<_Up>(__x))
             - (__fcxx_bit_digits<_Up>::value - __fcxx_bit_digits<_Tp>::value);
    }
    static constexpr int __ctz(_Tp __x) noexcept { return __fcxx_bit_builtin_ctz(static_cast<_Up>(__x)); }
    static constexpr int __popcount(_Tp __x) noexcept { return __fcxx_bit_builtin_popcount(static_cast<_Up>(__x)); }
};

/* unsigned __int128, which the builtins for every type count, where the
 * compiler has them, and otherwise the builtins for unsigned long long, as
 * two halves. */
#if _FCXX_HAS_BUILTIN(__builtin_clzg) && _FCXX_HAS_BUILTIN(__builtin_ctzg) && _FCXX_HAS_BUILTIN(__builtin_popcountg)
template <class _Tp>
struct __fcxx_bit_builtin<_Tp, true> {
    static constexpr int __clz(_Tp __x) noexcept { return __builtin_clzg(__x); }
    static constexpr int __ctz(_Tp __x) noexcept { return __builtin_ctzg(__x); }
    static constexpr int __popcount(_Tp __x) noexcept { return __builtin_popcountg(__x); }
};
#else
template <class _Tp>
struct __fcxx_bit_builtin<_Tp, true> {
    static_assert(sizeof(_Tp) == 2 * sizeof(unsigned long long), "an integer type that is not two unsigned long longs");

    static constexpr int __half = __fcxx_bit_digits<unsigned long long>::value;

    static constexpr unsigned long long __high(_Tp __x) noexcept {
        return static_cast<unsigned long long>(__x >> __half);
    }
    static constexpr unsigned long long __low(_Tp __x) noexcept { return static_cast<unsigned long long>(__x); }

    static constexpr int __clz(_Tp __x) noexcept {
        return __high(__x) != 0 ? __builtin_clzll(__high(__x)) : __builtin_clzll(__low(__x)) + __half;
    }
    static constexpr int __ctz(_Tp __x) noexcept {
        return __low(__x) != 0 ? __builtin_ctzll(__low(__x)) : __builtin_ctzll(__high(__x)) + __half;
    }
    static constexpr int __popcount(_Tp __x) noexcept {
        return __builtin_popcountll(__high(__x)) + __builtin_popcountll(__low(__x));
    }
};
#endif

/* [bit.count], counting, for the unsigned integer types. */

template <class _Tp>
constexpr int __fcxx_popcount(_Tp __x) noexcept {
    return __fcxx_bit_builtin<_Tp>::__popcount(__x);
}

template <class _Tp>
constexpr int __fcxx_countl_zero(_Tp __x) noexcept {
    return __x == 0 ? __fcxx_bit_digits<_Tp>::value : __fcxx_bit_builtin<_Tp>::__clz(__x);
}

template <class _Tp>
constexpr int __fcxx_countr_zero(_Tp __x) noexcept {
    return __x == 0 ? __fcxx_bit_digits<_Tp>::value : __fcxx_bit_builtin<_Tp>::__ctz(__x);
}

template <class _Tp>
constexpr int __fcxx_countl_one(_Tp __x) noexcept {
    return __fcxx_countl_zero(static_cast<_Tp>(~static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x)));
}

template <class _Tp>
constexpr int __fcxx_countr_one(_Tp __x) noexcept {
    return __fcxx_countr_zero(static_cast<_Tp>(~static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x)));
}

/* [bit.pow.two], integral powers of 2. The ceiling is 0 when it cannot be
 * represented, as the shift then carries its bit out of the type. */

template <class _Tp>
constexpr bool __fcxx_has_single_bit(_Tp __x) noexcept {
    return __x != 0
        && (static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x)
            & (static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x) - 1)) == 0;
}

template <class _Tp>
constexpr int __fcxx_bit_width(_Tp __x) noexcept {
    return __fcxx_bit_digits<_Tp>::value - __fcxx_countl_zero(__x);
}

template <class _Tp>
constexpr _Tp __fcxx_bit_floor(_Tp __x) noexcept {
    return __x == 0 ? _Tp(0)
                    : static_cast<_Tp>(static_cast<typename __fcxx_bit_uint<_Tp>::type>(1)
                                       << (__fcxx_bit_width(__x) - 1));
}

template <class _Tp>
constexpr _Tp __fcxx_bit_ceil(_Tp __x) noexcept {
    return __x <= _Tp(1) ? _Tp(1)
                         : static_cast<_Tp>(static_cast<typename __fcxx_bit_uint<_Tp>::type>(2)
                                            << (__fcxx_bit_width(static_cast<_Tp>(__x - 1)) - 1));
}

/* [bit.rotate], rotating, where the rotations by 0 < r < N bits are those
 * that shift both ways. */

template <class _Tp>
constexpr _Tp __fcxx_bit_rotl_by(_Tp __x, int __r) noexcept {
    return static_cast<_Tp>((static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x) << __r)
                            | (static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x)
                               >> (__fcxx_bit_digits<_Tp>::value - __r)));
}

template <class _Tp>
constexpr _Tp __fcxx_bit_rotr_by(_Tp __x, int __r) noexcept {
    return static_cast<_Tp>((static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x) >> __r)
                            | (static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x)
                               << (__fcxx_bit_digits<_Tp>::value - __r)));
}

template <class _Tp>
constexpr _Tp __fcxx_rotl(_Tp __x, int __s) noexcept {
    return __s % __fcxx_bit_digits<_Tp>::value == 0 ? __x
         : __s % __fcxx_bit_digits<_Tp>::value > 0 ? __fcxx_bit_rotl_by(__x, __s % __fcxx_bit_digits<_Tp>::value)
                                                   : __fcxx_bit_rotr_by(__x, -(__s % __fcxx_bit_digits<_Tp>::value));
}

template <class _Tp>
constexpr _Tp __fcxx_rotr(_Tp __x, int __s) noexcept {
    return __s % __fcxx_bit_digits<_Tp>::value == 0 ? __x
         : __s % __fcxx_bit_digits<_Tp>::value > 0 ? __fcxx_bit_rotr_by(__x, __s % __fcxx_bit_digits<_Tp>::value)
                                                   : __fcxx_bit_rotl_by(__x, -(__s % __fcxx_bit_digits<_Tp>::value));
}

/* [bit.byteswap], for the unsigned integer types: the builtins swap 2, 4 or
 * 8 bytes, of 8 bits, as on every target of GCC and Clang, and unsigned
 * __int128 is swapped as two halves. */
template <class _Tp, int = static_cast<int>(sizeof(_Tp))>
struct __fcxx_bit_bswap {
    static_assert(sizeof(_Tp) == 2 * sizeof(unsigned long long), "an integer type that is not two unsigned long longs");

    static constexpr int __half = __fcxx_bit_digits<unsigned long long>::value;

    static constexpr _Tp __swap(_Tp __x) noexcept {
        return static_cast<_Tp>(static_cast<_Tp>(__builtin_bswap64(static_cast<unsigned long long>(__x))) << __half
                                | __builtin_bswap64(static_cast<unsigned long long>(__x >> __half)));
    }
};

template <class _Tp>
struct __fcxx_bit_bswap<_Tp, 1> {
    static constexpr _Tp __swap(_Tp __x) noexcept { return __x; }
};

template <class _Tp>
struct __fcxx_bit_bswap<_Tp, 2> {
    static constexpr _Tp __swap(_Tp __x) noexcept { return static_cast<_Tp>(__builtin_bswap16(__x)); }
};

template <class _Tp>
struct __fcxx_bit_bswap<_Tp, 4> {
    static constexpr _Tp __swap(_Tp __x) noexcept { return static_cast<_Tp>(__builtin_bswap32(__x)); }
};

template <class _Tp>
struct __fcxx_bit_bswap<_Tp, 8> {
    static constexpr _Tp __swap(_Tp __x) noexcept { return static_cast<_Tp>(__builtin_bswap64(__x)); }
};

template <class _Tp>
constexpr _Tp __fcxx_byteswap(_Tp __x) noexcept {
    return __fcxx_bit_bswap<_Tp>::__swap(__x);
}

} /* namespace std */

#endif
