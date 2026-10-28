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


/* The non-modifying sequence operations of <algorithm>, of namespace std,
 * which are constexpr. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_NONMODIFYING_H
#define __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_NONMODIFYING_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX
#include <__fcxx/algorithm_impl.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
#include <utility>
#include <type_traits>

namespace std {

/* [alg.all.of], [alg.any.of], [alg.none.of] */
template <class _InputIterator, class _Predicate>
[[nodiscard]] constexpr bool all_of(_InputIterator __first, _InputIterator __last, _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (!bool(__pred(*__first))) {
            return false;
        }
    }
    return true;
}

template <class _InputIterator, class _Predicate>
[[nodiscard]] constexpr bool any_of(_InputIterator __first, _InputIterator __last, _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (bool(__pred(*__first))) {
            return true;
        }
    }
    return false;
}

template <class _InputIterator, class _Predicate>
[[nodiscard]] constexpr bool none_of(_InputIterator __first, _InputIterator __last, _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (bool(__pred(*__first))) {
            return false;
        }
    }
    return true;
}

/* [alg.foreach], for each */
template <class _InputIterator, class _Function>
constexpr _Function for_each(_InputIterator __first, _InputIterator __last, _Function __f) {
    for (; __first != __last; ++__first) {
        __f(*__first);
    }
    return __f;
}

template <class _InputIterator, class _Size, class _Function>
constexpr _InputIterator for_each_n(_InputIterator __first, _Size __n, _Function __f) {
    for (typename __fcxx_algo_size<_Size>::type __i = __n; __i > 0; --__i, (void)++__first) {
        __f(*__first);
    }
    return __first;
}

/* [alg.find], find */
template <class _InputIterator, class _Tp = typename iterator_traits<_InputIterator>::value_type>
[[nodiscard]] constexpr _InputIterator find(_InputIterator __first, _InputIterator __last,
                                                      const _Tp& __value) {
    for (; __first != __last; ++__first) {
        if (bool(*__first == __value)) {
            break;
        }
    }
    return __first;
}

template <class _InputIterator, class _Predicate>
[[nodiscard]] constexpr _InputIterator find_if(_InputIterator __first, _InputIterator __last,
                                                         _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (bool(__pred(*__first))) {
            break;
        }
    }
    return __first;
}

template <class _InputIterator, class _Predicate>
[[nodiscard]] constexpr _InputIterator find_if_not(_InputIterator __first, _InputIterator __last,
                                                             _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (!bool(__pred(*__first))) {
            break;
        }
    }
    return __first;
}

/* [alg.find.end], find end, which tries no more positions than there is
 * room for the sequence at, from the back where it can. */
template <class _It1, class _It2, class _Pred>
constexpr _It1 __fcxx_find_end(_It1 __first1, _It1 __last1, _It2 __first2, _It2 __last2, _Pred& __pred,
                                       forward_iterator_tag) {
    const typename iterator_traits<_It1>::difference_type __len1 = std::distance(__first1, __last1);
    const typename iterator_traits<_It2>::difference_type __len2 = std::distance(__first2, __last2);
    if (__len2 == 0 || __len1 < __len2) {
        return __last1;
    }
    _It1 __result = __last1;
    for (typename iterator_traits<_It1>::difference_type __n = __len1 - __len2; __n >= 0; --__n, (void)++__first1) {
        _It1 __i1 = __first1;
        _It2 __i2 = __first2;
        while (__i2 != __last2 && bool(__pred(*__i1, *__i2))) {
            ++__i1;
            ++__i2;
        }
        if (__i2 == __last2) {
            __result = __first1;
        }
    }
    return __result;
}

template <class _It1, class _It2, class _Pred>
constexpr _It1 __fcxx_find_end(_It1 __first1, _It1 __last1, _It2 __first2, _It2 __last2, _Pred& __pred,
                                       bidirectional_iterator_tag) {
    const typename iterator_traits<_It1>::difference_type __len1 = std::distance(__first1, __last1);
    const typename iterator_traits<_It2>::difference_type __len2 = std::distance(__first2, __last2);
    if (__len2 == 0 || __len1 < __len2) {
        return __last1;
    }
    _It1 __start = __first1;
    std::advance(__start, __len1 - __len2);
    while (true) {
        _It1 __i1 = __start;
        _It2 __i2 = __first2;
        while (__i2 != __last2 && bool(__pred(*__i1, *__i2))) {
            ++__i1;
            ++__i2;
        }
        if (__i2 == __last2) {
            return __start;
        }
        if (__start == __first1) {
            return __last1;
        }
        --__start;
    }
}

