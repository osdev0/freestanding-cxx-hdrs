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


/* The sorting and related operations of <algorithm>, of namespace std, but
 * min, max, minmax and clamp, and those that allocate (stable_sort,
 * stable_partition and inplace_merge), which are not freestanding. They are
 * constexpr. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_SORTING_H
#define __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_SORTING_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX
#include <__fcxx/algorithm_impl.h>
#include <__fcxx/algorithm_modifying.h>
#include <__fcxx/algorithm_nonmodifying.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
#include <utility>
#include <compare>
#define _FCXX_ALGO_THREE_WAY 1

namespace std {

/* [alg.sort], sort: an introsort, in O(N log N) comparisons. */
template <class _RandomAccessIterator, class _Compare>
constexpr void sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    std::__fcxx_sort<__fcxx_std_ops>(__first, __last, __comp);
}

template <class _RandomAccessIterator>
constexpr void sort(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __fcxx_less_op __comp;
    std::__fcxx_sort<__fcxx_std_ops>(__first, __last, __comp);
}

/* [partial.sort], partial_sort */
template <class _RandomAccessIterator, class _Compare>
constexpr void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle,
                                    _RandomAccessIterator __last, _Compare __comp) {
    std::__fcxx_partial_sort<__fcxx_std_ops>(__first, __middle, __last, __comp);
}

template <class _RandomAccessIterator>
constexpr void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle,
                                    _RandomAccessIterator __last) {
    __fcxx_less_op __comp;
    std::__fcxx_partial_sort<__fcxx_std_ops>(__first, __middle, __last, __comp);
}

/* [partial.sort.copy], partial_sort_copy */
template <class _InputIterator, class _RandomAccessIterator, class _Compare>
constexpr _RandomAccessIterator partial_sort_copy(_InputIterator __first, _InputIterator __last,
                                                          _RandomAccessIterator __result_first,
                                                          _RandomAccessIterator __result_last, _Compare __comp) {
    return std::__fcxx_partial_sort_copy<__fcxx_std_ops>(__first, __last, __result_first, __result_last, __comp,
                                                         __comp);
}

template <class _InputIterator, class _RandomAccessIterator>
constexpr _RandomAccessIterator partial_sort_copy(_InputIterator __first, _InputIterator __last,
                                                          _RandomAccessIterator __result_first,
                                                          _RandomAccessIterator __result_last) {
    __fcxx_less_op __comp;
    return std::__fcxx_partial_sort_copy<__fcxx_std_ops>(__first, __last, __result_first, __result_last, __comp,
                                                         __comp);
}

/* [is.sorted], is_sorted */
template <class _ForwardIterator, class _Compare>
[[nodiscard]] constexpr _ForwardIterator is_sorted_until(_ForwardIterator __first, _ForwardIterator __last,
                                                                   _Compare __comp) {
    if (__first != __last) {
        _ForwardIterator __next = __first;
        while (++__next != __last) {
            if (bool(__comp(*__next, *__first))) {
                return __next;
            }
            __first = __next;
        }
    }
    return __last;
}

template <class _ForwardIterator>
[[nodiscard]] constexpr _ForwardIterator is_sorted_until(_ForwardIterator __first, _ForwardIterator __last) {
    return std::is_sorted_until(__first, __last, __fcxx_less_op());
}

template <class _ForwardIterator, class _Compare>
[[nodiscard]] constexpr bool is_sorted(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp) {
    return std::is_sorted_until(__first, __last, __comp) == __last;
}

template <class _ForwardIterator>
[[nodiscard]] constexpr bool is_sorted(_ForwardIterator __first, _ForwardIterator __last) {
    return std::is_sorted_until(__first, __last, __fcxx_less_op()) == __last;
}

/* [alg.nth.element], nth_element */
template <class _RandomAccessIterator, class _Compare>
constexpr void nth_element(_RandomAccessIterator __first, _RandomAccessIterator __nth,
                                   _RandomAccessIterator __last, _Compare __comp) {
    std::__fcxx_nth_element<__fcxx_std_ops>(__first, __nth, __last, __comp);
}

