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


/* The iterator operations of <iterator>, advance, distance, next and prev,
 * and ranges::advance, ranges::next and ranges::prev. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ITERATOR_OPS_H
#define __FREESTANDING_CXX_HDRS_FCXX_ITERATOR_OPS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
#include <__fcxx/iterator_base.h>

namespace std {

/* [iterator.operations], iterator operations. They go by the categories of
 * the C++17 requirements that the iterators meet, which iterator_category
 * tells. */
template <class _It, class _Dist>
constexpr void __fcxx_advance(_It& __i, _Dist __n, input_iterator_tag) {
    for (; __n > 0; --__n) {
        ++__i;
    }
}

template <class _It, class _Dist>
constexpr void __fcxx_advance(_It& __i, _Dist __n, bidirectional_iterator_tag) {
    typename iterator_traits<_It>::difference_type __d = __n;
    if (__d >= 0) {
        for (; __d > 0; --__d) {
            ++__i;
        }
    } else {
        for (; __d < 0; ++__d) {
            --__i;
        }
    }
}

template <class _It, class _Dist>
constexpr void __fcxx_advance(_It& __i, _Dist __n, random_access_iterator_tag) {
    __i += __n;
}

template <class _It, class _Dist>
constexpr void advance(_It& __i, _Dist __n) {
    std::__fcxx_advance(__i, __n, typename iterator_traits<_It>::iterator_category());
}

template <class _It>
constexpr typename iterator_traits<_It>::difference_type __fcxx_distance(_It __first, _It __last,
                                                                                 input_iterator_tag) {
    typename iterator_traits<_It>::difference_type __n = 0;
    for (; __first != __last; ++__first) {
        ++__n;
    }
    return __n;
}

template <class _It>
constexpr typename iterator_traits<_It>::difference_type __fcxx_distance(_It __first, _It __last,
                                                                                 random_access_iterator_tag) {
    return __last - __first;
}

template <class _It>
constexpr typename iterator_traits<_It>::difference_type distance(_It __first, _It __last) {
    return std::__fcxx_distance(__first, __last, typename iterator_traits<_It>::iterator_category());
}

template <class _It>
constexpr _It next(_It __x, typename iterator_traits<_It>::difference_type __n = 1) {
    std::advance(__x, __n);
    return __x;
}

template <class _It>
constexpr _It prev(_It __x, typename iterator_traits<_It>::difference_type __n = 1) {
    std::advance(__x, -__n);
    return __x;
}

/* [range.iter.ops], range iterator operations, which are algorithm function
 * objects. ranges::distance, which needs the range concepts, is with them. */
namespace ranges {

struct __advance_fn {
    template <input_or_output_iterator _It>
    constexpr void operator()(_It& __i, iter_difference_t<_It> __n) const {
        if constexpr (random_access_iterator<_It>) {
            __i += __n;
        } else {
            for (; __n > 0; --__n) {
                ++__i;
            }
            if constexpr (bidirectional_iterator<_It>) {
                for (; __n < 0; ++__n) {
                    --__i;
                }
            }
        }
    }

    template <input_or_output_iterator _It, sentinel_for<_It> _Sp>
    constexpr void operator()(_It& __i, _Sp __bound) const {
        if constexpr (assignable_from<_It&, _Sp>) {
            __i = std::move(__bound);
        } else if constexpr (sized_sentinel_for<_Sp, _It>) {
            (*this)(__i, __bound - __i);
        } else {
            while (static_cast<bool>(__i != __bound)) {
                ++__i;
            }
        }
    }

    template <input_or_output_iterator _It, sentinel_for<_It> _Sp>
    constexpr iter_difference_t<_It> operator()(_It& __i, iter_difference_t<_It> __n, _Sp __bound) const {
        if constexpr (sized_sentinel_for<_Sp, _It>) {
            /* |n| >= |bound - i|, where the preconditions give them the same
             * sign, or bound - i is 0. */
            const iter_difference_t<_It> __d = __bound - __i;
            if ((__n > 0 && __n >= __d) || (__n < 0 && __n <= __d) || __d == 0) {
                (*this)(__i, std::move(__bound));
                return __n - __d;
            }
            (*this)(__i, __n);
            return 0;
        } else {
            for (; __n > 0 && static_cast<bool>(__i != __bound); --__n) {
                ++__i;
            }
            if constexpr (bidirectional_iterator<_It> && same_as<_It, _Sp>) {
                for (; __n < 0 && static_cast<bool>(__i != __bound); ++__n) {
                    --__i;
                }
            }
            return __n;
        }
    }
};

inline constexpr __advance_fn advance{};

struct __next_fn {
    template <input_or_output_iterator _It>
    constexpr _It operator()(_It __x) const {
        ++__x;
        return __x;
    }

    template <input_or_output_iterator _It>
    constexpr _It operator()(_It __x, iter_difference_t<_It> __n) const {
        ranges::advance(__x, __n);
        return __x;
    }

    template <input_or_output_iterator _It, sentinel_for<_It> _Sp>
    constexpr _It operator()(_It __x, _Sp __bound) const {
        ranges::advance(__x, __bound);
        return __x;
    }

    template <input_or_output_iterator _It, sentinel_for<_It> _Sp>
    constexpr _It operator()(_It __x, iter_difference_t<_It> __n, _Sp __bound) const {
        ranges::advance(__x, __n, __bound);
        return __x;
    }
};

inline constexpr __next_fn next{};

struct __prev_fn {
    template <bidirectional_iterator _It>
    constexpr _It operator()(_It __x) const {
        --__x;
        return __x;
    }

    template <bidirectional_iterator _It>
    constexpr _It operator()(_It __x, iter_difference_t<_It> __n) const {
        ranges::advance(__x, -__n);
        return __x;
    }

    template <bidirectional_iterator _It>
    constexpr _It operator()(_It __x, iter_difference_t<_It> __n, _It __bound) const {
        ranges::advance(__x, -__n, __bound);
        return __x;
    }
};

inline constexpr __prev_fn prev{};

} /* namespace ranges */

} /* namespace std */

#endif
