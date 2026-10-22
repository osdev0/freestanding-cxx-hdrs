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


/* The mutating sequence operations of <algorithm>, of namespace std, which
 * are constexpr, but sample and shuffle. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_MODIFYING_H
#define __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_MODIFYING_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
#include <__fcxx/integer_product.h>
#include <__fcxx/algorithm_impl.h>
#include <__fcxx/algorithm_nonmodifying.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
#include <utility>
#include <type_traits>

namespace std {

/* [alg.copy], copy. Ranges of trivially copyable elements are copied with
 * memmove, through pointers, but in constant evaluation. */
template <class _InputIterator, class _OutputIterator>
constexpr _OutputIterator __fcxx_copy_loop(_InputIterator __first, _InputIterator __last,
                                                   _OutputIterator __result) {
    for (; __first != __last; ++__first, (void)++__result) {
        *__result = *__first;
    }
    return __result;
}

template <class _InputIterator, class _OutputIterator>
constexpr _OutputIterator __fcxx_copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result,
                                              false_type) {
    return std::__fcxx_copy_loop(__first, __last, __result);
}

template <class _Tp, class _Up>
constexpr _Up* __fcxx_copy(_Tp* __first, _Tp* __last, _Up* __result, true_type) {
    if (std::__fcxx_can_memmove()) {
        return std::__fcxx_memmove_forward(__first, __last, __result);
    }
    return std::__fcxx_copy_loop(__first, __last, __result);
}

template <class _InputIterator, class _OutputIterator>
constexpr _OutputIterator copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result) {
    return std::__fcxx_copy(__first, __last, __result,
                            __fcxx_memmove_copyable<_InputIterator, _OutputIterator>());
}

/* The input iterator is not incremented past the last element copied,
 * which an input iterator would read the next element for. */
template <class _InputIterator, class _Size, class _OutputIterator>
constexpr _OutputIterator copy_n(_InputIterator __first, _Size __n, _OutputIterator __result) {
    typename __fcxx_algo_size<_Size>::type __i = __n;
    if (__i > 0) {
        *__result = *__first;
        ++__result;
        for (--__i; __i > 0; --__i) {
            ++__first;
            *__result = *__first;
            ++__result;
        }
    }
    return __result;
}

template <class _InputIterator, class _OutputIterator, class _Predicate>
constexpr _OutputIterator copy_if(_InputIterator __first, _InputIterator __last, _OutputIterator __result,
                                          _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (bool(__pred(*__first))) {
            *__result = *__first;
            ++__result;
        }
    }
    return __result;
}

template <class _BidirectionalIterator1, class _BidirectionalIterator2>
constexpr _BidirectionalIterator2 __fcxx_copy_backward_loop(_BidirectionalIterator1 __first,
                                                                    _BidirectionalIterator1 __last,
                                                                    _BidirectionalIterator2 __result) {
    while (__first != __last) {
        *--__result = *--__last;
    }
    return __result;
}

template <class _BidirectionalIterator1, class _BidirectionalIterator2>
constexpr _BidirectionalIterator2 __fcxx_copy_backward(_BidirectionalIterator1 __first,
                                                               _BidirectionalIterator1 __last,
                                                               _BidirectionalIterator2 __result, false_type) {
    return std::__fcxx_copy_backward_loop(__first, __last, __result);
}

template <class _Tp, class _Up>
constexpr _Up* __fcxx_copy_backward(_Tp* __first, _Tp* __last, _Up* __result, true_type) {
    if (std::__fcxx_can_memmove()) {
        return std::__fcxx_memmove_backward(__first, __last, __result);
    }
    return std::__fcxx_copy_backward_loop(__first, __last, __result);
}

template <class _BidirectionalIterator1, class _BidirectionalIterator2>
constexpr _BidirectionalIterator2 copy_backward(_BidirectionalIterator1 __first,
                                                        _BidirectionalIterator1 __last,
                                                        _BidirectionalIterator2 __result) {
    return std::__fcxx_copy_backward(__first, __last, __result,
                                     __fcxx_memmove_copyable<_BidirectionalIterator1, _BidirectionalIterator2>());
}