template <class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate>
[[nodiscard]] constexpr _ForwardIterator1 find_end(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                                                             _ForwardIterator2 __first2, _ForwardIterator2 __last2,
                                                             _BinaryPredicate __pred) {
    return std::__fcxx_find_end(__first1, __last1, __first2, __last2, __pred,
                                typename iterator_traits<_ForwardIterator1>::iterator_category());
}

template <class _ForwardIterator1, class _ForwardIterator2>
[[nodiscard]] constexpr _ForwardIterator1 find_end(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                                                             _ForwardIterator2 __first2, _ForwardIterator2 __last2) {
    return std::find_end(__first1, __last1, __first2, __last2, __fcxx_equal_op());
}

/* [alg.find.first.of], find first of */
template <class _InputIterator, class _ForwardIterator, class _BinaryPredicate>
[[nodiscard]] constexpr _InputIterator find_first_of(_InputIterator __first1, _InputIterator __last1,
                                                               _ForwardIterator __first2, _ForwardIterator __last2,
                                                               _BinaryPredicate __pred) {
    for (; __first1 != __last1; ++__first1) {
        for (_ForwardIterator __i = __first2; __i != __last2; ++__i) {
            if (bool(__pred(*__first1, *__i))) {
                return __first1;
            }
        }
    }
    return __first1;
}

template <class _InputIterator, class _ForwardIterator>
[[nodiscard]] constexpr _InputIterator find_first_of(_InputIterator __first1, _InputIterator __last1,
                                                               _ForwardIterator __first2, _ForwardIterator __last2) {
    return std::find_first_of(__first1, __last1, __first2, __last2, __fcxx_equal_op());
}

/* [alg.adjacent.find], adjacent find */
template <class _ForwardIterator, class _BinaryPredicate>
[[nodiscard]] constexpr _ForwardIterator adjacent_find(_ForwardIterator __first, _ForwardIterator __last,
                                                                 _BinaryPredicate __pred) {
    if (__first == __last) {
        return __last;
    }
    _ForwardIterator __next = __first;
    while (++__next != __last) {
        if (bool(__pred(*__first, *__next))) {
            return __first;
        }
        __first = __next;
    }
    return __last;
}

template <class _ForwardIterator>
[[nodiscard]] constexpr _ForwardIterator adjacent_find(_ForwardIterator __first, _ForwardIterator __last) {
    return std::adjacent_find(__first, __last, __fcxx_equal_op());
}

/* [alg.count], count */
template <class _InputIterator, class _Tp = typename iterator_traits<_InputIterator>::value_type>
[[nodiscard]] constexpr typename iterator_traits<_InputIterator>::difference_type
count(_InputIterator __first, _InputIterator __last, const _Tp& __value) {
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    for (; __first != __last; ++__first) {
        if (bool(*__first == __value)) {
            ++__n;
        }
    }
    return __n;
}

template <class _InputIterator, class _Predicate>
[[nodiscard]] constexpr typename iterator_traits<_InputIterator>::difference_type
count_if(_InputIterator __first, _InputIterator __last, _Predicate __pred) {
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    for (; __first != __last; ++__first) {
        if (bool(__pred(*__first))) {
            ++__n;
        }
    }
    return __n;
}

/* [alg.mismatch], mismatch */
template <class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
[[nodiscard]] constexpr pair<_InputIterator1, _InputIterator2>
mismatch(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _BinaryPredicate __pred) {
    for (; __first1 != __last1; ++__first1, (void)++__first2) {
        if (!bool(__pred(*__first1, *__first2))) {
            break;
        }
    }
    return pair<_InputIterator1, _InputIterator2>(__first1, __first2);
}