template <class _RandomAccessIterator>
constexpr void nth_element(_RandomAccessIterator __first, _RandomAccessIterator __nth,
                                   _RandomAccessIterator __last) {
    __fcxx_less_op __comp;
    std::__fcxx_nth_element<__fcxx_std_ops>(__first, __nth, __last, __comp);
}

/* [alg.binary.search], binary search, which halves the length of the range
 * with each comparison. */
template <class _ForwardIterator, class _Tp, class _Compare>
constexpr _ForwardIterator __fcxx_lower_bound(_ForwardIterator __first,
                                                      typename iterator_traits<_ForwardIterator>::difference_type __len,
                                                      const _Tp& __value, _Compare& __comp) {
    while (__len > 0) {
        const typename iterator_traits<_ForwardIterator>::difference_type __half = __len / 2;
        _ForwardIterator __mid = __first;
        std::advance(__mid, __half);
        if (bool(__comp(*__mid, __value))) {
            __first = ++__mid;
            __len -= __half + 1;
        } else {
            __len = __half;
        }
    }
    return __first;
}

template <class _ForwardIterator, class _Tp, class _Compare>
constexpr _ForwardIterator __fcxx_upper_bound(_ForwardIterator __first,
                                                      typename iterator_traits<_ForwardIterator>::difference_type __len,
                                                      const _Tp& __value, _Compare& __comp) {
    while (__len > 0) {
        const typename iterator_traits<_ForwardIterator>::difference_type __half = __len / 2;
        _ForwardIterator __mid = __first;
        std::advance(__mid, __half);
        if (!bool(__comp(__value, *__mid))) {
            __first = ++__mid;
            __len -= __half + 1;
        } else {
            __len = __half;
        }
    }
    return __first;
}

template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type, class _Compare>
[[nodiscard]] constexpr _ForwardIterator lower_bound(_ForwardIterator __first, _ForwardIterator __last,
                                                               const _Tp& __value, _Compare __comp) {
    return std::__fcxx_lower_bound(__first, std::distance(__first, __last), __value, __comp);
}

template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
[[nodiscard]] constexpr _ForwardIterator lower_bound(_ForwardIterator __first, _ForwardIterator __last,
                                                               const _Tp& __value) {
    __fcxx_less_op __comp;
    return std::__fcxx_lower_bound(__first, std::distance(__first, __last), __value, __comp);
}

template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type, class _Compare>
[[nodiscard]] constexpr _ForwardIterator upper_bound(_ForwardIterator __first, _ForwardIterator __last,
                                                               const _Tp& __value, _Compare __comp) {
    return std::__fcxx_upper_bound(__first, std::distance(__first, __last), __value, __comp);
}

template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
[[nodiscard]] constexpr _ForwardIterator upper_bound(_ForwardIterator __first, _ForwardIterator __last,
                                                               const _Tp& __value) {
    __fcxx_less_op __comp;
    return std::__fcxx_upper_bound(__first, std::distance(__first, __last), __value, __comp);
}

/* An element equivalent to the value is found first, and the bounds are
 * then found on either side of it: at most 2 log N comparisons. */
template <class _ForwardIterator, class _Tp, class _Compare>
constexpr pair<_ForwardIterator, _ForwardIterator> __fcxx_equal_range(_ForwardIterator __first,
                                                                              _ForwardIterator __last,
                                                                              const _Tp& __value, _Compare& __comp) {
    typedef typename iterator_traits<_ForwardIterator>::difference_type _Dist;
    _Dist __len = std::distance(__first, __last);
    while (__len > 0) {
        const _Dist __half = __len / 2;
        _ForwardIterator __mid = __first;
        std::advance(__mid, __half);
        if (bool(__comp(*__mid, __value))) {
            __first = ++__mid;
            __len -= __half + 1;
        } else if (bool(__comp(__value, *__mid))) {
            __len = __half;
        } else {
            _ForwardIterator __right = __mid;
            ++__right;
            return pair<_ForwardIterator, _ForwardIterator>(
                std::__fcxx_lower_bound(__first, __half, __value, __comp),
                std::__fcxx_upper_bound(__right, static_cast<_Dist>(__len - __half - 1), __value, __comp));
        }
    }
    return pair<_ForwardIterator, _ForwardIterator>(__first, __first);
}

