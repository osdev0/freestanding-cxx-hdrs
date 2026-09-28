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


/* synth-three-way, the three-way comparison of the library, which falls back
 * on operator< for the types without operator<=>. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_SYNTH_THREE_WAY_H
#define __FREESTANDING_CXX_HDRS_FCXX_SYNTH_THREE_WAY_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <compare>

namespace std {

struct __fcxx_synth_three_way_fn {
    template <class _Tp, class _Up>
        requires requires(const _Tp& __t, const _Up& __u) {
            { __t < __u } -> __boolean_testable;
            { __u < __t } -> __boolean_testable;
        }
    constexpr auto operator()(const _Tp& __t, const _Up& __u) const {
        if constexpr (three_way_comparable_with<_Tp, _Up>) {
            /* Workaround: GCC 14 to 16, all targets: <=> of floating-point
             * values evaluated twice in a function is miscompiled, and GCC 11
             * cannot evaluate it for a NaN in a constant expression;
             * __fcxx_three_way of <compare> works the result out otherwise with
             * those. */
            return std::__fcxx_three_way(__t, __u);
        } else {
            if (__t < __u) {
                return weak_ordering::less;
            }
            if (__u < __t) {
                return weak_ordering::greater;
            }
            return weak_ordering::equivalent;
        }
    }
};

inline constexpr __fcxx_synth_three_way_fn __fcxx_synth_three_way{};

template <class _Tp, class _Up = _Tp>
using __fcxx_synth_three_way_result = decltype(__fcxx_synth_three_way(std::declval<_Tp&>(), std::declval<_Up&>()));

} /* namespace std */

#endif
