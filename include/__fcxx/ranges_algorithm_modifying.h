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


/* The mutating sequence operations of <algorithm>, of namespace ranges,
 * with their deleted parallel overloads. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_ALGORITHM_MODIFYING_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_ALGORITHM_MODIFYING_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <__fcxx/algorithm_modifying.h>
#include <__fcxx/ranges_algorithm_base.h>
#include <__fcxx/ranges_algorithm_nonmodifying.h>

namespace std {
namespace ranges {

/* [alg.copy], copy. Ranges of trivially copyable elements are copied with
 * memmove, through pointers, as by the algorithm of namespace std. */
template <class _It, class _Out>
using copy_result = in_out_result<_It, _Out>;

struct __fcxx_copy_fn {
    template <class _It, class _Sp, class _Out>
    static constexpr copy_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result) {
        if constexpr (is_pointer_v<_It> && is_pointer_v<_Out> && same_as<_It, _Sp>) {
            _Out __end = std::__fcxx_copy(__first, __last, __result, __fcxx_memmove_copyable<_It, _Out>());
            return {std::move(__last), std::move(__end)};
        } else {
            for (; __first != __last; ++__first, (void)++__result) {
                *__result = *__first;
            }
            return {std::move(__first), std::move(__result)};
        }
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out>
        requires indirectly_copyable<_It, _Out>
    constexpr copy_result<_It, _Out> operator()(_It __first, _Sp __last, _Out __result) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result));
    }

    template <input_range _Rp, weakly_incrementable _Out>
        requires indirectly_copyable<iterator_t<_Rp>, _Out>
    constexpr copy_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result));
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS>
        requires indirectly_copyable<_It, _Out>
    copy_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR>
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR> >
    copy_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> > operator()(_Ep&&, _Rp&&,
                                                                                  _OutR&&) const = delete;
};

inline constexpr __fcxx_copy_fn copy{};

template <class _It, class _Out>
using copy_n_result = in_out_result<_It, _Out>;

struct __fcxx_copy_n_fn {
    template <input_iterator _It, weakly_incrementable _Out>
        requires indirectly_copyable<_It, _Out>
    constexpr copy_n_result<_It, _Out> operator()(_It __first, iter_difference_t<_It> __n, _Out __result) const {
        if constexpr (random_access_iterator<_It>) {
            if (__n <= 0) {
                return {std::move(__first), std::move(__result)};
            }
            _It __last = __first + __n;
            return __fcxx_copy_fn::__impl(std::move(__first), std::move(__last), std::move(__result));
        } else {
            for (; __n > 0; --__n, (void)++__first, (void)++__result) {
                *__result = *__first;
            }
            return {std::move(__first), std::move(__result)};
        }
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, random_access_iterator _Out,
              sized_sentinel_for<_Out> _OutS>
        requires indirectly_copyable<_It, _Out>
    copy_n_result<_It, _Out> operator()(_Ep&&, _It, iter_difference_t<_It>, _Out, _OutS) const = delete;
};

inline constexpr __fcxx_copy_n_fn copy_n{};

template <class _It, class _Out>
using copy_if_result = in_out_result<_It, _Out>;

struct __fcxx_copy_if_fn {
    template <class _It, class _Sp, class _Out, class _Pred, class _Proj>
    static constexpr copy_if_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result, _Pred& __pred,
                                                      _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                *__result = *__first;
                ++__result;
            }
        }
        return {std::move(__first), std::move(__result)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_copyable<_It, _Out>
    constexpr copy_if_result<_It, _Out> operator()(_It __first, _Sp __last, _Out __result, _Pred __pred,
                                                   _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result), __pred, __proj);
    }

    template <input_range _Rp, weakly_incrementable _Out, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_copyable<iterator_t<_Rp>, _Out>
    constexpr copy_if_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result, _Pred __pred,
                                                                        _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_copyable<_It, _Out>
    copy_if_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR> >
    copy_if_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> > operator()(_Ep&&, _Rp&&, _OutR&&, _Pred,
                                                                                     _Proj = {}) const = delete;
};

inline constexpr __fcxx_copy_if_fn copy_if{};

template <class _I1, class _I2>
using copy_backward_result = in_out_result<_I1, _I2>;

struct __fcxx_copy_backward_fn {
    template <class _I1, class _S1, class _I2>
    static constexpr copy_backward_result<_I1, _I2> __impl(_I1 __first, _S1 __last, _I2 __result) {
        _I1 __end = ranges::next(__first, std::move(__last));
        if constexpr (is_pointer_v<_I1> && is_pointer_v<_I2>) {
            _I2 __begin = std::__fcxx_copy_backward(__first, __end, __result, __fcxx_memmove_copyable<_I1, _I2>());
            return {std::move(__end), std::move(__begin)};
        } else {
            _I1 __i = __end;
            while (__i != __first) {
                *--__result = *--__i;
            }
            return {std::move(__end), std::move(__result)};
        }
    }

    template <bidirectional_iterator _I1, sentinel_for<_I1> _S1, bidirectional_iterator _I2>
        requires indirectly_copyable<_I1, _I2>
    constexpr copy_backward_result<_I1, _I2> operator()(_I1 __first, _S1 __last, _I2 __result) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result));
    }

    template <bidirectional_range _Rp, bidirectional_iterator _It>
        requires indirectly_copyable<iterator_t<_Rp>, _It>
    constexpr copy_backward_result<borrowed_iterator_t<_Rp>, _It> operator()(_Rp&& __r, _It __result) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result));
    }
};

inline constexpr __fcxx_copy_backward_fn copy_backward{};

/* [alg.move], move */
template <class _It, class _Out>
using move_result = in_out_result<_It, _Out>;

