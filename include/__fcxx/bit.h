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


/* The bit manipulation of the unsigned integer types, which <bit> and
 * <stdbit.h> are made of. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_BIT_H
#define __FREESTANDING_CXX_HDRS_FCXX_BIT_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

/* Whether the target has instructions to count the leading and the trailing
 * zero bits, to count the one bits and to swap the bytes of a word, which
 * the builtins of the compiler are then used for. Elsewhere, GCC and Clang
 * can turn the builtins into calls of functions of their runtime libraries
 * (such as that counting the one bits, for x86 without POPCNT), so the
 * operations are written out instead, needing none. */
#if defined(__i386__) || defined(__x86_64__) || defined(__aarch64__) || defined(__powerpc__) \
    || defined(__powerpc64__) || (defined(__arm__) && defined(__ARM_FEATURE_CLZ)) \
    || (defined(__riscv) && defined(__riscv_zbb))
#  define _FCXX_BIT_NATIVE_CLZ 1
#else
#  define _FCXX_BIT_NATIVE_CLZ 0
#endif

#if ((defined(__i386__) || defined(__x86_64__)) && defined(__POPCNT__)) \
    || (defined(__aarch64__) && (defined(__ARM_NEON) || defined(__ARM_FEATURE_CSSC))) \
    || (defined(__riscv) && defined(__riscv_zbb)) \
    || ((defined(__powerpc__) || defined(__powerpc64__)) && defined(_ARCH_PWR7))
#  define _FCXX_BIT_NATIVE_POPCOUNT 1
#else
#  define _FCXX_BIT_NATIVE_POPCOUNT 0
#endif

#if __CHAR_BIT__ == 8 \
    && (defined(__i386__) || defined(__x86_64__) || defined(__aarch64__) || defined(__powerpc__) \
        || defined(__powerpc64__) || (defined(__arm__) && defined(__ARM_ARCH) && __ARM_ARCH >= 6) \
        || (defined(__riscv) && defined(__riscv_zbb)))
#  define _FCXX_BIT_NATIVE_BSWAP 1
#else
#  define _FCXX_BIT_NATIVE_BSWAP 0
#endif

namespace std {

/* The traits the operations need, which are of their own rather than those
 * of <type_traits>, as <stdbit.h>, a C header, is to include no other header
 * of the library. */

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

/* One type or the other, as conditional. */
template <bool _Bp, class _If, class _Then>
struct __fcxx_bit_select {
    typedef _If type;
};

template <class _If, class _Then>
struct __fcxx_bit_select<false, _If, _Then> {
    typedef _Then type;
};

/* The type to compute in for an unsigned integer type, which is unsigned int
 * for the types that would otherwise be promoted to int. */
template <class _Tp>
struct __fcxx_bit_uint {
    typedef typename __fcxx_bit_select<(sizeof(_Tp) < sizeof(unsigned int)), unsigned int, _Tp>::type type;
};

/* The builtins, for unsigned int and for the word of the target, which is
 * unsigned long unless it is narrower than a pointer, or the target has
 * unsigned __int128, which only those with 64-bit registers have, whatever
 * the size of long, as x32 does. The types wider than a word, unsigned long
 * long on the 32-bit targets and unsigned __int128 on the 64-bit ones, are
 * handled as two words, which leaves nothing to the runtime library. The
 * counting builtins are undefined for 0, which the callers handle. */
#if __SIZEOF_LONG__ >= __SIZEOF_POINTER__ && !defined(__SIZEOF_INT128__)
typedef unsigned long __fcxx_bit_word;
#else
typedef unsigned long long __fcxx_bit_word;
#endif

constexpr int __fcxx_bit_builtin_clz(unsigned int __x) noexcept { return __builtin_clz(__x); }
constexpr int __fcxx_bit_builtin_clz(unsigned long __x) noexcept { return __builtin_clzl(__x); }
constexpr int __fcxx_bit_builtin_clz(unsigned long long __x) noexcept { return __builtin_clzll(__x); }
constexpr int __fcxx_bit_builtin_ctz(unsigned int __x) noexcept { return __builtin_ctz(__x); }
constexpr int __fcxx_bit_builtin_ctz(unsigned long __x) noexcept { return __builtin_ctzl(__x); }
constexpr int __fcxx_bit_builtin_ctz(unsigned long long __x) noexcept { return __builtin_ctzll(__x); }
constexpr int __fcxx_bit_builtin_popcount(unsigned int __x) noexcept { return __builtin_popcount(__x); }
constexpr int __fcxx_bit_builtin_popcount(unsigned long __x) noexcept { return __builtin_popcountl(__x); }
constexpr int __fcxx_bit_builtin_popcount(unsigned long long __x) noexcept { return __builtin_popcountll(__x); }

template <class _Tp,
          int = (sizeof(_Tp) <= sizeof(unsigned int) ? 0 : sizeof(_Tp) <= sizeof(__fcxx_bit_word) ? 1 : 2)>
struct __fcxx_bit_builtin {
    typedef typename __fcxx_bit_select<(sizeof(_Tp) <= sizeof(unsigned int)), unsigned int, __fcxx_bit_word>::type _Up;