template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type, class _Compare>
[[nodiscard]] constexpr pair<_ForwardIterator, _ForwardIterator>
equal_range(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value, _Compare __comp) {
    return std::__fcxx_equal_range(__first, __last, __value, __comp);
}

template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
[[nodiscard]] constexpr pair<_ForwardIterator, _ForwardIterator>
equal_range(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
    __fcxx_less_op __comp;
    return std::__fcxx_equal_range(__first, __last, __value, __comp);
}

template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type, class _Compare>
[[nodiscard]] constexpr bool binary_search(_ForwardIterator __first, _ForwardIterator __last,
                                                     const _Tp& __value, _Compare __comp) {
    __first = std::__fcxx_lower_bound(__first, std::distance(__first, __last), __value, __comp);
    return __first != __last && !bool(__comp(__value, *__first));
}

template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
[[nodiscard]] constexpr bool binary_search(_ForwardIterator __first, _ForwardIterator __last,
                                                     const _Tp& __value) {
    return std::binary_search(__first, __last, __value, __fcxx_less_op());
}

/* [alg.partitions], partitions */
template <class _InputIterator, class _Predicate>
[[nodiscard]] constexpr bool is_partitioned(_InputIterator __first, _InputIterator __last,
                                                      _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (!bool(__pred(*__first))) {
            break;
        }
    }
    if (__first == __last) {
        return true;
    }
    for (++__first; __first != __last; ++__first) {
        if (bool(__pred(*__first))) {
            return false;
        }
    }
    return true;
}

template <class _ForwardIterator, class _Predicate>
constexpr _ForwardIterator partition(_ForwardIterator __first, _ForwardIterator __last, _Predicate __pred) {
    return std::__fcxx_partition<__fcxx_std_ops>(__first, __last, __pred,
                                                 typename iterator_traits<_ForwardIterator>::iterator_category());
}

template <class _InputIterator, class _OutputIterator1, class _OutputIterator2, class _Predicate>
constexpr pair<_OutputIterator1, _OutputIterator2> partition_copy(_InputIterator __first,
                                                                          _InputIterator __last,
                                                                          _OutputIterator1 __out_true,
                                                                          _OutputIterator2 __out_false,
                                                                          _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (bool(__pred(*__first))) {
            *__out_true = *__first;
            ++__out_true;
        } else {
            *__out_false = *__first;
            ++__out_false;
        }
    }
    return pair<_OutputIterator1, _OutputIterator2>(__out_true, __out_false);
}

template <class _ForwardIterator, class _Predicate>
[[nodiscard]] constexpr _ForwardIterator partition_point(_ForwardIterator __first, _ForwardIterator __last,
                                                                   _Predicate __pred) {
    typedef typename iterator_traits<_ForwardIterator>::difference_type _Dist;
    _Dist __len = std::distance(__first, __last);
    while (__len > 0) {
        const _Dist __half = __len / 2;
        _ForwardIterator __mid = __first;
        std::advance(__mid, __half);
        if (bool(__pred(*__mid))) {
            __first = ++__mid;
            __len -= __half + 1;
        } else {
            __len = __half;
        }
    }
    return __first;
}

/* [alg.merge], merge, which takes the element of the first range where the
 * elements are equivalent. */
