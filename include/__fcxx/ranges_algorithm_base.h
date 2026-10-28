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


/* What the algorithms of namespace ranges, of <algorithm> and <numeric>,
 * share: the access to the elements that the implementations of
 * <__fcxx/algorithm_impl.h> take, with ranges::iter_move and
 * ranges::iter_swap, the comparisons applying projections, and the
 * exposition-only concepts. The algorithms are function objects, which
 * are not found by argument-dependent lookup, and whose overloads for
 * ranges call those for iterators and sentinels through their
 * implementations, whose constraints the constraints of the ranges imply. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_ALGORITHM_BASE_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_ALGORITHM_BASE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <__fcxx/algorithm_impl.h>
#include <__fcxx/algorithm_results.h>
#include <__fcxx/execution_policy.h>
#include <__fcxx/invoke.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
#include <__fcxx/operations.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_core.h>
#include <__fcxx/uniform_random_bit_generator.h>

/* The default template arguments of the values of the algorithms, from
 * C++26 on, as P2248 has it. */

namespace std {
namespace ranges {

/* The access to the elements that the algorithms of namespace ranges have. */
struct __fcxx_ranges_ops {
    template <class _It>
    struct __value {
        using type = iter_value_t<_It>;
    };

    template <class _It>
    struct __difference {
        using type = iter_difference_t<_It>;
    };

    template <class _It>
    static constexpr iter_rvalue_reference_t<_It> __move(_It& __i) noexcept(noexcept(ranges::iter_move(__i))) {
        return ranges::iter_move(__i);
    }

    template <class _I1, class _I2>
    static constexpr void __swap(_I1& __a, _I2& __b) noexcept(noexcept(ranges::iter_swap(__a, __b))) {
        ranges::iter_swap(__a, __b);
    }
};

/* The category of an iterator, by the concepts it models, for the
 * implementations taking iterator tags. */
template <class _It>
using __fcxx_iter_tag_t = conditional_t<
    random_access_iterator<_It>, random_access_iterator_tag,
    conditional_t<bidirectional_iterator<_It>, bidirectional_iterator_tag,
                  conditional_t<forward_iterator<_It>, forward_iterator_tag, input_iterator_tag> > >;

/* A comparison of two elements, or values, of a range through a projection,
 * which gives bool. */
template <class _Comp, class _Proj>
struct __fcxx_proj_comp {
    _Comp& __comp;
    _Proj& __proj;

    template <class _Tp, class _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const {
        return bool(std::invoke(__comp, std::invoke(__proj, std::forward<_Tp>(__t)),
                                std::invoke(__proj, std::forward<_Up>(__u))));
    }
};

/* A comparison of an element of a range, through its projection, with an
 * element of another range, through its own. */
template <class _Comp, class _Proj1, class _Proj2>
struct __fcxx_proj_comp2 {
    _Comp& __comp;
    _Proj1& __proj1;
    _Proj2& __proj2;

    template <class _Tp, class _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const {
        return bool(std::invoke(__comp, std::invoke(__proj1, std::forward<_Tp>(__t)),
                                std::invoke(__proj2, std::forward<_Up>(__u))));
    }
};

/* A predicate on an element of a range, through a projection. */
template <class _Pred, class _Proj>
struct __fcxx_proj_pred {
    _Pred& __pred;
    _Proj& __proj;

    template <class _Tp>
    constexpr bool operator()(_Tp&& __t) const {
        return bool(std::invoke(__pred, std::invoke(__proj, std::forward<_Tp>(__t))));
    }
};

} /* namespace ranges */
} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