/* [alg.move], move */
template <class _InputIterator, class _OutputIterator>
constexpr _OutputIterator __fcxx_move(_InputIterator __first, _InputIterator __last, _OutputIterator __result,
                                              false_type) {
    for (; __first != __last; ++__first, (void)++__result) {
        *__result = std::move(*__first);
    }
    return __result;
}

template <class _Tp, class _Up>
constexpr _Up* __fcxx_move(_Tp* __first, _Tp* __last, _Up* __result, true_type) {
    if (std::__fcxx_can_memmove()) {
        return std::__fcxx_memmove_forward(__first, __last, __result);
    }
    return std::__fcxx_move(__first, __last, __result, false_type());
}

template <class _InputIterator, class _OutputIterator>
constexpr _OutputIterator move(_InputIterator __first, _InputIterator __last, _OutputIterator __result) {
    return std::__fcxx_move(__first, __last, __result, __fcxx_memmove_movable<_InputIterator, _OutputIterator>());
}

template <class _BidirectionalIterator1, class _BidirectionalIterator2>
constexpr _BidirectionalIterator2 __fcxx_move_backward(_BidirectionalIterator1 __first,
                                                               _BidirectionalIterator1 __last,
                                                               _BidirectionalIterator2 __result, false_type) {
    while (__first != __last) {
        *--__result = std::move(*--__last);
    }
    return __result;
}

template <class _Tp, class _Up>
constexpr _Up* __fcxx_move_backward(_Tp* __first, _Tp* __last, _Up* __result, true_type) {
    if (std::__fcxx_can_memmove()) {
        return std::__fcxx_memmove_backward(__first, __last, __result);
    }
    return std::__fcxx_move_backward(__first, __last, __result, false_type());
}

template <class _BidirectionalIterator1, class _BidirectionalIterator2>
constexpr _BidirectionalIterator2 move_backward(_BidirectionalIterator1 __first,
                                                        _BidirectionalIterator1 __last,
                                                        _BidirectionalIterator2 __result) {
    return std::__fcxx_move_backward(__first, __last, __result,
                                     __fcxx_memmove_movable<_BidirectionalIterator1, _BidirectionalIterator2>());
}

/* [alg.swap], swap_ranges; swap is in <utility>, and iter_swap in
 * <__fcxx/algorithm_impl.h>. */
template <class _ForwardIterator1, class _ForwardIterator2>
constexpr _ForwardIterator2 swap_ranges(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                                                _ForwardIterator2 __first2) {
    for (; __first1 != __last1; ++__first1, (void)++__first2) {
        std::iter_swap(__first1, __first2);
    }
    return __first2;
}

