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


/* The result types of the algorithms of namespace ranges, of <algorithm>,
 * which the algorithms of <numeric> and <memory> return too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_RESULTS_H
#define __FREESTANDING_CXX_HDRS_FCXX_ALGORITHM_RESULTS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <concepts>
#include <type_traits>

namespace std {
namespace ranges {

/* [algorithms.results], algorithm result types. They are aggregates, with
 * only the members the standard lists; their conversions are templates,
 * whose constraints are only checked where they are used. */
template <class _It, class _Out>
struct in_out_result {
    _FCXX_NO_UNIQUE_ADDRESS _It in;
    _FCXX_NO_UNIQUE_ADDRESS _Out out;

    template <class _I2, class _O2>
        requires convertible_to<const _It&, _I2> && convertible_to<const _Out&, _O2>
    constexpr operator in_out_result<_I2, _O2>() const& {
        return {in, out};
    }

    template <class _I2, class _O2>
        requires convertible_to<_It, _I2> && convertible_to<_Out, _O2>
    constexpr operator in_out_result<_I2, _O2>() && {
        return {std::move(in), std::move(out)};
    }
};

/* The others, of <algorithm>, which is freestanding from C++26 on. The one
 * above is also that of the uninitialized algorithms of <memory>, which is
 * freestanding from C++23 on. */
#  if _FCXX_STD >= 2026
template <class _It, class _Fp>
struct in_fun_result {
    _FCXX_NO_UNIQUE_ADDRESS _It in;
    _FCXX_NO_UNIQUE_ADDRESS _Fp fun;

    template <class _I2, class _F2>
        requires convertible_to<const _It&, _I2> && convertible_to<const _Fp&, _F2>
    constexpr operator in_fun_result<_I2, _F2>() const& {
        return {in, fun};
    }

    template <class _I2, class _F2>
        requires convertible_to<_It, _I2> && convertible_to<_Fp, _F2>
    constexpr operator in_fun_result<_I2, _F2>() && {
        return {std::move(in), std::move(fun)};
    }
};

template <class _I1, class _I2>
struct in_in_result {
    _FCXX_NO_UNIQUE_ADDRESS _I1 in1;
    _FCXX_NO_UNIQUE_ADDRESS _I2 in2;

    template <class _II1, class _II2>
        requires convertible_to<const _I1&, _II1> && convertible_to<const _I2&, _II2>
    constexpr operator in_in_result<_II1, _II2>() const& {
        return {in1, in2};
    }

    template <class _II1, class _II2>
        requires convertible_to<_I1, _II1> && convertible_to<_I2, _II2>
    constexpr operator in_in_result<_II1, _II2>() && {
        return {std::move(in1), std::move(in2)};
    }
};

template <class _I1, class _I2, class _Out>
struct in_in_out_result {
    _FCXX_NO_UNIQUE_ADDRESS _I1 in1;
    _FCXX_NO_UNIQUE_ADDRESS _I2 in2;
    _FCXX_NO_UNIQUE_ADDRESS _Out out;

    template <class _II1, class _II2, class _OO>
        requires convertible_to<const _I1&, _II1> && convertible_to<const _I2&, _II2>
              && convertible_to<const _Out&, _OO>
    constexpr operator in_in_out_result<_II1, _II2, _OO>() const& {
        return {in1, in2, out};
    }

    template <class _II1, class _II2, class _OO>
        requires convertible_to<_I1, _II1> && convertible_to<_I2, _II2> && convertible_to<_Out, _OO>
    constexpr operator in_in_out_result<_II1, _II2, _OO>() && {
        return {std::move(in1), std::move(in2), std::move(out)};
    }
};

template <class _It, class _O1, class _O2>
struct in_out_out_result {
    _FCXX_NO_UNIQUE_ADDRESS _It in;
    _FCXX_NO_UNIQUE_ADDRESS _O1 out1;
    _FCXX_NO_UNIQUE_ADDRESS _O2 out2;

    template <class _II, class _OO1, class _OO2>
        requires convertible_to<const _It&, _II> && convertible_to<const _O1&, _OO1>
              && convertible_to<const _O2&, _OO2>
    constexpr operator in_out_out_result<_II, _OO1, _OO2>() const& {
        return {in, out1, out2};
    }

    template <class _II, class _OO1, class _OO2>
        requires convertible_to<_It, _II> && convertible_to<_O1, _OO1> && convertible_to<_O2, _OO2>
    constexpr operator in_out_out_result<_II, _OO1, _OO2>() && {
        return {std::move(in), std::move(out1), std::move(out2)};
    }
};

template <class _Tp>
struct min_max_result {
    _FCXX_NO_UNIQUE_ADDRESS _Tp min;
    _FCXX_NO_UNIQUE_ADDRESS _Tp max;

    template <class _T2>
        requires convertible_to<const _Tp&, _T2>
    constexpr operator min_max_result<_T2>() const& {
        return {min, max};
    }

    template <class _T2>
        requires convertible_to<_Tp, _T2>
    constexpr operator min_max_result<_T2>() && {
        return {std::move(min), std::move(max)};
    }
};

template <class _It>
struct in_found_result {
    _FCXX_NO_UNIQUE_ADDRESS _It in;
    bool found;

    template <class _I2>
        requires convertible_to<const _It&, _I2>
    constexpr operator in_found_result<_I2>() const& {
        return {in, found};
    }

    template <class _I2>
        requires convertible_to<_It, _I2>
    constexpr operator in_found_result<_I2>() && {
        return {std::move(in), found};
    }
};

template <class _It, class _Tp>
struct in_value_result {
    _FCXX_NO_UNIQUE_ADDRESS _It in;
    _FCXX_NO_UNIQUE_ADDRESS _Tp value;

    template <class _I2, class _T2>
        requires convertible_to<const _It&, _I2> && convertible_to<const _Tp&, _T2>
    constexpr operator in_value_result<_I2, _T2>() const& {
        return {in, value};
    }

    template <class _I2, class _T2>
        requires convertible_to<_It, _I2> && convertible_to<_Tp, _T2>
    constexpr operator in_value_result<_I2, _T2>() && {
        return {std::move(in), std::move(value)};
    }
};

template <class _Out, class _Tp>
struct out_value_result {
    _FCXX_NO_UNIQUE_ADDRESS _Out out;
    _FCXX_NO_UNIQUE_ADDRESS _Tp value;

    template <class _O2, class _T2>
        requires convertible_to<const _Out&, _O2> && convertible_to<const _Tp&, _T2>
    constexpr operator out_value_result<_O2, _T2>() const& {
        return {out, value};
    }

    template <class _O2, class _T2>
        requires convertible_to<_Out, _O2> && convertible_to<_Tp, _T2>
    constexpr operator out_value_result<_O2, _T2>() && {
        return {std::move(out), std::move(value)};
    }
};
#  endif

} /* namespace ranges */
} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