template <class _InputIterator1, class _InputIterator2>
[[nodiscard]] constexpr pair<_InputIterator1, _InputIterator2>
mismatch(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2) {
    return std::mismatch(__first1, __last1, __first2, __fcxx_equal_op());
}

template <class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
[[nodiscard]] constexpr pair<_InputIterator1, _InputIterator2>
mismatch(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2,
         _BinaryPredicate __pred) {
    for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2) {
        if (!bool(__pred(*__first1, *__first2))) {
            break;
        }
    }
    return pair<_InputIterator1, _InputIterator2>(__first1, __first2);
}

template <class _InputIterator1, class _InputIterator2>
[[nodiscard]] constexpr pair<_InputIterator1, _InputIterator2>
mismatch(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2) {
    return std::mismatch(__first1, __last1, __first2, __last2, __fcxx_equal_op());
}

/* [alg.equal], equal. Ranges of integers, through pointers, are compared
 * with memcmp, but in constant evaluation. */
template <class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
[[nodiscard]] constexpr bool equal(_InputIterator1 __first1, _InputIterator1 __last1,
                                             _InputIterator2 __first2, _BinaryPredicate __pred) {
    for (; __first1 != __last1; ++__first1, (void)++__first2) {
        if (!bool(__pred(*__first1, *__first2))) {
            return false;
        }
    }
    return true;
}

template <class _I1, class _I2>
struct __fcxx_memcmp_equal : false_type {};

template <class _Tp, class _Up>
struct __fcxx_memcmp_equal<_Tp*, _Up*>
    : __fcxx_bool_constant<is_same<typename remove_const<_Tp>::type, typename remove_const<_Up>::type>::value
                           && is_integral<_Tp>::value && !is_same<typename remove_const<_Tp>::type, bool>::value
                           && !is_volatile<_Tp>::value && !is_volatile<_Up>::value> {};

template <class _InputIterator1, class _InputIterator2>
constexpr bool __fcxx_equal(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2,
                                    false_type) {
    return std::equal(__first1, __last1, __first2, __fcxx_equal_op());
}

template <class _Tp, class _Up>
constexpr bool __fcxx_equal(_Tp* __first1, _Tp* __last1, _Up* __first2, true_type) {
    if (!std::__fcxx_can_memmove()) {
        return std::equal(__first1, __last1, __first2, __fcxx_equal_op());
    }
    const ptrdiff_t __n = __last1 - __first1;
    return __n <= 0 || __builtin_memcmp(__first1, __first2, sizeof(_Tp) * static_cast<size_t>(__n)) == 0;
}

template <class _InputIterator1, class _InputIterator2>
[[nodiscard]] constexpr bool equal(_InputIterator1 __first1, _InputIterator1 __last1,
                                             _InputIterator2 __first2) {
    return std::__fcxx_equal(__first1, __last1, __first2,
                             __fcxx_memcmp_equal<_InputIterator1, _InputIterator2>());
}

/* Ranges of random access iterators of different lengths are not equal,
 * without comparing any elements. */
template <class _It1, class _It2, class _Pred>
constexpr bool __fcxx_equal4(_It1 __first1, _It1 __last1, _It2 __first2, _It2 __last2, _Pred& __pred,
                                     input_iterator_tag, input_iterator_tag) {
    for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2) {
        if (!bool(__pred(*__first1, *__first2))) {
            return false;
        }
    }
    return __first1 == __last1 && __first2 == __last2;
}

template <class _It1, class _It2, class _Pred>
constexpr bool __fcxx_equal4(_It1 __first1, _It1 __last1, _It2 __first2, _It2 __last2, _Pred& __pred,
                                     random_access_iterator_tag, random_access_iterator_tag) {
    if (__last1 - __first1 != __last2 - __first2) {
        return false;
    }
    return std::equal(__first1, __last1, __first2, __pred);
}

template <class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
[[nodiscard]] constexpr bool equal(_InputIterator1 __first1, _InputIterator1 __last1,
                                             _InputIterator2 __first2, _InputIterator2 __last2,
                                             _BinaryPredicate __pred) {
    return std::__fcxx_equal4(__first1, __last1, __first2, __last2, __pred,
                              typename iterator_traits<_InputIterator1>::iterator_category(),
                              typename iterator_traits<_InputIterator2>::iterator_category());
}