/* [alg.transform], transform */
template <class _InputIterator, class _OutputIterator, class _UnaryOperation>
constexpr _OutputIterator transform(_InputIterator __first1, _InputIterator __last1, _OutputIterator __result,
                                            _UnaryOperation __op) {
    for (; __first1 != __last1; ++__first1, (void)++__result) {
        *__result = __op(*__first1);
    }
    return __result;
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _BinaryOperation>
constexpr _OutputIterator transform(_InputIterator1 __first1, _InputIterator1 __last1,
                                            _InputIterator2 __first2, _OutputIterator __result,
                                            _BinaryOperation __binary_op) {
    for (; __first1 != __last1; ++__first1, (void)++__first2, (void)++__result) {
        *__result = __binary_op(*__first1, *__first2);
    }
    return __result;
}

/* [alg.replace], replace. The replacement values default to the value type
 * from C++26 on, as P2248 has it. */
template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
constexpr void replace(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __old_value,
                               const _Tp& __new_value) {
    for (; __first != __last; ++__first) {
        if (bool(*__first == __old_value)) {
            *__first = __new_value;
        }
    }
}

template <class _ForwardIterator, class _Predicate, class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
constexpr void replace_if(_ForwardIterator __first, _ForwardIterator __last, _Predicate __pred,
                                  const _Tp& __new_value) {
    for (; __first != __last; ++__first) {
        if (bool(__pred(*__first))) {
            *__first = __new_value;
        }
    }
}

template <class _InputIterator, class _OutputIterator, class _Tp>
constexpr _OutputIterator replace_copy(_InputIterator __first, _InputIterator __last,
                                               _OutputIterator __result, const _Tp& __old_value,
                                               const _Tp& __new_value) {
    for (; __first != __last; ++__first, (void)++__result) {
        if (bool(*__first == __old_value)) {
            *__result = __new_value;
        } else {
            *__result = *__first;
        }
    }
    return __result;
}

template <class _InputIterator, class _OutputIterator, class _Predicate,
          class _Tp = typename iterator_traits<_OutputIterator>::value_type>
constexpr _OutputIterator replace_copy_if(_InputIterator __first, _InputIterator __last,
                                                  _OutputIterator __result, _Predicate __pred,
                                                  const _Tp& __new_value) {
    for (; __first != __last; ++__first, (void)++__result) {
        if (bool(__pred(*__first))) {
            *__result = __new_value;
        } else {
            *__result = *__first;
        }
    }
    return __result;
}

/* [alg.fill], fill. fill_n and generate_n return the end of what they
 * write, and write nothing for counts that are not positive. */
template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
constexpr void fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
    for (; __first != __last; ++__first) {
        *__first = __value;
    }
}

template <class _OutputIterator, class _Size, class _Tp = typename iterator_traits<_OutputIterator>::value_type>
constexpr _OutputIterator fill_n(_OutputIterator __first, _Size __n, const _Tp& __value) {
    for (typename __fcxx_algo_size<_Size>::type __i = __n; __i > 0; --__i, (void)++__first) {
        *__first = __value;
    }
    return __first;
}

/* [alg.generate], generate */
template <class _ForwardIterator, class _Generator>
constexpr void generate(_ForwardIterator __first, _ForwardIterator __last, _Generator __gen) {
    for (; __first != __last; ++__first) {
        *__first = __gen();
    }
}

template <class _OutputIterator, class _Size, class _Generator>
constexpr _OutputIterator generate_n(_OutputIterator __first, _Size __n, _Generator __gen) {
    for (typename __fcxx_algo_size<_Size>::type __i = __n; __i > 0; --__i, (void)++__first) {
        *__first = __gen();
    }
    return __first;
}

/* [alg.remove], remove, which moves the elements that are kept over those
 * that are not, from the first that is not. */
template <class _ForwardIterator, class _Predicate>
constexpr _ForwardIterator remove_if(_ForwardIterator __first, _ForwardIterator __last, _Predicate __pred) {
    __first = std::find_if(__first, __last, __pred);
    if (__first != __last) {
        _ForwardIterator __i = __first;
        while (++__i != __last) {
            if (!bool(__pred(*__i))) {
                *__first = std::move(*__i);
                ++__first;
            }
        }
    }
    return __first;
}

template <class _ForwardIterator, class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
constexpr _ForwardIterator remove(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
    __first = std::find(__first, __last, __value);
    if (__first != __last) {
        _ForwardIterator __i = __first;
        while (++__i != __last) {
            if (!bool(*__i == __value)) {
                *__first = std::move(*__i);
                ++__first;
            }
        }
    }
    return __first;
}

template <class _InputIterator, class _OutputIterator, class _Tp = typename iterator_traits<_InputIterator>::value_type>
constexpr _OutputIterator remove_copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result,
                                              const _Tp& __value) {
    for (; __first != __last; ++__first) {
        if (!bool(*__first == __value)) {
            *__result = *__first;
            ++__result;
        }
    }
    return __result;
}