struct __fcxx_move_fn {
    template <class _It, class _Sp, class _Out>
    static constexpr move_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result) {
        if constexpr (is_pointer_v<_It> && is_pointer_v<_Out> && same_as<_It, _Sp>) {
            _Out __end = std::__fcxx_move(__first, __last, __result, __fcxx_memmove_movable<_It, _Out>());
            return {std::move(__last), std::move(__end)};
        } else {
            for (; __first != __last; ++__first, (void)++__result) {
                *__result = ranges::iter_move(__first);
            }
            return {std::move(__first), std::move(__result)};
        }
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out>
        requires indirectly_movable<_It, _Out>
    constexpr move_result<_It, _Out> operator()(_It __first, _Sp __last, _Out __result) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result));
    }

    template <input_range _Rp, weakly_incrementable _Out>
        requires indirectly_movable<iterator_t<_Rp>, _Out>
    constexpr move_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result));
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS>
        requires indirectly_movable<_It, _Out>
    move_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR>
        requires indirectly_movable<iterator_t<_Rp>, iterator_t<_OutR> >
    move_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> > operator()(_Ep&&, _Rp&&,
                                                                                  _OutR&&) const = delete;
};

inline constexpr __fcxx_move_fn move{};

template <class _I1, class _I2>
using move_backward_result = in_out_result<_I1, _I2>;

struct __fcxx_move_backward_fn {
    template <class _I1, class _S1, class _I2>
    static constexpr move_backward_result<_I1, _I2> __impl(_I1 __first, _S1 __last, _I2 __result) {
        _I1 __end = ranges::next(__first, std::move(__last));
        if constexpr (is_pointer_v<_I1> && is_pointer_v<_I2>) {
            _I2 __begin = std::__fcxx_move_backward(__first, __end, __result, __fcxx_memmove_movable<_I1, _I2>());
            return {std::move(__end), std::move(__begin)};
        } else {
            _I1 __i = __end;
            while (__i != __first) {
                *--__result = ranges::iter_move(--__i);
            }
            return {std::move(__end), std::move(__result)};
        }
    }

    template <bidirectional_iterator _I1, sentinel_for<_I1> _S1, bidirectional_iterator _I2>
        requires indirectly_movable<_I1, _I2>
    constexpr move_backward_result<_I1, _I2> operator()(_I1 __first, _S1 __last, _I2 __result) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result));
    }

    template <bidirectional_range _Rp, bidirectional_iterator _It>
        requires indirectly_movable<iterator_t<_Rp>, _It>
    constexpr move_backward_result<borrowed_iterator_t<_Rp>, _It> operator()(_Rp&& __r, _It __result) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result));
    }
};

inline constexpr __fcxx_move_backward_fn move_backward{};

/* [alg.swap], swap_ranges */
template <class _I1, class _I2>
using swap_ranges_result = in_in_result<_I1, _I2>;

struct __fcxx_swap_ranges_fn {
    template <class _I1, class _S1, class _I2, class _S2>
    static constexpr swap_ranges_result<_I1, _I2> __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2) {
        for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2) {
            ranges::iter_swap(__first1, __first2);
        }
        return {std::move(__first1), std::move(__first2)};
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2>
        requires indirectly_swappable<_I1, _I2>
    constexpr swap_ranges_result<_I1, _I2> operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2));
    }

    template <input_range _R1, input_range _R2>
        requires indirectly_swappable<iterator_t<_R1>, iterator_t<_R2> >
    constexpr swap_ranges_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2> > operator()(_R1&& __r1,
                                                                                                 _R2&& __r2) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2));
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2>
        requires indirectly_swappable<_I1, _I2>
    swap_ranges_result<_I1, _I2> operator()(_Ep&&, _I1, _S1, _I2, _S2) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2>
        requires indirectly_swappable<iterator_t<_R1>, iterator_t<_R2> >
    swap_ranges_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2> > operator()(_Ep&&, _R1&&,
                                                                                       _R2&&) const = delete;
};

inline constexpr __fcxx_swap_ranges_fn swap_ranges{};

/* [alg.transform], transform */
template <class _It, class _Out>
using unary_transform_result = in_out_result<_It, _Out>;

template <class _I1, class _I2, class _Out>
using binary_transform_result = in_in_out_result<_I1, _I2, _Out>;

