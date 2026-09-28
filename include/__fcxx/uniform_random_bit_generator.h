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

/* [rand.req.urng], the concept of uniform_random_bit_generator of <random>,
 * under a name of its own, for the random algorithms of <algorithm> too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_UNIFORM_RANDOM_BIT_GENERATOR_H
#define __FREESTANDING_CXX_HDRS_FCXX_UNIFORM_RANDOM_BIT_GENERATOR_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <concepts>
#include <type_traits>

namespace std {

template <class _Gp>
concept __fcxx_uniform_random_bit_generator =
    invocable<_Gp&> && unsigned_integral<invoke_result_t<_Gp&> > && requires {
        { _Gp::min() } -> same_as<invoke_result_t<_Gp&> >;
        { _Gp::max() } -> same_as<invoke_result_t<_Gp&> >;
        requires bool_constant<(_Gp::min() < _Gp::max())>::value;
    };

} /* namespace std */

#endif
