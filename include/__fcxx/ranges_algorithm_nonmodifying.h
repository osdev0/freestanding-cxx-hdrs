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


/* The non-modifying sequence operations of <algorithm>, of namespace ranges,
 * with their deleted parallel overloads. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_ALGORITHM_NONMODIFYING_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_ALGORITHM_NONMODIFYING_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <__fcxx/algorithm_nonmodifying.h>
#include <__fcxx/ranges_algorithm_base.h>
#include <optional>

namespace std {
namespace ranges {

/* [alg.all.of], [alg.any.of], [alg.none.of] */
struct __fcxx_all_of_fn {
    template <class _It, class _Sp, class _Pred, class _Proj>
    static constexpr bool __impl(_It __first, _Sp __last, _Pred& __pred, _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (!bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                return false;
            }
        }
        return true;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    [[nodiscard]] constexpr bool operator()(_It __first, _Sp __last, _Pred __pred, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __pred, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    [[nodiscard]] constexpr bool operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    bool operator()(_Ep&&, _It, _Sp, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    bool operator()(_Ep&&, _Rp&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_all_of_fn all_of{};

struct __fcxx_any_of_fn {
    template <class _It, class _Sp, class _Pred, class _Proj>
    static constexpr bool __impl(_It __first, _Sp __last, _Pred& __pred, _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                return true;
            }
        }
        return false;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    [[nodiscard]] constexpr bool operator()(_It __first, _Sp __last, _Pred __pred, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __pred, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    [[nodiscard]] constexpr bool operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    bool operator()(_Ep&&, _It, _Sp, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    bool operator()(_Ep&&, _Rp&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_any_of_fn any_of{};

struct __fcxx_none_of_fn {
    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    [[nodiscard]] constexpr bool operator()(_It __first, _Sp __last, _Pred __pred, _Proj __proj = {}) const {
        return !__fcxx_any_of_fn::__impl(std::move(__first), std::move(__last), __pred, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    [[nodiscard]] constexpr bool operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        return !__fcxx_any_of_fn::__impl(ranges::begin(__r), ranges::end(__r), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    bool operator()(_Ep&&, _It, _Sp, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    bool operator()(_Ep&&, _Rp&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_none_of_fn none_of{};

/* [alg.find], find */
struct __fcxx_find_fn {
    template <class _It, class _Sp, class _Tp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, const _Tp& __value, _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (bool(std::invoke(__proj, *__first) == __value)) {
                break;
            }
        }
        return __first;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>>
        requires indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, const _Tp& __value, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __value, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>>
        requires indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, const _Tp& __value,
                                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __value, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, class _Tp = projected_value_t<_It, _Proj> >
        requires indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    _It operator()(_Ep&&, _It, _Sp, const _Tp&, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj> >
        requires indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, const _Tp&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_find_fn find{};

struct __fcxx_find_if_fn {
    template <class _It, class _Sp, class _Pred, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Pred& __pred, _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                break;
            }
        }
        return __first;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, _Pred __pred, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __pred, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    _It operator()(_Ep&&, _It, _Sp, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_find_if_fn find_if{};

struct __fcxx_find_if_not_fn {
    template <class _It, class _Sp, class _Pred, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Pred& __pred, _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (!bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                break;
            }
        }
        return __first;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, _Pred __pred, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __pred, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    _It operator()(_Ep&&, _It, _Sp, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_find_if_not_fn find_if_not{};

/* [alg.contains], contains */
struct __fcxx_contains_fn {
    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>>
        requires indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    [[nodiscard]] constexpr bool operator()(_It __first, _Sp __last, const _Tp& __value, _Proj __proj = {}) const {
        return __fcxx_find_fn::__impl(std::move(__first), __last, __value, __proj) != __last;
    }

    template <input_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>>
        requires indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    [[nodiscard]] constexpr bool operator()(_Rp&& __r, const _Tp& __value, _Proj __proj = {}) const {
        return __fcxx_find_fn::__impl(ranges::begin(__r), ranges::end(__r), __value, __proj) != ranges::end(__r);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, class _Tp = projected_value_t<_It, _Proj> >
        requires indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    bool operator()(_Ep&&, _It, _Sp, const _Tp&, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj> >
        requires indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    bool operator()(_Ep&&, _Rp&&, const _Tp&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_contains_fn contains{};

/* [alg.foreach], for each */
template <class _It, class _Fp>
using for_each_result = in_fun_result<_It, _Fp>;

struct __fcxx_for_each_fn {
    template <class _It, class _Sp, class _Fun, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Fun& __f, _Proj& __proj) {
        for (; __first != __last; ++__first) {
            std::invoke(__f, std::invoke(__proj, *__first));
        }
        return __first;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirectly_unary_invocable<projected<_It, _Proj> > _Fun>
    constexpr for_each_result<_It, _Fun> operator()(_It __first, _Sp __last, _Fun __f, _Proj __proj = {}) const {
        _It __end = __impl(std::move(__first), std::move(__last), __f, __proj);
        return {std::move(__end), std::move(__f)};
    }

    template <input_range _Rp, class _Proj = identity,
              indirectly_unary_invocable<projected<iterator_t<_Rp>, _Proj> > _Fun>
    constexpr for_each_result<borrowed_iterator_t<_Rp>, _Fun> operator()(_Rp&& __r, _Fun __f,
                                                                         _Proj __proj = {}) const {
        iterator_t<_Rp> __end = __impl(ranges::begin(__r), ranges::end(__r), __f, __proj);
        return {std::move(__end), std::move(__f)};
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirectly_unary_invocable<projected<_It, _Proj> > _Fun>
    _It operator()(_Ep&&, _It, _Sp, _Fun, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirectly_unary_invocable<projected<iterator_t<_Rp>, _Proj> > _Fun>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Fun, _Proj = {}) const = delete;
};

inline constexpr __fcxx_for_each_fn for_each{};

template <class _It, class _Fp>
using for_each_n_result = in_fun_result<_It, _Fp>;

struct __fcxx_for_each_n_fn {
    template <input_iterator _It, class _Proj = identity, indirectly_unary_invocable<projected<_It, _Proj> > _Fun>
    constexpr for_each_n_result<_It, _Fun> operator()(_It __first, iter_difference_t<_It> __n, _Fun __f,
                                                      _Proj __proj = {}) const {
        for (; __n > 0; --__n, (void)++__first) {
            std::invoke(__f, std::invoke(__proj, *__first));
        }
        return {std::move(__first), std::move(__f)};
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, class _Proj = identity,
              indirectly_unary_invocable<projected<_It, _Proj> > _Fun>
    _It operator()(_Ep&&, _It, iter_difference_t<_It>, _Fun, _Proj = {}) const = delete;
};

inline constexpr __fcxx_for_each_n_fn for_each_n{};

/* [alg.find.last], find last, which searches from the back where the
 * iterators are bidirectional. */
template <class _It, class _Sp, class _Pred>
constexpr subrange<_It> __fcxx_find_last(_It __first, _Sp __last, _Pred& __pred) {
    if constexpr (bidirectional_iterator<_It>) {
        _It __end = ranges::next(__first, std::move(__last));
        _It __i = __end;
        while (__i != __first) {
            --__i;
            if (__pred(*__i)) {
                return {std::move(__i), std::move(__end)};
            }
        }
        return {__end, __end};
    } else {
        _It __found = __first;
        bool __any = false;
        for (; __first != __last; ++__first) {
            if (__pred(*__first)) {
                __found = __first;
                __any = true;
            }
        }
        if (!__any) {
            return {__first, __first};
        }
        return {std::move(__found), std::move(__first)};
    }
}

template <class _Tp, class _Proj>
struct __fcxx_equal_value_pred {
    const _Tp& __value;
    _Proj& __proj;

    template <class _Up>
    constexpr bool operator()(_Up&& __u) const {
        return bool(std::invoke(__proj, std::forward<_Up>(__u)) == __value);
    }
};

template <class _Pred, class _Proj>
struct __fcxx_proj_not_pred {
    _Pred& __pred;
    _Proj& __proj;

    template <class _Up>
    constexpr bool operator()(_Up&& __u) const {
        return !bool(std::invoke(__pred, std::invoke(__proj, std::forward<_Up>(__u))));
    }
};

struct __fcxx_find_last_fn {
    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>>
        requires indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    [[nodiscard]] constexpr subrange<_It> operator()(_It __first, _Sp __last, const _Tp& __value,
                                                       _Proj __proj = {}) const {
        __fcxx_equal_value_pred<_Tp, _Proj> __pred{__value, __proj};
        return ranges::__fcxx_find_last(std::move(__first), std::move(__last), __pred);
    }

    template <forward_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>>
        requires indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    [[nodiscard]] constexpr borrowed_subrange_t<_Rp>
    operator()(_Rp&& __r, const _Tp& __value, _Proj __proj = {}) const {
        __fcxx_equal_value_pred<_Tp, _Proj> __pred{__value, __proj};
        return ranges::__fcxx_find_last(ranges::begin(__r), ranges::end(__r), __pred);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, class _Tp = projected_value_t<_It, _Proj> >
        requires indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    subrange<_It> operator()(_Ep&&, _It, _Sp, const _Tp&, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj> >
        requires indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    borrowed_subrange_t<_Rp> operator()(_Ep&&, _Rp&&, const _Tp&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_find_last_fn find_last{};

struct __fcxx_find_last_if_fn {
    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    [[nodiscard]] constexpr subrange<_It> operator()(_It __first, _Sp __last, _Pred __pred,
                                                       _Proj __proj = {}) const {
        __fcxx_proj_pred<_Pred, _Proj> __p{__pred, __proj};
        return ranges::__fcxx_find_last(std::move(__first), std::move(__last), __p);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    [[nodiscard]] constexpr borrowed_subrange_t<_Rp>
    operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        __fcxx_proj_pred<_Pred, _Proj> __p{__pred, __proj};
        return ranges::__fcxx_find_last(ranges::begin(__r), ranges::end(__r), __p);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    subrange<_It> operator()(_Ep&&, _It, _Sp, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    borrowed_subrange_t<_Rp> operator()(_Ep&&, _Rp&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_find_last_if_fn find_last_if{};

struct __fcxx_find_last_if_not_fn {
    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    [[nodiscard]] constexpr subrange<_It> operator()(_It __first, _Sp __last, _Pred __pred,
                                                       _Proj __proj = {}) const {
        __fcxx_proj_not_pred<_Pred, _Proj> __p{__pred, __proj};
        return ranges::__fcxx_find_last(std::move(__first), std::move(__last), __p);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    [[nodiscard]] constexpr borrowed_subrange_t<_Rp>
    operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        __fcxx_proj_not_pred<_Pred, _Proj> __p{__pred, __proj};
        return ranges::__fcxx_find_last(ranges::begin(__r), ranges::end(__r), __p);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    subrange<_It> operator()(_Ep&&, _It, _Sp, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    borrowed_subrange_t<_Rp> operator()(_Ep&&, _Rp&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_find_last_if_not_fn find_last_if_not{};

/* [alg.find.end], find end, which tries no more positions than there is
 * room for the sequence at, from the back where it can. */
struct __fcxx_find_end_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Pred, class _Proj1, class _Proj2>
    static constexpr subrange<_I1> __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2, _Pred& __pred,
                                          _Proj1& __proj1, _Proj2& __proj2) {
        const iter_difference_t<_I2> __len2 = ranges::distance(__first2, __last2);
        if constexpr (bidirectional_iterator<_I1>) {
            const iter_difference_t<_I1> __len1 = ranges::distance(__first1, __last1);
            _I1 __end1 = ranges::next(__first1, std::move(__last1));
            if (__len2 == 0 || __len1 < __len2) {
                return {__end1, __end1};
            }
            _I1 __start = ranges::next(__first1, static_cast<iter_difference_t<_I1> >(__len1 - __len2));
            while (true) {
                _I1 __i1 = __start;
                _I2 __i2 = __first2;
                while (__i2 != __last2
                       && bool(std::invoke(__pred, std::invoke(__proj1, *__i1), std::invoke(__proj2, *__i2)))) {
                    ++__i1;
                    ++__i2;
                }
                if (__i2 == __last2) {
                    return {std::move(__start), std::move(__i1)};
                }
                if (__start == __first1) {
                    return {__end1, __end1};
                }
                --__start;
            }
        } else {
            const iter_difference_t<_I1> __len1 = ranges::distance(__first1, __last1);
            if (__len2 == 0 || __len1 < __len2) {
                _I1 __end1 = ranges::next(std::move(__first1), std::move(__last1));
                return {__end1, __end1};
            }
            _I1 __found = __first1;
            _I1 __found_end = __first1;
            bool __any = false;
            for (iter_difference_t<_I1> __n = __len1 - __len2; __n >= 0; --__n, (void)++__first1) {
                _I1 __i1 = __first1;
                _I2 __i2 = __first2;
                while (__i2 != __last2
                       && bool(std::invoke(__pred, std::invoke(__proj1, *__i1), std::invoke(__proj2, *__i2)))) {
                    ++__i1;
                    ++__i2;
                }
                if (__i2 == __last2) {
                    __found = __first1;
                    __found_end = std::move(__i1);
                    __any = true;
                }
            }
            if (!__any) {
                _I1 __end1 = ranges::next(std::move(__first1), std::move(__last1));
                return {__end1, __end1};
            }
            return {std::move(__found), std::move(__found_end)};
        }
    }

    template <forward_iterator _I1, sentinel_for<_I1> _S1, forward_iterator _I2, sentinel_for<_I2> _S2,
              class _Pred = ranges::equal_to, class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr subrange<_I1> operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                                       _Pred __pred = {}, _Proj1 __proj1 = {},
                                                       _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __pred,
                      __proj1, __proj2);
    }

    template <forward_range _R1, forward_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr borrowed_subrange_t<_R1>
    operator()(_R1&& __r1, _R2&& __r2, _Pred __pred = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2), __pred,
                      __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Pred = ranges::equal_to,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    subrange<_I1> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    borrowed_subrange_t<_R1>
    operator()(_Ep&&, _R1&&, _R2&&, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_find_end_fn find_end{};

/* [alg.find.first.of], find first of */
struct __fcxx_find_first_of_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Pred, class _Proj1, class _Proj2>
    static constexpr _I1 __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2, _Pred& __pred,
                                _Proj1& __proj1, _Proj2& __proj2) {
        for (; __first1 != __last1; ++__first1) {
            for (_I2 __i = __first2; __i != __last2; ++__i) {
                if (bool(std::invoke(__pred, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__i)))) {
                    return __first1;
                }
            }
        }
        return __first1;
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, forward_iterator _I2, sentinel_for<_I2> _S2,
              class _Pred = ranges::equal_to, class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr _I1 operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2, _Pred __pred = {},
                                             _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __pred,
                      __proj1, __proj2);
    }

    template <input_range _R1, forward_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr borrowed_iterator_t<_R1> operator()(_R1&& __r1, _R2&& __r2, _Pred __pred = {},
                                                                  _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2), __pred,
                      __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Pred = ranges::equal_to,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    _I1 operator()(_Ep&&, _I1, _S1, _I2, _S2, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    borrowed_iterator_t<_R1> operator()(_Ep&&, _R1&&, _R2&&, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_find_first_of_fn find_first_of{};

/* [alg.adjacent.find], adjacent find */
struct __fcxx_adjacent_find_fn {
    template <class _It, class _Sp, class _Pred, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Pred& __pred, _Proj& __proj) {
        if (__first == __last) {
            return __first;
        }
        _It __next = __first;
        while (++__next != __last) {
            if (bool(std::invoke(__pred, std::invoke(__proj, *__first), std::invoke(__proj, *__next)))) {
                return __first;
            }
            __first = __next;
        }
        return __next;
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_binary_predicate<projected<_It, _Proj>, projected<_It, _Proj> > _Pred = ranges::equal_to>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, _Pred __pred = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __pred, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_binary_predicate<projected<iterator_t<_Rp>, _Proj>, projected<iterator_t<_Rp>, _Proj> > _Pred =
                  ranges::equal_to>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Pred __pred = {},
                                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity,
              indirect_binary_predicate<projected<_It, _Proj>, projected<_It, _Proj> > _Pred = ranges::equal_to>
    _It operator()(_Ep&&, _It, _Sp, _Pred = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_binary_predicate<projected<iterator_t<_Rp>, _Proj>, projected<iterator_t<_Rp>, _Proj> >
                  _Pred = ranges::equal_to>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Pred = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_adjacent_find_fn adjacent_find{};

/* [alg.count], count */
struct __fcxx_count_fn {
    template <class _It, class _Sp, class _Tp, class _Proj>
    static constexpr iter_difference_t<_It> __impl(_It __first, _Sp __last, const _Tp& __value, _Proj& __proj) {
        iter_difference_t<_It> __n = 0;
        for (; __first != __last; ++__first) {
            if (bool(std::invoke(__proj, *__first) == __value)) {
                ++__n;
            }
        }
        return __n;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>>
        requires indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    [[nodiscard]] constexpr iter_difference_t<_It> operator()(_It __first, _Sp __last, const _Tp& __value,
                                                                _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __value, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>>
        requires indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    [[nodiscard]] constexpr range_difference_t<_Rp> operator()(_Rp&& __r, const _Tp& __value,
                                                                 _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __value, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, class _Tp = projected_value_t<_It, _Proj> >
        requires indirect_binary_predicate<ranges::equal_to, projected<_It, _Proj>, const _Tp*>
    iter_difference_t<_It> operator()(_Ep&&, _It, _Sp, const _Tp&, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj> >
        requires indirect_binary_predicate<ranges::equal_to, projected<iterator_t<_Rp>, _Proj>, const _Tp*>
    range_difference_t<_Rp> operator()(_Ep&&, _Rp&&, const _Tp&, _Proj = {}) const = delete;
};

inline constexpr __fcxx_count_fn count{};

struct __fcxx_count_if_fn {
    template <class _It, class _Sp, class _Pred, class _Proj>
    static constexpr iter_difference_t<_It> __impl(_It __first, _Sp __last, _Pred& __pred, _Proj& __proj) {
        iter_difference_t<_It> __n = 0;
        for (; __first != __last; ++__first) {
            if (bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                ++__n;
            }
        }
        return __n;
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    [[nodiscard]] constexpr iter_difference_t<_It> operator()(_It __first, _Sp __last, _Pred __pred,
                                                                _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __pred, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    [[nodiscard]] constexpr range_difference_t<_Rp> operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    iter_difference_t<_It> operator()(_Ep&&, _It, _Sp, _Pred, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    range_difference_t<_Rp> operator()(_Ep&&, _Rp&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_count_if_fn count_if{};

/* [alg.mismatch], mismatch */
template <class _I1, class _I2>
using mismatch_result = in_in_result<_I1, _I2>;

struct __fcxx_mismatch_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Pred, class _Proj1, class _Proj2>
    static constexpr mismatch_result<_I1, _I2> __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                                      _Pred& __pred, _Proj1& __proj1, _Proj2& __proj2) {
        for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2) {
            if (!bool(std::invoke(__pred, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2)))) {
                break;
            }
        }
        return {std::move(__first1), std::move(__first2)};
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              class _Pred = ranges::equal_to, class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr mismatch_result<_I1, _I2> operator()(_I1 __first1, _S1 __last1, _I2 __first2,
                                                                   _S2 __last2, _Pred __pred = {},
                                                                   _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __pred,
                      __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr mismatch_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2> >
    operator()(_R1&& __r1, _R2&& __r2, _Pred __pred = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2), __pred,
                      __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Pred = ranges::equal_to,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    mismatch_result<_I1, _I2> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Pred = {}, _Proj1 = {},
                                         _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    mismatch_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2> >
    operator()(_Ep&&, _R1&&, _R2&&, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_mismatch_fn mismatch{};

/* [alg.equal], equal, which compares no elements where the ranges have
 * lengths that are known and different. */
struct __fcxx_equal_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Pred, class _Proj1, class _Proj2>
    static constexpr bool __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2, _Pred& __pred,
                                 _Proj1& __proj1, _Proj2& __proj2) {
        for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2) {
            if (!bool(std::invoke(__pred, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2)))) {
                return false;
            }
        }
        return __first1 == __last1 && __first2 == __last2;
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              class _Pred = ranges::equal_to, class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr bool operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                              _Pred __pred = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        if constexpr (sized_sentinel_for<_S1, _I1> && sized_sentinel_for<_S2, _I2>) {
            if (__last1 - __first1 != __last2 - __first2) {
                return false;
            }
        }
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __pred,
                      __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr bool operator()(_R1&& __r1, _R2&& __r2, _Pred __pred = {}, _Proj1 __proj1 = {},
                                              _Proj2 __proj2 = {}) const {
        if constexpr (sized_range<_R1> && sized_range<_R2>) {
            if (ranges::distance(__r1) != ranges::distance(__r2)) {
                return false;
            }
        }
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2), __pred,
                      __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Pred = ranges::equal_to,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    bool operator()(_Ep&&, _I1, _S1, _I2, _S2, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    bool operator()(_Ep&&, _R1&&, _R2&&, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_equal_fn equal{};

/* [alg.is.permutation], is permutation, which counts the elements of the
 * rest of the ranges after their common prefix, as the algorithm of
 * namespace std does. */
struct __fcxx_is_permutation_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Pred, class _Proj1, class _Proj2>
    static constexpr bool __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2, _Pred& __pred,
                                 _Proj1& __proj1, _Proj2& __proj2) {
        for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2) {
            if (!bool(std::invoke(__pred, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2)))) {
                break;
            }
        }
        if (__first1 == __last1 || __first2 == __last2) {
            return __first1 == __last1 && __first2 == __last2;
        }
        if (ranges::distance(__first1, __last1) != ranges::distance(__first2, __last2)) {
            return false;
        }
        _I1 __end1 = ranges::next(__first1, std::move(__last1));
        _I2 __end2 = ranges::next(__first2, std::move(__last2));
        __fcxx_proj_comp<_Pred, _Proj1> __pred11{__pred, __proj1};
        __fcxx_proj_comp2<_Pred, _Proj1, _Proj2> __pred12{__pred, __proj1, __proj2};
        return std::__fcxx_is_permutation_rest<iter_difference_t<_I1> >(std::move(__first1), std::move(__end1),
                                                                        std::move(__first2), std::move(__end2),
                                                                        __pred11, __pred12);
    }

    template <forward_iterator _I1, sentinel_for<_I1> _S1, forward_iterator _I2, sentinel_for<_I2> _S2,
              class _Proj1 = identity, class _Proj2 = identity,
              indirect_equivalence_relation<projected<_I1, _Proj1>, projected<_I2, _Proj2> > _Pred = ranges::equal_to>
    [[nodiscard]] constexpr bool operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                              _Pred __pred = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        if constexpr (sized_sentinel_for<_S1, _I1> && sized_sentinel_for<_S2, _I2>) {
            if (__last1 - __first1 != __last2 - __first2) {
                return false;
            }
        }
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __pred,
                      __proj1, __proj2);
    }

    template <forward_range _R1, forward_range _R2, class _Proj1 = identity, class _Proj2 = identity,
              indirect_equivalence_relation<projected<iterator_t<_R1>, _Proj1>, projected<iterator_t<_R2>, _Proj2> >
                  _Pred = ranges::equal_to>
    [[nodiscard]] constexpr bool operator()(_R1&& __r1, _R2&& __r2, _Pred __pred = {}, _Proj1 __proj1 = {},
                                              _Proj2 __proj2 = {}) const {
        if constexpr (sized_range<_R1> && sized_range<_R2>) {
            if (ranges::distance(__r1) != ranges::distance(__r2)) {
                return false;
            }
        }
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2), __pred,
                      __proj1, __proj2);
    }
};

inline constexpr __fcxx_is_permutation_fn is_permutation{};

/* [alg.search], search, which tries each position in turn. */
struct __fcxx_search_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Pred, class _Proj1, class _Proj2>
    static constexpr subrange<_I1> __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2, _Pred& __pred,
                                          _Proj1& __proj1, _Proj2& __proj2) {
        if (__first2 == __last2) {
            return {__first1, __first1};
        }
        for (;; ++__first1) {
            _I1 __i1 = __first1;
            _I2 __i2 = __first2;
            while (true) {
                if (__i1 == __last1) {
                    return {__i1, __i1};
                }
                if (!bool(std::invoke(__pred, std::invoke(__proj1, *__i1), std::invoke(__proj2, *__i2)))) {
                    break;
                }
                ++__i1;
                if (++__i2 == __last2) {
                    return {std::move(__first1), std::move(__i1)};
                }
            }
        }
    }

    template <forward_iterator _I1, sentinel_for<_I1> _S1, forward_iterator _I2, sentinel_for<_I2> _S2,
              class _Pred = ranges::equal_to, class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr subrange<_I1> operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                                       _Pred __pred = {}, _Proj1 __proj1 = {},
                                                       _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __pred,
                      __proj1, __proj2);
    }

    template <forward_range _R1, forward_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr borrowed_subrange_t<_R1>
    operator()(_R1&& __r1, _R2&& __r2, _Pred __pred = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2), __pred,
                      __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Pred = ranges::equal_to,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    subrange<_I1> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    borrowed_subrange_t<_R1>
    operator()(_Ep&&, _R1&&, _R2&&, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_search_fn search{};

/* A count that is not positive, or a sequence of count elements, each of
 * which is tested once. */
struct __fcxx_search_n_fn {
    template <class _It, class _Sp, class _Tp, class _Pred, class _Proj>
    static constexpr subrange<_It> __impl(_It __first, _Sp __last, iter_difference_t<_It> __count,
                                          const _Tp& __value, _Pred& __pred, _Proj& __proj) {
        if (__count <= 0) {
            return {__first, __first};
        }
        for (; __first != __last; ++__first) {
            if (!bool(std::invoke(__pred, std::invoke(__proj, *__first), __value))) {
                continue;
            }
            _It __start = __first;
            iter_difference_t<_It> __n = 1;
            while (true) {
                if (__n == __count) {
                    ++__first;
                    return {std::move(__start), std::move(__first)};
                }
                if (++__first == __last) {
                    return {__first, __first};
                }
                if (!bool(std::invoke(__pred, std::invoke(__proj, *__first), __value))) {
                    break;
                }
                ++__n;
            }
        }
        return {__first, __first};
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Pred = ranges::equal_to, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>>
        requires indirectly_comparable<_It, const _Tp*, _Pred, _Proj>
    [[nodiscard]] constexpr subrange<_It> operator()(_It __first, _Sp __last, iter_difference_t<_It> __count,
                                                       const _Tp& __value, _Pred __pred = {},
                                                       _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __count, __value, __pred, __proj);
    }

    template <forward_range _Rp, class _Pred = ranges::equal_to, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>>
        requires indirectly_comparable<iterator_t<_Rp>, const _Tp*, _Pred, _Proj>
    [[nodiscard]] constexpr borrowed_subrange_t<_Rp>
    operator()(_Rp&& __r, range_difference_t<_Rp> __count, const _Tp& __value, _Pred __pred = {},
               _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __count, __value, __pred, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Pred = ranges::equal_to, class _Proj = identity, class _Tp = projected_value_t<_It, _Proj> >
        requires indirectly_comparable<_It, const _Tp*, _Pred, _Proj>
    subrange<_It> operator()(_Ep&&, _It, _Sp, iter_difference_t<_It>, const _Tp&, _Pred = {},
                             _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Pred = ranges::equal_to,
              class _Proj = identity, class _Tp = projected_value_t<iterator_t<_Rp>, _Proj> >
        requires indirectly_comparable<iterator_t<_Rp>, const _Tp*, _Pred, _Proj>
    borrowed_subrange_t<_Rp>
    operator()(_Ep&&, _Rp&&, range_difference_t<_Rp>, const _Tp&, _Pred = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_search_n_fn search_n{};

/* [alg.contains], contains_subrange */
struct __fcxx_contains_subrange_fn {
    template <forward_iterator _I1, sentinel_for<_I1> _S1, forward_iterator _I2, sentinel_for<_I2> _S2,
              class _Pred = ranges::equal_to, class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr bool operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                              _Pred __pred = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __first2 == __last2 || !__fcxx_search_fn::__impl(std::move(__first1), std::move(__last1),
                                                                std::move(__first2), std::move(__last2), __pred,
                                                                __proj1, __proj2)
                                            .empty();
    }

    template <forward_range _R1, forward_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr bool operator()(_R1&& __r1, _R2&& __r2, _Pred __pred = {}, _Proj1 __proj1 = {},
                                              _Proj2 __proj2 = {}) const {
        iterator_t<_R2> __first2 = ranges::begin(__r2);
        sentinel_t<_R2> __last2 = ranges::end(__r2);
        return __first2 == __last2 || !__fcxx_search_fn::__impl(ranges::begin(__r1), ranges::end(__r1),
                                                                std::move(__first2), std::move(__last2), __pred,
                                                                __proj1, __proj2)
                                            .empty();
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Pred = ranges::equal_to,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    bool operator()(_Ep&&, _I1, _S1, _I2, _S2, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    bool operator()(_Ep&&, _R1&&, _R2&&, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_contains_subrange_fn contains_subrange{};

/* [alg.starts.with], starts with */
struct __fcxx_starts_with_fn {
    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              class _Pred = ranges::equal_to, class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr bool operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                              _Pred __pred = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __fcxx_mismatch_fn::__impl(std::move(__first1), std::move(__last1), std::move(__first2), __last2,
                                          __pred, __proj1, __proj2)
                   .in2
               == __last2;
    }

    template <input_range _R1, input_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr bool operator()(_R1&& __r1, _R2&& __r2, _Pred __pred = {}, _Proj1 __proj1 = {},
                                              _Proj2 __proj2 = {}) const {
        return __fcxx_mismatch_fn::__impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2),
                                          ranges::end(__r2), __pred, __proj1, __proj2)
                   .in2
               == ranges::end(__r2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Pred = ranges::equal_to,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    bool operator()(_Ep&&, _I1, _S1, _I2, _S2, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    bool operator()(_Ep&&, _R1&&, _R2&&, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_starts_with_fn starts_with{};

/* [alg.ends.with], ends with, which compares the last elements of the first
 * range, as many as there are elements in the second. */
struct __fcxx_ends_with_fn {
    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              class _Pred = ranges::equal_to, class _Proj1 = identity, class _Proj2 = identity>
        requires(forward_iterator<_I1> || sized_sentinel_for<_S1, _I1>)
                && (forward_iterator<_I2> || sized_sentinel_for<_S2, _I2>)
                && indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr bool operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                              _Pred __pred = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        const iter_difference_t<_I1> __n1 = ranges::distance(__first1, __last1);
        const iter_difference_t<_I2> __n2 = ranges::distance(__first2, __last2);
        if (__n1 < __n2) {
            return false;
        }
        ranges::advance(__first1, static_cast<iter_difference_t<_I1> >(__n1 - __n2));
        return __fcxx_equal_fn::__impl(std::move(__first1), std::move(__last1), std::move(__first2),
                                       std::move(__last2), __pred, __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires(forward_range<_R1> || sized_range<_R1>) && (forward_range<_R2> || sized_range<_R2>)
                && indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    [[nodiscard]] constexpr bool operator()(_R1&& __r1, _R2&& __r2, _Pred __pred = {}, _Proj1 __proj1 = {},
                                              _Proj2 __proj2 = {}) const {
        const range_difference_t<_R1> __n1 = ranges::distance(__r1);
        const range_difference_t<_R2> __n2 = ranges::distance(__r2);
        if (__n1 < __n2) {
            return false;
        }
        iterator_t<_R1> __first1 = ranges::begin(__r1);
        ranges::advance(__first1, static_cast<range_difference_t<_R1> >(__n1 - __n2), ranges::end(__r1));
        return __fcxx_equal_fn::__impl(std::move(__first1), ranges::end(__r1), ranges::begin(__r2),
                                       ranges::end(__r2), __pred, __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Pred = ranges::equal_to,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_comparable<_I1, _I2, _Pred, _Proj1, _Proj2>
    bool operator()(_Ep&&, _I1, _S1, _I2, _S2, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Pred = ranges::equal_to, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_comparable<iterator_t<_R1>, iterator_t<_R2>, _Pred, _Proj1, _Proj2>
    bool operator()(_Ep&&, _R1&&, _R2&&, _Pred = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_ends_with_fn ends_with{};

/* [alg.fold], fold */
template <class _Fp>
class __fcxx_flipped {
    _Fp __f;

public:
    template <class _Tp, class _Up>
        requires invocable<_Fp&, _Up, _Tp>
    invoke_result_t<_Fp&, _Up, _Tp> operator()(_Tp&&, _Up&&);
};

template <class _Fp, class _Tp, class _It, class _Up>
concept __fcxx_indirectly_binary_left_foldable_impl =
    movable<_Tp> && movable<_Up> && convertible_to<_Tp, _Up> && invocable<_Fp&, _Up, iter_reference_t<_It> >
    && assignable_from<_Up&, invoke_result_t<_Fp&, _Up, iter_reference_t<_It> > >;

template <class _Fp, class _Tp, class _It>
concept __fcxx_indirectly_binary_left_foldable =
    copy_constructible<_Fp> && indirectly_readable<_It> && invocable<_Fp&, _Tp, iter_reference_t<_It> >
    && convertible_to<invoke_result_t<_Fp&, _Tp, iter_reference_t<_It> >,
                      decay_t<invoke_result_t<_Fp&, _Tp, iter_reference_t<_It> > > >
    && __fcxx_indirectly_binary_left_foldable_impl<_Fp, _Tp, _It,
                                                   decay_t<invoke_result_t<_Fp&, _Tp, iter_reference_t<_It> > > >;

template <class _Fp, class _Tp, class _It>
concept __fcxx_indirectly_binary_right_foldable = __fcxx_indirectly_binary_left_foldable<__fcxx_flipped<_Fp>, _Tp, _It>;

template <class _It, class _Tp>
using fold_left_with_iter_result = in_value_result<_It, _Tp>;

template <class _It, class _Tp>
using fold_left_first_with_iter_result = in_value_result<_It, _Tp>;

struct __fcxx_fold_left_with_iter_fn {
    template <class _It, class _Sp, class _Tp, class _Fp>
    static constexpr auto __impl(_It __first, _Sp __last, _Tp __init, _Fp& __f) {
        using _Up = decay_t<invoke_result_t<_Fp&, _Tp, iter_reference_t<_It> > >;
        using _Ret = fold_left_with_iter_result<_It, _Up>;
        if (__first == __last) {
            return _Ret{std::move(__first), _Up(std::move(__init))};
        }
        _Up __accum = std::invoke(__f, std::move(__init), *__first);
        for (++__first; __first != __last; ++__first) {
            __accum = std::invoke(__f, std::move(__accum), *__first);
        }
        return _Ret{std::move(__first), std::move(__accum)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, class _Tp = iter_value_t<_It>,
              __fcxx_indirectly_binary_left_foldable<_Tp, _It> _Fp>
    constexpr auto operator()(_It __first, _Sp __last, _Tp __init, _Fp __f) const {
        return __impl(std::move(__first), std::move(__last), std::move(__init), __f);
    }

    template <input_range _Rp, class _Tp = range_value_t<_Rp>,
              __fcxx_indirectly_binary_left_foldable<_Tp, iterator_t<_Rp> > _Fp>
    constexpr auto operator()(_Rp&& __r, _Tp __init, _Fp __f) const {
        using _Up = decay_t<invoke_result_t<_Fp&, _Tp, range_reference_t<_Rp> > >;
        auto __res = __impl(ranges::begin(__r), ranges::end(__r), std::move(__init), __f);
        return fold_left_with_iter_result<borrowed_iterator_t<_Rp>, _Up>{std::move(__res.in), std::move(__res.value)};
    }
};

inline constexpr __fcxx_fold_left_with_iter_fn fold_left_with_iter{};

struct __fcxx_fold_left_fn {
    template <input_iterator _It, sentinel_for<_It> _Sp, class _Tp = iter_value_t<_It>,
              __fcxx_indirectly_binary_left_foldable<_Tp, _It> _Fp>
    [[nodiscard]] constexpr auto operator()(_It __first, _Sp __last, _Tp __init, _Fp __f) const {
        return __fcxx_fold_left_with_iter_fn::__impl(std::move(__first), std::move(__last), std::move(__init), __f)
            .value;
    }

    template <input_range _Rp, class _Tp = range_value_t<_Rp>,
              __fcxx_indirectly_binary_left_foldable<_Tp, iterator_t<_Rp> > _Fp>
    [[nodiscard]] constexpr auto operator()(_Rp&& __r, _Tp __init, _Fp __f) const {
        return __fcxx_fold_left_with_iter_fn::__impl(ranges::begin(__r), ranges::end(__r), std::move(__init), __f)
            .value;
    }
};

inline constexpr __fcxx_fold_left_fn fold_left{};

struct __fcxx_fold_left_first_with_iter_fn {
    template <class _It, class _Sp, class _Fp>
    static constexpr auto __impl(_It __first, _Sp __last, _Fp& __f) {
        using _Up = decay_t<invoke_result_t<_Fp&, iter_value_t<_It>, iter_reference_t<_It> > >;
        using _Ret = fold_left_first_with_iter_result<_It, optional<_Up> >;
        if (__first == __last) {
            return _Ret{std::move(__first), optional<_Up>()};
        }
        optional<_Up> __init(in_place, *__first);
        for (++__first; __first != __last; ++__first) {
            *__init = std::invoke(__f, std::move(*__init), *__first);
        }
        return _Ret{std::move(__first), std::move(__init)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp,
              __fcxx_indirectly_binary_left_foldable<iter_value_t<_It>, _It> _Fp>
        requires constructible_from<iter_value_t<_It>, iter_reference_t<_It> >
    constexpr auto operator()(_It __first, _Sp __last, _Fp __f) const {
        return __impl(std::move(__first), std::move(__last), __f);
    }

    template <input_range _Rp, __fcxx_indirectly_binary_left_foldable<range_value_t<_Rp>, iterator_t<_Rp> > _Fp>
        requires constructible_from<range_value_t<_Rp>, range_reference_t<_Rp> >
    constexpr auto operator()(_Rp&& __r, _Fp __f) const {
        using _Up = decay_t<invoke_result_t<_Fp&, range_value_t<_Rp>, range_reference_t<_Rp> > >;
        auto __res = __impl(ranges::begin(__r), ranges::end(__r), __f);
        return fold_left_first_with_iter_result<borrowed_iterator_t<_Rp>, optional<_Up> >{std::move(__res.in),
                                                                                            std::move(__res.value)};
    }
};

inline constexpr __fcxx_fold_left_first_with_iter_fn fold_left_first_with_iter{};

struct __fcxx_fold_left_first_fn {
    template <input_iterator _It, sentinel_for<_It> _Sp,
              __fcxx_indirectly_binary_left_foldable<iter_value_t<_It>, _It> _Fp>
        requires constructible_from<iter_value_t<_It>, iter_reference_t<_It> >
    [[nodiscard]] constexpr auto operator()(_It __first, _Sp __last, _Fp __f) const {
        return __fcxx_fold_left_first_with_iter_fn::__impl(std::move(__first), std::move(__last), __f).value;
    }

    template <input_range _Rp, __fcxx_indirectly_binary_left_foldable<range_value_t<_Rp>, iterator_t<_Rp> > _Fp>
        requires constructible_from<range_value_t<_Rp>, range_reference_t<_Rp> >
    [[nodiscard]] constexpr auto operator()(_Rp&& __r, _Fp __f) const {
        return __fcxx_fold_left_first_with_iter_fn::__impl(ranges::begin(__r), ranges::end(__r), __f).value;
    }
};

inline constexpr __fcxx_fold_left_first_fn fold_left_first{};

/* Workaround: GCC 16, all targets: where a right fold of a range of one
 * element is inlined along with other code, its -Warray-bounds (of -Wall)
 * can take the *--tail of fold_right for a read before the first element, on
 * a path that cannot be taken, as fold_right returns at once for the range
 * of no element that fold_right_last gives it, and it warns even where the
 * headers are system headers. The warning is turned off for the right
 * folds. */
#  if _FCXX_GCC >= 1600
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Warray-bounds"
#  endif
struct __fcxx_fold_right_fn {
    template <class _It, class _Sp, class _Tp, class _Fp>
    static constexpr auto __impl(_It __first, _Sp __last, _Tp __init, _Fp& __f) {
        using _Up = decay_t<invoke_result_t<_Fp&, iter_reference_t<_It>, _Tp> >;
        if (__first == __last) {
            return _Up(std::move(__init));
        }
        _It __tail = ranges::next(__first, std::move(__last));
        _Up __accum = std::invoke(__f, *--__tail, std::move(__init));
        while (__first != __tail) {
            __accum = std::invoke(__f, *--__tail, std::move(__accum));
        }
        return __accum;
    }

    template <bidirectional_iterator _It, sentinel_for<_It> _Sp, class _Tp = iter_value_t<_It>,
              __fcxx_indirectly_binary_right_foldable<_Tp, _It> _Fp>
    [[nodiscard]] constexpr auto operator()(_It __first, _Sp __last, _Tp __init, _Fp __f) const {
        return __impl(std::move(__first), std::move(__last), std::move(__init), __f);
    }

    template <bidirectional_range _Rp, class _Tp = range_value_t<_Rp>,
              __fcxx_indirectly_binary_right_foldable<_Tp, iterator_t<_Rp> > _Fp>
    [[nodiscard]] constexpr auto operator()(_Rp&& __r, _Tp __init, _Fp __f) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__init), __f);
    }
};

inline constexpr __fcxx_fold_right_fn fold_right{};

struct __fcxx_fold_right_last_fn {
    template <class _It, class _Sp, class _Fp>
    static constexpr auto __impl(_It __first, _Sp __last, _Fp& __f) {
        using _Up = decay_t<invoke_result_t<_Fp&, iter_reference_t<_It>, iter_value_t<_It> > >;
        if (__first == __last) {
            return optional<_Up>();
        }
        _It __tail = ranges::prev(ranges::next(__first, std::move(__last)));
        iter_value_t<_It> __init(*__tail);
        return optional<_Up>(
            in_place, __fcxx_fold_right_fn::__impl(std::move(__first), std::move(__tail), std::move(__init), __f));
    }

    template <bidirectional_iterator _It, sentinel_for<_It> _Sp,
              __fcxx_indirectly_binary_right_foldable<iter_value_t<_It>, _It> _Fp>
        requires constructible_from<iter_value_t<_It>, iter_reference_t<_It> >
    [[nodiscard]] constexpr auto operator()(_It __first, _Sp __last, _Fp __f) const {
        return __impl(std::move(__first), std::move(__last), __f);
    }

    template <bidirectional_range _Rp,
              __fcxx_indirectly_binary_right_foldable<range_value_t<_Rp>, iterator_t<_Rp> > _Fp>
        requires constructible_from<range_value_t<_Rp>, range_reference_t<_Rp> >
    [[nodiscard]] constexpr auto operator()(_Rp&& __r, _Fp __f) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __f);
    }
};

inline constexpr __fcxx_fold_right_last_fn fold_right_last{};
#  if _FCXX_GCC >= 1600
#    pragma GCC diagnostic pop
#  endif

} /* namespace ranges */
} /* namespace std */

#endif