template <class _InputIterator1, class _InputIterator2>
constexpr bool __fcxx_equal4_default(_InputIterator1 __first1, _InputIterator1 __last1,
                                             _InputIterator2 __first2, _InputIterator2 __last2, input_iterator_tag,
                                             input_iterator_tag) {
    __fcxx_equal_op __pred;
    return std::__fcxx_equal4(__first1, __last1, __first2, __last2, __pred, input_iterator_tag(),
                              input_iterator_tag());
}

template <class _InputIterator1, class _InputIterator2>
constexpr bool __fcxx_equal4_default(_InputIterator1 __first1, _InputIterator1 __last1,
                                             _InputIterator2 __first2, _InputIterator2 __last2,
                                             random_access_iterator_tag, random_access_iterator_tag) {
    if (__last1 - __first1 != __last2 - __first2) {
        return false;
    }
    return std::equal(__first1, __last1, __first2);
}

template <class _InputIterator1, class _InputIterator2>
[[nodiscard]] constexpr bool equal(_InputIterator1 __first1, _InputIterator1 __last1,
                                             _InputIterator2 __first2, _InputIterator2 __last2) {
    return std::__fcxx_equal4_default(__first1, __last1, __first2, __last2,
                                      typename iterator_traits<_InputIterator1>::iterator_category(),
                                      typename iterator_traits<_InputIterator2>::iterator_category());
}

/* [alg.is.permutation], is permutation. After the common prefix, each
 * distinct element of the rest of the first range is counted in both
 * ranges, which have the same length, whose difference type the counts
 * have, with the comparisons of the elements of the first range with each
 * other and with those of the second. */
template <class _Dist, class _It1, class _It2, class _Pred11, class _Pred12>
constexpr bool __fcxx_is_permutation_rest(_It1 __first1, _It1 __last1, _It2 __first2, _It2 __last2,
                                                  _Pred11& __pred11, _Pred12& __pred12) {
    for (_It1 __i = __first1; __i != __last1; ++__i) {
        _It1 __j = __first1;
        while (__j != __i && !bool(__pred11(*__j, *__i))) {
            ++__j;
        }
        if (__j != __i) {
            continue;
        }
        _Dist __count = 0;
        for (_It2 __k = __first2; __k != __last2; ++__k) {
            if (bool(__pred12(*__i, *__k))) {
                ++__count;
            }
        }
        if (__count == 0) {
            return false;
        }
        for (_It1 __k = __i; __k != __last1; ++__k) {
            if (bool(__pred11(*__i, *__k))) {
                --__count;
            }
        }
        if (__count != 0) {
            return false;
        }
    }
    return true;
}

template <class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate>
[[nodiscard]] constexpr bool is_permutation(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                                                      _ForwardIterator2 __first2, _BinaryPredicate __pred) {
    for (; __first1 != __last1; ++__first1, (void)++__first2) {
        if (!bool(__pred(*__first1, *__first2))) {
            break;
        }
    }
    if (__first1 == __last1) {
        return true;
    }
    _ForwardIterator2 __last2 = __first2;
    std::advance(__last2, std::distance(__first1, __last1));
    return std::__fcxx_is_permutation_rest<typename iterator_traits<_ForwardIterator1>::difference_type>(
        __first1, __last1, __first2, __last2, __pred, __pred);
}

template <class _ForwardIterator1, class _ForwardIterator2>
[[nodiscard]] constexpr bool is_permutation(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                                                      _ForwardIterator2 __first2) {
    return std::is_permutation(__first1, __last1, __first2, __fcxx_equal_op());
}

template <class _It1, class _It2, class _Pred>
constexpr bool __fcxx_is_permutation4(_It1 __first1, _It1 __last1, _It2 __first2, _It2 __last2,
                                              _Pred& __pred, forward_iterator_tag, forward_iterator_tag) {
    for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2) {
        if (!bool(__pred(*__first1, *__first2))) {
            break;
        }
    }
    if (__first1 == __last1 || __first2 == __last2) {
        return __first1 == __last1 && __first2 == __last2;
    }
    if (std::distance(__first1, __last1) != std::distance(__first2, __last2)) {
        return false;
    }
    return std::__fcxx_is_permutation_rest<typename iterator_traits<_It1>::difference_type>(
        __first1, __last1, __first2, __last2, __pred, __pred);
}

