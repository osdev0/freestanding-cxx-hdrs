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


/* invoke and invoke_r, of <functional>, which other headers need too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_INVOKE_H
#define __FREESTANDING_CXX_HDRS_FCXX_INVOKE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <type_traits>

namespace std {

/* [func.invoke], invoke functions */

template <class _Fp, class... _Args>
constexpr typename __fcxx_invoke_result<_Fp, _Args...>::type invoke(_Fp&& __f, _Args&&... __args) noexcept(
    __fcxx_is_nothrow_invocable<_Fp, _Args...>::value) {
    return std::__fcxx_invoke(std::forward<_Fp>(__f), std::forward<_Args>(__args)...);
}

template <class _Rp, class _Fp, class... _Args>
    requires(__fcxx_is_invocable_r<_Rp, _Fp, _Args...>::value)
constexpr _Rp invoke_r(_Fp&& __f,
                       _Args&&... __args) noexcept(__fcxx_is_nothrow_invocable_r<_Rp, _Fp, _Args...>::value) {
    if constexpr (is_void<_Rp>::value) {
        static_cast<void>(std::__fcxx_invoke(std::forward<_Fp>(__f), std::forward<_Args>(__args)...));
    } else {
        return std::__fcxx_invoke(std::forward<_Fp>(__f), std::forward<_Args>(__args)...);
    }
}

} /* namespace std */

#endif
