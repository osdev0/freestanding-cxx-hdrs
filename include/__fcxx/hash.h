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


/* The class template hash, of <functional>, with the specializations that
 * every header declaring it provides, for which several headers need it. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_HASH_H
#define __FREESTANDING_CXX_HDRS_FCXX_HASH_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <type_traits>

namespace std {

/* FNV-1a over the bytes of an object: that of 64 bits where size_t has
 * them, and that of 32 bits otherwise, folded into a size_t of 16 bits. */
inline size_t __fcxx_hash_bytes(const void* __p, size_t __n) noexcept {
    const unsigned char* __b = static_cast<const unsigned char*>(__p);
#if __SIZEOF_SIZE_T__ >= 8
    unsigned long long __hash = 14695981039346656037ULL;
    for (size_t __i = 0; __i < __n; ++__i) {
        __hash = (__hash ^ __b[__i]) * 1099511628211ULL;
    }
    return static_cast<size_t>(__hash);
#else
    unsigned long __hash = 2166136261UL;
    for (size_t __i = 0; __i < __n; ++__i) {
        __hash = ((__hash ^ __b[__i]) * 16777619UL) & 0xFFFFFFFFUL;
    }
    return static_cast<size_t>(__SIZEOF_SIZE_T__ >= 4 ? __hash : __hash ^ (__hash >> 16));
#endif
}

/* A specialization that is disabled, as those of the types not otherwise
 * provided for are. */
struct __fcxx_disabled_hash {
    __fcxx_disabled_hash() = delete;
    __fcxx_disabled_hash(const __fcxx_disabled_hash&) = delete;
    __fcxx_disabled_hash& operator=(const __fcxx_disabled_hash&) = delete;
};

template <class _Tp>
struct __fcxx_scalar_hash {

    size_t operator()(_Tp __v) const noexcept {
        return static_cast<size_t>(__v);
    }
};

template <class _Tp>
struct __fcxx_enum_hash {

    size_t operator()(_Tp __v) const noexcept {
        return static_cast<size_t>(static_cast<typename underlying_type<_Tp>::type>(__v));
    }
};

/* The floating-point types are hashed by the bytes of their value, with the
 * zeros made the same, and only the bytes that hold the value of an x87
 * extended-precision long double, whose other bytes are padding. */
template <class _Tp>
struct __fcxx_float_hash {

    size_t operator()(_Tp __v) const noexcept {
        if (__v == _Tp(0)) {
            return 0;
        }
#if __LDBL_MANT_DIG__ == 106
        /* The IBM double-double long double, whose values are equal when
         * both of their parts are, which a low part of zero is with either
         * sign. */
        if (is_same<_Tp, long double>::value) {
            double __parts[2];
            __builtin_memcpy(__parts, &__v, sizeof(__parts));
            if (__parts[1] == 0) {
                __parts[1] = 0;
            }
            return __fcxx_hash_bytes(__parts, sizeof(__parts));
        }
#endif
        const size_t __size = (sizeof(_Tp) > 10 && __LDBL_MANT_DIG__ == 64 && is_same<_Tp, long double>::value)
                                ? 10 : sizeof(_Tp);
        return __fcxx_hash_bytes(&__v, __size);
    }
};

template <class _Tp>
struct __fcxx_pointer_hash {

    size_t operator()(_Tp* __p) const noexcept {
        return static_cast<size_t>(reinterpret_cast<_FCXX_UINTPTR>(__p));
    }
};

template <class _Tp, bool = is_enum<_Tp>::value>
struct __fcxx_hash_primary : __fcxx_disabled_hash {};

template <class _Tp>
struct __fcxx_hash_primary<_Tp, true> : __fcxx_enum_hash<_Tp> {};

template <class _Tp>
struct hash : __fcxx_hash_primary<_Tp> {};

template <class _Tp>
struct hash<_Tp*> : __fcxx_pointer_hash<_Tp> {};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

template <>
struct hash<nullptr_t> {

    size_t operator()(nullptr_t) const noexcept {
        return 0;
    }
};

#pragma GCC diagnostic pop

#define _FCXX_SCALAR_HASH(type) \
    template <> struct hash<type> : __fcxx_scalar_hash<type> {};
_FCXX_SCALAR_HASH(bool)
_FCXX_SCALAR_HASH(char)
_FCXX_SCALAR_HASH(signed char)
_FCXX_SCALAR_HASH(unsigned char)
_FCXX_SCALAR_HASH(wchar_t)
#if _FCXX_HAS_CHAR8_T
_FCXX_SCALAR_HASH(char8_t)
#endif
_FCXX_SCALAR_HASH(char16_t)
_FCXX_SCALAR_HASH(char32_t)
_FCXX_SCALAR_HASH(short)
_FCXX_SCALAR_HASH(unsigned short)
_FCXX_SCALAR_HASH(int)
_FCXX_SCALAR_HASH(unsigned int)
_FCXX_SCALAR_HASH(long)
_FCXX_SCALAR_HASH(unsigned long)
_FCXX_SCALAR_HASH(long long)
_FCXX_SCALAR_HASH(unsigned long long)
#ifdef __SIZEOF_INT128__
_FCXX_SCALAR_HASH(__fcxx_int128)
_FCXX_SCALAR_HASH(__fcxx_uint128)
#endif
#undef _FCXX_SCALAR_HASH

template <> struct hash<float> : __fcxx_float_hash<float> {};
template <> struct hash<double> : __fcxx_float_hash<double> {};
template <> struct hash<long double> : __fcxx_float_hash<long double> {};
#if _FCXX_STD >= 2023
#  ifdef __STDCPP_FLOAT16_T__
template <> struct hash<_Float16> : __fcxx_float_hash<_Float16> {};
#  endif
#  ifdef __STDCPP_FLOAT32_T__
template <> struct hash<_Float32> : __fcxx_float_hash<_Float32> {};
#  endif
#  ifdef __STDCPP_FLOAT64_T__
template <> struct hash<_Float64> : __fcxx_float_hash<_Float64> {};
#  endif
#  ifdef __STDCPP_FLOAT128_T__
template <> struct hash<_Float128> : __fcxx_float_hash<_Float128> {};
#  endif
#  ifdef __STDCPP_BFLOAT16_T__
template <> struct hash<__bf16> : __fcxx_float_hash<__bf16> {};
#  endif
#endif

/* Whether a specialization of hash is enabled, for the headers whose own
 * specializations depend on those of other types. */
template <class _Tp>
struct __fcxx_is_hash_enabled : is_default_constructible<hash<_Tp> > {};

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
