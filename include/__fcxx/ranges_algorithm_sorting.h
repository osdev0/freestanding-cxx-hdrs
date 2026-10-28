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


/* The sorting and related operations of <algorithm>, of namespace ranges,
 * but stable_sort, stable_partition and inplace_merge, which allocate and
 * are not freestanding, with their deleted parallel overloads. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_ALGORITHM_SORTING_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_ALGORITHM_SORTING_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <__fcxx/algorithm_sorting.h>
#include <__fcxx/ranges_algorithm_base.h>
#include <__fcxx/ranges_algorithm_modifying.h>
#include <__fcxx/ranges_algorithm_nonmodifying.h>

namespace std {
namespace ranges {

/* [alg.sort], sort: an introsort, in O(N log N) comparisons. */
struct __fcxx_sort_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        _It __end = ranges::next(__first, std::move(__last));
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        std::__fcxx_sort<__fcxx_ranges_ops>(std::move(__first), __end, __c);
        return __end;
    }

    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    constexpr _It operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <random_access_range _Rp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    _It operator()(_Ep&&, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Comp = ranges::less,
              class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_sort_fn sort{};

/* [partial.sort], partial_sort */
struct __fcxx_partial_sort_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _It __middle, _Sp __last, _Comp& __comp, _Proj& __proj) {
        _It __end = ranges::next(__middle, std::move(__last));
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        std::__fcxx_partial_sort<__fcxx_ranges_ops>(std::move(__first), std::move(__middle), __end, __c);
        return __end;
    }

    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    constexpr _It operator()(_It __first, _It __middle, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__middle), std::move(__last), __comp, __proj);
    }

    template <random_access_range _Rp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, iterator_t<_Rp> __middle, _Comp __comp = {},
                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), std::move(__middle), ranges::end(__r), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    _It operator()(_Ep&&, _It, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Comp = ranges::less,
              class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, iterator_t<_Rp>, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_partial_sort_fn partial_sort{};

/* [partial.sort.copy], partial_sort_copy */
template <class _It, class _Out>
using partial_sort_copy_result = in_out_result<_It, _Out>;

struct __fcxx_partial_sort_copy_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Comp, class _Proj1, class _Proj2>
    static constexpr partial_sort_copy_result<_I1, _I2> __impl(_I1 __first, _S1 __last, _I2 __result_first,
                                                               _S2 __result_last, _Comp& __comp, _Proj1& __proj1,
                                                               _Proj2& __proj2) {
        __fcxx_proj_comp2<_Comp, _Proj1, _Proj2> __c12{__comp, __proj1, __proj2};
        __fcxx_proj_comp<_Comp, _Proj2> __c22{__comp, __proj2};
        _I2 __r = std::__fcxx_partial_sort_copy<__fcxx_ranges_ops>(__first, __last, std::move(__result_first),
                                                                   std::move(__result_last), __c12, __c22);
        ranges::advance(__first, std::move(__last));
        return {std::move(__first), std::move(__r)};
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, random_access_iterator _I2, sentinel_for<_I2> _S2,
              class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_copyable<_I1, _I2> && sortable<_I2, _Comp, _Proj2>
                 && indirect_strict_weak_order<_Comp, projected<_I1, _Proj1>, projected<_I2, _Proj2> >
    constexpr partial_sort_copy_result<_I1, _I2> operator()(_I1 __first, _S1 __last, _I2 __result_first,
                                                            _S2 __result_last, _Comp __comp = {}, _Proj1 __proj1 = {},
                                                            _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first), std::move(__last), std::move(__result_first), std::move(__result_last),
                      __comp, __proj1, __proj2);
    }

    template <input_range _R1, random_access_range _R2, class _Comp = ranges::less, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_copyable<iterator_t<_R1>, iterator_t<_R2> > && sortable<iterator_t<_R2>, _Comp, _Proj2>
                 && indirect_strict_weak_order<_Comp, projected<iterator_t<_R1>, _Proj1>,
                                               projected<iterator_t<_R2>, _Proj2> >
    constexpr partial_sort_copy_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2> >
    operator()(_R1&& __r, _R2&& __result_r, _Comp __comp = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), ranges::begin(__result_r), ranges::end(__result_r),
                      __comp, __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Comp = ranges::less,
              class _Proj1 = identity, class _Proj2 = identity>
        requires indirectly_copyable<_I1, _I2> && sortable<_I2, _Comp, _Proj2> &&
                     indirect_strict_weak_order<_Comp, projected<_I1, _Proj1>, projected<_I2, _Proj2> >
    partial_sort_copy_result<_I1, _I2> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Comp = {}, _Proj1 = {},
                                                  _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Comp = ranges::less, class _Proj1 = identity,
              class _Proj2 = identity>
        requires indirectly_copyable<iterator_t<_R1>, iterator_t<_R2> > &&
                     sortable<iterator_t<_R2>, _Comp, _Proj2> &&
                     indirect_strict_weak_order<_Comp, projected<iterator_t<_R1>, _Proj1>,
                                                projected<iterator_t<_R2>, _Proj2> >
    partial_sort_copy_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2> >
    operator()(_Ep&&, _R1&&, _R2&&, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_partial_sort_copy_fn partial_sort_copy{};

/* [is.sorted], is_sorted */
struct __fcxx_is_sorted_until_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        if (__first == __last) {
            return __first;
        }
        _It __next = __first;
        while (++__next != __last) {
            if (bool(std::invoke(__comp, std::invoke(__proj, *__next), std::invoke(__proj, *__first)))) {
                return __next;
            }
            __first = __next;
        }
        return __next;
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {},
                                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    _It operator()(_Ep&&, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_is_sorted_until_fn is_sorted_until{};

struct __fcxx_is_sorted_fn {
    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __fcxx_is_sorted_until_fn::__impl(std::move(__first), __last, __comp, __proj) == __last;
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return __fcxx_is_sorted_until_fn::__impl(ranges::begin(__r), ranges::end(__r), __comp, __proj)
               == ranges::end(__r);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    bool operator()(_Ep&&, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    bool operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_is_sorted_fn is_sorted{};

/* [alg.nth.element], nth_element */
struct __fcxx_nth_element_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _It __nth, _Sp __last, _Comp& __comp, _Proj& __proj) {
        _It __end = ranges::next(__nth, std::move(__last));
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        std::__fcxx_nth_element<__fcxx_ranges_ops>(std::move(__first), std::move(__nth), __end, __c);
        return __end;
    }

    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    constexpr _It operator()(_It __first, _It __nth, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__nth), std::move(__last), __comp, __proj);
    }

    template <random_access_range _Rp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, iterator_t<_Rp> __nth, _Comp __comp = {},
                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), std::move(__nth), ranges::end(__r), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    _It operator()(_Ep&&, _It, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Comp = ranges::less,
              class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, iterator_t<_Rp>, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_nth_element_fn nth_element{};

/* [alg.binary.search], binary search, which halves the length of the range
 * with each comparison. */
struct __fcxx_lower_bound_fn {
    template <class _It, class _Tp, class _Comp, class _Proj>
    static constexpr _It __impl_n(_It __first, iter_difference_t<_It> __len, const _Tp& __value, _Comp& __comp,
                                  _Proj& __proj) {
        while (__len > 0) {
            const iter_difference_t<_It> __half = __len / 2;
            _It __mid = ranges::next(__first, __half);
            if (bool(std::invoke(__comp, std::invoke(__proj, *__mid), __value))) {
                __first = ++__mid;
                __len -= __half + 1;
            } else {
                __len = __half;
            }
        }
        return __first;
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>,
              indirect_strict_weak_order<const _Tp*, projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, const _Tp& __value, _Comp __comp = {},
                                             _Proj __proj = {}) const {
        const iter_difference_t<_It> __len = ranges::distance(__first, __last);
        return __impl_n(std::move(__first), __len, __value, __comp, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>,
              indirect_strict_weak_order<const _Tp*, projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, const _Tp& __value, _Comp __comp = {},
                                                                  _Proj __proj = {}) const {
        return __impl_n(ranges::begin(__r), ranges::distance(__r), __value, __comp, __proj);
    }
};

inline constexpr __fcxx_lower_bound_fn lower_bound{};

struct __fcxx_upper_bound_fn {
    template <class _It, class _Tp, class _Comp, class _Proj>
    static constexpr _It __impl_n(_It __first, iter_difference_t<_It> __len, const _Tp& __value, _Comp& __comp,
                                  _Proj& __proj) {
        while (__len > 0) {
            const iter_difference_t<_It> __half = __len / 2;
            _It __mid = ranges::next(__first, __half);
            if (!bool(std::invoke(__comp, __value, std::invoke(__proj, *__mid)))) {
                __first = ++__mid;
                __len -= __half + 1;
            } else {
                __len = __half;
            }
        }
        return __first;
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>,
              indirect_strict_weak_order<const _Tp*, projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, const _Tp& __value, _Comp __comp = {},
                                             _Proj __proj = {}) const {
        const iter_difference_t<_It> __len = ranges::distance(__first, __last);
        return __impl_n(std::move(__first), __len, __value, __comp, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>,
              indirect_strict_weak_order<const _Tp*, projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, const _Tp& __value, _Comp __comp = {},
                                                                  _Proj __proj = {}) const {
        return __impl_n(ranges::begin(__r), ranges::distance(__r), __value, __comp, __proj);
    }
};

inline constexpr __fcxx_upper_bound_fn upper_bound{};

/* An element equivalent to the value is found first, and the bounds are
 * then found on either side of it: at most 2 log N comparisons. */
struct __fcxx_equal_range_fn {
    template <class _It, class _Tp, class _Comp, class _Proj>
    static constexpr subrange<_It> __impl_n(_It __first, iter_difference_t<_It> __len, const _Tp& __value,
                                            _Comp& __comp, _Proj& __proj) {
        while (__len > 0) {
            const iter_difference_t<_It> __half = __len / 2;
            _It __mid = ranges::next(__first, __half);
            if (bool(std::invoke(__comp, std::invoke(__proj, *__mid), __value))) {
                __first = ++__mid;
                __len -= __half + 1;
            } else if (bool(std::invoke(__comp, __value, std::invoke(__proj, *__mid)))) {
                __len = __half;
            } else {
                _It __right = ranges::next(__mid);
                return {__fcxx_lower_bound_fn::__impl_n(std::move(__first), __half, __value, __comp, __proj),
                        __fcxx_upper_bound_fn::__impl_n(std::move(__right), __len - __half - 1, __value, __comp,
                                                        __proj)};
            }
        }
        return {__first, __first};
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>,
              indirect_strict_weak_order<const _Tp*, projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr subrange<_It> operator()(_It __first, _Sp __last, const _Tp& __value, _Comp __comp = {},
                                                       _Proj __proj = {}) const {
        const iter_difference_t<_It> __len = ranges::distance(__first, __last);
        return __impl_n(std::move(__first), __len, __value, __comp, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>,
              indirect_strict_weak_order<const _Tp*, projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr borrowed_subrange_t<_Rp>
    operator()(_Rp&& __r, const _Tp& __value, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl_n(ranges::begin(__r), ranges::distance(__r), __value, __comp, __proj);
    }
};

inline constexpr __fcxx_equal_range_fn equal_range{};

struct __fcxx_binary_search_fn {
    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              class _Tp = projected_value_t<_It, _Proj>,
              indirect_strict_weak_order<const _Tp*, projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_It __first, _Sp __last, const _Tp& __value, _Comp __comp = {},
                                              _Proj __proj = {}) const {
        const iter_difference_t<_It> __len = ranges::distance(__first, __last);
        __first = __fcxx_lower_bound_fn::__impl_n(std::move(__first), __len, __value, __comp, __proj);
        return __first != __last && !bool(std::invoke(__comp, __value, std::invoke(__proj, *__first)));
    }

    template <forward_range _Rp, class _Proj = identity,
              class _Tp = projected_value_t<iterator_t<_Rp>, _Proj>,
              indirect_strict_weak_order<const _Tp*, projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_Rp&& __r, const _Tp& __value, _Comp __comp = {},
                                              _Proj __proj = {}) const {
        iterator_t<_Rp> __first = __fcxx_lower_bound_fn::__impl_n(ranges::begin(__r), ranges::distance(__r),
                                                                  __value, __comp, __proj);
        return __first != ranges::end(__r) && !bool(std::invoke(__comp, __value, std::invoke(__proj, *__first)));
    }
};

inline constexpr __fcxx_binary_search_fn binary_search{};

/* [alg.partitions], partitions */
struct __fcxx_is_partitioned_fn {
    template <class _It, class _Sp, class _Pred, class _Proj>
    static constexpr bool __impl(_It __first, _Sp __last, _Pred& __pred, _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (!bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                break;
            }
        }
        if (__first == __last) {
            return true;
        }
        for (++__first; __first != __last; ++__first) {
            if (bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
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

inline constexpr __fcxx_is_partitioned_fn is_partitioned{};

struct __fcxx_partition_fn {
    template <class _It, class _Sp, class _Pred, class _Proj>
    static constexpr subrange<_It> __impl(_It __first, _Sp __last, _Pred& __pred, _Proj& __proj) {
        _It __end = ranges::next(__first, std::move(__last));
        __fcxx_proj_pred<_Pred, _Proj> __p{__pred, __proj};
        _It __mid = std::__fcxx_partition<__fcxx_ranges_ops>(std::move(__first), __end, __p, __fcxx_iter_tag_t<_It>());
        return {std::move(__mid), std::move(__end)};
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

inline constexpr __fcxx_partition_fn partition{};

template <class _It, class _O1, class _O2>
using partition_copy_result = in_out_out_result<_It, _O1, _O2>;

struct __fcxx_partition_copy_fn {
    template <class _It, class _Sp, class _O1, class _O2, class _Pred, class _Proj>
    static constexpr partition_copy_result<_It, _O1, _O2> __impl(_It __first, _Sp __last, _O1 __out_true,
                                                                 _O2 __out_false, _Pred& __pred, _Proj& __proj) {
        for (; __first != __last; ++__first) {
            if (bool(std::invoke(__pred, std::invoke(__proj, *__first)))) {
                *__out_true = *__first;
                ++__out_true;
            } else {
                *__out_false = *__first;
                ++__out_false;
            }
        }
        return {std::move(__first), std::move(__out_true), std::move(__out_false)};
    }

    template <input_iterator _It, sentinel_for<_It> _Sp, weakly_incrementable _O1, weakly_incrementable _O2,
              class _Proj = identity, indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_copyable<_It, _O1> && indirectly_copyable<_It, _O2>
    constexpr partition_copy_result<_It, _O1, _O2> operator()(_It __first, _Sp __last, _O1 __out_true,
                                                              _O2 __out_false, _Pred __pred,
                                                              _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), std::move(__out_true), std::move(__out_false), __pred,
                      __proj);
    }

    template <input_range _Rp, weakly_incrementable _O1, weakly_incrementable _O2, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_copyable<iterator_t<_Rp>, _O1> && indirectly_copyable<iterator_t<_Rp>, _O2>
    constexpr partition_copy_result<borrowed_iterator_t<_Rp>, _O1, _O2> operator()(_Rp&& __r, _O1 __out_true,
                                                                                   _O2 __out_false, _Pred __pred,
                                                                                   _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), std::move(__out_true), std::move(__out_false), __pred,
                      __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              random_access_iterator _O1, sized_sentinel_for<_O1> _OutS1, random_access_iterator _O2,
              sized_sentinel_for<_O2> _OutS2, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
        requires indirectly_copyable<_It, _O1> && indirectly_copyable<_It, _O2>
    partition_copy_result<_It, _O1, _O2> operator()(_Ep&&, _It, _Sp, _O1, _OutS1, _O2, _OutS2, _Pred,
                                                    _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp,
              __fcxx_sized_random_access_range _OutR1, __fcxx_sized_random_access_range _OutR2,
              class _Proj = identity, indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
        requires indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR1> > &&
                     indirectly_copyable<iterator_t<_Rp>, iterator_t<_OutR2> >
    partition_copy_result<borrowed_iterator_t<_Rp>, borrowed_iterator_t<_OutR1>, borrowed_iterator_t<_OutR2> >
    operator()(_Ep&&, _Rp&&, _OutR1&&, _OutR2&&, _Pred, _Proj = {}) const = delete;
};

inline constexpr __fcxx_partition_copy_fn partition_copy{};

struct __fcxx_partition_point_fn {
    template <class _It, class _Pred, class _Proj>
    static constexpr _It __impl_n(_It __first, iter_difference_t<_It> __len, _Pred& __pred, _Proj& __proj) {
        while (__len > 0) {
            const iter_difference_t<_It> __half = __len / 2;
            _It __mid = ranges::next(__first, __half);
            if (bool(std::invoke(__pred, std::invoke(__proj, *__mid)))) {
                __first = ++__mid;
                __len -= __half + 1;
            } else {
                __len = __half;
            }
        }
        return __first;
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_unary_predicate<projected<_It, _Proj> > _Pred>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, _Pred __pred, _Proj __proj = {}) const {
        const iter_difference_t<_It> __len = ranges::distance(__first, __last);
        return __impl_n(std::move(__first), __len, __pred, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_unary_predicate<projected<iterator_t<_Rp>, _Proj> > _Pred>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Pred __pred, _Proj __proj = {}) const {
        return __impl_n(ranges::begin(__r), ranges::distance(__r), __pred, __proj);
    }
};

inline constexpr __fcxx_partition_point_fn partition_point{};

/* [alg.merge], merge, which takes the element of the first range where the
 * elements are equivalent. */
template <class _I1, class _I2, class _Out>
using merge_result = in_in_out_result<_I1, _I2, _Out>;

struct __fcxx_merge_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp, class _Proj1, class _Proj2>
    static constexpr merge_result<_I1, _I2, _Out> __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                                         _Out __result, _Comp& __comp, _Proj1& __proj1,
                                                         _Proj2& __proj2) {
        while (__first1 != __last1 && __first2 != __last2) {
            if (bool(std::invoke(__comp, std::invoke(__proj2, *__first2), std::invoke(__proj1, *__first1)))) {
                *__result = *__first2;
                ++__first2;
            } else {
                *__result = *__first1;
                ++__first1;
            }
            ++__result;
        }
        copy_result<_I1, _Out> __rest1 =
            __fcxx_copy_fn::__impl(std::move(__first1), std::move(__last1), std::move(__result));
        copy_result<_I2, _Out> __rest2 =
            __fcxx_copy_fn::__impl(std::move(__first2), std::move(__last2), std::move(__rest1.out));
        return {std::move(__rest1.in), std::move(__rest2.in), std::move(__rest2.out)};
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              weakly_incrementable _Out, class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    constexpr merge_result<_I1, _I2, _Out> operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                                      _Out __result, _Comp __comp = {}, _Proj1 __proj1 = {},
                                                      _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, weakly_incrementable _Out, class _Comp = ranges::less,
              class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, _Out, _Comp, _Proj1, _Proj2>
    constexpr merge_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, _Out>
    operator()(_R1&& __r1, _R2&& __r2, _Out __result, _Comp __comp = {}, _Proj1 __proj1 = {},
               _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, random_access_iterator _Out,
              sized_sentinel_for<_Out> _OutS, class _Comp = ranges::less, class _Proj1 = identity,
              class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    merge_result<_I1, _I2, _Out> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Out, _OutS, _Comp = {}, _Proj1 = {},
                                            _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, __fcxx_sized_random_access_range _OutR,
              class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, iterator_t<_OutR>, _Comp, _Proj1, _Proj2>
    merge_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _R1&&, _R2&&, _OutR&&, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_merge_fn merge{};

/* [alg.set.operations], set operations on sorted structures */
struct __fcxx_includes_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Comp, class _Proj1, class _Proj2>
    static constexpr bool __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2, _Comp& __comp,
                                 _Proj1& __proj1, _Proj2& __proj2) {
        for (; __first2 != __last2; ++__first1) {
            if (__first1 == __last1
                || bool(std::invoke(__comp, std::invoke(__proj2, *__first2), std::invoke(__proj1, *__first1)))) {
                return false;
            }
            if (!bool(std::invoke(__comp, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2)))) {
                ++__first2;
            }
        }
        return true;
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              class _Proj1 = identity, class _Proj2 = identity,
              indirect_strict_weak_order<projected<_I1, _Proj1>, projected<_I2, _Proj2> > _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                              _Comp __comp = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __comp,
                      __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, class _Proj1 = identity, class _Proj2 = identity,
              indirect_strict_weak_order<projected<iterator_t<_R1>, _Proj1>, projected<iterator_t<_R2>, _Proj2> >
                  _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_R1&& __r1, _R2&& __r2, _Comp __comp = {}, _Proj1 __proj1 = {},
                                              _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2), __comp,
                      __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Proj1 = identity,
              class _Proj2 = identity,
              indirect_strict_weak_order<projected<_I1, _Proj1>, projected<_I2, _Proj2> > _Comp = ranges::less>
    bool operator()(_Ep&&, _I1, _S1, _I2, _S2, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Proj1 = identity, class _Proj2 = identity,
              indirect_strict_weak_order<projected<iterator_t<_R1>, _Proj1>, projected<iterator_t<_R2>, _Proj2> >
                  _Comp = ranges::less>
    bool operator()(_Ep&&, _R1&&, _R2&&, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_includes_fn includes{};

template <class _I1, class _I2, class _Out>
using set_union_result = in_in_out_result<_I1, _I2, _Out>;

struct __fcxx_set_union_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp, class _Proj1, class _Proj2>
    static constexpr set_union_result<_I1, _I2, _Out> __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                                             _Out __result, _Comp& __comp, _Proj1& __proj1,
                                                             _Proj2& __proj2) {
        while (__first1 != __last1 && __first2 != __last2) {
            if (bool(std::invoke(__comp, std::invoke(__proj2, *__first2), std::invoke(__proj1, *__first1)))) {
                *__result = *__first2;
                ++__first2;
            } else {
                if (!bool(std::invoke(__comp, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2)))) {
                    ++__first2;
                }
                *__result = *__first1;
                ++__first1;
            }
            ++__result;
        }
        copy_result<_I1, _Out> __rest1 =
            __fcxx_copy_fn::__impl(std::move(__first1), std::move(__last1), std::move(__result));
        copy_result<_I2, _Out> __rest2 =
            __fcxx_copy_fn::__impl(std::move(__first2), std::move(__last2), std::move(__rest1.out));
        return {std::move(__rest1.in), std::move(__rest2.in), std::move(__rest2.out)};
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              weakly_incrementable _Out, class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    constexpr set_union_result<_I1, _I2, _Out> operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                                          _Out __result, _Comp __comp = {}, _Proj1 __proj1 = {},
                                                          _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, weakly_incrementable _Out, class _Comp = ranges::less,
              class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, _Out, _Comp, _Proj1, _Proj2>
    constexpr set_union_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, _Out>
    operator()(_R1&& __r1, _R2&& __r2, _Out __result, _Comp __comp = {}, _Proj1 __proj1 = {},
               _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, random_access_iterator _Out,
              sized_sentinel_for<_Out> _OutS, class _Comp = ranges::less, class _Proj1 = identity,
              class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    set_union_result<_I1, _I2, _Out> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Out, _OutS, _Comp = {}, _Proj1 = {},
                                                _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, __fcxx_sized_random_access_range _OutR,
              class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, iterator_t<_OutR>, _Comp, _Proj1, _Proj2>
    set_union_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _R1&&, _R2&&, _OutR&&, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_set_union_fn set_union{};

template <class _I1, class _I2, class _Out>
using set_intersection_result = in_in_out_result<_I1, _I2, _Out>;

struct __fcxx_set_intersection_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp, class _Proj1, class _Proj2>
    static constexpr set_intersection_result<_I1, _I2, _Out> __impl(_I1 __first1, _S1 __last1, _I2 __first2,
                                                                    _S2 __last2, _Out __result, _Comp& __comp,
                                                                    _Proj1& __proj1, _Proj2& __proj2) {
        while (__first1 != __last1 && __first2 != __last2) {
            if (bool(std::invoke(__comp, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2)))) {
                ++__first1;
            } else {
                if (!bool(std::invoke(__comp, std::invoke(__proj2, *__first2), std::invoke(__proj1, *__first1)))) {
                    *__result = *__first1;
                    ++__result;
                    ++__first1;
                }
                ++__first2;
            }
        }
        ranges::advance(__first1, std::move(__last1));
        ranges::advance(__first2, std::move(__last2));
        return {std::move(__first1), std::move(__first2), std::move(__result)};
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              weakly_incrementable _Out, class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    constexpr set_intersection_result<_I1, _I2, _Out> operator()(_I1 __first1, _S1 __last1, _I2 __first2,
                                                                 _S2 __last2, _Out __result, _Comp __comp = {},
                                                                 _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, weakly_incrementable _Out, class _Comp = ranges::less,
              class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, _Out, _Comp, _Proj1, _Proj2>
    constexpr set_intersection_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, _Out>
    operator()(_R1&& __r1, _R2&& __r2, _Out __result, _Comp __comp = {}, _Proj1 __proj1 = {},
               _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, random_access_iterator _Out,
              sized_sentinel_for<_Out> _OutS, class _Comp = ranges::less, class _Proj1 = identity,
              class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    set_intersection_result<_I1, _I2, _Out> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Out, _OutS, _Comp = {},
                                                       _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, __fcxx_sized_random_access_range _OutR,
              class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, iterator_t<_OutR>, _Comp, _Proj1, _Proj2>
    set_intersection_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _R1&&, _R2&&, _OutR&&, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_set_intersection_fn set_intersection{};

template <class _It, class _Out>
using set_difference_result = in_out_result<_It, _Out>;

template <class _I1, class _I2, class _Out>
using set_difference_truncated_result = in_in_out_result<_I1, _I2, _Out>;

struct __fcxx_set_difference_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp, class _Proj1, class _Proj2>
    static constexpr set_difference_result<_I1, _Out> __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                                             _Out __result, _Comp& __comp, _Proj1& __proj1,
                                                             _Proj2& __proj2) {
        while (__first1 != __last1 && __first2 != __last2) {
            if (bool(std::invoke(__comp, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2)))) {
                *__result = *__first1;
                ++__result;
                ++__first1;
            } else {
                if (!bool(std::invoke(__comp, std::invoke(__proj2, *__first2), std::invoke(__proj1, *__first1)))) {
                    ++__first1;
                }
                ++__first2;
            }
        }
        return __fcxx_copy_fn::__impl(std::move(__first1), std::move(__last1), std::move(__result));
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              weakly_incrementable _Out, class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    constexpr set_difference_result<_I1, _Out> operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                                          _Out __result, _Comp __comp = {}, _Proj1 __proj1 = {},
                                                          _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, weakly_incrementable _Out, class _Comp = ranges::less,
              class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, _Out, _Comp, _Proj1, _Proj2>
    constexpr set_difference_result<borrowed_iterator_t<_R1>, _Out> operator()(_R1&& __r1, _R2&& __r2,
                                                                               _Out __result, _Comp __comp = {},
                                                                               _Proj1 __proj1 = {},
                                                                               _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, random_access_iterator _Out,
              sized_sentinel_for<_Out> _OutS, class _Comp = ranges::less, class _Proj1 = identity,
              class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    set_difference_truncated_result<_I1, _I2, _Out> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Out, _OutS, _Comp = {},
                                                               _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, __fcxx_sized_random_access_range _OutR,
              class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, iterator_t<_OutR>, _Comp, _Proj1, _Proj2>
    set_difference_truncated_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _R1&&, _R2&&, _OutR&&, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_set_difference_fn set_difference{};

template <class _I1, class _I2, class _Out>
using set_symmetric_difference_result = in_in_out_result<_I1, _I2, _Out>;

struct __fcxx_set_symmetric_difference_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp, class _Proj1, class _Proj2>
    static constexpr set_symmetric_difference_result<_I1, _I2, _Out>
    __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2, _Out __result, _Comp& __comp, _Proj1& __proj1,
           _Proj2& __proj2) {
        while (__first1 != __last1 && __first2 != __last2) {
            if (bool(std::invoke(__comp, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2)))) {
                *__result = *__first1;
                ++__result;
                ++__first1;
            } else {
                if (bool(std::invoke(__comp, std::invoke(__proj2, *__first2), std::invoke(__proj1, *__first1)))) {
                    *__result = *__first2;
                    ++__result;
                } else {
                    ++__first1;
                }
                ++__first2;
            }
        }
        copy_result<_I1, _Out> __rest1 =
            __fcxx_copy_fn::__impl(std::move(__first1), std::move(__last1), std::move(__result));
        copy_result<_I2, _Out> __rest2 =
            __fcxx_copy_fn::__impl(std::move(__first2), std::move(__last2), std::move(__rest1.out));
        return {std::move(__rest1.in), std::move(__rest2.in), std::move(__rest2.out)};
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              weakly_incrementable _Out, class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    constexpr set_symmetric_difference_result<_I1, _I2, _Out> operator()(_I1 __first1, _S1 __last1, _I2 __first2,
                                                                         _S2 __last2, _Out __result,
                                                                         _Comp __comp = {}, _Proj1 __proj1 = {},
                                                                         _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, weakly_incrementable _Out, class _Comp = ranges::less,
              class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, _Out, _Comp, _Proj1, _Proj2>
    constexpr set_symmetric_difference_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, _Out>
    operator()(_R1&& __r1, _R2&& __r2, _Out __result, _Comp __comp = {}, _Proj1 __proj1 = {},
               _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2),
                      std::move(__result), __comp, __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, random_access_iterator _Out,
              sized_sentinel_for<_Out> _OutS, class _Comp = ranges::less, class _Proj1 = identity,
              class _Proj2 = identity>
        requires mergeable<_I1, _I2, _Out, _Comp, _Proj1, _Proj2>
    set_symmetric_difference_result<_I1, _I2, _Out> operator()(_Ep&&, _I1, _S1, _I2, _S2, _Out, _OutS, _Comp = {},
                                                               _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, __fcxx_sized_random_access_range _OutR,
              class _Comp = ranges::less, class _Proj1 = identity, class _Proj2 = identity>
        requires mergeable<iterator_t<_R1>, iterator_t<_R2>, iterator_t<_OutR>, _Comp, _Proj1, _Proj2>
    set_symmetric_difference_result<borrowed_iterator_t<_R1>, borrowed_iterator_t<_R2>, borrowed_iterator_t<_OutR> >
    operator()(_Ep&&, _R1&&, _R2&&, _OutR&&, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_set_symmetric_difference_fn set_symmetric_difference{};

/* [alg.heap.operations], heap operations */
struct __fcxx_push_heap_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        _It __end = ranges::next(__first, std::move(__last));
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        std::__fcxx_push_heap<__fcxx_ranges_ops>(__first, static_cast<iter_difference_t<_It> >(__end - __first), __c);
        return __end;
    }

    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    constexpr _It operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <random_access_range _Rp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }
};

inline constexpr __fcxx_push_heap_fn push_heap{};

struct __fcxx_pop_heap_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        _It __end = ranges::next(__first, std::move(__last));
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        std::__fcxx_pop_heap<__fcxx_ranges_ops>(__first, static_cast<iter_difference_t<_It> >(__end - __first), __c);
        return __end;
    }

    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    constexpr _It operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <random_access_range _Rp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }
};

inline constexpr __fcxx_pop_heap_fn pop_heap{};

struct __fcxx_make_heap_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        _It __end = ranges::next(__first, std::move(__last));
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        std::__fcxx_make_heap<__fcxx_ranges_ops>(__first, static_cast<iter_difference_t<_It> >(__end - __first), __c);
        return __end;
    }

    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    constexpr _It operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <random_access_range _Rp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }
};

inline constexpr __fcxx_make_heap_fn make_heap{};

struct __fcxx_sort_heap_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        _It __end = ranges::next(__first, std::move(__last));
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        std::__fcxx_sort_heap<__fcxx_ranges_ops>(__first, static_cast<iter_difference_t<_It> >(__end - __first), __c);
        return __end;
    }

    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    constexpr _It operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <random_access_range _Rp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }
};

inline constexpr __fcxx_sort_heap_fn sort_heap{};

struct __fcxx_is_heap_until_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        const iter_difference_t<_It> __len = ranges::distance(__first, __last);
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        return std::__fcxx_is_heap_until(std::move(__first), __len, __c);
    }

    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {},
                                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    _It operator()(_Ep&&, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_is_heap_until_fn is_heap_until{};

struct __fcxx_is_heap_fn {
    template <random_access_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __fcxx_is_heap_until_fn::__impl(std::move(__first), __last, __comp, __proj) == __last;
    }

    template <random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return __fcxx_is_heap_until_fn::__impl(ranges::begin(__r), ranges::end(__r), __comp, __proj)
               == ranges::end(__r);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    bool operator()(_Ep&&, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    bool operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_is_heap_fn is_heap{};

/* [alg.min.max], minimum and maximum */
struct __fcxx_min_element_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        if (__first == __last) {
            return __first;
        }
        _It __m = __first;
        while (++__first != __last) {
            if (bool(std::invoke(__comp, std::invoke(__proj, *__first), std::invoke(__proj, *__m)))) {
                __m = __first;
            }
        }
        return __m;
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {},
                                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    _It operator()(_Ep&&, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_min_element_fn min_element{};

struct __fcxx_max_element_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr _It __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        if (__first == __last) {
            return __first;
        }
        _It __m = __first;
        while (++__first != __last) {
            if (bool(std::invoke(__comp, std::invoke(__proj, *__m), std::invoke(__proj, *__first)))) {
                __m = __first;
            }
        }
        return __m;
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr _It operator()(_It __first, _Sp __last, _Comp __comp = {}, _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {},
                                                                  _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    _It operator()(_Ep&&, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_max_element_fn max_element{};

template <class _It>
using minmax_element_result = min_max_result<_It>;

/* The first of the least elements and the last of the greatest, found by
 * taking the elements by pairs: three comparisons for each two elements,
 * but the first two, which take one. */
struct __fcxx_minmax_element_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr minmax_element_result<_It> __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        _It __min = __first;
        _It __max = __first;
        if (__first == __last || ++__first == __last) {
            return {std::move(__min), std::move(__max)};
        }
        if (__c(*__first, *__min)) {
            __min = __first;
        } else {
            __max = __first;
        }
        while (++__first != __last) {
            _It __i = __first;
            if (++__first == __last) {
                if (__c(*__i, *__min)) {
                    __min = __i;
                } else if (!__c(*__i, *__max)) {
                    __max = __i;
                }
                break;
            }
            if (__c(*__first, *__i)) {
                if (__c(*__first, *__min)) {
                    __min = __first;
                }
                if (!__c(*__i, *__max)) {
                    __max = __i;
                }
            } else {
                if (__c(*__i, *__min)) {
                    __min = __i;
                }
                if (!__c(*__first, *__max)) {
                    __max = __first;
                }
            }
        }
        return {std::move(__min), std::move(__max)};
    }

    template <forward_iterator _It, sentinel_for<_It> _Sp, class _Proj = identity,
              indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr minmax_element_result<_It> operator()(_It __first, _Sp __last, _Comp __comp = {},
                                                                    _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <forward_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr minmax_element_result<borrowed_iterator_t<_Rp> > operator()(_Rp&& __r,
                                                                                          _Comp __comp = {},
                                                                                          _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _Sp,
              class _Proj = identity, indirect_strict_weak_order<projected<_It, _Proj> > _Comp = ranges::less>
    minmax_element_result<_It> operator()(_Ep&&, _It, _Sp, _Comp = {}, _Proj = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
    minmax_element_result<borrowed_iterator_t<_Rp> > operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_minmax_element_fn minmax_element{};

/* The least and the greatest of values, and of the elements of ranges,
 * which are the leftmost of those equivalent to the least, and to the
 * greatest, and, for minmax, the rightmost of those equivalent to the
 * greatest. The elements of input ranges are copied as they are read. */
struct __fcxx_min_fn {
    template <class _Tp, class _Proj = identity,
              indirect_strict_weak_order<projected<const _Tp*, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr const _Tp& operator()(const _Tp& __a, const _Tp& __b, _Comp __comp = {},
                                                    _Proj __proj = {}) const {
        return bool(std::invoke(__comp, std::invoke(__proj, __b), std::invoke(__proj, __a))) ? __b : __a;
    }

    template <copyable _Tp, class _Proj = identity,
              indirect_strict_weak_order<projected<const _Tp*, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr _Tp operator()(initializer_list<_Tp> __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return *__fcxx_min_element_fn::__impl(__r.begin(), __r.end(), __comp, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
        requires indirectly_copyable_storable<iterator_t<_Rp>, range_value_t<_Rp>*>
    [[nodiscard]] constexpr range_value_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        if constexpr (forward_range<_Rp>) {
            return static_cast<range_value_t<_Rp> >(
                *__fcxx_min_element_fn::__impl(ranges::begin(__r), ranges::end(__r), __comp, __proj));
        } else {
            iterator_t<_Rp> __first = ranges::begin(__r);
            sentinel_t<_Rp> __last = ranges::end(__r);
            range_value_t<_Rp> __result(*__first);
            while (++__first != __last) {
                if (bool(std::invoke(__comp, std::invoke(__proj, *__first), std::invoke(__proj, __result)))) {
                    __result = *__first;
                }
            }
            return __result;
        }
    }

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
        requires indirectly_copyable_storable<iterator_t<_Rp>, range_value_t<_Rp>*>
    range_value_t<_Rp> operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_min_fn min{};

struct __fcxx_max_fn {
    template <class _Tp, class _Proj = identity,
              indirect_strict_weak_order<projected<const _Tp*, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr const _Tp& operator()(const _Tp& __a, const _Tp& __b, _Comp __comp = {},
                                                    _Proj __proj = {}) const {
        return bool(std::invoke(__comp, std::invoke(__proj, __a), std::invoke(__proj, __b))) ? __b : __a;
    }

    template <copyable _Tp, class _Proj = identity,
              indirect_strict_weak_order<projected<const _Tp*, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr _Tp operator()(initializer_list<_Tp> __r, _Comp __comp = {}, _Proj __proj = {}) const {
        return *__fcxx_max_element_fn::__impl(__r.begin(), __r.end(), __comp, __proj);
    }

    template <input_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
        requires indirectly_copyable_storable<iterator_t<_Rp>, range_value_t<_Rp>*>
    [[nodiscard]] constexpr range_value_t<_Rp> operator()(_Rp&& __r, _Comp __comp = {}, _Proj __proj = {}) const {
        if constexpr (forward_range<_Rp>) {
            return static_cast<range_value_t<_Rp> >(
                *__fcxx_max_element_fn::__impl(ranges::begin(__r), ranges::end(__r), __comp, __proj));
        } else {
            iterator_t<_Rp> __first = ranges::begin(__r);
            sentinel_t<_Rp> __last = ranges::end(__r);
            range_value_t<_Rp> __result(*__first);
            while (++__first != __last) {
                if (bool(std::invoke(__comp, std::invoke(__proj, __result), std::invoke(__proj, *__first)))) {
                    __result = *__first;
                }
            }
            return __result;
        }
    }

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
        requires indirectly_copyable_storable<iterator_t<_Rp>, range_value_t<_Rp>*>
    range_value_t<_Rp> operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_max_fn max{};

template <class _Tp>
using minmax_result = min_max_result<_Tp>;

struct __fcxx_minmax_fn {
    template <class _Tp, class _Proj = identity,
              indirect_strict_weak_order<projected<const _Tp*, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr minmax_result<const _Tp&> operator()(const _Tp& __a, const _Tp& __b, _Comp __comp = {},
                                                                   _Proj __proj = {}) const {
        if (bool(std::invoke(__comp, std::invoke(__proj, __b), std::invoke(__proj, __a)))) {
            return {__b, __a};
        }
        return {__a, __b};
    }

    template <copyable _Tp, class _Proj = identity,
              indirect_strict_weak_order<projected<const _Tp*, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr minmax_result<_Tp> operator()(initializer_list<_Tp> __r, _Comp __comp = {},
                                                            _Proj __proj = {}) const {
        minmax_element_result<const _Tp*> __res =
            __fcxx_minmax_element_fn::__impl(__r.begin(), __r.end(), __comp, __proj);
        return {*__res.min, *__res.max};
    }

    template <input_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
        requires indirectly_copyable_storable<iterator_t<_Rp>, range_value_t<_Rp>*>
    [[nodiscard]] constexpr minmax_result<range_value_t<_Rp> > operator()(_Rp&& __r, _Comp __comp = {},
                                                                            _Proj __proj = {}) const {
        using _Vp = range_value_t<_Rp>;
        if constexpr (forward_range<_Rp>) {
            minmax_element_result<iterator_t<_Rp> > __res =
                __fcxx_minmax_element_fn::__impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
            return {static_cast<_Vp>(*__res.min), static_cast<_Vp>(*__res.max)};
        } else {
            __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
            iterator_t<_Rp> __first = ranges::begin(__r);
            sentinel_t<_Rp> __last = ranges::end(__r);
            _Vp __value(*__first);
            minmax_result<_Vp> __res{__value, std::move(__value)};
            if (++__first == __last) {
                return __res;
            }
            {
                _Vp __second(*__first);
                if (__c(__second, __res.min)) {
                    __res.min = std::move(__second);
                } else {
                    __res.max = std::move(__second);
                }
            }
            while (++__first != __last) {
                _Vp __i(*__first);
                if (++__first == __last) {
                    if (__c(__i, __res.min)) {
                        __res.min = std::move(__i);
                    } else if (!__c(__i, __res.max)) {
                        __res.max = std::move(__i);
                    }
                    break;
                }
                _Vp __j(*__first);
                if (__c(__j, __i)) {
                    if (__c(__j, __res.min)) {
                        __res.min = std::move(__j);
                    }
                    if (!__c(__i, __res.max)) {
                        __res.max = std::move(__i);
                    }
                } else {
                    if (__c(__i, __res.min)) {
                        __res.min = std::move(__i);
                    }
                    if (!__c(__j, __res.max)) {
                        __res.max = std::move(__j);
                    }
                }
            }
            return __res;
        }
    }

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _Rp, class _Proj = identity,
              indirect_strict_weak_order<projected<iterator_t<_Rp>, _Proj> > _Comp = ranges::less>
        requires indirectly_copyable_storable<iterator_t<_Rp>, range_value_t<_Rp>*>
    minmax_result<range_value_t<_Rp> > operator()(_Ep&&, _Rp&&, _Comp = {}, _Proj = {}) const = delete;
};

inline constexpr __fcxx_minmax_fn minmax{};

/* [alg.clamp], bounded value */
struct __fcxx_clamp_fn {
    template <class _Tp, class _Proj = identity,
              indirect_strict_weak_order<projected<const _Tp*, _Proj> > _Comp = ranges::less>
    [[nodiscard]] constexpr const _Tp& operator()(const _Tp& __v, const _Tp& __lo, const _Tp& __hi,
                                                    _Comp __comp = {}, _Proj __proj = {}) const {
        auto&& __pv = std::invoke(__proj, __v);
        if (bool(std::invoke(__comp, std::forward<decltype(__pv)>(__pv), std::invoke(__proj, __lo)))) {
            return __lo;
        }
        if (bool(std::invoke(__comp, std::invoke(__proj, __hi), std::forward<decltype(__pv)>(__pv)))) {
            return __hi;
        }
        return __v;
    }
};

inline constexpr __fcxx_clamp_fn clamp{};

/* [alg.lex.comparison], lexicographical comparison */
struct __fcxx_lexicographical_compare_fn {
    template <class _I1, class _S1, class _I2, class _S2, class _Comp, class _Proj1, class _Proj2>
    static constexpr bool __impl(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2, _Comp& __comp,
                                 _Proj1& __proj1, _Proj2& __proj2) {
        for (; __first1 != __last1 && __first2 != __last2; ++__first1, (void)++__first2) {
            if (bool(std::invoke(__comp, std::invoke(__proj1, *__first1), std::invoke(__proj2, *__first2)))) {
                return true;
            }
            if (bool(std::invoke(__comp, std::invoke(__proj2, *__first2), std::invoke(__proj1, *__first1)))) {
                return false;
            }
        }
        return __first1 == __last1 && __first2 != __last2;
    }

    template <input_iterator _I1, sentinel_for<_I1> _S1, input_iterator _I2, sentinel_for<_I2> _S2,
              class _Proj1 = identity, class _Proj2 = identity,
              indirect_strict_weak_order<projected<_I1, _Proj1>, projected<_I2, _Proj2> > _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_I1 __first1, _S1 __last1, _I2 __first2, _S2 __last2,
                                              _Comp __comp = {}, _Proj1 __proj1 = {}, _Proj2 __proj2 = {}) const {
        return __impl(std::move(__first1), std::move(__last1), std::move(__first2), std::move(__last2), __comp,
                      __proj1, __proj2);
    }

    template <input_range _R1, input_range _R2, class _Proj1 = identity, class _Proj2 = identity,
              indirect_strict_weak_order<projected<iterator_t<_R1>, _Proj1>, projected<iterator_t<_R2>, _Proj2> >
                  _Comp = ranges::less>
    [[nodiscard]] constexpr bool operator()(_R1&& __r1, _R2&& __r2, _Comp __comp = {}, _Proj1 __proj1 = {},
                                              _Proj2 __proj2 = {}) const {
        return __impl(ranges::begin(__r1), ranges::end(__r1), ranges::begin(__r2), ranges::end(__r2), __comp,
                      __proj1, __proj2);
    }

    template <__fcxx_execution_policy _Ep, random_access_iterator _I1, sized_sentinel_for<_I1> _S1,
              random_access_iterator _I2, sized_sentinel_for<_I2> _S2, class _Proj1 = identity,
              class _Proj2 = identity,
              indirect_strict_weak_order<projected<_I1, _Proj1>, projected<_I2, _Proj2> > _Comp = ranges::less>
    bool operator()(_Ep&&, _I1, _S1, _I2, _S2, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _R1,
              __fcxx_sized_random_access_range _R2, class _Proj1 = identity, class _Proj2 = identity,
              indirect_strict_weak_order<projected<iterator_t<_R1>, _Proj1>, projected<iterator_t<_R2>, _Proj2> >
                  _Comp = ranges::less>
    bool operator()(_Ep&&, _R1&&, _R2&&, _Comp = {}, _Proj1 = {}, _Proj2 = {}) const = delete;
};

inline constexpr __fcxx_lexicographical_compare_fn lexicographical_compare{};

/* [alg.permutation.generators], permutation generators, the previous
 * permutation being the next one in the reverse order. */
template <class _It>
using next_permutation_result = in_found_result<_It>;

template <class _It>
using prev_permutation_result = in_found_result<_It>;

struct __fcxx_next_permutation_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr next_permutation_result<_It> __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        _It __end = ranges::next(__first, std::move(__last));
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        const bool __found = std::__fcxx_next_permutation<__fcxx_ranges_ops>(std::move(__first), __end, __c);
        return {std::move(__end), __found};
    }

    template <bidirectional_iterator _It, sentinel_for<_It> _Sp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    constexpr next_permutation_result<_It> operator()(_It __first, _Sp __last, _Comp __comp = {},
                                                      _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <bidirectional_range _Rp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    constexpr next_permutation_result<borrowed_iterator_t<_Rp> > operator()(_Rp&& __r, _Comp __comp = {},
                                                                            _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }
};

inline constexpr __fcxx_next_permutation_fn next_permutation{};

struct __fcxx_prev_permutation_fn {
    template <class _It, class _Sp, class _Comp, class _Proj>
    static constexpr prev_permutation_result<_It> __impl(_It __first, _Sp __last, _Comp& __comp, _Proj& __proj) {
        _It __end = ranges::next(__first, std::move(__last));
        __fcxx_proj_comp<_Comp, _Proj> __c{__comp, __proj};
        __fcxx_swapped_comp<__fcxx_proj_comp<_Comp, _Proj> > __swapped(__c);
        const bool __found = std::__fcxx_next_permutation<__fcxx_ranges_ops>(std::move(__first), __end, __swapped);
        return {std::move(__end), __found};
    }

    template <bidirectional_iterator _It, sentinel_for<_It> _Sp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<_It, _Comp, _Proj>
    constexpr prev_permutation_result<_It> operator()(_It __first, _Sp __last, _Comp __comp = {},
                                                      _Proj __proj = {}) const {
        return __impl(std::move(__first), std::move(__last), __comp, __proj);
    }

    template <bidirectional_range _Rp, class _Comp = ranges::less, class _Proj = identity>
        requires sortable<iterator_t<_Rp>, _Comp, _Proj>
    constexpr prev_permutation_result<borrowed_iterator_t<_Rp> > operator()(_Rp&& __r, _Comp __comp = {},
                                                                            _Proj __proj = {}) const {
        return __impl(ranges::begin(__r), ranges::end(__r), __comp, __proj);
    }
};

inline constexpr __fcxx_prev_permutation_fn prev_permutation{};

} /* namespace ranges */
} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
