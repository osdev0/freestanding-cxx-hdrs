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


/* min, max, minmax and clamp, of <algorithm>, which other headers need
 * too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_MINMAX_H
#define __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_MINMAX_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX
#include <__fcxx/algorithm_impl.h>
#include <utility>
#include <initializer_list>

namespace std {

/* [alg.min.max], minimum and maximum, which give the first argument where
 * the arguments are equivalent. */
template <class _Tp>
[[nodiscard]] constexpr const _Tp& min(const _Tp& __a, const _Tp& __b) {
    return bool(__b < __a) ? __b : __a;
}

template <class _Tp, class _Compare>
[[nodiscard]] constexpr const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp) {
    return bool(__comp(__b, __a)) ? __b : __a;
}

template <class _Tp>
[[nodiscard]] constexpr const _Tp& max(const _Tp& __a, const _Tp& __b) {
    return bool(__a < __b) ? __b : __a;
}

template <class _Tp, class _Compare>
[[nodiscard]] constexpr const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp) {
    return bool(__comp(__a, __b)) ? __b : __a;
}

/* The copies of the least and the greatest elements of initializer lists,
 * the leftmost of those equivalent to the least, and the leftmost of those
 * equivalent to the greatest. */
template <class _Tp, class _Compare>
[[nodiscard]] constexpr _Tp min(initializer_list<_Tp> __t, _Compare __comp) {
    const _Tp* __first = __t.begin();
    const _Tp* __last = __t.end();
    const _Tp* __m = __first;
    for (++__first; __first != __last; ++__first) {
        if (bool(__comp(*__first, *__m))) {
            __m = __first;
        }
    }
    return *__m;
}

template <class _Tp>
[[nodiscard]] constexpr _Tp min(initializer_list<_Tp> __t) {
    return std::min(__t, __fcxx_less_op());
}

template <class _Tp, class _Compare>
[[nodiscard]] constexpr _Tp max(initializer_list<_Tp> __t, _Compare __comp) {
    const _Tp* __first = __t.begin();
    const _Tp* __last = __t.end();
    const _Tp* __m = __first;
    for (++__first; __first != __last; ++__first) {
        if (bool(__comp(*__m, *__first))) {
            __m = __first;
        }
    }
    return *__m;
}

template <class _Tp>
[[nodiscard]] constexpr _Tp max(initializer_list<_Tp> __t) {
    return std::max(__t, __fcxx_less_op());
}

template <class _Tp>
[[nodiscard]] constexpr pair<const _Tp&, const _Tp&> minmax(const _Tp& __a, const _Tp& __b) {
    return bool(__b < __a) ? pair<const _Tp&, const _Tp&>(__b, __a) : pair<const _Tp&, const _Tp&>(__a, __b);
}

template <class _Tp, class _Compare>
[[nodiscard]] constexpr pair<const _Tp&, const _Tp&> minmax(const _Tp& __a, const _Tp& __b,
                                                                      _Compare __comp) {
    return bool(__comp(__b, __a)) ? pair<const _Tp&, const _Tp&>(__b, __a) : pair<const _Tp&, const _Tp&>(__a, __b);
}

/* The leftmost of the least elements and the rightmost of the greatest, of
 * pointers to them, found from the elements taken by pairs: three
 * comparisons for each two elements, but the first two, which take one. */
template <class _Tp, class _Compare>
constexpr void __fcxx_minmax_ptr(const _Tp*& __min, const _Tp*& __max, const _Tp* __first, const _Tp* __last,
                                         _Compare& __comp) {
    __min = __first;
    __max = __first;
    if (__first == __last || ++__first == __last) {
        return;
    }
    if (bool(__comp(*__first, *__min))) {
        __min = __first;
    } else {
        __max = __first;
    }
    for (++__first; __first != __last; ++__first) {
        const _Tp* __i = __first;
        if (++__first == __last) {
            if (bool(__comp(*__i, *__min))) {
                __min = __i;
            } else if (!bool(__comp(*__i, *__max))) {
                __max = __i;
            }
            break;
        }
        if (bool(__comp(*__first, *__i))) {
            if (bool(__comp(*__first, *__min))) {
                __min = __first;
            }
            if (!bool(__comp(*__i, *__max))) {
                __max = __i;
            }
        } else {
            if (bool(__comp(*__i, *__min))) {
                __min = __i;
            }
            if (!bool(__comp(*__first, *__max))) {
                __max = __first;
            }
        }
    }
}

template <class _Tp, class _Compare>
[[nodiscard]] constexpr pair<_Tp, _Tp> minmax(initializer_list<_Tp> __t, _Compare __comp) {
    const _Tp* __min = __t.begin();
    const _Tp* __max = __t.begin();
    std::__fcxx_minmax_ptr(__min, __max, __t.begin(), __t.end(), __comp);
    return pair<_Tp, _Tp>(*__min, *__max);
}

template <class _Tp>
[[nodiscard]] constexpr pair<_Tp, _Tp> minmax(initializer_list<_Tp> __t) {
    return std::minmax(__t, __fcxx_less_op());
}

/* [alg.clamp], bounded value */
template <class _Tp, class _Compare>
[[nodiscard]] constexpr const _Tp& clamp(const _Tp& __v, const _Tp& __lo, const _Tp& __hi, _Compare __comp) {
    return bool(__comp(__v, __lo)) ? __lo : bool(__comp(__hi, __v)) ? __hi : __v;
}

template <class _Tp>
[[nodiscard]] constexpr const _Tp& clamp(const _Tp& __v, const _Tp& __lo, const _Tp& __hi) {
    return bool(__v < __lo) ? __lo : bool(__hi < __v) ? __hi : __v;
}

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
