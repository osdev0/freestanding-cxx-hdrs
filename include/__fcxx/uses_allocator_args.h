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


/* Uses-allocator construction, of <memory>, which the constructors of tuple
 * taking an allocator are defined with. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_USES_ALLOCATOR_ARGS_H
#define __FREESTANDING_CXX_HDRS_FCXX_USES_ALLOCATOR_ARGS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX
#include <__fcxx/uses_allocator.h>

#include <tuple>
#include <utility>

namespace std {

template <class _Tp>
struct __fcxx_is_pair_spec : false_type {};

template <class _T1, class _T2>
struct __fcxx_is_pair_spec<pair<_T1, _T2> > : true_type {};

template <class _Tp>
struct __fcxx_uses_allocator_ill_formed : false_type {};

template <class _Tp, class _Alloc, class... _Args>
    requires (!__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value)
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc, _Args&&... __args) noexcept {
    if constexpr (!uses_allocator<__fcxx_remove_cv_t<_Tp>, _Alloc>::value && is_constructible<_Tp, _Args...>::value) {
        return std::forward_as_tuple(std::forward<_Args>(__args)...);
    } else if constexpr (uses_allocator<__fcxx_remove_cv_t<_Tp>, _Alloc>::value
                         && is_constructible<_Tp, allocator_arg_t, const _Alloc&, _Args...>::value) {
        return tuple<allocator_arg_t, const _Alloc&, _Args&&...>(allocator_arg, __alloc,
                                                                 std::forward<_Args>(__args)...);
    } else if constexpr (uses_allocator<__fcxx_remove_cv_t<_Tp>, _Alloc>::value
                         && is_constructible<_Tp, _Args..., const _Alloc&>::value) {
        return std::forward_as_tuple(std::forward<_Args>(__args)..., __alloc);
    } else {
        static_assert(__fcxx_uses_allocator_ill_formed<_Tp>::value,
                      "uses-allocator construction of a type that cannot be constructed so");
    }
}

template <class _Tp, class _Alloc, class _Tuple1, class _Tuple2>
    requires (__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value)
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc, piecewise_construct_t, _Tuple1&& __x,
                                                _Tuple2&& __y) noexcept {
    return std::make_tuple(
        piecewise_construct,
        std::apply(
            [&__alloc](auto&&... __args1) {
                return std::uses_allocator_construction_args<typename _Tp::first_type>(
                    __alloc, std::forward<decltype(__args1)>(__args1)...);
            },
            std::forward<_Tuple1>(__x)),
        std::apply(
            [&__alloc](auto&&... __args2) {
                return std::uses_allocator_construction_args<typename _Tp::second_type>(
                    __alloc, std::forward<decltype(__args2)>(__args2)...);
            },
            std::forward<_Tuple2>(__y)));
}

template <class _Tp, class _Alloc>
    requires (__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value)
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc) noexcept {
    return std::uses_allocator_construction_args<_Tp>(__alloc, piecewise_construct, tuple<>{}, tuple<>{});
}

template <class _Tp, class _Alloc, class _Up, class _Vp>
    requires (__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value)
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc, _Up&& __u, _Vp&& __v) noexcept {
    return std::uses_allocator_construction_args<_Tp>(__alloc, piecewise_construct,
                                                      std::forward_as_tuple(std::forward<_Up>(__u)),
                                                      std::forward_as_tuple(std::forward<_Vp>(__v)));
}

template <class _Tp, class _Alloc, class _Up, class _Vp>
    requires (__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value)
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc, pair<_Up, _Vp>& __pr) noexcept {
    return std::uses_allocator_construction_args<_Tp>(__alloc, piecewise_construct, std::forward_as_tuple(__pr.first),
                                                      std::forward_as_tuple(__pr.second));
}

template <class _Tp, class _Alloc, class _Up, class _Vp>
    requires (__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value)
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc, const pair<_Up, _Vp>& __pr) noexcept {
    return std::uses_allocator_construction_args<_Tp>(__alloc, piecewise_construct, std::forward_as_tuple(__pr.first),
                                                      std::forward_as_tuple(__pr.second));
}

template <class _Tp, class _Alloc, class _Up, class _Vp>
    requires (__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value)
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc, pair<_Up, _Vp>&& __pr) noexcept {
    return std::uses_allocator_construction_args<_Tp>(__alloc, piecewise_construct,
                                                      std::forward_as_tuple(std::get<0>(std::move(__pr))),
                                                      std::forward_as_tuple(std::get<1>(std::move(__pr))));
}

template <class _Tp, class _Alloc, class _Up, class _Vp>
    requires (__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value)
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc, const pair<_Up, _Vp>&& __pr) noexcept {
    return std::uses_allocator_construction_args<_Tp>(__alloc, piecewise_construct,
                                                      std::forward_as_tuple(std::get<0>(std::move(__pr))),
                                                      std::forward_as_tuple(std::get<1>(std::move(__pr))));
}

template <class _Tp, class _Alloc, __fcxx_pair_like _Pp>
    requires (__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value
              && !__fcxx_is_subrange<__fcxx_remove_cvref_t<_Pp> >::value)
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc, _Pp&& __p) noexcept {
    return std::uses_allocator_construction_args<_Tp>(
        __alloc, piecewise_construct, std::forward_as_tuple(std::__fcxx_adl_get<0>(std::forward<_Pp>(__p))),
        std::forward_as_tuple(std::__fcxx_adl_get<1>(std::forward<_Pp>(__p))));
}

/* The pair from the conversion of an object to a pair, constructed with the
 * allocator. */
template <class _Tp, class _Alloc, class _Up>
class __fcxx_pair_constructor {
    using _Pair = __fcxx_remove_cv_t<_Tp>;

    constexpr _Pair __construct(const _Pair& __p) const {
        return std::make_obj_using_allocator<_Pair>(__alloc, __p);
    }

    constexpr _Pair __construct(_Pair&& __p) const {
        return std::make_obj_using_allocator<_Pair>(__alloc, std::move(__p));
    }

    const _Alloc& __alloc;
    _Up& __u;

public:
    constexpr __fcxx_pair_constructor(const _Alloc& __a, _Up& __v) noexcept : __alloc(__a), __u(__v) {}

    constexpr operator _Pair() const {
        return __construct(std::forward<_Up>(__u));
    }
};

template <class _Ap, class _Bp>
void __fcxx_pair_conversion_test(const pair<_Ap, _Bp>&);

template <class _Up>
concept __fcxx_converts_to_pair = requires(_Up& __u) { std::__fcxx_pair_conversion_test(__u); };

template <class _Tp, class _Alloc, class _Up>
    requires (__fcxx_is_pair_spec<__fcxx_remove_cv_t<_Tp> >::value
              && (__fcxx_is_subrange<__fcxx_remove_cvref_t<_Up> >::value
                  || (!__fcxx_pair_like<_Up> && !__fcxx_converts_to_pair<_Up>)))
constexpr auto uses_allocator_construction_args(const _Alloc& __alloc, _Up&& __u) noexcept {
    return std::make_tuple(__fcxx_pair_constructor<_Tp, _Alloc, _Up>(__alloc, __u));
}

template <class _Tp, class _Alloc, class... _Args>
constexpr _Tp make_obj_using_allocator(const _Alloc& __alloc, _Args&&... __args) {
    return std::make_from_tuple<_Tp>(
        std::uses_allocator_construction_args<_Tp>(__alloc, std::forward<_Args>(__args)...));
}

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
