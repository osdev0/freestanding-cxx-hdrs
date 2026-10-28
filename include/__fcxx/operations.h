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


/* The function objects of the operators, of <functional>, which other
 * headers need too: [arithmetic.operations], [comparisons],
 * [logical.operations], [bitwise.operations], [func.identity] and
 * [range.cmp]. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_OPERATIONS_H
#define __FREESTANDING_CXX_HDRS_FCXX_OPERATIONS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX
#include <__fcxx/integer_product.h>

#include <type_traits>
#include <concepts>

/* The function objects take void for their parameter by default, which
 * the specializations for void are transparent for. */

namespace std {

/* The builtin ordered comparisons of pointers to functions, which the order
 * of pointers makes for them, and which Clang from version 13 on warns
 * about, as their order is unspecified. */
#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-warning-option"
#  pragma clang diagnostic ignored "-Wordered-compare-function-pointers"
#endif

/* The strict total order of pointers, which the builtin comparison is only
 * in constant evaluation, and which is that of their addresses at run time.
 * Before the compilers can tell whether they are in constant evaluation,
 * the builtin comparison is used, which orders the addresses too. */
template <class _Tp, class _Up>
constexpr bool __fcxx_pointer_less(_Tp __a, _Up __b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return __a < __b;
    }
    return reinterpret_cast<_FCXX_UINTPTR>(__a) < reinterpret_cast<_FCXX_UINTPTR>(__b);
}

/* [arithmetic.operations], arithmetic operations; [comparisons],
 * comparisons; [logical.operations], logical operations */

template <class _Tp = void>
struct plus {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x + __y;
    }
};

template <class _Tp = void>
struct minus {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x - __y;
    }
};

template <class _Tp = void>
struct multiplies {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return _FCXX_MUL(__x, __y);
    }
};

template <class _Tp = void>
struct divides {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x / __y;
    }
};

template <class _Tp = void>
struct modulus {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x % __y;
    }
};

template <class _Tp = void>
struct negate {
    constexpr _Tp operator()(const _Tp& __x) const {
        return -__x;
    }
};

template <class _Tp = void>
struct equal_to {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x == __y;
    }
};

template <class _Tp = void>
struct not_equal_to {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x != __y;
    }
};

template <class _Tp = void>
struct greater {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x > __y;
    }
};

template <class _Tp = void>
struct less {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x < __y;
    }
};

template <class _Tp = void>
struct greater_equal {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x >= __y;
    }
};

template <class _Tp = void>
struct less_equal {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x <= __y;
    }
};

template <class _Tp>
struct greater<_Tp*> {
    constexpr bool operator()(_Tp* __x, _Tp* __y) const {
        return std::__fcxx_pointer_less(__y, __x);
    }
};

template <class _Tp>
struct less<_Tp*> {
    constexpr bool operator()(_Tp* __x, _Tp* __y) const {
        return std::__fcxx_pointer_less(__x, __y);
    }
};

template <class _Tp>
struct greater_equal<_Tp*> {
    constexpr bool operator()(_Tp* __x, _Tp* __y) const {
        return !std::__fcxx_pointer_less(__x, __y);
    }
};

template <class _Tp>
struct less_equal<_Tp*> {
    constexpr bool operator()(_Tp* __x, _Tp* __y) const {
        return !std::__fcxx_pointer_less(__y, __x);
    }
};

template <class _Tp = void>
struct logical_and {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x && __y;
    }
};

template <class _Tp = void>
struct logical_or {
    constexpr bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x || __y;
    }
};

template <class _Tp = void>
struct logical_not {
    constexpr bool operator()(const _Tp& __x) const {
        return !__x;
    }
};

/* [bitwise.operations], bitwise operations */

template <class _Tp = void>
struct bit_and {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x & __y;
    }
};

template <class _Tp = void>
struct bit_or {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x | __y;
    }
};

template <class _Tp = void>
struct bit_xor {
    constexpr _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x ^ __y;
    }
};

template <class _Tp = void>
struct bit_not {
    constexpr _Tp operator()(const _Tp& __x) const {
        return ~__x;
    }
};

/* The transparent specializations. */

template <>
struct plus<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) + std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) + std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) + std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct minus<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) - std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) - std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) - std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct multiplies<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) * std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) * std::forward<_Up>(__u)) {
        return _FCXX_MUL(std::forward<_Tp>(__t), std::forward<_Up>(__u));
    }

    typedef void is_transparent;
};

template <>
struct divides<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) / std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) / std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) / std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct modulus<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) % std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) % std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) % std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct negate<void> {
    template <class _Tp>
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(-std::forward<_Tp>(__t)))
        -> decltype(-std::forward<_Tp>(__t)) {
        return -std::forward<_Tp>(__t);
    }

    typedef void is_transparent;
};