struct __fcxx_transform_fn {
    template <class _It, class _Sp, class _Out, class _Fp, class _Proj>
    static constexpr unary_transform_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result, _Fp& __op,
                                                              _Proj& __proj) {
        for (; __first != __last; ++__first, (void)++__result) {
            *__result = std::invoke(__op, std::invoke(__proj, *__first));
        }
        return {std::move(__first), std::move(__result)};
    }

    template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Fp, class _Proj1, class _Proj2>
    static constexpr binary_transform_result<_I1, _I2, _Out> __impl(_I1 __first1, _S1 __last1, _I2 __first2,
                                                                    _S2 __last2, _Out __result, _Fp& __op,
                                                                    _Proj1& __proj1, _Proj2& __proj2) {
        for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2, (void)++__result) {
            *__result = std::invoke(__op, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2));
        }
        return {std::move(__first1), std::move(__first2), std::move(__result)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out, copy_constructible _Fp,
              class _Proj = identity>
        requires indirectly_writable<_Out, indirect_result_t<_Fp&, projected<_It, _Proj> > >
    constexpr unary_transform_result<_It, _Out> operator()(_It __first1, _Sp __last1, _Out __result, _Fp __op,
                                                           _Proj __proj = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__result), __op, __proj);
    }

    template <input_range _Rp, weakly_incrementable _Out, copy_constructible _Fp, class _Proj = identity>
        requires indirectly_writable<_Out, indirect_result_t<_Fp&, projected<iterator_t<_Rp>, _Proj> > >
    constexpr unary_transform_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result, _Fp __op,
                                                                                _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result), __op, __proj);
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              weakly_incrementable _Out, copy_constructible _Fp, class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_writable<_Out, indirect_result_t<_Fp&, projected<_I1, _Proj1>, projected<_I2, _Proj2> > >
    constexpr binary_transform_result<_I1, _I2, _Out> operator()(_I1 __first1, _S1 __last1, _I2 __first2,
                                                                 _S2 __last2, _Out __result, _Fp __binary_op,
                                                                 _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2),
                      std::move(__result), __binary_op, __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, weakly_incrementable _Out, copy_constructible _Fp,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_writable<_Out, indirect_result_t<_Fp&, projected<iterator_t<_R1>, _Proj1>,
                                                             projected<iterator_t<_R2>, _Proj2> > >
    constexpr binary_transform_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, _Out>
    operator()(_R1&& __r1, _R2&& __r2, _Out __result, _Fp __binary_op, _Proj1 __proj1 = {},
               _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2),
                      std::move(__result), __binary_op, __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS, copy_constructible _Fp,
              class _Proj = identity>
        requires indirectly_writable<_Out, indirect_result_t<_Fp&, projected<_It, _Proj> > >
    unary_transform_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS, _Fp, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR, copy_constructible _Fp, class _Proj = identity>
        requires indirectly_writable<iterator_t<_OutR>, indirect_result_t<_Fp&, projected<iterator_t<_Rp>, _Proj> > >
    unary_transform_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _Rp&&, _OutR&&, _Fp, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, random_access_iterator _Out,
              sized_sentinel_for<_Out> _OutS, copy_constructible _Fp, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_writable<_Out, indirect_result_t<_Fp&, projected<_I1, _Proj1>, projected<_I2, _Proj2> > >
    binary_transform_result<_I1, _I2, _Out> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Out, _OutS, _Fp, _Proj1 = {},
                                                       _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, __fcxx_sized_random_access_range _OutR, copy_constructible _Fp,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_writable<iterator_t<_OutR>, indirect_result_t<_Fp&, projected<iterator_t<_R1>, _Proj1>,
                                                                          projected<iterator_t<_R2>, _Proj2> > >
    binary_transform_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _R1&&, _R2&&, _OutR&&, _Fp, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_transform_fn transform{};

/* [alg.replace], replace */
struct __fcxx_replace_fn {
    template <class _It, class _Sp, class _T1, class _T2, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, const _T1& __old_value, const _T2& __new_value,
                                _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (bool(std::invoke(__proj, *__first) == __old_value)) {
                *__first = __new_value;
            }
        }
        return __first;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _T1 = projected_value_t<_It, _Proj>,
              class _T2 = iter_value_t<_It>>
        requires indirectly_writable<_It, const _T2&>
                 && indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _T1*>
    constexpr _It operator()(_It __first, _Sp __last, const _T1& __old_value, const _T2& __new_value,
                             _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __old_value, __new_value, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              class _T1 = projected_value_t<iterator_t<_Rp>, _Proj>,
              class _T2 = range_value_t<_Rp>>
        requires indirectly_writable<iterator_t<_Rp>, const _T2&>
                 && indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _T1*>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, const _T1& __old_value, const _T2& __new_value,
                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __old_value, __new_value, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, class _T1 = projected_value_t<_It, _Proj>, class _T2 = iter_value_t<_It> >
        requires indirectly_writable<_It, const _T2&> &&
                     indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _T1*>
    _It operator()(_Ep&&, _It, _Sp, const _T1&, const _T2&, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              class _T1 = projected_value_t<iterator_t<_Rp>, _Proj>, class _T2 = range_value_t<_Rp> >
        requires indirectly_writable<iterator_t<_Rp>, const _T2&> &&
                     indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _T1*>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, const _T1&, const _T2&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_replace_fn replace{};

struct __fcxx_replace_if_fn {
    template <class _It, class _Sp, class _Pred, class _Tp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Pred& __pred, const _Tp& __new_value, _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                *__first = __new_value;
            }
        }
        return __first;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = iter_value_t<_It>, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_writable<_It, const _Tp&>
    constexpr _It operator()(_It __first, _Sp __last, _Pred __pred, const _Tp& __new_value, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __pred, __new_value, __proj);
    }

    template <input_range _Rp, class _Proj = identity, class _Tp = range_value_t<_Rp>,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_writable<iterator_t<_Rp>, const _Tp&>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Pred __pred, const _Tp& __new_value,
                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __pred, __new_value, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, class _Tp = iter_value_t<_It>,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_writable<_It, const _Tp&>
    _It operator()(_Ep&&, _It, _Sp, _Pred, const _Tp&, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              class _Tp = range_value_t<_Rp>, indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_writable<iterator_t<_Rp>, const _Tp&>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Pred, const _Tp&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_replace_if_fn replace_if{};

template <class _It, class _Out>
using replace_copy_result = in_out_result<_It, _Out>;

struct __fcxx_replace_copy_fn {
    template <class _It, class _Sp, class _Out, class _T1, class _T2, class _Proj>
    static constexpr replace_copy_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result,
                                                           const _T1& __old_value, const _T2& __new_value,
                                                           _Proj& __proj) {
        for (; __first != __last; ++__first, (void)++__result) {
            if (bool(std::invoke(__proj, *__first) == __old_value)) {
                *__result = __new_value;
            } else {
                *__result = *__first;
            }
        }
        return {std::move(__first), std::move(__result)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Out, class _Proj = identity,
              class _T1 = projected_value_t<_It, _Proj>,
              class _T2 = iter_value_t<_Out>>
        requires indirectly_copyable<_It, _Out>
                 && indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _T1*>
                 && output_iterator<_Out, const _T2&>
    constexpr replace_copy_result<_It, _Out> operator()(_It __first, _Sp __last, _Out __result,
                                                        const _T1& __old_value, const _T2& __new_value,
                                                        _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result), __old_value, __new_value, __proj);
    }

    template <input_range _Rp, class _Out, class _Proj = identity,
              class _T1 = projected_value_t<iterator_t<_Rp>, _Proj>,
              class _T2 = iter_value_t<_Out>>
        requires indirectly_copyable<iterator_t<_Rp>, _Out>
                 && indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _T1*>
                 && output_iterator<_Out, const _T2&>
    constexpr replace_copy_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result,
                                                                             const _T1& __old_value,
                                                                             const _T2& __new_value,
                                                                             _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result), __old_value, __new_value, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS, class _Proj = identity,
              class _T1 = projected_value_t<_It, _Proj>, class _T2 = iter_value_t<_Out> >
        requires indirectly_copyable<_It, _Out> &&
                     indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _T1*> &&
                     indirectly_writable<_Out, const _T2&>
    replace_copy_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS, const _T1&, const _T2&,
                                              _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR, class _Proj = identity,
              class _T1 = projected_value_t<iterator_t<_Rp>, _Proj>, class _T2 = range_value_t<_OutR> >
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR> > &&
                     indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _T1*> &&
                     indirectly_writable<iterator_t<_OutR>, const _T2&>
    replace_copy_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _Rp&&, _OutR&&, const _T1&, const _T2&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_replace_copy_fn replace_copy{};