    static constexpr int __clz(_Tp __x) noexcept {
        return __fcxx_bit_builtin_clz(static_cast<_Up>(__x))
             - (__fcxx_bit_digits<_Up>::value - __fcxx_bit_digits<_Tp>::value);
    }
    static constexpr int __ctz(_Tp __x) noexcept { return __fcxx_bit_builtin_ctz(static_cast<_Up>(__x)); }
    static constexpr int __popcount(_Tp __x) noexcept { return __fcxx_bit_builtin_popcount(static_cast<_Up>(__x)); }
};

template <class _Tp>
struct __fcxx_bit_builtin<_Tp, 2> {
    static_assert(sizeof(_Tp) <= 2 * sizeof(__fcxx_bit_word), "an integer type wider than two words");

    static constexpr int __word = __fcxx_bit_digits<__fcxx_bit_word>::value;
    static constexpr int __digits = __fcxx_bit_digits<_Tp>::value;

    static constexpr __fcxx_bit_word __high(_Tp __x) noexcept {
        return static_cast<__fcxx_bit_word>(__x >> __word);
    }
    static constexpr __fcxx_bit_word __low(_Tp __x) noexcept { return static_cast<__fcxx_bit_word>(__x); }

    static constexpr int __clz(_Tp __x) noexcept {
        return __high(__x) != 0 ? __fcxx_bit_builtin_clz(__high(__x)) - (2 * __word - __digits)
                                : __fcxx_bit_builtin_clz(__low(__x)) + (__digits - __word);
    }
    static constexpr int __ctz(_Tp __x) noexcept {
        return __low(__x) != 0 ? __fcxx_bit_builtin_ctz(__low(__x)) : __fcxx_bit_builtin_ctz(__high(__x)) + __word;
    }
    static constexpr int __popcount(_Tp __x) noexcept {
        return __fcxx_bit_builtin_popcount(__high(__x)) + __fcxx_bit_builtin_popcount(__low(__x));
    }
};

/* The one bits counted without the builtins: in parallel, first for each
 * pair of bits, then for each 4 bits and each 8 bits, whose counts are then
 * added up into the lowest 8 bits. The masks are those of the pairs, the 4
 * bits and the 8 bits of which the lowest bits are set. */
template <class _Up>
constexpr _Up __fcxx_bit_count_pairs(_Up __x) noexcept {
    return static_cast<_Up>(__x - ((__x >> 1) & (static_cast<_Up>(~_Up(0)) / 3)));
}

template <class _Up>
constexpr _Up __fcxx_bit_count_nibbles(_Up __x) noexcept {
    return static_cast<_Up>((__x & (static_cast<_Up>(~_Up(0)) / 5)) + ((__x >> 2) & (static_cast<_Up>(~_Up(0)) / 5)));
}

template <class _Up>
constexpr _Up __fcxx_bit_count_octets(_Up __x) noexcept {
    return static_cast<_Up>((__x + (__x >> 4)) & (static_cast<_Up>(~_Up(0)) / 17));
}

/* The recursions go through class templates, so that the shifts and the
 * masks are constants, and the code is straight. */
template <class _Up, int _Shift, int _Digits, bool = (_Shift < _Digits)>
struct __fcxx_bit_sum_octets {
    static constexpr _Up __sum(_Up __x) noexcept {
        return __fcxx_bit_sum_octets<_Up, _Shift * 2, _Digits>::__sum(static_cast<_Up>(__x + (__x >> _Shift)));
    }
};

template <class _Up, int _Shift, int _Digits>
struct __fcxx_bit_sum_octets<_Up, _Shift, _Digits, false> {
    static constexpr _Up __sum(_Up __x) noexcept { return __x; }
};

template <class _Tp>
constexpr int __fcxx_bit_popcount_generic(_Tp __x) noexcept {
    typedef typename __fcxx_bit_uint<_Tp>::type _Up;
    typedef __fcxx_bit_sum_octets<_Up, 8, __fcxx_bit_digits<_Tp>::value> _Sum;
    return static_cast<int>(
        _Sum::__sum(__fcxx_bit_count_octets(__fcxx_bit_count_nibbles(__fcxx_bit_count_pairs<_Up>(__x)))) & 0xff);
}

/* The value with all of the bits below its highest one bit set too. */
template <class _Up, int _Shift, int _Digits, bool = (_Shift < _Digits)>
struct __fcxx_bit_smear {
    static constexpr _Up __smear(_Up __x) noexcept {
        return __fcxx_bit_smear<_Up, _Shift * 2, _Digits>::__smear(static_cast<_Up>(__x | (__x >> _Shift)));
    }
};

template <class _Up, int _Shift, int _Digits>
struct __fcxx_bit_smear<_Up, _Shift, _Digits, false> {
    static constexpr _Up __smear(_Up __x) noexcept { return __x; }
};

/* [bit.count], counting, for the unsigned integer types. The trailing zero
 * bits are counted as the one bits of the mask of them. */

template <class _Tp>
constexpr int __fcxx_popcount(_Tp __x) noexcept {
#if _FCXX_BIT_NATIVE_POPCOUNT
    return __fcxx_bit_builtin<_Tp>::__popcount(__x);
#else
    return __fcxx_bit_popcount_generic(__x);
#endif
}

template <class _Tp>
constexpr int __fcxx_countl_zero(_Tp __x) noexcept {
#if _FCXX_BIT_NATIVE_CLZ
    return __x == 0 ? __fcxx_bit_digits<_Tp>::value : __fcxx_bit_builtin<_Tp>::__clz(__x);
#else
    return __fcxx_bit_digits<_Tp>::value
         - __fcxx_popcount(static_cast<_Tp>(
             __fcxx_bit_smear<typename __fcxx_bit_uint<_Tp>::type, 1, __fcxx_bit_digits<_Tp>::value>::__smear(__x)));
#endif
}

template <class _Tp>
constexpr int __fcxx_countr_zero(_Tp __x) noexcept {
#if _FCXX_BIT_NATIVE_CLZ
    return __x == 0 ? __fcxx_bit_digits<_Tp>::value : __fcxx_bit_builtin<_Tp>::__ctz(__x);
#else
    return __fcxx_popcount(static_cast<_Tp>(~static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x)
                                            & (static_cast<typename __fcxx_bit_uint<_Tp>::type>(__x) - 1)));
#endif
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

/* [bit.byteswap], for the unsigned integer types. Without the builtins, the
 * bytes are swapped in pairs, then the pairs of bytes, and so on, with the
 * masks of the groups of bits of which the lowest ones are set. */

template <class _Up, int _Shift, int _Digits, bool = (_Shift < _Digits)>
struct __fcxx_bit_swap_groups {
    static constexpr _Up __mask = static_cast<_Up>(~_Up(0) / ((_Up(1) << _Shift) + 1));