template <class _It1, class _It2, class _Pred>
constexpr bool __fcxx_is_permutation4(_It1 __first1, _It1 __last1, _It2 __first2, _It2 __last2,
                                              _Pred& __pred, random_access_iterator_tag,
                                              random_access_iterator_tag) {
    if (__last1 - __first1 != __last2 - __first2) {
        return false;
    }
    return std::__fcxx_is_permutation4(__first1, __last1, __first2, __last2, __pred, forward_iterator_tag(),
                                       forward_iterator_tag());
}

template <class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate>
[[nodiscard]] constexpr bool is_permutation(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                                                      _ForwardIterator2 __first2, _ForwardIterator2 __last2,
                                                      _BinaryPredicate __pred) {
    return std::__fcxx_is_permutation4(__first1, __last1, __first2, __last2, __pred,
                                       typename iterator_traits<_ForwardIterator1>::iterator_category(),
                                       typename iterator_traits<_ForwardIterator2>::iterator_category());
}

template <class _ForwardIterator1, class _ForwardIterator2>
[[nodiscard]] constexpr bool is_permutation(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                                                      _ForwardIterator2 __first2, _ForwardIterator2 __last2) {
    return std::is_permutation(__first1, __last1, __first2, __last2, __fcxx_equal_op());
}

/* [alg.search], search, which tries each position in turn. */
template <class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate>
[[nodiscard]] constexpr _ForwardIterator1 search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                                                           _ForwardIterator2 __first2, _ForwardIterator2 __last2,
                                                           _BinaryPredicate __pred) {
    if (__first2 == __last2) {
        return __first1;
    }
    for (;; ++__first1) {
        _ForwardIterator1 __i1 = __first1;
        _ForwardIterator2 __i2 = __first2;
        while (true) {
            if (__i1 == __last1) {
                return __last1;
            }
            if (!bool(__pred(*__i1, *__i2))) {
                break;
            }
            ++__i1;
            if (++__i2 == __last2) {
                return __first1;
            }
        }
    }
}

template <class _ForwardIterator1, class _ForwardIterator2>
[[nodiscard]] constexpr _ForwardIterator1 search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                                                           _ForwardIterator2 __first2, _ForwardIterator2 __last2) {
    return std::search(__first1, __last1, __first2, __last2, __fcxx_equal_op());
}

/* The count is not positive, or a sequence of count elements, each of
 * which is tested once. */
template <class _ForwardIterator, class _Size, class _Tp = typename iterator_traits<_ForwardIterator>::value_type,
          class _BinaryPredicate>
[[nodiscard]] constexpr _ForwardIterator search_n(_ForwardIterator __first, _ForwardIterator __last,
                                                            _Size __count, const _Tp& __value,
                                                            _BinaryPredicate __pred) {
    typedef typename __fcxx_algo_size<_Size>::type _Count;
    const _Count __n = __count;
    if (!(__n > 0)) {
        return __first;
    }
    for (; __first != __last; ++__first) {
        if (!bool(__pred(*__first, __value))) {
            continue;
        }
        _ForwardIterator __candidate = __first;
        _Count __m = 1;
        while (true) {
            if (__m == __n) {
                return __candidate;
            }
            if (++__first == __last) {
                return __last;
            }
            if (!bool(__pred(*__first, __value))) {
                break;
            }
            ++__m;
        }
    }
    return __last;
}

template <class _ForwardIterator, class _Size, class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
[[nodiscard]] constexpr _ForwardIterator search_n(_ForwardIterator __first, _ForwardIterator __last,
                                                            _Size __count, const _Tp& __value) {
    return std::search_n(__first, __last, __count, __value, __fcxx_equal_op());
}

template <class _ForwardIterator, class _Searcher>
[[nodiscard]] constexpr _ForwardIterator search(_ForwardIterator __first, _ForwardIterator __last,
                                                          const _Searcher& __searcher) {
    return __searcher(__first, __last).first;
}

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
