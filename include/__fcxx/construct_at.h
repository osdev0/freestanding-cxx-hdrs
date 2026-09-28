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


/* construct_at and destroy_at, of <memory>, which the headers constructing
 * objects in place need. The compilers only construct objects in place in
 * constant expressions in std::construct_at, before C++26. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_CONSTRUCT_AT_H
#define __FREESTANDING_CXX_HDRS_FCXX_CONSTRUCT_AT_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <new>
#include <type_traits>
#include <__fcxx/addressof.h>

namespace std {

/* The address of an object, as a pointer to void, which is not taken of a
 * const object from C++26 on. */
template <class _Tp>
constexpr void* __fcxx_voidify(_Tp& __obj) noexcept {
#if _FCXX_STD >= 2026
    return std::addressof(__obj);
#else
    return const_cast<void*>(static_cast<const volatile void*>(std::addressof(__obj)));
#endif
}

template <class _Tp, class... _Args>
struct __fcxx_can_construct_at {
    template <class _Up, class = decltype(::new (std::declval<void*>()) _Up(std::declval<_Args>()...))>
    static true_type __test(int);

    template <class _Up>
    static false_type __test(...);

    static constexpr bool value = decltype(__test<_Tp>(0))::value
#if _FCXX_STD >= 2026
                                  && !is_unbounded_array<_Tp>::value
#endif
        ;
};

/* [specialized.construct], construct_at */
template <class _Tp, class... _Args, __fcxx_enable_if_t<__fcxx_can_construct_at<_Tp, _Args...>::value, int> = 0>
constexpr _Tp* construct_at(_Tp* __location, _Args&&... __args) {
#if _FCXX_STD >= 2026
    if constexpr (is_array<_Tp>::value) {
        static_assert(sizeof...(_Args) == 0, "std::construct_at of an array with arguments");
        return ::new (std::__fcxx_voidify(*__location)) _Tp[1]();
    } else {
        return ::new (std::__fcxx_voidify(*__location)) _Tp(std::forward<_Args>(__args)...);
    }
#else
    return ::new (std::__fcxx_voidify(*__location)) _Tp(std::forward<_Args>(__args)...);
#endif
}

/* [specialized.destroy], destroy_at, which destroys the elements of arrays
 * too */
template <class _Tp>
constexpr void destroy_at(_Tp* __location) {
    if constexpr (is_array<_Tp>::value) {
        for (auto& __elem : *__location) {
            std::destroy_at(std::addressof(__elem));
        }
    } else {
        __location->~_Tp();
    }
}

} /* namespace std */

#endif