template <class _InputIterator, class _OutputIterator, class _Predicate>
constexpr _OutputIterator remove_copy_if(_InputIterator __first, _InputIterator __last,
                                                 _OutputIterator __result, _Predicate __pred) {
    for (; __first != __last; ++__first) {
        if (!bool(__pred(*__first))) {
            *__result = *__first;
            ++__result;
        }
    }
    return __result;
}

/* [alg.unique], unique, which compares each element with the last one kept,
 * as the predicate is an equivalence relation. */
template <class _ForwardIterator, class _BinaryPredicate>
constexpr _ForwardIterator unique(_ForwardIterator __first, _ForwardIterator __last,
                                          _BinaryPredicate __pred) {
    __first = std::adjacent_find(__first, __last, __pred);
    if (__first != __last) {
        _ForwardIterator __i = __first;
        ++__i;
        while (++__i != __last) {
            if (!bool(__pred(*__first, *__i))) {
                *++__first = std::move(*__i);
            }
        }
        ++__first;
    }
    return __first;
}

template <class _ForwardIterator>
constexpr _ForwardIterator unique(_ForwardIterator __first, _ForwardIterator __last) {
    return std::unique(__first, __last, __fcxx_equal_op());
}

/* unique_copy compares each element with the last one copied: through a
 * forward iterator to it in the input, where the input is a forward range;
 * in the output, where the output is a forward range of the same value
 * type; and in a copy of it otherwise. */
template <class _Tp, class _Up>
struct __fcxx_algo_same {
    enum { value = 0 };
};

template <class _Tp>
struct __fcxx_algo_same<_Tp, _Tp> {
    enum { value = 1 };
};

template <int _Kind>
struct __fcxx_unique_copy_kind {};

template <class _InputIterator, class _OutputIterator, class _BinaryPredicate>
constexpr _OutputIterator __fcxx_unique_copy(_InputIterator __first, _InputIterator __last,
                                                     _OutputIterator __result, _BinaryPredicate& __pred,
                                                     __fcxx_unique_copy_kind<0>) {
    _InputIterator __kept = __first;
    *__result = *__first;
    ++__result;
    while (++__first != __last) {
        if (!bool(__pred(*__kept, *__first))) {
            *__result = *__first;
            ++__result;
            __kept = __first;
        }
    }
    return __result;
}

template <class _InputIterator, class _OutputIterator, class _BinaryPredicate>
constexpr _OutputIterator __fcxx_unique_copy(_InputIterator __first, _InputIterator __last,
                                                     _OutputIterator __result, _BinaryPredicate& __pred,
                                                     __fcxx_unique_copy_kind<1>) {
    *__result = *__first;
    while (++__first != __last) {
        if (!bool(__pred(*__result, *__first))) {
            *++__result = *__first;
        }
    }
    return ++__result;
}

template <class _InputIterator, class _OutputIterator, class _BinaryPredicate>
constexpr _OutputIterator __fcxx_unique_copy(_InputIterator __first, _InputIterator __last,
                                                     _OutputIterator __result, _BinaryPredicate& __pred,
                                                     __fcxx_unique_copy_kind<2>) {
    typename iterator_traits<_InputIterator>::value_type __kept(*__first);
    *__result = __kept;
    ++__result;
    while (++__first != __last) {
        if (!bool(__pred(__kept, *__first))) {
            __kept = *__first;
            *__result = __kept;
            ++__result;
        }
    }
    return __result;
}

/* Whether an iterator category is that of forward iterators. */
template <class _Category>
struct __fcxx_forward_category {
    static char __test(forward_iterator_tag);
    static long __test(...);
    enum { value = sizeof(__test(_Category())) == 1 };
};

/* Whether an iterator is a forward iterator, by its concept. */
template <class _It>
struct __fcxx_models_forward {
    enum { value = forward_iterator<_It> };
};

template <class _InputIterator, class _OutputIterator>
struct __fcxx_unique_copy_kind_of {
    enum {
        __output = __fcxx_forward_category<typename iterator_traits<_OutputIterator>::iterator_category>::value
                   && __fcxx_algo_same<typename iterator_traits<_InputIterator>::value_type,
                                       typename iterator_traits<_OutputIterator>::value_type>::value,
        value = __fcxx_models_forward<_InputIterator>::value ? 0 : __output ? 1 : 2
    };
};