template <>
struct equal_to<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) == std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) == std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) == std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct not_equal_to<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) != std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) != std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) != std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

/* Whether a comparison of two objects is one of the builtin comparisons of
 * pointers: when the objects convert to pointers, and there is no operator
 * of their own to compare them with. */
template <class _Tp, class _Up>
struct __fcxx_converts_to_pointers
    : __fcxx_bool_constant<is_convertible<_Tp, const volatile void*>::value
                           && is_convertible<_Up, const volatile void*>::value> {};

#define _FCXX_OPS_BUILTIN_PTR_CMP(name, op)                                                                        \
        template <class _Tp, class _Up, class = void>                                                                  \
        struct __fcxx_has_member_##name : false_type {};                                                               \
        template <class _Tp, class _Up>                                                                                \
        struct __fcxx_has_member_##                                                                                    \
            name<_Tp, _Up, __fcxx_void_t<decltype(std::declval<_Tp>().operator op(std::declval<_Up>()))>>              \
            : true_type {};                                                                                            \
        template <class _Tp, class _Up, class = void>                                                                  \
        struct __fcxx_has_free_##name : false_type {};                                                                 \
        template <class _Tp, class _Up>                                                                                \
        struct __fcxx_has_free_##                                                                                      \
            name<_Tp, _Up, __fcxx_void_t<decltype(operator op(std::declval<_Tp>(), std::declval<_Up>()))>>             \
            : true_type {};                                                                                            \
        template <class _Tp, class _Up>                                                                                \
        struct __fcxx_builtin_ptr_##name                                                                               \
            : __fcxx_and<__fcxx_converts_to_pointers<_Tp, _Up>, __fcxx_not<__fcxx_has_member_##name<_Tp, _Up>>,        \
                         __fcxx_not<__fcxx_has_free_##name<_Tp, _Up>>> {};

_FCXX_OPS_BUILTIN_PTR_CMP(less, <)
_FCXX_OPS_BUILTIN_PTR_CMP(greater, >)
_FCXX_OPS_BUILTIN_PTR_CMP(less_equal, <=)
_FCXX_OPS_BUILTIN_PTR_CMP(greater_equal, >=)
_FCXX_OPS_BUILTIN_PTR_CMP(equal_to, ==)

#undef _FCXX_OPS_BUILTIN_PTR_CMP

template <>
struct greater<void> {
    template <class _Tp, class _Up, __fcxx_enable_if_t<!__fcxx_builtin_ptr_greater<_Tp, _Up>::value, int> = 0>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) > std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) > std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) > std::forward<_Up>(__u);
    }

    template <class _Tp, class _Up, __fcxx_enable_if_t<__fcxx_builtin_ptr_greater<_Tp, _Up>::value, int> = 0>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const noexcept
        -> decltype(std::forward<_Tp>(__t) > std::forward<_Up>(__u)) {
        return std::__fcxx_pointer_less(static_cast<const volatile void*>(std::forward<_Up>(__u)),
                                            static_cast<const volatile void*>(std::forward<_Tp>(__t)));
    }

    typedef void is_transparent;
};

template <>
struct less<void> {
    template <class _Tp, class _Up, __fcxx_enable_if_t<!__fcxx_builtin_ptr_less<_Tp, _Up>::value, int> = 0>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) < std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) < std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) < std::forward<_Up>(__u);
    }

    template <class _Tp, class _Up, __fcxx_enable_if_t<__fcxx_builtin_ptr_less<_Tp, _Up>::value, int> = 0>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const noexcept
        -> decltype(std::forward<_Tp>(__t) < std::forward<_Up>(__u)) {
        return std::__fcxx_pointer_less(static_cast<const volatile void*>(std::forward<_Tp>(__t)),
                                            static_cast<const volatile void*>(std::forward<_Up>(__u)));
    }

    typedef void is_transparent;
};

template <>
struct greater_equal<void> {
    template <class _Tp, class _Up, __fcxx_enable_if_t<!__fcxx_builtin_ptr_greater_equal<_Tp, _Up>::value, int> = 0>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) >= std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) >= std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) >= std::forward<_Up>(__u);
    }

    template <class _Tp, class _Up, __fcxx_enable_if_t<__fcxx_builtin_ptr_greater_equal<_Tp, _Up>::value, int> = 0>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const noexcept
        -> decltype(std::forward<_Tp>(__t) >= std::forward<_Up>(__u)) {
        return !std::__fcxx_pointer_less(static_cast<const volatile void*>(std::forward<_Tp>(__t)),
                                             static_cast<const volatile void*>(std::forward<_Up>(__u)));
    }

    typedef void is_transparent;
};

