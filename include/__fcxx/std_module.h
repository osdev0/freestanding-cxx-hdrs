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


/* The headers of the named modules std and std.compat (modules/std.cppm and
 * modules/std.compat.cppm): every freestanding header of the revision in use,
 * from C++23 on, which has the modules, with GCC 15 and later and Clang 17 and
 * later.
 *
 * Workaround: GCC 16, all targets, with -fmodules: it takes the #include of a
 * standard header from a file outside of the directory of that header to
 * name <bits/stdc++.h> instead, and stops where there is no such header, as
 * with -nostdinc. The module units include the headers through this one,
 * which is in the same directory as they are. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_STD_MODULE_H
#define __FREESTANDING_CXX_HDRS_FCXX_STD_MODULE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#if _FCXX_STD < 2023
#  error "The modules std and std.compat require C++23 or later"
#elif _FCXX_GCC && _FCXX_GCC < 1500
#  error "The modules std and std.compat require GCC 15 or later"
#elif _FCXX_CLANG && _FCXX_CLANG < 1700
#  error "The modules std and std.compat require Clang 17 or later"
#endif

#if _FCXX_STD >= 2023
#  include <atomic>
#  include <bit>
#  include <cfloat>
#  include <climits>
#  include <compare>
#  include <concepts>
#  include <coroutine>
#  include <cstdarg>
#  include <cstddef>
#  include <cstdint>
#  include <cstdlib>
#  include <exception>
#  include <functional>
#  include <initializer_list>
#  include <iterator>
#  include <limits>
#  include <memory>
#  include <new>
#  include <ranges>
#  include <ratio>
#  include <source_location>
#  include <tuple>
#  include <type_traits>
#  include <typeinfo>
#  include <utility>
#  include <version>
#endif

#if _FCXX_STD >= 2026
#  include <algorithm>
#  include <array>
#  include <charconv>
#  include <cmath>
#  include <contracts>
#  include <cstring>
#  include <cwchar>
#  include <debugging>
#  include <execution>
#  include <expected>
#  include <inplace_vector>
#  include <mdspan>
#  include <numeric>
#  include <optional>
#  include <random>
#  include <span>
#  include <stdbit.h>
#  include <string>
#  include <string_view>
#  include <system_error>
#  include <variant>
#endif

_FCXX_END_EXTERN_CXX
#endif