template <class _It, class _Out>
using replace_copy_if_result = in_out_result<_It, _Out>;

struct __fcxx_replace_copy_if_fn {
    template <class _It, class _Sp, class _Out, class _Pred, class _Tp, class _Proj>
    static constexpr replace_copy_if_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result, _Pred& __pred,
                                                              const _Tp& __new_value, _Proj& __proj) {
        for (; __first != __last; ++__first, (void)++__result) {
            if (bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                *__result = __new_value;
            } else {
                *__result = *__first;
            }
        }
        return {std::move(__first), std::move(__result)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Out, class _Tp = iter_value_t<_Out>,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_copyable<_It, _Out> && output_iterator<_Out, const _Tp&>
    constexpr replace_copy_if_result<_It, _Out> operator()(_It __first, _Sp __last, _Out __result, _Pred __pred,
                                                           const _Tp& __new_value, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result), __pred, __new_value, __proj);
    }

    template <input_range _Rp, class _Out, class _Tp = iter_value_t<_Out>, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_copyable<iterator_t<_Rp>, _Out> && output_iterator<_Out, const _Tp&>
    constexpr replace_copy_if_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result,
                                                                                _Pred __pred, const _Tp& __new_value,
                                                                                _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result), __pred, __new_value, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS, class _Tp = iter_value_t<_Out>,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_copyable<_It, _Out> && indirectly_writable<_Out, const _Tp&>
    replace_copy_if_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS, _Pred, const _Tp&,
                                                 _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR, class _Tp = range_value_t<_OutR>, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR> > &&
                     indirectly_writable<iterator_t<_OutR>, const _Tp&>
    replace_copy_if_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _Rp&&, _OutR&&, _Pred, const _Tp&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_replace_copy_if_fn replace_copy_if{};

/* [alg.fill], fill */
struct __fcxx_fill_fn {
    template <class _Out, class _Sp, class _Tp>
    static constexpr _Out __impl(_Out __first, _Sp __last, const _Tp& __value) {
        for (; __first != __last; ++__first) {
            *__first = __value;
        }
        return __first;
    }

    template <class _Out, sentinel_for<_Out> _Sp, class _Tp = iter_value_t<_Out>>
        requires output_iterator<_Out, const _Tp&>
    constexpr _Out operator()(_Out __first, _Sp __last, const _Tp& __value) const {
        return __impl(std::move(__first), std::move(__last), __value);
    }

    template <class _Rp, class _Tp = range_value_t<_Rp>>
        requires output_range<_Rp, const _Tp&>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, const _Tp& __value) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __value);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _Out, sized_sentinel_for<_Out> _Sp,
              class _Tp = iter_value_t<_Out> >
        requires indirectly_writable<_Out, const _Tp&>
    _Out operator()(_Ep&&, _Out, _Sp, const _Tp&) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Tp = range_value_t<_Rp> >
        requires indirectly_writable<iterator_t<_Rp>, const _Tp&>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, const _Tp&) const = delete;
};

inline constexpr __fcxx_fill_fn fill{};

struct __fcxx_fill_n_fn {
    template <class _Out, class _Tp = iter_value_t<_Out>>
        requires output_iterator<_Out, const _Tp&>
    constexpr _Out operator()(_Out __first, iter_difference_t<_Out> __n, const _Tp& __value) const {
        for (; __n > 0; --__n, (void)++__first) {
            *__first = __value;
        }
        return __first;
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _Out, class _Tp = iter_value_t<_Out> >
        requires indirectly_writable<_Out, const _Tp&>
    _Out operator()(_Ep&&, _Out, iter_difference_t<_Out>, const _Tp&) const = delete;
};

inline constexpr __fcxx_fill_n_fn fill_n{};

/* [alg.generate], generate */
struct __fcxx_generate_fn {
    template <class _Out, class _Sp, class _Fp>
    static constexpr _Out __impl(_Out __first, _Sp __last, _Fp& __gen) {
        for (; __first != __last; ++__first) {
            *__first = std::invoke(__gen);
        }
        return __first;
    }

    template <input_or_output_iterator _Out, sentinel_for<_Out> _Sp, copy_constructible _Fp>
        requires invocable<_Fp&> && indirectly_writable<_Out, invoke_result_t<_Fp&> >
    constexpr _Out operator()(_Out __first, _Sp __last, _Fp __gen) const {
        return __impl(std::move(__first), std::move(__last), __gen);
    }

    template <class _Rp, copy_constructible _Fp>
        requires invocable<_Fp&> && output_range<_Rp, invoke_result_t<_Fp&> >
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Fp __gen) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __gen);
    }
};

inline constexpr __fcxx_generate_fn generate{};

