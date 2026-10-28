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


/* [algorithms.parallel.overloads], the parallel overloads of the algorithms
 * of <algorithm>, of namespace std, which are freestanding deleted, but
 * those of stable_sort, stable_partition and inplace_merge, which are not
 * freestanding. They do not participate in overload resolution unless their
 * first argument is an execution policy. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_PARALLEL_H
#define __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_PARALLEL_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <utility>
#include <__fcxx/algorithm_impl.h>
#include <__fcxx/execution_policy.h>
#include <__fcxx/iterator_base.h>

namespace std {

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool all_of(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool any_of(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool none_of(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Function,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void for_each(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Function) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Size, class _Function,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator for_each_n(_ExecutionPolicy&&, _ForwardIterator, _Size, _Function) = delete;

template <class _ExecutionPolicy, class _ForwardIterator,
          class _Tp = typename iterator_traits<_ForwardIterator>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator find(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator find_if(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator find_if_not(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator1 find_end(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                           _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator1 find_end(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                           _ForwardIterator2, _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator1 find_first_of(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator1 find_first_of(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                _ForwardIterator2, _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator adjacent_find(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator adjacent_find(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator,
          class _Tp = typename iterator_traits<_ForwardIterator>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
typename iterator_traits<_ForwardIterator>::difference_type count(_ExecutionPolicy&&, _ForwardIterator,
                                                                  _ForwardIterator, const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
typename iterator_traits<_ForwardIterator>::difference_type count_if(_ExecutionPolicy&&, _ForwardIterator,
                                                                     _ForwardIterator, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
pair<_ForwardIterator1, _ForwardIterator2> mismatch(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1,
                                                    _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
pair<_ForwardIterator1, _ForwardIterator2> mismatch(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1,
                                                    _ForwardIterator2, _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
pair<_ForwardIterator1, _ForwardIterator2> mismatch(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1,
                                                    _ForwardIterator2, _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
pair<_ForwardIterator1, _ForwardIterator2> mismatch(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1,
                                                    _ForwardIterator2, _ForwardIterator2, _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool equal(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool equal(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool equal(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool equal(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _ForwardIterator2,
           _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator1 search(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                         _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator1 search(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _ForwardIterator2,
                         _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Size,
          class _Tp = typename iterator_traits<_ForwardIterator>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator search_n(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Size, const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Size,
          class _Tp = typename iterator_traits<_ForwardIterator>::value_type, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator search_n(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Size, const _Tp&,
                          _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 copy(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _Size, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 copy_n(_ExecutionPolicy&&, _ForwardIterator1, _Size, _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 copy_if(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                          _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 move(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 swap_ranges(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _UnaryOperation,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 transform(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                            _UnaryOperation) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          class _BinaryOperation, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator transform(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                           _ForwardIterator, _BinaryOperation) = delete;

template <class _ExecutionPolicy, class _ForwardIterator,
          class _Tp = typename iterator_traits<_ForwardIterator>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void replace(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, const _Tp&, const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          class _Tp = typename iterator_traits<_ForwardIterator>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void replace_if(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Predicate, const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _Tp,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 replace_copy(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, const _Tp&,
                               const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _Predicate,
          class _Tp = typename iterator_traits<_ForwardIterator2>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 replace_copy_if(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                  _Predicate, const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator,
          class _Tp = typename iterator_traits<_ForwardIterator>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void fill(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Size,
          class _Tp = typename iterator_traits<_ForwardIterator>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator fill_n(_ExecutionPolicy&&, _ForwardIterator, _Size, const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Generator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void generate(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Generator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Size, class _Generator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator generate_n(_ExecutionPolicy&&, _ForwardIterator, _Size, _Generator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator,
          class _Tp = typename iterator_traits<_ForwardIterator>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator remove(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator remove_if(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          class _Tp = typename iterator_traits<_ForwardIterator1>::value_type,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 remove_copy(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                              const _Tp&) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 remove_copy_if(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                 _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator unique(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator unique(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 unique_copy(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 unique_copy(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                              _BinaryPredicate) = delete;

template <class _ExecutionPolicy, class _BidirectionalIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void reverse(_ExecutionPolicy&&, _BidirectionalIterator, _BidirectionalIterator) = delete;

template <class _ExecutionPolicy, class _BidirectionalIterator, class _ForwardIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator reverse_copy(_ExecutionPolicy&&, _BidirectionalIterator, _BidirectionalIterator,
                              _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator rotate(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator2 rotate_copy(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator1,
                              _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator shift_left(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator,
                            typename iterator_traits<_ForwardIterator>::difference_type) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator shift_right(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator,
                             typename iterator_traits<_ForwardIterator>::difference_type) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void sort(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void sort(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void partial_sort(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator, _RandomAccessIterator) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void partial_sort(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator, _RandomAccessIterator,
                  _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _RandomAccessIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_RandomAccessIterator partial_sort_copy(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _RandomAccessIterator,
                                        _RandomAccessIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _RandomAccessIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_RandomAccessIterator partial_sort_copy(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _RandomAccessIterator,
                                        _RandomAccessIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool is_sorted(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool is_sorted(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator is_sorted_until(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator is_sorted_until(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void nth_element(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator, _RandomAccessIterator) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
void nth_element(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator, _RandomAccessIterator,
                 _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool is_partitioned(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Predicate,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator partition(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _ForwardIterator1, class _ForwardIterator2,
          class _Predicate, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
pair<_ForwardIterator1, _ForwardIterator2> partition_copy(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator,
                                                          _ForwardIterator1, _ForwardIterator2, _Predicate) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator merge(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _ForwardIterator2,
                       _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          class _Compare, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator merge(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _ForwardIterator2,
                       _ForwardIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool includes(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool includes(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2, _ForwardIterator2,
              _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator set_union(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                           _ForwardIterator2, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          class _Compare, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator set_union(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                           _ForwardIterator2, _ForwardIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator set_intersection(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                  _ForwardIterator2, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          class _Compare, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator set_intersection(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                  _ForwardIterator2, _ForwardIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator set_difference(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                _ForwardIterator2, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          class _Compare, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator set_difference(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                _ForwardIterator2, _ForwardIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator set_symmetric_difference(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                          _ForwardIterator2, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _ForwardIterator,
          class _Compare, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator set_symmetric_difference(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                                          _ForwardIterator2, _ForwardIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool is_heap(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool is_heap(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_RandomAccessIterator is_heap_until(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator) = delete;

template <class _ExecutionPolicy, class _RandomAccessIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_RandomAccessIterator is_heap_until(_ExecutionPolicy&&, _RandomAccessIterator, _RandomAccessIterator,
                                    _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator min_element(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator min_element(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator max_element(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
_ForwardIterator max_element(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator, _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
pair<_ForwardIterator, _ForwardIterator> minmax_element(_ExecutionPolicy&&, _ForwardIterator,
                                                        _ForwardIterator) = delete;

template <class _ExecutionPolicy, class _ForwardIterator, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
pair<_ForwardIterator, _ForwardIterator> minmax_element(_ExecutionPolicy&&, _ForwardIterator, _ForwardIterator,
                                                        _Compare) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool lexicographical_compare(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                             _ForwardIterator2) = delete;

template <class _ExecutionPolicy, class _ForwardIterator1, class _ForwardIterator2, class _Compare,
          __fcxx_enable_if_execution_policy_t<_ExecutionPolicy> = 0>
bool lexicographical_compare(_ExecutionPolicy&&, _ForwardIterator1, _ForwardIterator1, _ForwardIterator2,
                             _ForwardIterator2, _Compare) = delete;

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