template <>
struct less_equal<void> {
    template <class _Tp, class _Up, __fcxx_enable_if_t<!__fcxx_builtin_ptr_less_equal<_Tp, _Up>::value, int> = 0>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) <= std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) <= std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) <= std::forward<_Up>(__u);
    }

    template <class _Tp, class _Up, __fcxx_enable_if_t<__fcxx_builtin_ptr_less_equal<_Tp, _Up>::value, int> = 0>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const noexcept
        -> decltype(std::forward<_Tp>(__t) <= std::forward<_Up>(__u)) {
        return !std::__fcxx_pointer_less(static_cast<const volatile void*>(std::forward<_Up>(__u)),
                                             static_cast<const volatile void*>(std::forward<_Tp>(__t)));
    }

    typedef void is_transparent;
};

template <>
struct logical_and<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) && std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) && std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) && std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct logical_or<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) || std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) || std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) || std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct logical_not<void> {
    template <class _Tp>
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(!std::forward<_Tp>(__t)))
        -> decltype(!std::forward<_Tp>(__t)) {
        return !std::forward<_Tp>(__t);
    }

    typedef void is_transparent;
};

template <>
struct bit_and<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) & std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) & std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) & std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct bit_or<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) | std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) | std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) | std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct bit_xor<void> {
    template <class _Tp, class _Up>
    constexpr auto operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(std::forward<_Tp>(__t) ^ std::forward<_Up>(__u)))
        -> decltype(std::forward<_Tp>(__t) ^ std::forward<_Up>(__u)) {
        return std::forward<_Tp>(__t) ^ std::forward<_Up>(__u);
    }

    typedef void is_transparent;
};

template <>
struct bit_not<void> {
    template <class _Tp>
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(~std::forward<_Tp>(__t)))
        -> decltype(~std::forward<_Tp>(__t)) {
        return ~std::forward<_Tp>(__t);
    }

    typedef void is_transparent;
};

/* [func.identity], identity */
struct identity {
    template <class _Tp>
    constexpr _Tp&& operator()(_Tp&& __t) const noexcept {
        return std::forward<_Tp>(__t);
    }

    using is_transparent = void;
};

/* [range.cmp], concept-constrained comparisons */
namespace ranges {

struct equal_to {
    template <class _Tp, class _Up>
        requires equality_comparable_with<_Tp, _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(bool(std::forward<_Tp>(__t) == std::forward<_Up>(__u)))) {
        if constexpr (__fcxx_builtin_ptr_equal_to<_Tp, _Up>::value) {
            return static_cast<const volatile void*>(std::forward<_Tp>(__t))
                == static_cast<const volatile void*>(std::forward<_Up>(__u));
        } else {
            return std::forward<_Tp>(__t) == std::forward<_Up>(__u);
        }
    }

    using is_transparent = void;
};

struct not_equal_to {
    template <class _Tp, class _Up>
        requires equality_comparable_with<_Tp, _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(bool(std::forward<_Tp>(__t) == std::forward<_Up>(__u)))) {
        return !ranges::equal_to{}(std::forward<_Tp>(__t), std::forward<_Up>(__u));
    }

    using is_transparent = void;
};

struct less {
    template <class _Tp, class _Up>
        requires totally_ordered_with<_Tp, _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(bool(std::forward<_Tp>(__t) < std::forward<_Up>(__u)))) {
        if constexpr (__fcxx_builtin_ptr_less<_Tp, _Up>::value) {
            return std::__fcxx_pointer_less(static_cast<const volatile void*>(std::forward<_Tp>(__t)),
                                            static_cast<const volatile void*>(std::forward<_Up>(__u)));
        } else {
            return std::forward<_Tp>(__t) < std::forward<_Up>(__u);
        }
    }

    using is_transparent = void;
};

struct greater {
    template <class _Tp, class _Up>
        requires totally_ordered_with<_Tp, _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(bool(std::forward<_Up>(__u) < std::forward<_Tp>(__t)))) {
        return ranges::less{}(std::forward<_Up>(__u), std::forward<_Tp>(__t));
    }

    using is_transparent = void;
};

struct greater_equal {
    template <class _Tp, class _Up>
        requires totally_ordered_with<_Tp, _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(bool(std::forward<_Tp>(__t) < std::forward<_Up>(__u)))) {
        return !ranges::less{}(std::forward<_Tp>(__t), std::forward<_Up>(__u));
    }

    using is_transparent = void;
};

struct less_equal {
    template <class _Tp, class _Up>
        requires totally_ordered_with<_Tp, _Up>
    constexpr bool operator()(_Tp&& __t, _Up&& __u) const
        noexcept(noexcept(bool(std::forward<_Up>(__u) < std::forward<_Tp>(__t)))) {
        return !ranges::less{}(std::forward<_Up>(__u), std::forward<_Tp>(__t));
    }

    using is_transparent = void;
};

} /* namespace ranges */

} /* namespace std */

#if defined(__clang__)
#  pragma clang diagnostic pop
#endif

_FCXX_END_EXTERN_CXX
#endif