    static constexpr _Up __swap(_Up __x) noexcept {
        return __fcxx_bit_swap_groups<_Up, _Shift * 2, _Digits>::__swap(
            static_cast<_Up>(((__x & __mask) << _Shift) | ((__x >> _Shift) & __mask)));
    }
};

template <class _Up, int _Shift, int _Digits>
struct __fcxx_bit_swap_groups<_Up, _Shift, _Digits, false> {
    static constexpr _Up __swap(_Up __x) noexcept { return __x; }
};

/* The builtins swap 2, 4 or 8 bytes, as far as they fit in a word, and the
 * types wider than a word are swapped as two words. */
template <class _Tp, int = (!_FCXX_BIT_NATIVE_BSWAP ? 0
                            : sizeof(_Tp) > sizeof(__fcxx_bit_word) ? -1
                                                                    : static_cast<int>(sizeof(_Tp)))>
struct __fcxx_bit_bswap {
    static constexpr _Tp __swap(_Tp __x) noexcept {
        return static_cast<_Tp>(__fcxx_bit_swap_groups<typename __fcxx_bit_uint<_Tp>::type, __CHAR_BIT__,
                                                       __fcxx_bit_digits<_Tp>::value>::__swap(__x));
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
struct __fcxx_bit_bswap<_Tp, -1> {
    static_assert(sizeof(_Tp) == 2 * sizeof(__fcxx_bit_word), "an integer type that is not two words");

    static constexpr int __word = __fcxx_bit_digits<__fcxx_bit_word>::value;

    static constexpr _Tp __swap(_Tp __x) noexcept {
        return static_cast<_Tp>(
            static_cast<_Tp>(__fcxx_bit_bswap<__fcxx_bit_word>::__swap(static_cast<__fcxx_bit_word>(__x))) << __word
            | __fcxx_bit_bswap<__fcxx_bit_word>::__swap(static_cast<__fcxx_bit_word>(__x >> __word)));
    }
};

template <class _Tp>
constexpr _Tp __fcxx_byteswap(_Tp __x) noexcept {
    return __fcxx_bit_bswap<_Tp>::__swap(__x);
}

} /* namespace std */

#endif