struct __fcxx_generate_n_fn {
    template <input_or_output_iterator _Out, copy_constructible _Fp>
        requires invocable<_Fp&> && indirectly_writable<_Out, invoke_result_t<_Fp&> >
    constexpr _Out operator()(_Out __first, iter_difference_t<_Out> __n, _Fp __gen) const {
        for (; __n > 0; --__n, (void)++__first) {
            *__first = std::invoke(__gen);
        }
        return __first;
    }
};

inline constexpr __fcxx_generate_n_fn generate_n{};

/* [alg.remove], remove, which moves the elements that are kept over those
 * that are not, from the first that is not. */
struct __fcxx_remove_if_fn {
    template <class _It, class _Sp, class _Pred, class _Proj>
    static constexpr subrange<_It> __impl(_It __first, _Sp __last, _Pred& __pred, _Proj& __proj) {
        __first = __fcxx_find_if_fn::__impl(std::move(__first), __last, __pred, __proj);
        if (__first == __last) {
            return {__first, __first};
        }
        _It __i = __first;
        while (++__i != __last) {
            if (!bool(std::invoke(__pred, std::invoke(__proj, *__i)))) {
                *__first = ranges::iter_move(__i);
                ++__first;
            }
        }
        return {std::move(__first), std::move(__i)};
    }

