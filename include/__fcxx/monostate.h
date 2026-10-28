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


/* monostate, of <variant>, and of <utility> from C++26 on. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_MONOSTATE_H
#define __FREESTANDING_CXX_HDRS_FCXX_MONOSTATE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <__fcxx/hash.h>
#include <compare>

namespace std {

struct monostate {};

constexpr bool operator==(monostate, monostate) noexcept {
    return true;
}

constexpr strong_ordering operator<=>(monostate, monostate) noexcept {
    return strong_ordering::equal;
}

template <>
struct hash<monostate> {

    size_t operator()(monostate) const noexcept {
        return static_cast<size_t>(0x6D6F6E6FUL);
    }
};

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