template <class _InputIterator, class _OutputIterator, class _BinaryPredicate>
constexpr _OutputIterator unique_copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result,
                                              _BinaryPredicate __pred) {
    if (__first == __last) {
        return __result;
    }
    return std::__fcxx_unique_copy(
        __first, __last, __result, __pred,
        __fcxx_unique_copy_kind<__fcxx_unique_copy_kind_of<_InputIterator, _OutputIterator>::value>());
}

template <class _InputIterator, class _OutputIterator>
constexpr _OutputIterator unique_copy(_InputIterator __first, _InputIterator __last,
                                              _OutputIterator __result) {
    return std::unique_copy(__first, __last, __result, __fcxx_equal_op());
}

/* [alg.reverse], reverse */
template <class _BidirectionalIterator>
constexpr void reverse(_BidirectionalIterator __first, _BidirectionalIterator __last) {
    std::__fcxx_reverse<__fcxx_std_ops>(__first, __last,
                                        typename iterator_traits<_BidirectionalIterator>::iterator_category());
}

template <class _BidirectionalIterator, class _OutputIterator>
constexpr _OutputIterator reverse_copy(_BidirectionalIterator __first, _BidirectionalIterator __last,
                                               _OutputIterator __result) {
    while (__first != __last) {
        --__last;
        *__result = *__last;
        ++__result;
    }
    return __result;
}

/* [alg.rotate], rotate, which returns where the first element went. */
template <class _ForwardIterator>
constexpr _ForwardIterator rotate(_ForwardIterator __first, _ForwardIterator __middle,
                                          _ForwardIterator __last) {
    return std::__fcxx_rotate<__fcxx_std_ops>(__first, __middle, __last,
                                              typename iterator_traits<_ForwardIterator>::iterator_category());
}

template <class _ForwardIterator, class _OutputIterator>
constexpr _OutputIterator rotate_copy(_ForwardIterator __first, _ForwardIterator __middle,
                                              _ForwardIterator __last, _OutputIterator __result) {
    return std::copy(__first, __middle, std::copy(__middle, __last, __result));
}

/* A number uniformly distributed from 0 to a bound, of an unsigned type,
 * from the numbers of a uniform random bit generator: those of the range
 * of the generator that fit a whole number of times, divided into as many
 * buckets as there are results, which takes the high bits of the numbers
 * (better than the low bits of weak generators), where the range of the
 * generator is larger; and otherwise several numbers, as the digits of a
 * larger number, which is rejected where it is too large. */
template <class _Up, class _Gen>
_Up __fcxx_uniform_int(_Gen& __g, _Up __bound) {
    typedef typename remove_reference<_Gen>::type _Gr;
    const _Up __gmin = static_cast<_Up>(_Gr::min());
    const _Up __grange = static_cast<_Up>(static_cast<_Up>(_Gr::max()) - __gmin);
    if (__grange == __bound) {
        return static_cast<_Up>(static_cast<_Up>(__g()) - __gmin);
    }
    if (__grange > __bound) {
        const _Up __ext = static_cast<_Up>(__bound + 1);
        const _Up __limit = static_cast<_Up>(__grange - static_cast<_Up>((__grange % __ext + 1) % __ext));
        const _Up __bucket = static_cast<_Up>(__limit / __ext + 1);
        _Up __v = 0;
        do {
            __v = static_cast<_Up>(static_cast<_Up>(__g()) - __gmin);
        } while (__v > __limit);
        return static_cast<_Up>(__v / __bucket);
    }
    const _Up __gext = static_cast<_Up>(__grange + 1);
    while (true) {
        const _Up __high =
            static_cast<_Up>(_FCXX_MUL(std::__fcxx_uniform_int(__g, static_cast<_Up>(__bound / __gext)), __gext));
        const _Up __v = static_cast<_Up>(__high + static_cast<_Up>(static_cast<_Up>(__g()) - __gmin));
        if (__v >= __high && __v <= __bound) {
            return __v;
        }
    }
}