    template <permutable _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    constexpr subrange<_It> operator()(_It __first, _Sp __last, _Pred __pred, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __pred, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires permutable<iterator_t<_Rp> >
    constexpr borrowed_subrange_t<_Rp> operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires permutable<_It>
    subrange<_It> operator()(_Ep&&, _It, _Sp, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires permutable<iterator_t<_Rp> >
    borrowed_subrange_t<_Rp> operator()(_Ep&&, _Rp&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_remove_if_fn remove_if{};

struct __fcxx_remove_fn {
    template <class _It, class _Sp, class _Tp, class _Proj>
    static constexpr subrange<_It> __impl(_It __first, _Sp __last, const _Tp& __value, _Proj& __proj) {
        __first = __fcxx_find_fn::__impl(std::move(__first), __last, __value, __proj);
        if (__first == __last) {
            return {__first, __first};
        }
        _It __i = __first;
        while (++__i != __last) {
            if (!bool(std::invoke(__proj, *__i) == __value)) {
                *__first = ranges::iter_move(__i);
                ++__first;
            }
        }
        return {std::move(__first), std::move(__i)};
    }

    template <permutable _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>>
        requires indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    constexpr subrange<_It> operator()(_It __first, _Sp __last, const _Tp& __value, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __value, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>>
        requires permutable<iterator_t<_Rp> >
                 && indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    constexpr borrowed_subrange_t<_Rp> operator()(_Rp&& __r, const _Tp& __value, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __value, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, class _Tp = projected_value_t<_It, _Proj> >
        requires permutable<_It> && indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    subrange<_It> operator()(_Ep&&, _It, _Sp, const _Tp&, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj> >
        requires permutable<iterator_t<_Rp> > &&
                     indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    borrowed_subrange_t<_Rp> operator()(_Ep&&, _Rp&&, const _Tp&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_remove_fn remove{};

template <class _It, class _Out>
using remove_copy_result = in_out_result<_It, _Out>;

struct __fcxx_remove_copy_fn {
    template <class _It, class _Sp, class _Out, class _Tp, class _Proj>
    static constexpr remove_copy_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result, const _Tp& __value,
                                                          _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (!bool(std::invoke(__proj, *__first) == __value)) {
                *__result = *__first;
                ++__result;
            }
        }
        return {std::move(__first), std::move(__result)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>>
        requires indirectly_copyable<_It, _Out>
                 && indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    constexpr remove_copy_result<_It, _Out> operator()(_It __first, _Sp __last, _Out __result, const _Tp& __value,
                                                       _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result), __value, __proj);
    }

    template <input_range _Rp, weakly_incrementable _Out, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>>
        requires indirectly_copyable<iterator_t<_Rp>, _Out>
                 && indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    constexpr remove_copy_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result,
                                                                            const _Tp& __value,
                                                                            _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result), __value, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj> >
        requires indirectly_copyable<_It, _Out> &&
                     indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    remove_copy_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS, const _Tp&, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj> >
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR> > &&
                     indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    remove_copy_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _Rp&&, _OutR&&, const _Tp&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_remove_copy_fn remove_copy{};

template <class _It, class _Out>
using remove_copy_if_result = in_out_result<_It, _Out>;

struct __fcxx_remove_copy_if_fn {
    template <class _It, class _Sp, class _Out, class _Pred, class _Proj>
    static constexpr remove_copy_if_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result, _Pred& __pred,
                                                             _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (!bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                *__result = *__first;
                ++__result;
            }
        }
        return {std::move(__first), std::move(__result)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_copyable<_It, _Out>
    constexpr remove_copy_if_result<_It, _Out> operator()(_It __first, _Sp __last, _Out __result, _Pred __pred,
                                                          _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result), __pred, __proj);
    }

    template <input_range _Rp, weakly_incrementable _Out, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_copyable<iterator_t<_Rp>, _Out>
    constexpr remove_copy_if_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result,
                                                                               _Pred __pred,
                                                                               _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_copyable<_It, _Out>
    remove_copy_if_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR> >
    remove_copy_if_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _Rp&&, _OutR&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_remove_copy_if_fn remove_copy_if{};

/* [alg.unique], unique, which compares each element with the last one kept,
 * as the comparison is an equivalence relation. */
struct __fcxx_unique_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr subrange<_It> __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        __first = __fcxx_adjacent_find_fn::__impl(std::move(__first), __last, __comp, __proj);
        if (__first == __last) {
            return {__first, __first};
        }
        _It __i = __first;
        ++__i;
        while (++__i != __last) {
            if (!bool(std::invoke(__comp, std::invoke(__proj, *__first), std::invoke(__proj, *__i)))) {
                *++__first = ranges::iter_move(__i);
            }
        }
        ++__first;
        return {std::move(__first), std::move(__i)};
    }

    template <permutable _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_equivalence_relation<projected<_It, _Proj> > _Comp = ranges::equal_to>
    constexpr subrange<_It> operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_equivalence_relation<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::equal_to>
        requires permutable<iterator_t<_Rp> >
    constexpr borrowed_subrange_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_equivalence_relation<projected<_It, _Proj> > _Comp = ranges::equal_to>
        requires permutable<_It>
    subrange<_It> operator()(_Ep&&, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_equivalence_relation<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::equal_to>
        requires permutable<iterator_t<_Rp> >
    borrowed_subrange_t<_Rp> operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_unique_fn unique{};

/* unique_copy compares each element with the last one copied: through a
 * forward iterator to it in the input, where the input is a forward range;
 * in the output, where the output is readable with the same value type; and
 * in a copy of it otherwise. */
template <class _It, class _Out>
using unique_copy_result = in_out_result<_It, _Out>;

/* A concept, rather than the expression, so that the value type of the
 * output is not asked for where it is not an input iterator. */
template <class _It, class _Out>
concept __fcxx_unique_copy_reads_output = input_iterator<_Out> && same_as<iter_value_t<_It>, iter_value_t<_Out> >;

struct __fcxx_unique_copy_fn {
    template <class _It, class _Sp, class _Out, class _Comp, class _Proj>
    static constexpr unique_copy_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result, _Comp& __comp,
                                                          _Proj& __proj) {
        if (__first == __last) {
            return {std::move(__first), std::move(__result)};
        }
        if constexpr (forward_iterator<_It>) {
            _It __kept = __first;
            *__result = *__first;
            ++__result;
            while (++__first != __last) {
                if (!bool(std::invoke(__comp, std::invoke(__proj, *__kept), std::invoke(__proj, *__first)))) {
                    *__result = *__first;
                    ++__result;
                    __kept = __first;
                }
            }
        } else if constexpr (__fcxx_unique_copy_reads_output<_It, _Out>) {
            *__result = *__first;
            while (++__first != __last) {
                if (!bool(std::invoke(__comp, std::invoke(__proj, *__result), std::invoke(__proj, *__first)))) {
                    *++__result = *__first;
                }
            }
            ++__result;
        } else {
            iter_value_t<_It> __kept(*__first);
            *__result = __kept;
            ++__result;
            while (++__first != __last) {
                if (!bool(std::invoke(__comp, std::invoke(__proj, __kept), std::invoke(__proj, *__first)))) {
                    __kept = *__first;
                    *__result = __kept;
                    ++__result;
                }
            }
        }
        return {std::move(__first), std::move(__result)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out, class _Proj = identity,
              indirect_equivalence_relation<projected<_It, _Proj> > _Comp = ranges::equal_to>
        requires indirectly_copyable<_It, _Out>
                 && (forward_iterator<_It> || (input_iterator<_Out> && same_as<iter_value_t<_It>, iter_value_t<_Out> >)
                     || indirectly_copyable_storable<_It, _Out>)
    constexpr unique_copy_result<_It, _Out> operator()(_It __first, _Sp __last, _Out __result, _Comp __comp = {},
                                                       _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result), __comp, __proj);
    }

    template <input_range _Rp, weakly_incrementable _Out, class _Proj = identity,
              indirect_equivalence_relation<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::equal_to>
        requires indirectly_copyable<iterator_t<_Rp>, _Out>
                 && (forward_iterator<iterator_t<_Rp> >
                     || (input_iterator<_Out> && same_as<range_value_t<_Rp>, iter_value_t<_Out> >)
                     || indirectly_copyable_storable<iterator_t<_Rp>, _Out>)
    constexpr unique_copy_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result,
                                                                            _Comp __comp = {},
                                                                            _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS, class _Proj = identity,
              indirect_equivalence_relation<projected<_It, _Proj> > _Comp = ranges::equal_to>
        requires indirectly_copyable<_It, _Out>
    unique_copy_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR, class _Proj = identity,
              indirect_equivalence_relation<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::equal_to>
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR> >
    unique_copy_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _Rp&&, _OutR&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_unique_copy_fn unique_copy{};

/* [alg.reverse], reverse */
struct __fcxx_reverse_fn {
    template <class _It, class _Sp>
    static constexpr _It __impl(_It __first, _Sp __last) {
        _It __end = ranges::next(__first, std::move(__last));
        std::__fcxx_reverse<__fcxx_ranges_ops>(std::move(__first), __end, __fcxx_iter_tag_t<_It>());
        return __end;
    }

    template <bidirectional_iterator _It, sentinel_for<_It> _Sp>
        requires permutable<_It>
    constexpr _It operator()(_It __first, _Sp __last) const {
        return __impl(std::move(__first), std::move(__last));
    }

    template <bidirectional_range _Rp>
        requires permutable<iterator_t<_Rp> >
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r) const {
        return __impl(ranges::begin(__r), ranges::end(__r));
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp>
        requires permutable<_It>
    _It operator()(_Ep&&, _It, _Sp) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp>
        requires permutable<iterator_t<_Rp> >
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&) const = delete;
};

inline constexpr __fcxx_reverse_fn reverse{};

template <class _It, class _Out>
using reverse_copy_result = in_out_result<_It, _Out>;

template <class _It, class _Out>
using reverse_copy_truncated_result = in_in_out_result<_It, _It, _Out>;

struct __fcxx_reverse_copy_fn {
    template <class _It, class _Sp, class _Out>
    static constexpr reverse_copy_result<_It, _Out> __impl(_It __first, _Sp __last, _Out __result) {
        _It __end = ranges::next(__first, std::move(__last));
        _It __i = __end;
        while (__i != __first) {
            --__i;
            *__result = *__i;
            ++__result;
        }
        return {std::move(__end), std::move(__result)};
    }

    template <bidirectional_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out>
        requires indirectly_copyable<_It, _Out>
    constexpr reverse_copy_result<_It, _Out> operator()(_It __first, _Sp __last, _Out __result) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result));
    }

    template <bidirectional_range _Rp, weakly_incrementable _Out>
        requires indirectly_copyable<iterator_t<_Rp>, _Out>
    constexpr reverse_copy_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, _Out __result) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__result));
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS>
        requires indirectly_copyable<_It, _Out>
    reverse_copy_truncated_result<_It, _Out> operator()(_Ep&&, _It, _Sp, _Out, _OutS) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR>
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR> >
    reverse_copy_truncated_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _Rp&&, _OutR&&) const = delete;
};