template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
constexpr _OutputIterator merge(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2,
                                        _InputIterator2 __last2, _OutputIterator __result, _Compare __comp) {
    for (; __first1 != __last1; ++__result) {
        if (__first2 == __last2) {
            return std::copy(__first1, __last1, __result);
        }
        if (bool(__comp(*__first2, *__first1))) {
            *__result = *__first2;
            ++__first2;
        } else {
            *__result = *__first1;
            ++__first1;
        }
    }
    return std::copy(__first2, __last2, __result);
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
constexpr _OutputIterator merge(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2,
                                        _InputIterator2 __last2, _OutputIterator __result) {
    return std::merge(__first1, __last1, __first2, __last2, __result, __fcxx_less_op());
}

/* [alg.set.operations], set operations on sorted structures */
template <class _InputIterator1, class _InputIterator2, class _Compare>
[[nodiscard]] constexpr bool includes(_InputIterator1 __first1, _InputIterator1 __last1,
                                                _InputIterator2 __first2, _InputIterator2 __last2, _Compare __comp) {
    for (; __first2 != __last2; ++__first1) {
        if (__first1 == __last1 || bool(__comp(*__first2, *__first1))) {
            return false;
        }
        if (!bool(__comp(*__first1, *__first2))) {
            ++__first2;
        }
    }
    return true;
}

template <class _InputIterator1, class _InputIterator2>
[[nodiscard]] constexpr bool includes(_InputIterator1 __first1, _InputIterator1 __last1,
                                                _InputIterator2 __first2, _InputIterator2 __last2) {
    return std::includes(__first1, __last1, __first2, __last2, __fcxx_less_op());
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
constexpr _OutputIterator set_union(_InputIterator1 __first1, _InputIterator1 __last1,
                                            _InputIterator2 __first2, _InputIterator2 __last2,
                                            _OutputIterator __result, _Compare __comp) {
    for (; __first1 != __last1; ++__result) {
        if (__first2 == __last2) {
            return std::copy(__first1, __last1, __result);
        }
        if (bool(__comp(*__first2, *__first1))) {
            *__result = *__first2;
            ++__first2;
        } else {
            if (!bool(__comp(*__first1, *__first2))) {
                ++__first2;
            }
            *__result = *__first1;
            ++__first1;
        }
    }
    return std::copy(__first2, __last2, __result);
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
constexpr _OutputIterator set_union(_InputIterator1 __first1, _InputIterator1 __last1,
                                            _InputIterator2 __first2, _InputIterator2 __last2,
                                            _OutputIterator __result) {
    return std::set_union(__first1, __last1, __first2, __last2, __result, __fcxx_less_op());
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
constexpr _OutputIterator set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
                                                   _InputIterator2 __first2, _InputIterator2 __last2,
                                                   _OutputIterator __result, _Compare __comp) {
    while (__first1 != __last1 && __first2 != __last2) {
        if (bool(__comp(*__first1, *__first2))) {
            ++__first1;
        } else {
            if (!bool(__comp(*__first2, *__first1))) {
                *__result = *__first1;
                ++__result;
                ++__first1;
            }
            ++__first2;
        }
    }
    return __result;
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
constexpr _OutputIterator set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
                                                   _InputIterator2 __first2, _InputIterator2 __last2,
                                                   _OutputIterator __result) {
    return std::set_intersection(__first1, __last1, __first2, __last2, __result, __fcxx_less_op());
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
constexpr _OutputIterator set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
                                                 _InputIterator2 __first2, _InputIterator2 __last2,
                                                 _OutputIterator __result, _Compare __comp) {
    while (__first1 != __last1) {
        if (__first2 == __last2) {
            return std::copy(__first1, __last1, __result);
        }
        if (bool(__comp(*__first1, *__first2))) {
            *__result = *__first1;
            ++__result;
            ++__first1;
        } else {
            if (!bool(__comp(*__first2, *__first1))) {
                ++__first1;
            }
            ++__first2;
        }
    }
    return __result;
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
constexpr _OutputIterator set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
                                                 _InputIterator2 __first2, _InputIterator2 __last2,
                                                 _OutputIterator __result) {
    return std::set_difference(__first1, __last1, __first2, __last2, __result, __fcxx_less_op());
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
constexpr _OutputIterator set_symmetric_difference(_InputIterator1 __first1, _InputIterator1 __last1,
                                                           _InputIterator2 __first2, _InputIterator2 __last2,
                                                           _OutputIterator __result, _Compare __comp) {
    while (__first1 != __last1) {
        if (__first2 == __last2) {
            return std::copy(__first1, __last1, __result);
        }
        if (bool(__comp(*__first1, *__first2))) {
            *__result = *__first1;
            ++__result;
            ++__first1;
        } else {
            if (bool(__comp(*__first2, *__first1))) {
                *__result = *__first2;
                ++__result;
            } else {
                ++__first1;
            }
            ++__first2;
        }
    }
    return std::copy(__first2, __last2, __result);
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
constexpr _OutputIterator set_symmetric_difference(_InputIterator1 __first1, _InputIterator1 __last1,
                                                           _InputIterator2 __first2, _InputIterator2 __last2,
                                                           _OutputIterator __result) {
    return std::set_symmetric_difference(__first1, __last1, __first2, __last2, __result, __fcxx_less_op());
}

/* [alg.heap.operations], heap operations */
template <class _RandomAccessIterator, class _Compare>
constexpr void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    std::__fcxx_push_heap<__fcxx_std_ops>(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator>
constexpr void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __fcxx_less_op __comp;
    std::__fcxx_push_heap<__fcxx_std_ops>(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator, class _Compare>
constexpr void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    std::__fcxx_pop_heap<__fcxx_std_ops>(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator>
constexpr void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __fcxx_less_op __comp;
    std::__fcxx_pop_heap<__fcxx_std_ops>(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator, class _Compare>
constexpr void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    std::__fcxx_make_heap<__fcxx_std_ops>(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator>
constexpr void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __fcxx_less_op __comp;
    std::__fcxx_make_heap<__fcxx_std_ops>(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator, class _Compare>
constexpr void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    std::__fcxx_sort_heap<__fcxx_std_ops>(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator>
constexpr void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __fcxx_less_op __comp;
    std::__fcxx_sort_heap<__fcxx_std_ops>(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator, class _Compare>
[[nodiscard]] constexpr _RandomAccessIterator is_heap_until(_RandomAccessIterator __first,
                                                                      _RandomAccessIterator __last,
                                                                      _Compare __comp) {
    return std::__fcxx_is_heap_until(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator>
[[nodiscard]] constexpr _RandomAccessIterator is_heap_until(_RandomAccessIterator __first,
                                                                      _RandomAccessIterator __last) {
    __fcxx_less_op __comp;
    return std::__fcxx_is_heap_until(__first, __last - __first, __comp);
}

template <class _RandomAccessIterator, class _Compare>
[[nodiscard]] constexpr bool is_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                               _Compare __comp) {
    return std::__fcxx_is_heap_until(__first, __last - __first, __comp) == __last;
}

template <class _RandomAccessIterator>
[[nodiscard]] constexpr bool is_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __fcxx_less_op __comp;
    return std::__fcxx_is_heap_until(__first, __last - __first, __comp) == __last;
}

/* [alg.min.max], the least and the greatest elements. min, max and minmax
 * are in <__fcxx/algorithm_minmax.h>. */
template <class _ForwardIterator, class _Compare>
[[nodiscard]] constexpr _ForwardIterator min_element(_ForwardIterator __first, _ForwardIterator __last,
                                                               _Compare __comp) {
    if (__first == __last) {
        return __last;
    }
    _ForwardIterator __m = __first;
    while (++__first != __last) {
        if (bool(__comp(*__first, *__m))) {
            __m = __first;
        }
    }
    return __m;
}

template <class _ForwardIterator>
[[nodiscard]] constexpr _ForwardIterator min_element(_ForwardIterator __first, _ForwardIterator __last) {
    return std::min_element(__first, __last, __fcxx_less_op());
}

template <class _ForwardIterator, class _Compare>
[[nodiscard]] constexpr _ForwardIterator max_element(_ForwardIterator __first, _ForwardIterator __last,
                                                               _Compare __comp) {
    if (__first == __last) {
        return __last;
    }
    _ForwardIterator __m = __first;
    while (++__first != __last) {
        if (bool(__comp(*__m, *__first))) {
            __m = __first;
        }
    }
    return __m;
}

template <class _ForwardIterator>
[[nodiscard]] constexpr _ForwardIterator max_element(_ForwardIterator __first, _ForwardIterator __last) {
    return std::max_element(__first, __last, __fcxx_less_op());
}

/* The first of the least elements and the last of the greatest, found by
 * taking the elements by pairs: three comparisons for each two elements,
 * but the first two, which take one. */
template <class _ForwardIterator, class _Compare>
[[nodiscard]] constexpr pair<_ForwardIterator, _ForwardIterator>
minmax_element(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp) {
    _ForwardIterator __min = __first;
    _ForwardIterator __max = __first;
    if (__first == __last || ++__first == __last) {
        return pair<_ForwardIterator, _ForwardIterator>(__min, __max);
    }
    if (bool(__comp(*__first, *__min))) {
        __min = __first;
    } else {
        __max = __first;
    }
    while (++__first != __last) {
        _ForwardIterator __i = __first;
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
    return pair<_ForwardIterator, _ForwardIterator>(__min, __max);
}

template <class _ForwardIterator>
[[nodiscard]] constexpr pair<_ForwardIterator, _ForwardIterator> minmax_element(_ForwardIterator __first,
                                                                                          _ForwardIterator __last) {
    return std::minmax_element(__first, __last, __fcxx_less_op());
}

/* [alg.lex.comparison], lexicographical comparison */
template <class _InputIterator1, class _InputIterator2, class _Compare>
[[nodiscard]] constexpr bool lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1,
                                                               _InputIterator2 __first2, _InputIterator2 __last2,
                                                               _Compare __comp) {
    for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2) {
        if (bool(__comp(*__first1, *__first2))) {
            return true;
        }
        if (bool(__comp(*__first2, *__first1))) {
            return false;
        }
    }
    return __first1 == __last1 && __first2 != __last2;
}

template <class _InputIterator1, class _InputIterator2>
[[nodiscard]] constexpr bool lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1,
                                                               _InputIterator2 __first2, _InputIterator2 __last2) {
    return std::lexicographical_compare(__first1, __last1, __first2, __last2, __fcxx_less_op());
}

#if _FCXX_ALGO_THREE_WAY
/* [alg.three.way], three-way comparison algorithms */
template <class _InputIterator1, class _InputIterator2, class _Cmp>
[[nodiscard]] constexpr auto lexicographical_compare_three_way(_InputIterator1 __b1, _InputIterator1 __e1,
                                                                 _InputIterator2 __b2, _InputIterator2 __e2,
                                                                 _Cmp __comp) -> decltype(__comp(*__b1, *__b2)) {
    using _Rp = decltype(__comp(*__b1, *__b2));
    static_assert(__fcxx_cmp_category_rank<_Rp>::value != 0,
                  "std::lexicographical_compare_three_way takes a comparison giving a comparison category type");
    bool __end1 = __b1 == __e1;
    bool __end2 = __b2 == __e2;
    while (!__end1 && !__end2) {
        _Rp __c = __comp(*__b1, *__b2);
        if (__c != 0) {
            return __c;
        }
        __end1 = ++__b1 == __e1;
        __end2 = ++__b2 == __e2;
    }
    return !__end1 ? strong_ordering::greater : !__end2 ? strong_ordering::less : strong_ordering::equal;
}

template <class _InputIterator1, class _InputIterator2>
[[nodiscard]] constexpr auto lexicographical_compare_three_way(_InputIterator1 __b1, _InputIterator1 __e1,
                                                                 _InputIterator2 __b2, _InputIterator2 __e2) {
    return std::lexicographical_compare_three_way(__b1, __e1, __b2, __e2, compare_three_way());
}
#endif

/* [alg.permutation.generators], permutation generators, the previous
 * permutation being the next one in the reverse order. */
template <class _BidirectionalIterator, class _Compare>
constexpr bool next_permutation(_BidirectionalIterator __first, _BidirectionalIterator __last,
                                        _Compare __comp) {
    return std::__fcxx_next_permutation<__fcxx_std_ops>(__first, __last, __comp);
}

template <class _BidirectionalIterator>
constexpr bool next_permutation(_BidirectionalIterator __first, _BidirectionalIterator __last) {
    __fcxx_less_op __comp;
    return std::__fcxx_next_permutation<__fcxx_std_ops>(__first, __last, __comp);
}

template <class _BidirectionalIterator, class _Compare>
constexpr bool prev_permutation(_BidirectionalIterator __first, _BidirectionalIterator __last,
                                        _Compare __comp) {
    __fcxx_swapped_comp<_Compare> __swapped(__comp);
    return std::__fcxx_next_permutation<__fcxx_std_ops>(__first, __last, __swapped);
}

template <class _BidirectionalIterator>
constexpr bool prev_permutation(_BidirectionalIterator __first, _BidirectionalIterator __last) {
    __fcxx_less_op __comp;
    __fcxx_swapped_comp<__fcxx_less_op> __swapped(__comp);
    return std::__fcxx_next_permutation<__fcxx_std_ops>(__first, __last, __swapped);
}

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
