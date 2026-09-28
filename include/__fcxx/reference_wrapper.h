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


/* reference_wrapper, of <functional>, which other headers need too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_REFERENCE_WRAPPER_H
#define __FREESTANDING_CXX_HDRS_FCXX_REFERENCE_WRAPPER_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <type_traits>
#include <__fcxx/addressof.h>
#include <concepts>
#if _FCXX_STD >= 2026
#  include <__fcxx/synth_three_way.h>
#endif

namespace std {

/* The functions FUN that reference_wrapper is constructed with, which only
 * bind a reference to lvalues. */
template <class _Tp>
struct __fcxx_refwrap_fun {
    static constexpr _Tp& __fun(_Tp& __r) noexcept {
        return __r;
    }

    static void __fun(_Tp&&) = delete;
};

template <class _Tp, class _Up, class = void>
struct __fcxx_refwrap_binds : false_type {};

template <class _Tp, class _Up>
struct __fcxx_refwrap_binds<_Tp, _Up, __fcxx_void_t<decltype(__fcxx_refwrap_fun<_Tp>::__fun(std::declval<_Up>()))> >
    : true_type {};

/* [refwrap], class template reference_wrapper */
template <class _Tp>
class reference_wrapper {
public:
    typedef _Tp type;

    template <class _Up,
              __fcxx_enable_if_t<__fcxx_and<__fcxx_not<is_same<__fcxx_remove_cvref_t<_Up>, reference_wrapper>>,
                                            __fcxx_refwrap_binds<_Tp, _Up>>::value,
                                 int> = 0>
    constexpr
    reference_wrapper(_Up&& __u) noexcept(noexcept(__fcxx_refwrap_fun<_Tp>::__fun(std::declval<_Up>())))
        : __ptr(std::addressof(__fcxx_refwrap_fun<_Tp>::__fun(std::forward<_Up>(__u)))) {}

    reference_wrapper(const reference_wrapper&) noexcept = default;
    reference_wrapper& operator=(const reference_wrapper&) noexcept = default;

    constexpr operator _Tp&() const noexcept {
        return *__ptr;
    }

    constexpr _Tp& get() const noexcept {
        return *__ptr;
    }

    template <class... _Args>
    constexpr typename __fcxx_invoke_result<_Tp&, _Args...>::type operator()(_Args&&... __args) const
        noexcept(__fcxx_is_nothrow_invocable<_Tp&, _Args...>::value) {
        return std::__fcxx_invoke(*__ptr, std::forward<_Args>(__args)...);
    }

#if _FCXX_STD >= 2026
    /* [refwrap.comparisons], comparisons */
    friend constexpr bool operator==(reference_wrapper __x, reference_wrapper __y)
        requires requires {
            { std::declval<_Tp&>() == std::declval<_Tp&>() } -> convertible_to<bool>;
        }
    {
        return __x.get() == __y.get();
    }

    friend constexpr bool operator==(reference_wrapper __x, const _Tp& __y)
        requires requires {
            { std::declval<_Tp&>() == __y } -> convertible_to<bool>;
        }
    {
        return __x.get() == __y;
    }

    friend constexpr bool operator==(reference_wrapper __x, reference_wrapper<const _Tp> __y)
        requires (!is_const_v<_Tp>) && requires {
            { std::declval<_Tp&>() == std::declval<const _Tp&>() } -> convertible_to<bool>;
        }
    {
        return __x.get() == __y.get();
    }

    friend constexpr auto operator<=>(reference_wrapper __x, reference_wrapper __y)
        requires requires { __fcxx_synth_three_way(std::declval<_Tp&>(), std::declval<_Tp&>()); }
    {
        return __fcxx_synth_three_way(__x.get(), __y.get());
    }

    friend constexpr auto operator<=>(reference_wrapper __x, const _Tp& __y)
        requires requires { __fcxx_synth_three_way(std::declval<_Tp&>(), __y); }
    {
        return __fcxx_synth_three_way(__x.get(), __y);
    }

    friend constexpr auto operator<=>(reference_wrapper __x, reference_wrapper<const _Tp> __y)
        requires (!is_const_v<_Tp>) && requires {
            __fcxx_synth_three_way(std::declval<_Tp&>(), std::declval<const _Tp&>());
        }
    {
        return __fcxx_synth_three_way(__x.get(), __y.get());
    }
#endif

private:
    _Tp* __ptr;
};

template <class _Tp>
reference_wrapper(_Tp&) -> reference_wrapper<_Tp>;

/* [refwrap.helpers], helper functions */

template <class _Tp>
constexpr reference_wrapper<_Tp> ref(_Tp& __t) noexcept {
    return reference_wrapper<_Tp>(__t);
}

template <class _Tp>
constexpr reference_wrapper<_Tp> ref(reference_wrapper<_Tp> __t) noexcept {
    return __t;
}

template <class _Tp>
void ref(const _Tp&&) = delete;

template <class _Tp>
constexpr reference_wrapper<const _Tp> cref(const _Tp& __t) noexcept {
    return reference_wrapper<const _Tp>(__t);
}

template <class _Tp>
constexpr reference_wrapper<const _Tp> cref(reference_wrapper<_Tp> __t) noexcept {
    return __t;
}

template <class _Tp>
void cref(const _Tp&&) = delete;

/* [refwrap.common.ref], common_reference related specializations */
template <class _Rp, class _Tp, class _RQ, class _TQ>
concept __fcxx_ref_wrap_common_reference_exists_with = __fcxx_is_reference_wrapper<_Rp>::value
    && requires { typename common_reference_t<typename _Rp::type&, _TQ>; }
    && convertible_to<_RQ, common_reference_t<typename _Rp::type&, _TQ> >;

template <class _Rp, class _Tp, template <class> class _RQual, template <class> class _TQual>
    requires (__fcxx_ref_wrap_common_reference_exists_with<_Rp, _Tp, _RQual<_Rp>, _TQual<_Tp> >
              && !__fcxx_ref_wrap_common_reference_exists_with<_Tp, _Rp, _TQual<_Tp>, _RQual<_Rp> >)
struct basic_common_reference<_Rp, _Tp, _RQual, _TQual> {
    using type = common_reference_t<typename _Rp::type&, _TQual<_Tp> >;
};

template <class _Tp, class _Rp, template <class> class _TQual, template <class> class _RQual>
    requires (__fcxx_ref_wrap_common_reference_exists_with<_Rp, _Tp, _RQual<_Rp>, _TQual<_Tp> >
              && !__fcxx_ref_wrap_common_reference_exists_with<_Tp, _Rp, _TQual<_Tp>, _RQual<_Rp> >)
struct basic_common_reference<_Tp, _Rp, _TQual, _RQual> {
    using type = common_reference_t<typename _Rp::type&, _TQual<_Tp> >;
};

} /* namespace std */

#endif
