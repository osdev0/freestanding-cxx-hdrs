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


/* What the algorithms of <algorithm> share: iter_swap, the function objects
 * of the operators the algorithms use by default, and the implementations of
 * the algorithms that the algorithms of namespace std and those of namespace
 * ranges both use.
 *
 * Those implementations are templates on a policy of access to the
 * elements, which moves and swaps them as the algorithms of namespace std do
 * (with std::move and std::iter_swap), or as those of namespace ranges do
 * (with ranges::iter_move and ranges::iter_swap), and on a comparison, which
 * is the comparison of the algorithm, or, for the algorithms of namespace
 * ranges, an object applying the projection to the arguments of the
 * comparison. They take their comparisons by reference, so as not to copy
 * them. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_IMPL_H
#define __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_IMPL_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
/* swap and pair. */
#include <utility>
#include <type_traits>

namespace std {

/* [alg.swap], iter_swap */
template <class _I1, class _I2>
constexpr void iter_swap(_I1 __a, _I2 __b) {
    using std::swap;
    swap(*__a, *__b);
}

/* The operators that the algorithms use when they are given no function
 * object. */
struct __fcxx_less_op {
    template <class _Tp, class _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const {
        return bool(static_cast<_Tp&&>(__t) < static_cast<_Up&&>(__u));
    }
};

struct __fcxx_equal_op {
    template <class _Tp, class _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const {
        return bool(static_cast<_Tp&&>(__t) == static_cast<_Up&&>(__u));
    }
};

/* A comparison with its arguments swapped, which the permutation generators
 * and the searches from the end use. */
template <class _Comp>
struct __fcxx_swapped_comp {
    _Comp& __comp;

    constexpr explicit __fcxx_swapped_comp(_Comp& __c) : __comp(__c) {}

    template <class _Tp, class _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const {
        return bool(__comp(static_cast<_Up&&>(__u), static_cast<_Tp&&>(__t)));
    }
};

/* The type that the Size arguments of the algorithms are counted down in:
 * that of the argument if it is an integer type, and ptrdiff_t, which the
 * argument is converted to, otherwise (for floating-point types, and for
 * class types that convert to integer types). */
template <class _Size>
struct __fcxx_algo_size {
    typedef __fcxx_conditional_t<is_integral<_Size>::value && !is_same<_Size, bool>::value, _Size, ptrdiff_t> type;
};

/* The types of an iterator, and the moves and swaps of the elements, as
 * the algorithms of namespace std do them: moving the elements with
 * std::move, of what the iterator refers to, or of its value when it gives
 * a prvalue, and swapping them with std::iter_swap. */
template <class _It, class _Ref = decltype(*std::declval<_It&>())>
struct __fcxx_std_rvalue {
    typedef __fcxx_conditional_t<is_lvalue_reference<_Ref>::value, typename remove_reference<_Ref>::type&&, _Ref> type;
};

struct __fcxx_std_ops {
    template <class _It>
    struct __value {
        typedef typename iterator_traits<_It>::value_type type;
    };

    template <class _It>
    struct __difference {
        typedef typename iterator_traits<_It>::difference_type type;
    };

    template <class _It>
    static constexpr typename __fcxx_std_rvalue<_It>::type __move(_It& __i) {
        return static_cast<typename __fcxx_std_rvalue<_It>::type>(*__i);
    }

