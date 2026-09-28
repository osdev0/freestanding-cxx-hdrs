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

/* char_traits, of <string>, which <string_view> needs too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_CHAR_TRAITS_H
#define __FREESTANDING_CXX_HDRS_FCXX_CHAR_TRAITS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

/* wint_t. */
#include <cwchar>
#include <cstdint>
#include <compare>

/* Whether a function of the specializations runs at run time, where the
 * builtin functions of the compiler do its work, rather than in a constant
 * expression, where they cannot. This is known without asking where the
 * function is not constexpr in the revision in use, for those constexpr from
 * C++17 on and those constexpr from C++20 on. Where the compiler cannot
 * tell, those of C++17 on do their work by themselves, which is right at run
 * time too, and those of C++20 on, which every compiler that has them can
 * tell for, are left to the builtins. */
#define _FCXX_CHAR_TRAITS_AT_RUN_TIME17 (!__builtin_is_constant_evaluated())

#define _FCXX_CHAR_TRAITS_AT_RUN_TIME20 (!__builtin_is_constant_evaluated())

namespace std {

/* [char.traits], character traits. The class template is only declared, as
 * a basis for the explicit specializations. */
template <class _CharT>
struct char_traits;

/* What the specializations have in common. They only differ by their types,
 * their end-of-file value, and the type _CmpT their characters are compared
 * as, and made into int_type values through: unsigned char for char, and the
 * character type itself otherwise. At run time, the characters of one byte
 * are compared and filled by memcmp and memset, which compare as unsigned
 * char, and all are moved and copied by memmove and memcpy.
 *
 * The types of the positions and offsets in streams, and of the state of
 * the conversions of their characters, pos_type, off_type and state_type,
 * are left out, as only the streams of a hosted environment have any use
 * for them, and the last is its mbstate_t (see <cwchar>). */
template <class _CharT, class _IntT, _IntT _Eof, class _CmpT>
struct __fcxx_char_traits_base {
    typedef _CharT char_type;
    typedef _IntT int_type;
    typedef strong_ordering comparison_category;

    static constexpr void assign(char_type& __c1, const char_type& __c2) noexcept { __c1 = __c2; }

    static constexpr bool eq(char_type __c1, char_type __c2) noexcept { return __c1 == __c2; }

    static constexpr bool lt(char_type __c1, char_type __c2) noexcept {
        return static_cast<_CmpT>(__c1) < static_cast<_CmpT>(__c2);
    }

    static constexpr int compare(const char_type* __s1, const char_type* __s2, size_t __n) {
        if (sizeof(char_type) == 1 && _FCXX_CHAR_TRAITS_AT_RUN_TIME17) {
            return __n == 0 ? 0 : __builtin_memcmp(__s1, __s2, __n);
        }
        for (size_t __i = 0; __i < __n; ++__i) {
            if (!eq(__s1[__i], __s2[__i])) {
                return lt(__s1[__i], __s2[__i]) ? -1 : 1;
            }
        }
        return 0;
    }

    static constexpr size_t length(const char_type* __s) {
        size_t __n = 0;
        while (!eq(__s[__n], char_type())) {
            ++__n;
        }
        return __n;
    }

    static constexpr const char_type* find(const char_type* __s, size_t __n, const char_type& __a) {
        for (; __n > 0; --__n, ++__s) {
            if (eq(*__s, __a)) {
                return __s;
            }
        }
        return 0;
    }

    /* In a constant expression, the ranges can only overlap where they are
     * in the same array, which is where the order of their starts is a
     * constant: the source is then copied backwards where it starts before
     * the destination, and forwards otherwise, as it is where they are in
     * different arrays. */
    static constexpr char_type* move(char_type* __s1, const char_type* __s2, size_t __n) {
        if (_FCXX_CHAR_TRAITS_AT_RUN_TIME20) {
            if (__n != 0) {
                __builtin_memmove(__s1, __s2, __n * sizeof(char_type));
            }
            return __s1;
        }
        if (__builtin_constant_p(__s2 < __s1) && __s2 < __s1) {
            for (size_t __i = __n; __i > 0; --__i) {
                __s1[__i - 1] = __s2[__i - 1];
            }
        } else {
            for (size_t __i = 0; __i < __n; ++__i) {
                __s1[__i] = __s2[__i];
            }
        }
        return __s1;
    }

    static constexpr char_type* copy(char_type* __s1, const char_type* __s2, size_t __n) {
        if (_FCXX_CHAR_TRAITS_AT_RUN_TIME20) {
            if (__n != 0) {
                __builtin_memcpy(__s1, __s2, __n * sizeof(char_type));
            }
            return __s1;
        }
        for (size_t __i = 0; __i < __n; ++__i) {
            __s1[__i] = __s2[__i];
        }
        return __s1;
    }

    static constexpr char_type* assign(char_type* __s, size_t __n, char_type __a) {
        if (sizeof(char_type) == 1 && _FCXX_CHAR_TRAITS_AT_RUN_TIME20) {
            if (__n != 0) {
                __builtin_memset(__s, static_cast<unsigned char>(__a), __n);
            }
            return __s;
        }
        for (size_t __i = 0; __i < __n; ++__i) {
            __s[__i] = __a;
        }
        return __s;
    }

    static constexpr int_type not_eof(int_type __c) noexcept {
        return __c == _Eof ? static_cast<int_type>(0) : __c;
    }

    static constexpr char_type to_char_type(int_type __c) noexcept {
        return static_cast<char_type>(__c);
    }

    static constexpr int_type to_int_type(char_type __c) noexcept {
        return static_cast<int_type>(static_cast<_CmpT>(__c));
    }

    static constexpr bool eq_int_type(int_type __c1, int_type __c2) noexcept { return __c1 == __c2; }

    static constexpr int_type eof() noexcept { return _Eof; }
};

/* [char.traits.specializations.char], whose characters compare as unsigned
 * char, and whose end-of-file value is -1, the value of EOF in every C
 * library, which the library cannot take from one. */
template <>
struct char_traits<char> : __fcxx_char_traits_base<char, int, -1, unsigned char> {};

/* [char.traits.specializations.wchar.t], whose end-of-file value is the
 * largest wint_t, the value of WEOF in every C library, likewise. */
template <>
struct char_traits<wchar_t> : __fcxx_char_traits_base<wchar_t, wint_t, static_cast<wint_t>(-1), wchar_t> {};

/* [char.traits.specializations.char16.t], whose end-of-file value is the
 * code unit 0xFFFF, the noncharacter U+FFFF, and
 * [char.traits.specializations.char32.t], whose end-of-file value is
 * 0xFFFFFFFF, which is not a code point. */
template <>
struct char_traits<char16_t>
    : __fcxx_char_traits_base<char16_t, uint_least16_t, static_cast<uint_least16_t>(-1), char16_t> {};

template <>
struct char_traits<char32_t>
    : __fcxx_char_traits_base<char32_t, uint_least32_t, static_cast<uint_least32_t>(-1), char32_t> {};

#if _FCXX_HAS_CHAR8_T
/* [char.traits.specializations.char8.t], whose end-of-file value is the
 * largest unsigned int, which is not a UTF-8 code unit. */
template <>
struct char_traits<char8_t> : __fcxx_char_traits_base<char8_t, unsigned int, static_cast<unsigned int>(-1), char8_t> {};
#endif

} /* namespace std */

#undef _FCXX_CHAR_TRAITS_AT_RUN_TIME17
#undef _FCXX_CHAR_TRAITS_AT_RUN_TIME20

#endif