/* The unsigned type that the numbers of a generator and the distances of
 * iterators both fit. */
template <class _Gen>
struct __fcxx_uniform_type {
    typedef decltype(std::declval<typename remove_reference<_Gen>::type&>()()) _Res;
    typedef __fcxx_conditional_t<(sizeof(_Res) > sizeof(unsigned long long)), _Res, unsigned long long> type;
};

/* [alg.random.shuffle], shuffle, by the Fisher-Yates shuffle, which does
 * not swap an element with itself. */
template <class _RandomAccessIterator, class _UniformRandomBitGenerator>
void shuffle(_RandomAccessIterator __first, _RandomAccessIterator __last, _UniformRandomBitGenerator&& __g) {
    typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Dist;
    typedef typename __fcxx_uniform_type<_UniformRandomBitGenerator>::type _Up;
    const _Dist __len = __last - __first;
    for (_Dist __i = __len - 1; __i > 0; --__i) {
        const _Dist __j = static_cast<_Dist>(std::__fcxx_uniform_int(__g, static_cast<_Up>(__i)));
        if (__j != __i) {
            std::iter_swap(__first + __i, __first + __j);
        }
    }
}

/* [alg.random.sample], sample: by selection sampling, which keeps the
 * order, where the population is a forward range, and by reservoir
 * sampling otherwise, which needs a random access output. The size of the
 * sample is taken in the difference type of the population, as far as it
 * fits. */
template <class _Dist, class _Distance>
_Dist __fcxx_sample_size(_Distance __n) {
    if (!(__n > 0)) {
        return 0;
    }
    typedef typename common_type<_Distance, _Dist>::type _Common;
    const _Dist __max = static_cast<_Dist>(static_cast<typename make_unsigned<_Dist>::type>(-1) >> 1);
    return static_cast<_Common>(__n) > static_cast<_Common>(__max) ? __max : static_cast<_Dist>(__n);
}

template <class _PopulationIterator, class _SampleIterator, class _Dist, class _Gen>
_SampleIterator __fcxx_sample(_PopulationIterator __first, _PopulationIterator __last, _SampleIterator __out,
                              _Dist __n, _Gen& __g, true_type) {
    typedef typename __fcxx_uniform_type<_Gen>::type _Up;
    _Dist __unsampled = std::distance(__first, __last);
    if (__n > __unsampled) {
        __n = __unsampled;
    }
    for (; __n > 0; ++__first) {
        if (std::__fcxx_uniform_int(__g, static_cast<_Up>(__unsampled - 1)) < static_cast<_Up>(__n)) {
            *__out = *__first;
            ++__out;
            --__n;
        }
        --__unsampled;
    }
    return __out;
}

template <class _PopulationIterator, class _SampleIterator, class _Dist, class _Gen>
_SampleIterator __fcxx_sample(_PopulationIterator __first, _PopulationIterator __last, _SampleIterator __out,
                              _Dist __n, _Gen& __g, false_type) {
    typedef typename __fcxx_uniform_type<_Gen>::type _Up;
    _Dist __size = 0;
    for (; __first != __last && __size < __n; ++__first, (void)++__size) {
        __out[__size] = *__first;
    }
    for (_Dist __k = __size; __first != __last; ++__first, (void)++__k) {
        const _Dist __r = static_cast<_Dist>(std::__fcxx_uniform_int(__g, static_cast<_Up>(__k)));
        if (__r < __size) {
            __out[__r] = *__first;
        }
    }
    return __out + __size;
}