    template <class _I1, class _I2>
    static constexpr void __swap(_I1& __a, _I2& __b) {
        std::iter_swap(__a, __b);
    }
};

/* Reversing a range, with the swaps of the policy, which is also what
 * rotating one does. */
template <class _Ops, class _It>
constexpr void __fcxx_reverse(_It __first, _It __last, bidirectional_iterator_tag) {
    while (__first != __last) {
        --__last;
        if (__first == __last) {
            break;
        }
        _Ops::__swap(__first, __last);
        ++__first;
    }
}

template <class _Ops, class _It>
constexpr void __fcxx_reverse(_It __first, _It __last, random_access_iterator_tag) {
    if (__first == __last) {
        return;
    }
    --__last;
    while (__first < __last) {
        _Ops::__swap(__first, __last);
        ++__first;
        --__last;
    }
}

/* [alg.rotate], rotating a range, where the elements of the front go
 * behind those of the back. Of forward iterators, the first elements of the
 * back are swapped with those of the front, as far as they go, repeatedly;
 * of bidirectional iterators, both parts are reversed, and then the whole.
 * Both take no more swaps than there are elements. */
template <class _Ops, class _It>
constexpr _It __fcxx_rotate(_It __first, _It __middle, _It __last, forward_iterator_tag) {
    if (__first == __middle) {
        return __last;
    }
    if (__middle == __last) {
        return __first;
    }
    _It __next = __middle;
    do {
        _Ops::__swap(__first, __next);
        ++__first;
        ++__next;
        if (__first == __middle) {
            __middle = __next;
        }
    } while (__next != __last);
    _It __ret = __first;
    __next = __middle;
    while (__next != __last) {
        _Ops::__swap(__first, __next);
        ++__first;
        ++__next;
        if (__first == __middle) {
            __middle = __next;
        } else if (__next == __last) {
            __next = __middle;
        }
    }
    return __ret;
}

template <class _Ops, class _It>
constexpr _It __fcxx_rotate(_It __first, _It __middle, _It __last, bidirectional_iterator_tag) {
    if (__first == __middle) {
        return __last;
    }
    if (__middle == __last) {
        return __first;
    }
    std::__fcxx_reverse<_Ops>(__first, __middle, bidirectional_iterator_tag());
    std::__fcxx_reverse<_Ops>(__middle, __last, bidirectional_iterator_tag());
    /* The whole is reversed from both ends until one reaches the middle,
     * which leaves the rest of the other part to reverse, and tells where
     * the first element went. */
    while (__first != __middle && __middle != __last) {
        --__last;
        _Ops::__swap(__first, __last);
        ++__first;
    }
    if (__first == __middle) {
        std::__fcxx_reverse<_Ops>(__middle, __last, bidirectional_iterator_tag());
        return __last;
    }
    std::__fcxx_reverse<_Ops>(__first, __middle, bidirectional_iterator_tag());
    return __first;
}

/* [alg.shift], shifting a range of forward iterators by n to the right,
 * given the iterator n elements past the first, which is not the end. The
 * first n elements form a circular buffer of those that are still to be
 * moved, from which each element in turn goes to its place, by a swap,
 * which puts the element that was there at the back of the buffer. Returns
 * the end. */
template <class _Ops, class _It, class _Sp>
constexpr _It __fcxx_shift_right_forward(_It __first, _It __buf_end, _Sp __last) {
    _It __buf = __first;
    _It __lead = __buf_end;
    while (__lead != __last) {
        _Ops::__swap(__buf, __lead);
        ++__lead;
        ++__buf;
        if (__buf == __buf_end) {
            __buf = __first;
        }
    }
    return __lead;
}

/* [alg.partitions], partition, of forward iterators with no more swaps than
 * elements, and of bidirectional iterators with no more than half as many,
 * applying the predicate once to each element. */
template <class _Ops, class _It, class _Pred>
constexpr _It __fcxx_partition(_It __first, _It __last, _Pred& __pred, forward_iterator_tag) {
    while (true) {
        if (__first == __last) {
            return __first;
        }
        if (!bool(__pred(*__first))) {
            break;
        }
        ++__first;
    }
    _It __i = __first;
    for (++__i; __i != __last; ++__i) {
        if (bool(__pred(*__i))) {
            _Ops::__swap(__i, __first);
            ++__first;
        }
    }
    return __first;
}

template <class _Ops, class _It, class _Pred>
constexpr _It __fcxx_partition(_It __first, _It __last, _Pred& __pred, bidirectional_iterator_tag) {
    while (true) {
        while (true) {
            if (__first == __last) {
                return __first;
            }
            if (!bool(__pred(*__first))) {
                break;
            }
            ++__first;
        }
        do {
            --__last;
            if (__first == __last) {
                return __first;
            }
        } while (!bool(__pred(*__last)));
        _Ops::__swap(__first, __last);
        ++__first;
    }
}

/* [alg.permutation.generators], the next permutation, in the order of the
 * comparison, which gives the previous one with the comparison swapped. */
template <class _Ops, class _It, class _Comp>
constexpr bool __fcxx_next_permutation(_It __first, _It __last, _Comp& __comp) {
    if (__first == __last) {
        return false;
    }
    _It __i = __last;
    --__i;
    if (__i == __first) {
        return false;
    }
    while (true) {
        _It __ii = __i;
        --__i;
        if (bool(__comp(*__i, *__ii))) {
            _It __j = __last;
            do {
                --__j;
            } while (!bool(__comp(*__i, *__j)));
            _Ops::__swap(__i, __j);
            std::__fcxx_reverse<_Ops>(__ii, __last, bidirectional_iterator_tag());
            return true;
        }
        if (__i == __first) {
            std::__fcxx_reverse<_Ops>(__first, __last, bidirectional_iterator_tag());
            return false;
        }
    }
}

/* [alg.heap.operations], the heaps, of random access iterators, whose
 * elements are each not less than their children, those at 2 * i + 1 and
 * 2 * i + 2. An element is moved down to its place, from start, through a
 * hole, by moving the greater of the children up for as long as it is
 * greater than the element: two comparisons for each level. */
template <class _Ops, class _It, class _Dist, class _Comp>
constexpr void __fcxx_sift_down(_It __first, _Dist __len, _Dist __start, _Comp& __comp) {
    if (__len < 2 || (__len - 2) / 2 < __start) {
        return;
    }
    _Dist __child = 2 * __start + 1;
    _It __child_it = __first + __child;
    if (__child + 1 < __len) {
        _It __right = __child_it + 1;
        if (bool(__comp(*__child_it, *__right))) {
            __child_it = __right;
            ++__child;
        }
    }
    _It __hole = __first + __start;
    if (bool(__comp(*__child_it, *__hole))) {
        return;
    }
    typename _Ops::template __value<_It>::type __tmp(_Ops::__move(__hole));
    do {
        *__hole = _Ops::__move(__child_it);
        __hole = __child_it;
        __start = __child;
        if ((__len - 2) / 2 < __start) {
            break;
        }
        __child = 2 * __start + 1;
        __child_it = __first + __child;
        if (__child + 1 < __len) {
            _It __right = __child_it + 1;
            if (bool(__comp(*__child_it, *__right))) {
                __child_it = __right;
                ++__child;
            }
        }
    } while (!bool(__comp(*__child_it, __tmp)));
    *__hole = std::move(__tmp);
}

/* [push.heap], moving the last element of a range up to its place in the
 * heap before it, with one comparison for each level. */
template <class _Ops, class _It, class _Dist, class _Comp>
constexpr void __fcxx_push_heap(_It __first, _Dist __len, _Comp& __comp) {
    if (__len < 2) {
        return;
    }
    _Dist __index = __len - 1;
    _Dist __parent = (__index - 1) / 2;
    _It __hole = __first + __index;
    _It __parent_it = __first + __parent;
    if (!bool(__comp(*__parent_it, *__hole))) {
        return;
    }
    typename _Ops::template __value<_It>::type __tmp(_Ops::__move(__hole));
    do {
        *__hole = _Ops::__move(__parent_it);
        __hole = __parent_it;
        __index = __parent;
        if (__index == 0) {
            break;
        }
        __parent = (__index - 1) / 2;
        __parent_it = __first + __parent;
    } while (bool(__comp(*__parent_it, __tmp)));
    *__hole = std::move(__tmp);
}

/* [pop.heap] */
template <class _Ops, class _It, class _Dist, class _Comp>
constexpr void __fcxx_pop_heap(_It __first, _Dist __len, _Comp& __comp) {
    if (__len > 1) {
        _It __back = __first + (__len - 1);
        _Ops::__swap(__first, __back);
        std::__fcxx_sift_down<_Ops>(__first, static_cast<_Dist>(__len - 1), static_cast<_Dist>(0), __comp);
    }
}

/* [make.heap], by moving each element that has children down to its place,
 * from the last one: no more than two comparisons for each element. */
template <class _Ops, class _It, class _Dist, class _Comp>
constexpr void __fcxx_make_heap(_It __first, _Dist __len, _Comp& __comp) {
    if (__len < 2) {
        return;
    }
    for (_Dist __start = (__len - 2) / 2;; --__start) {
        std::__fcxx_sift_down<_Ops>(__first, __len, __start, __comp);
        if (__start == 0) {
            break;
        }
    }
}

/* [sort.heap] */
template <class _Ops, class _It, class _Dist, class _Comp>
constexpr void __fcxx_sort_heap(_It __first, _Dist __len, _Comp& __comp) {
    for (; __len > 1; --__len) {
        std::__fcxx_pop_heap<_Ops>(__first, __len, __comp);
    }
}

/* [is.heap], the end of the longest heap at the front of a range. The
 * parent of the child c is (c - 1) / 2, which moves on after each even
 * child. */
template <class _It, class _Dist, class _Comp>
constexpr _It __fcxx_is_heap_until(_It __first, _Dist __len, _Comp& __comp) {
    _Dist __parent = 0;
    for (_Dist __child = 1; __child < __len; ++__child) {
        if (bool(__comp(*(__first + __parent), *(__first + __child)))) {
            return __first + __child;
        }
        if (__child % 2 == 0) {
            ++__parent;
        }
    }
    return __first + __len;
}

/* [partial.sort], by keeping the least elements seen in a heap, whose top
 * is the greatest of them, which each lesser element replaces, and sorting
 * the heap at the end: about N log M comparisons. */
template <class _Ops, class _It, class _Comp>
constexpr void __fcxx_partial_sort(_It __first, _It __middle, _It __last, _Comp& __comp) {
    typedef typename _Ops::template __difference<_It>::type _Dist;
    const _Dist __len = __middle - __first;
    if (__len == 0) {
        return;
    }
    std::__fcxx_make_heap<_Ops>(__first, __len, __comp);
    for (_It __i = __middle; __i != __last; ++__i) {
        if (bool(__comp(*__i, *__first))) {
            _Ops::__swap(__i, __first);
            std::__fcxx_sift_down<_Ops>(__first, __len, static_cast<_Dist>(0), __comp);
        }
    }
    std::__fcxx_sort_heap<_Ops>(__first, __len, __comp);
}

/* [partial.sort.copy], the same, with the heap in the output range, and a
 * comparison of the elements of the input with those of the output. Returns
 * the end of the elements copied to the output. */
template <class _Ops, class _In, class _Sp, class _It, class _Sp2, class _Comp12, class _Comp22>
constexpr _It __fcxx_partial_sort_copy(_In& __first, _Sp __last, _It __result_first, _Sp2 __result_last,
                                               _Comp12& __comp12, _Comp22& __comp22) {
    typedef typename _Ops::template __difference<_It>::type _Dist;
    _It __r = __result_first;
    for (; __first != __last && __r != __result_last; ++__first, (void)++__r) {
        *__r = *__first;
    }
    const _Dist __len = __r - __result_first;
    if (__len == 0) {
        return __r;
    }
    std::__fcxx_make_heap<_Ops>(__result_first, __len, __comp22);
    for (; __first != __last; ++__first) {
        if (bool(__comp12(*__first, *__result_first))) {
            *__result_first = *__first;
            std::__fcxx_sift_down<_Ops>(__result_first, __len, static_cast<_Dist>(0), __comp22);
        }
    }
    std::__fcxx_sort_heap<_Ops>(__result_first, __len, __comp22);
    return __r;
}

/* [alg.sort], the insertion sort of short ranges, which moves each element
 * back to its place. Where the element is less than the first, the whole
 * front is moved; otherwise, the first is not greater than it, and the
 * search for its place stops there at the latest. */
template <class _Ops, class _It, class _Comp>
constexpr void __fcxx_insertion_sort(_It __first, _It __last, _Comp& __comp) {
    if (__first == __last) {
        return;
    }
    _It __i = __first;
    for (++__i; __i != __last; ++__i) {
        _It __k = __i;
        --__k;
        if (!bool(__comp(*__i, *__k))) {
            continue;
        }
        typename _Ops::template __value<_It>::type __tmp(_Ops::__move(__i));
        _It __j = __i;
        if (bool(__comp(__tmp, *__first))) {
            while (__j != __first) {
                __k = __j;
                --__k;
                *__j = _Ops::__move(__k);
                __j = __k;
            }
        } else {
            do {
                *__j = _Ops::__move(__k);
                __j = __k;
                --__k;
            } while (bool(__comp(__tmp, *__k)));
        }
        *__j = std::move(__tmp);
    }
}

/* Sorts three elements, leaving the median in the second. */
template <class _Ops, class _It, class _Comp>
constexpr void __fcxx_sort3(_It __a, _It __b, _It __c, _Comp& __comp) {
    if (bool(__comp(*__b, *__a))) {
        _Ops::__swap(__a, __b);
    }
    if (bool(__comp(*__c, *__b))) {
        _Ops::__swap(__b, __c);
        if (bool(__comp(*__b, *__a))) {
            _Ops::__swap(__a, __b);
        }
    }
}

/* The ranges longer than this take the median of three medians of three
 * as the pivot, which is less easily fooled by patterns, such as organ
 * pipes, than the median of three. */
enum { __fcxx_ninther_threshold = 128 };

/* Partitions a range of more than three elements around a pivot, which
 * ends up at the returned position, with the elements before it not
 * greater, and those after it not less. The elements equal to it are
 * spread over both sides, which keeps the partitions balanced where there
 * are many of them. The pivot is the median of the second, middle and last
 * elements, whose least and greatest, left in the second and last places,
 * stop the scans; or, of long ranges, the median of the medians of three
 * groups of three, whose greatest stops the scan up, and the pivot itself,
 * in the first place, the scan down. */
template <class _Ops, class _It, class _Comp>
constexpr _It __fcxx_partition_pivot(_It __first, _It __last, _Comp& __comp) {
    typedef typename _Ops::template __difference<_It>::type _Dist;
    const _Dist __len = __last - __first;
    _It __mid = __first + __len / 2;
    _It __i = __first;
    _It __j = __last;
    if (__len > static_cast<_Dist>(__fcxx_ninther_threshold)) {
        const _Dist __step = __len / 8;
        _It __a = __first + 1;
        _It __c = __last - 1;
        std::__fcxx_sort3<_Ops>(__a, __a + __step, __a + 2 * __step, __comp);
        std::__fcxx_sort3<_Ops>(__mid - __step, __mid, __mid + __step, __comp);
        std::__fcxx_sort3<_Ops>(__c - 2 * __step, __c - __step, __c, __comp);
        std::__fcxx_sort3<_Ops>(__a + __step, __mid, __c - __step, __comp);
    } else {
        ++__i;
        --__j;
        std::__fcxx_sort3<_Ops>(__i, __mid, __j, __comp);
    }
    _Ops::__swap(__first, __mid);
    while (true) {
        do {
            ++__i;
        } while (bool(__comp(*__i, *__first)));
        do {
            --__j;
        } while (bool(__comp(*__first, *__j)));
        if (!(__i < __j)) {
            break;
        }
        _Ops::__swap(__i, __j);
    }
    _Ops::__swap(__first, __j);
    return __j;
}

/* The ranges of up to this many elements are left to the insertion sort. */
enum { __fcxx_sort_threshold = 16 };

/* Twice the base 2 logarithm of a length, which bounds how deep the
 * introsort partitions before it falls back to the heapsort. */
template <class _Dist>
constexpr _Dist __fcxx_sort_depth(_Dist __len) {
    _Dist __depth = 0;
    for (; __len > 1; __len /= 2) {
        __depth += 2;
    }
    return __depth;
}

/* The introsort: a quicksort with the median of three as the pivot, which
 * sorts the ranges too deep in the recursion with the heapsort, and leaves
 * the short ones to a final insertion sort, where no element has far to go.
 * It recurses into the shorter side only, which bounds the recursion to
 * log N levels. */
template <class _Ops, class _It, class _Dist, class _Comp>
constexpr void __fcxx_introsort_loop(_It __first, _It __last, _Dist __depth, _Comp& __comp) {
    while (__last - __first > static_cast<_Dist>(__fcxx_sort_threshold)) {
        if (__depth == 0) {
            const _Dist __len = __last - __first;
            std::__fcxx_make_heap<_Ops>(__first, __len, __comp);
            std::__fcxx_sort_heap<_Ops>(__first, __len, __comp);
            return;
        }
        --__depth;
        _It __cut = std::__fcxx_partition_pivot<_Ops>(__first, __last, __comp);
        _It __right = __cut;
        ++__right;
        if (__cut - __first < __last - __right) {
            std::__fcxx_introsort_loop<_Ops>(__first, __cut, __depth, __comp);
            __first = __right;
        } else {
            std::__fcxx_introsort_loop<_Ops>(__right, __last, __depth, __comp);
            __last = __cut;
        }
    }
}

template <class _Ops, class _It, class _Comp>
constexpr void __fcxx_sort(_It __first, _It __last, _Comp& __comp) {
    typedef typename _Ops::template __difference<_It>::type _Dist;
    const _Dist __len = __last - __first;
    if (__len < 2) {
        return;
    }
    std::__fcxx_introsort_loop<_Ops>(__first, __last, std::__fcxx_sort_depth(__len), __comp);
    std::__fcxx_insertion_sort<_Ops>(__first, __last, __comp);
}

/* [alg.nth.element], by partitioning the side that the nth position is in,
 * with the same pivots as the introsort, which is linear on average; too
 * deep, it falls back to the partial sort. */
template <class _Ops, class _It, class _Comp>
constexpr void __fcxx_nth_element(_It __first, _It __nth, _It __last, _Comp& __comp) {
    typedef typename _Ops::template __difference<_It>::type _Dist;
    if (__nth == __last) {
        return;
    }
    _Dist __depth = std::__fcxx_sort_depth(static_cast<_Dist>(__last - __first));
    while (__last - __first > 3) {
        if (__depth == 0) {
            _It __end = __nth;
            ++__end;
            std::__fcxx_partial_sort<_Ops>(__first, __end, __last, __comp);
            return;
        }
        --__depth;
        _It __cut = std::__fcxx_partition_pivot<_Ops>(__first, __last, __comp);
        if (__cut == __nth) {
            return;
        }
        if (__nth < __cut) {
            __last = __cut;
        } else {
            __first = __cut;
            ++__first;
        }
    }
    std::__fcxx_insertion_sort<_Ops>(__first, __last, __comp);
}

/* Whether a copy or a move of a range of elements through pointers can be
 * done with memmove: where the elements have the same type, are not
 * volatile, and are trivially copyable, and where the assignment that would
 * copy or move them is trivial. */
template <class _In, class _Out, class _Src>
struct __fcxx_memmove_assignable : false_type {};

template <class _Tp, class _Up, class _Src>
struct __fcxx_memmove_assignable<_Tp*, _Up*, _Src>
    : __fcxx_bool_constant<is_same<typename remove_const<_Tp>::type, _Up>::value && !is_volatile<_Tp>::value
                           && !is_volatile<_Up>::value && is_trivially_copyable<_Up>::value
                           && is_trivially_assignable<_Up&, _Src>::value> {};

template <class _In, class _Out>
struct __fcxx_memmove_copyable : __fcxx_memmove_assignable<_In, _Out, typename iterator_traits<_In>::reference> {};

template <class _In, class _Out>
struct __fcxx_memmove_movable
    : __fcxx_memmove_assignable<_In, _Out,
                                typename remove_reference<typename iterator_traits<_In>::reference>::type&&> {};

/* Whether memmove can be used, which it cannot in constant evaluation. */
constexpr bool __fcxx_can_memmove() noexcept {
    return !__builtin_is_constant_evaluated();
}

template <class _Tp, class _Up>
_Up* __fcxx_memmove_forward(_Tp* __first, _Tp* __last, _Up* __result) {
    const ptrdiff_t __n = __last - __first;
    if (__n > 0) {
        __builtin_memmove(__result, __first, sizeof(_Up) * static_cast<size_t>(__n));
    }
    return __result + __n;
}

template <class _Tp, class _Up>
_Up* __fcxx_memmove_backward(_Tp* __first, _Tp* __last, _Up* __result) {
    const ptrdiff_t __n = __last - __first;
    if (__n > 0) {
        __builtin_memmove(__result - __n, __first, sizeof(_Up) * static_cast<size_t>(__n));
    }
    return __result - __n;
}

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