inline constexpr __fcxx_reverse_copy_fn reverse_copy{};

/* [alg.rotate], rotate */
struct __fcxx_rotate_fn {
    template <class _It, class _Sp>
    static constexpr subrange<_It> __impl(_It __first, _It __middle, _Sp __last) {
        _It __end = ranges::next(__middle, std::move(__last));
        _It __new_first = std::__fcxx_rotate<__fcxx_ranges_ops>(std::move(__first), std::move(__middle), __end,
                                                                __fcxx_iter_tag_t<_It>());
        return {std::move(__new_first), std::move(__end)};
    }

    template <permutable _It, sentinel_for<_It> _Sp>
    constexpr subrange<_It> operator()(_It __first, _It __middle, _Sp __last) const {
        return __impl(std::move(__first), std::move(__middle), std::move(__last));
    }

    template <forward_range _Rp>
        requires permutable<iterator_t<_Rp> >
    constexpr borrowed_subrange_t<_Rp> operator()(_Rp&& __r, iterator_t<_Rp> __middle) const {
        return __impl(ranges::begin(__r), std::move(__middle), ranges::end(__r));
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp>
        requires permutable<_It>
    subrange<_It> operator()(_Ep&&, _It, _It, _Sp) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp>
        requires permutable<iterator_t<_Rp> >
    borrowed_subrange_t<_Rp> operator()(_Ep&&, _Rp&&, iterator_t<_Rp>) const = delete;
};

inline constexpr __fcxx_rotate_fn rotate{};

template <class _It, class _Out>
using rotate_copy_result = in_out_result<_It, _Out>;

template <class _It, class _Out>
using rotate_copy_truncated_result = in_in_out_result<_It, _It, _Out>;

struct __fcxx_rotate_copy_fn {
    template <class _It, class _Sp, class _Out>
    static constexpr rotate_copy_result<_It, _Out> __impl(_It __first, _It __middle, _Sp __last, _Out __result) {
        copy_result<_It, _Out> __back = __fcxx_copy_fn::__impl(__middle, std::move(__last), std::move(__result));
        copy_result<_It, _Out> __front =
            __fcxx_copy_fn::__impl(std::move(__first), std::move(__middle), std::move(__back.out));
        return {std::move(__back.in), std::move(__front.out)};
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out>
        requires indirectly_copyable<_It, _Out>
    constexpr rotate_copy_result<_It, _Out> operator()(_It __first, _It __middle, _Sp __last, _Out __result) const {
        return __impl(std::move(__first), std::move(__middle), std::move(__last), std::move(__result));
    }

    template <forward_range _Rp, weakly_incrementable _Out>
        requires indirectly_copyable<iterator_t<_Rp>, _Out>
    constexpr rotate_copy_result<borrowed_iterator_t<_Rp>, _Out> operator()(_Rp&& __r, iterator_t<_Rp> __middle,
                                                                            _Out __result) const {
        return __impl(ranges::begin(__r), std::move(__middle), ranges::end(__r), std::move(__result));
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _Out, sized_sentinel_for<_Out> _OutS>
        requires indirectly_copyable<_It, _Out>
    rotate_copy_truncated_result<_It, _Out> operator()(_Ep&&, _It, _It, _Sp, _Out, _OutS) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR>
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR> >
    rotate_copy_truncated_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _Rp&&, iterator_t<_Rp>, _OutR&&) const = delete;
};

inline constexpr __fcxx_rotate_copy_fn rotate_copy{};

/* [alg.random.sample], sample, by selection sampling, which keeps the
 * order, where the population is a forward range, and by reservoir
 * sampling otherwise, which needs a random access output. */
struct __fcxx_sample_fn {
    template <class _It, class _Sp, class _Out, class _Gen>
    static _Out __impl(_It __first, _Sp __last, _Out __out, iter_difference_t<_It> __n, _Gen& __g) {
        using _Dist = iter_difference_t<_It>;
        using _Up = typename __fcxx_uniform_type<_Gen>::type;
        if (__n <= 0) {
            return __out;
        }
        if constexpr (forward_iterator<_It>) {
            _Dist __unsampled = ranges::distance(__first, __last);
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
        } else {
            _Dist __size = 0;
            for (; __first != __last && __size < __n; ++__first, (void)++__size) {
                __out[static_cast<iter_difference_t<_Out> >(__size)] = *__first;
            }
            for (_Dist __k = __size; __first != __last; ++__first, (void)++__k) {
                const _Dist __r = static_cast<_Dist>(std::__fcxx_uniform_int(__g, static_cast<_Up>(__k)));
                if (__r < __size) {
                    __out[static_cast<iter_difference_t<_Out> >(__r)] = *__first;
                }
            }
            return __out + static_cast<iter_difference_t<_Out> >(__size);
        }
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _Out, class _Gen>
        requires(forward_iterator<_It> || random_access_iterator<_Out>) && indirectly_copyable<_It, _Out>
                && __fcxx_uniform_random_bit_generator<remove_reference_t<_Gen> >
    _Out operator()(_It __first, _Sp __last, _Out __out, iter_difference_t<_It> __n, _Gen&& __g) const {
        return __impl(std::move(__first), std::move(__last), std::move(__out), __n, __g);
    }

    template <input_range _Rp, weakly_incrementable _Out, class _Gen>
        requires(forward_range<_Rp> || random_access_iterator<_Out>) && indirectly_copyable<iterator_t<_Rp>, _Out>
                && __fcxx_uniform_random_bit_generator<remove_reference_t<_Gen> >
    _Out operator()(_Rp&& __r, _Out __out, range_difference_t<_Rp> __n, _Gen&& __g) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__out), __n, __g);
    }
};