template <class _PopulationIterator, class _SampleIterator, class _Distance, class _UniformRandomBitGenerator>
_SampleIterator sample(_PopulationIterator __first, _PopulationIterator __last, _SampleIterator __out,
                       _Distance __n, _UniformRandomBitGenerator&& __g) {
    static_assert(is_integral<_Distance>::value, "std::sample takes a number of elements of an integer type");
    typedef typename iterator_traits<_PopulationIterator>::difference_type _Dist;
    return std::__fcxx_sample(__first, __last, __out, std::__fcxx_sample_size<_Dist>(__n), __g,
                              __fcxx_bool_constant<__fcxx_models_forward<_PopulationIterator>::value>());
}

/* [alg.shift], shift. The iterator n elements past the first, which is not
 * found where there are no more than n elements. */
template <class _ForwardIterator>
constexpr bool __fcxx_shift_advance(_ForwardIterator& __it,
                                            typename iterator_traits<_ForwardIterator>::difference_type __n,
                                            _ForwardIterator __last, forward_iterator_tag) {
    for (; __n > 0; --__n) {
        if (__it == __last) {
            return false;
        }
        ++__it;
    }
    return __it != __last;
}

template <class _ForwardIterator>
constexpr bool __fcxx_shift_advance(_ForwardIterator& __it,
                                            typename iterator_traits<_ForwardIterator>::difference_type __n,
                                            _ForwardIterator __last, random_access_iterator_tag) {
    if (__n >= __last - __it) {
        return false;
    }
    __it += __n;
    return true;
}

template <class _ForwardIterator>
constexpr _ForwardIterator shift_left(_ForwardIterator __first, _ForwardIterator __last,
                                              typename iterator_traits<_ForwardIterator>::difference_type __n) {
    if (__n <= 0) {
        return __last;
    }
    _ForwardIterator __mid = __first;
    if (!std::__fcxx_shift_advance(__mid, __n, __last,
                                   typename iterator_traits<_ForwardIterator>::iterator_category())) {
        return __first;
    }
    return std::move(__mid, __last, __first);
}

/* Of bidirectional iterators, the elements are moved from the back, and of
 * forward iterators, they are swapped through a buffer at the front. */
template <class _ForwardIterator>
constexpr _ForwardIterator __fcxx_shift_right(_ForwardIterator __first, _ForwardIterator __last,
                                                      typename iterator_traits<_ForwardIterator>::difference_type __n,
                                                      forward_iterator_tag) {
    _ForwardIterator __buf_end = __first;
    if (!std::__fcxx_shift_advance(__buf_end, __n, __last, forward_iterator_tag())) {
        return __last;
    }
    std::__fcxx_shift_right_forward<__fcxx_std_ops>(__first, __buf_end, __last);
    return __buf_end;
}

template <class _BidirectionalIterator>
constexpr _BidirectionalIterator
__fcxx_shift_right(_BidirectionalIterator __first, _BidirectionalIterator __last,
                   typename iterator_traits<_BidirectionalIterator>::difference_type __n, bidirectional_iterator_tag) {
    _BidirectionalIterator __mid = __last;
    for (; __n > 0; --__n) {
        if (__mid == __first) {
            return __last;
        }
        --__mid;
    }
    if (__mid == __first) {
        return __last;
    }
    return std::move_backward(__first, __mid, __last);
}

template <class _RandomAccessIterator>
constexpr _RandomAccessIterator
__fcxx_shift_right(_RandomAccessIterator __first, _RandomAccessIterator __last,
                   typename iterator_traits<_RandomAccessIterator>::difference_type __n, random_access_iterator_tag) {
    if (__n >= __last - __first) {
        return __last;
    }
    return std::move_backward(__first, __last - __n, __last);
}

template <class _ForwardIterator>
constexpr _ForwardIterator shift_right(_ForwardIterator __first, _ForwardIterator __last,
                                               typename iterator_traits<_ForwardIterator>::difference_type __n) {
    if (__n <= 0) {
        return __first;
    }
    return std::__fcxx_shift_right(__first, __last, __n,
                                   typename iterator_traits<_ForwardIterator>::iterator_category());
}

} /* namespace std */

#endif