inline constexpr __fcxx_sample_fn sample{};

/* [alg.random.shuffle], shuffle, by the Fisher-Yates shuffle, which does
 * not swap an element with itself. */
struct __fcxx_shuffle_fn {
    template <class _It, class _Sp, class _Gen>
    static _It __impl(_It __first, _Sp __last, _Gen& __g) {
        using _Dist = iter_difference_t<_It>;
        using _Up = typename __fcxx_uniform_type<_Gen>::type;
        _It __end = ranges::next(__first, std::move(__last));
        for (_Dist __i = (__end - __first) - 1; __i > 0; --__i) {
            const _Dist __j = static_cast<_Dist>(std::__fcxx_uniform_int(__g, static_cast<_Up>(__i)));
            if (__j != __i) {
                ranges::iter_swap(__first + __i, __first + __j);
            }
        }
        return __end;
    }

    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Gen>
        requires permutable<_It> && __fcxx_uniform_random_bit_generator<remove_reference_t<_Gen> >
    _It operator()(_It __first, _Sp __last, _Gen&& __g) const {
        return __impl(std::move(__first), std::move(__last), __g);
    }

    template <random_access_range _Rp, class _Gen>
        requires permutable<iterator_t<_Rp> > && __fcxx_uniform_random_bit_generator<remove_reference_t<_Gen> >
    borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Gen&& __g) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __g);
    }
};

inline constexpr __fcxx_shuffle_fn shuffle{};

/* [alg.shift], shift */
struct __fcxx_shift_left_fn {
    template <class _It, class _Sp>
    static constexpr subrange<_It> __impl(_It __first, _Sp __last, iter_difference_t<_It> __n) {
        if (__n <= 0) {
            _It __end = ranges::next(__first, std::move(__last));
            return {std::move(__first), std::move(__end)};
        }
        _It __mid = __first;
        if (ranges::advance(__mid, __n, __last) != 0 || __mid == __last) {
            return {__first, __first};
        }
        move_result<_It, _It> __moved = __fcxx_move_fn::__impl(std::move(__mid), std::move(__last), __first);
        return {std::move(__first), std::move(__moved.out)};
    }

    template <permutable _It, sentinel_for<_It> _Sp>
    constexpr subrange<_It> operator()(_It __first, _Sp __last, iter_difference_t<_It> __n) const {
        return __impl(std::move(__first), std::move(__last), __n);
    }

    template <forward_range _Rp>
        requires permutable<iterator_t<_Rp> >
    constexpr borrowed_subrange_t<_Rp> operator()(_Rp&& __r, range_difference_t<_Rp> __n) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __n);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp>
        requires permutable<_It>
    subrange<_It> operator()(_Ep&&, _It, _Sp, iter_difference_t<_It>) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp>
        requires permutable<iterator_t<_Rp> >
    borrowed_subrange_t<_Rp> operator()(_Ep&&, _Rp&&, range_difference_t<_Rp>) const = delete;
};

inline constexpr __fcxx_shift_left_fn shift_left{};

/* The elements are moved from the back where the iterators are
 * bidirectional, and swapped through a buffer at the front otherwise. */
struct __fcxx_shift_right_fn {
    template <class _It, class _Sp>
    static constexpr subrange<_It> __impl(_It __first, _Sp __last, iter_difference_t<_It> __n) {
        if (__n <= 0) {
            _It __end = ranges::next(__first, std::move(__last));
            return {std::move(__first), std::move(__end)};
        }
        if constexpr (bidirectional_iterator<_It>) {
            _It __end = ranges::next(__first, std::move(__last));
            _It __mid = __end;
            if (ranges::advance(__mid, -__n, __first) != 0 || __mid == __first) {
                return {__end, __end};
            }
            move_backward_result<_It, _It> __moved =
                __fcxx_move_backward_fn::__impl(std::move(__first), std::move(__mid), __end);
            return {std::move(__moved.out), std::move(__end)};
        } else {
            _It __buf_end = __first;
            if (ranges::advance(__buf_end, __n, __last) != 0 || __buf_end == __last) {
                return {__buf_end, __buf_end};
            }
            _It __end = std::__fcxx_shift_right_forward<__fcxx_ranges_ops>(std::move(__first), __buf_end,
                                                                           std::move(__last));
            return {std::move(__buf_end), std::move(__end)};
        }
    }

    template <permutable _It, sentinel_for<_It> _Sp>
    constexpr subrange<_It> operator()(_It __first, _Sp __last, iter_difference_t<_It> __n) const {
        return __impl(std::move(__first), std::move(__last), __n);
    }

    template <forward_range _Rp>
        requires permutable<iterator_t<_Rp> >
    constexpr borrowed_subrange_t<_Rp> operator()(_Rp&& __r, range_difference_t<_Rp> __n) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __n);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp>
        requires permutable<_It>
    subrange<_It> operator()(_Ep&&, _It, _Sp, iter_difference_t<_It>) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp>
        requires permutable<iterator_t<_Rp> >
    borrowed_subrange_t<_Rp> operator()(_Ep&&, _Rp&&, range_difference_t<_Rp>) const = delete;
};

inline constexpr __fcxx_shift_right_fn shift_right{};

} /* namespace ranges */
} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
