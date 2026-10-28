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


/* [range.join], [range.join.with], [range.lazy.split], [range.split] and
 * [range.concat], the views of <ranges> that join ranges and split them:
 * join_view, join_with_view, lazy_split_view, split_view and concat_view.
 *
 * Workaround: GCC 11 and 12, all targets: they do not give the hidden friends
 * of a class nested in another the access to the private members of the
 * enclosing class that the nested class has ("... is private within this
 * context"), which Clang and GCC 13 give them. The hidden friends of the
 * iterators do what needs the private members of the views in static member
 * functions of the iterators. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_JOIN_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_JOIN_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <concepts>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <__fcxx/addressof.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/pack.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_adaptors.h>
#include <__fcxx/ranges_base.h>
#include <__fcxx/ranges_core.h>
#include <__fcxx/ranges_factories.h>
#include <__fcxx/ranges_util.h>
#if _FCXX_STD >= 2026
#  include <variant>
#endif

namespace std {
namespace ranges {

/* Calls a function with the index of the alternative of a variant, as an
 * integral_constant, which the iterators that hold variants of iterators
 * use to do what the standard does with visit. */
template <size_t _Ip, size_t _Np, class _Fp>
constexpr decltype(auto) __fcxx_visit_index(size_t __i, _Fp&& __f) {
    if constexpr (_Ip + 1 == _Np) {
        return std::forward<_Fp>(__f)(integral_constant<size_t, _Ip>());
    } else {
        if (__i == _Ip) {
            return std::forward<_Fp>(__f)(integral_constant<size_t, _Ip>());
        }
        return ranges::__fcxx_visit_index<_Ip + 1, _Np>(__i, std::forward<_Fp>(__f));
    }
}

/* [range.join.iterator], the iterator category of the iterator of
 * join_view. */
template <class _Base, bool _RefIsGlvalue>
struct __fcxx_join_category {};

template <class _Base>
    requires forward_range<_Base> && forward_range<range_reference_t<_Base> >
struct __fcxx_join_category<_Base, true> {
private:
    typedef typename iterator_traits<iterator_t<_Base> >::iterator_category _OuterC;
    typedef typename iterator_traits<iterator_t<range_reference_t<_Base> > >::iterator_category _InnerC;

public:
    using iterator_category = conditional_t<
        derived_from<_OuterC, bidirectional_iterator_tag> && derived_from<_InnerC, bidirectional_iterator_tag>
            && common_range<range_reference_t<_Base> >,
        bidirectional_iterator_tag,
        conditional_t<derived_from<_OuterC, forward_iterator_tag> && derived_from<_InnerC, forward_iterator_tag>,
                      forward_iterator_tag, input_iterator_tag> >;
};

/* [range.join.view], class template join_view. It joins the ranges of
 * prvalue ranges too, as P2328 has it, and caches the outer iterator of the
 * input ranges in itself, as P2770 has it.
 *
 * Workaround: Clang 12 to 15, all targets: the constraints of the members
 * that name the types of the const view are concepts of their own, here and
 * in join_with_view, as these compilers substitute into the whole
 * requires-clause of a non-template member of a class template when they
 * instantiate the class, where the types would not be valid (see
 * filter_view in ranges_adaptors.h). */
template <class _Vp>
concept __fcxx_join_const_iterable = forward_range<const _Vp> && is_reference_v<range_reference_t<const _Vp> >
                                  && input_range<range_reference_t<const _Vp> >;

template <input_range _Vp>
    requires view<_Vp> && input_range<range_reference_t<_Vp> >
class join_view : public view_interface<join_view<_Vp> > {
    using _InnerRng = range_reference_t<_Vp>;

    template <bool _Const>
    class __sentinel;

    /* [range.join.iterator], class template join_view::iterator */
    template <bool _Const>
    class __iterator : public __fcxx_join_category<__fcxx_maybe_const<_Const, _Vp>,
                                                   is_reference_v<range_reference_t<__fcxx_maybe_const<_Const, _Vp>>>> {
        friend join_view;

        template <bool>
        friend class __iterator;

        using _Parent = __fcxx_maybe_const<_Const, join_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;
        using _OuterIter = iterator_t<_Base>;
        using _InnerIter = iterator_t<range_reference_t<_Base> >;

        static constexpr bool __ref_is_glvalue = is_reference_v<range_reference_t<_Base> >;

        using _OuterStore = conditional_t<forward_range<_Base>, _OuterIter, __fcxx_no_cache>;

        _FCXX_NO_UNIQUE_ADDRESS _OuterStore __outer_ = _OuterStore();
        __fcxx_optional_value<_InnerIter> __inner_;
        _Parent* __parent_ = nullptr;

        constexpr _OuterIter& __outer() {
            if constexpr (forward_range<_Base>) {
                return __outer_;
            } else {
                return *__parent_->__outer_;
            }
        }

        constexpr decltype(auto) __update_inner(const iterator_t<_Base>& __x) {
            if constexpr (__ref_is_glvalue) {
                return *__x;
            } else {
                return __parent_->__inner_.__emplace_deref(__x);
            }
        }

        constexpr void __satisfy() {
            for (; __outer() != ranges::end(__parent_->__base_); ++__outer()) {
                auto&& __inner = __update_inner(__outer());
                __inner_ = ranges::begin(__inner);
                if (*__inner_ != ranges::end(__inner)) {
                    return;
                }
            }
            if constexpr (__ref_is_glvalue) {
                __inner_.reset();
            }
        }

        constexpr __iterator(_Parent& __parent, _OuterIter __outer)
            requires forward_range<_Base>
            : __outer_(std::move(__outer)), __parent_(std::addressof(__parent)) {
            __satisfy();
        }

        constexpr explicit __iterator(_Parent& __parent)
            requires(!forward_range<_Base>)
            : __parent_(std::addressof(__parent)) {
            __satisfy();
        }

    public:
        using iterator_concept = conditional_t<
            __ref_is_glvalue && bidirectional_range<_Base> && bidirectional_range<range_reference_t<_Base> >
                && common_range<range_reference_t<_Base> >,
            bidirectional_iterator_tag,
            conditional_t<__ref_is_glvalue && forward_range<_Base> && forward_range<range_reference_t<_Base> >,
                          forward_iterator_tag, input_iterator_tag> >;
        using value_type = range_value_t<range_reference_t<_Base> >;
        using difference_type = common_type_t<range_difference_t<_Base>, range_difference_t<range_reference_t<_Base>>>;

        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, _OuterIter>
                  && convertible_to<iterator_t<_InnerRng>, _InnerIter>
            : __outer_(std::move(__i.__outer_)), __inner_(std::move(__i.__inner_)), __parent_(__i.__parent_) {}

        /* outer(), for the sentinel. */
        constexpr const _OuterIter& __fcxx_outer() const {
            if constexpr (forward_range<_Base>) {
                return __outer_;
            } else {
                return *__parent_->__outer_;
            }
        }

        constexpr decltype(auto) operator*() const { return **__inner_; }

        constexpr _InnerIter operator->() const
            requires __fcxx_has_arrow<_InnerIter> && copyable<_InnerIter>
        {
            return *__inner_;
        }

        constexpr __iterator& operator++() {
            if constexpr (__ref_is_glvalue) {
                if (++*__inner_ == ranges::end(ranges::__fcxx_as_lvalue(*__outer()))) {
                    ++__outer();
                    __satisfy();
                }
            } else {
                if (++*__inner_ == ranges::end(ranges::__fcxx_as_lvalue(*__parent_->__inner_))) {
                    ++__outer();
                    __satisfy();
                }
            }
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        constexpr __iterator operator++(int)
            requires __ref_is_glvalue && forward_range<_Base> && forward_range<range_reference_t<_Base> >
        {
            auto __tmp = *this;
            ++*this;
            return __tmp;
        }

        constexpr __iterator& operator--()
            requires __ref_is_glvalue && bidirectional_range<_Base> && bidirectional_range<range_reference_t<_Base> >
                  && common_range<range_reference_t<_Base> >
        {
            if (__outer_ == ranges::end(__parent_->__base_)) {
                __inner_ = ranges::end(ranges::__fcxx_as_lvalue(*--__outer_));
            }
            while (*__inner_ == ranges::begin(ranges::__fcxx_as_lvalue(*__outer_))) {
                *__inner_ = ranges::end(ranges::__fcxx_as_lvalue(*--__outer_));
            }
            --*__inner_;
            return *this;
        }

        constexpr __iterator operator--(int)
            requires __ref_is_glvalue && bidirectional_range<_Base> && bidirectional_range<range_reference_t<_Base> >
                  && common_range<range_reference_t<_Base> >
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires __ref_is_glvalue
            && forward_range<_Base> && equality_comparable<iterator_t<range_reference_t<_Base>>>
        {
            return __x.__outer_ == __y.__outer_ && __x.__inner_ == __y.__inner_;
        }

        friend constexpr decltype(auto) iter_move(const __iterator& __i) noexcept(
            noexcept(ranges::iter_move(*__i.__inner_))) {
            return ranges::iter_move(*__i.__inner_);
        }

        friend constexpr void iter_swap(const __iterator& __x, const __iterator& __y) noexcept(
            noexcept(ranges::iter_swap(*__x.__inner_, *__y.__inner_)))
            requires indirectly_swappable<_InnerIter>
        {
            ranges::iter_swap(*__x.__inner_, *__y.__inner_);
        }
    };

    /* [range.join.sentinel], class template join_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend join_view;

        template <bool>
        friend class __sentinel;

        using _Parent = __fcxx_maybe_const<_Const, join_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        sentinel_t<_Base> __end_ = sentinel_t<_Base>();

        constexpr explicit __sentinel(_Parent& __parent) : __end_(ranges::end(__parent.__base_)) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __s)
            requires _Const && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __end_(std::move(__s.__end_)) {}

        template <bool _OtherConst>
            requires sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.__fcxx_outer() == __y.__end_;
        }
    };

    _Vp __base_ = _Vp();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<!forward_range<_Vp>, iterator_t<_Vp> > __outer_;
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<!is_reference_v<_InnerRng>, remove_cv_t<_InnerRng> > __inner_;

public:
    join_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit join_view(_Vp __base) : __base_(std::move(__base)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin() {
        if constexpr (forward_range<_Vp>) {
            constexpr bool __use_const = __fcxx_simple_view<_Vp> && is_reference_v<_InnerRng>;
            return __iterator<__use_const>(*this, ranges::begin(__base_));
        } else {
            __outer_ = ranges::begin(__base_);
            return __iterator<false>(*this);
        }
    }

    constexpr auto begin() const
        requires __fcxx_join_const_iterable<_Vp>
    {
        return __iterator<true>(*this, ranges::begin(__base_));
    }

    constexpr auto end() {
        if constexpr (forward_range<_Vp> && is_reference_v<_InnerRng> && forward_range<_InnerRng> && common_range<_Vp>
                      && common_range<_InnerRng>) {
            return __iterator<__fcxx_simple_view<_Vp> >(*this, ranges::end(__base_));
        } else {
            return __sentinel<__fcxx_simple_view<_Vp> >(*this);
        }
    }

    constexpr auto end() const
        requires __fcxx_join_const_iterable<_Vp>
    {
        if constexpr (forward_range<range_reference_t<const _Vp> > && common_range<const _Vp>
                      && common_range<range_reference_t<const _Vp> >) {
            return __iterator<true>(*this, ranges::end(__base_));
        } else {
            return __sentinel<true>(*this);
        }
    }
};

template <class _Rp>
explicit join_view(_Rp&&) -> join_view<views::all_t<_Rp> >;

namespace views {
struct __join_fn : __fcxx_adaptor_closure<__join_fn> {
    template <class _Rp>
        requires requires { join_view<all_t<_Rp> >{std::declval<_Rp>()}; }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(join_view<all_t<_Rp> >{std::forward<_Rp>(__r)})) {
        return join_view<all_t<_Rp> >{std::forward<_Rp>(__r)};
    }
};

inline constexpr __join_fn join{};
} /* namespace views */

/* The iterator of join_with_view holds an iterator into either the pattern
 * or an inner range, which [range.join.with.iterator] holds in a variant,
 * as a member for exposition only. They are held in a union of their own
 * here, as variant is only freestanding from C++26 on. As a variant, it
 * holds no object after the construction of one has thrown. */
template <class _Ap, class _Bp>
class __fcxx_either {
    union _Union {
        _Ap __a_;
        _Bp __b_;

        constexpr _Union() noexcept {}

        template <class... _Args>
        constexpr explicit _Union(in_place_index_t<0>, _Args&&... __args) : __a_(std::forward<_Args>(__args)...) {}

        template <class... _Args>
        constexpr explicit _Union(in_place_index_t<1>, _Args&&... __args) : __b_(std::forward<_Args>(__args)...) {}

        constexpr ~_Union() {}
    };

    static constexpr unsigned char __none = 2;

    _Union __u_;
    unsigned char __index_;

    constexpr void __destroy() noexcept {
        if (__index_ == 0) {
            std::destroy_at(std::addressof(__u_.__a_));
        } else if (__index_ == 1) {
            std::destroy_at(std::addressof(__u_.__b_));
        }
        __index_ = __none;
    }

    /* Constructs the object at an index, where there is none, with the
     * constructor of the whole union, as variant does. */
    template <size_t _Ip, class... _Args>
    constexpr void __construct(_Args&&... __args) {
        std::construct_at(std::addressof(__u_), in_place_index<_Ip>, std::forward<_Args>(__args)...);
        __index_ = static_cast<unsigned char>(_Ip);
    }

    template <class _Other>
    constexpr void __construct_from(_Other&& __other) {
        if (__other.__index_ == 0) {
            __construct<0>(std::forward<_Other>(__other).template __get<0>());
        } else if (__other.__index_ == 1) {
            __construct<1>(std::forward<_Other>(__other).template __get<1>());
        }
    }

    template <class _Other>
    constexpr void __assign_from(_Other&& __other) {
        if (__other.__index_ == __index_ && __index_ == 0) {
            __u_.__a_ = std::forward<_Other>(__other).template __get<0>();
        } else if (__other.__index_ == __index_ && __index_ == 1) {
            __u_.__b_ = std::forward<_Other>(__other).template __get<1>();
        } else {
            __destroy();
            __construct_from(std::forward<_Other>(__other));
        }
    }

public:
    constexpr __fcxx_either()
        requires default_initializable<_Ap>
        : __u_(in_place_index<0>), __index_(0) {}

    template <size_t _Ip, class... _Args>
    constexpr explicit __fcxx_either(in_place_index_t<_Ip> __tag, _Args&&... __args)
        : __u_(__tag, std::forward<_Args>(__args)...), __index_(static_cast<unsigned char>(_Ip)) {}

    constexpr __fcxx_either(const __fcxx_either& __other)
        requires copy_constructible<_Ap> && copy_constructible<_Bp>
        : __index_(__none) {
        __construct_from(__other);
    }

    constexpr __fcxx_either(__fcxx_either&& __other) noexcept(is_nothrow_move_constructible_v<_Ap>
                                                               && is_nothrow_move_constructible_v<_Bp>)
        : __index_(__none) {
        __construct_from(std::move(__other));
    }

    constexpr __fcxx_either& operator=(const __fcxx_either& __other)
        requires copy_constructible<_Ap> && copy_constructible<_Bp> && assignable_from<_Ap&, const _Ap&>
                 && assignable_from<_Bp&, const _Bp&>
    {
        if (this != std::addressof(__other)) {
            __assign_from(__other);
        }
        return *this;
    }

    constexpr __fcxx_either& operator=(__fcxx_either&& __other) noexcept(
        is_nothrow_move_constructible_v<_Ap> && is_nothrow_move_constructible_v<_Bp>
        && is_nothrow_move_assignable_v<_Ap> && is_nothrow_move_assignable_v<_Bp>) {
        if (this != std::addressof(__other)) {
            __assign_from(std::move(__other));
        }
        return *this;
    }

    constexpr ~__fcxx_either() { __destroy(); }

    constexpr size_t index() const noexcept { return __index_; }

    template <size_t _Ip, class... _Args>
    constexpr void emplace(_Args&&... __args) {
        __destroy();
        __construct<_Ip>(std::forward<_Args>(__args)...);
    }

    template <size_t _Ip>
    constexpr auto& __get() & noexcept {
        if constexpr (_Ip == 0) {
            return __u_.__a_;
        } else {
            return __u_.__b_;
        }
    }

    template <size_t _Ip>
    constexpr const auto& __get() const& noexcept {
        if constexpr (_Ip == 0) {
            return __u_.__a_;
        } else {
            return __u_.__b_;
        }
    }

    template <size_t _Ip>
    constexpr auto&& __get() && noexcept {
        return std::move(__get<_Ip>());
    }

    /* Equal where they hold objects of the same index that are equal, or
     * neither holds one, as variants are. */
    friend constexpr bool operator==(const __fcxx_either& __x, const __fcxx_either& __y)
        requires equality_comparable<_Ap> && equality_comparable<_Bp>
    {
        /* Workaround: GCC 11 and 12, all targets: where it cannot tell what
         * a copy of one holds, it warns in the code of the caller that the
         * object of the copy may be used uninitialized (-Wmaybe-uninitialized)
         * unless the index of each one is checked before its object is read,
         * so both are. */
        if (__x.__index_ == 0) {
            return __y.__index_ == 0 && static_cast<bool>(__x.__u_.__a_ == __y.__u_.__a_);
        }
        if (__x.__index_ == 1) {
            return __y.__index_ == 1 && static_cast<bool>(__x.__u_.__b_ == __y.__u_.__b_);
        }
        return __y.__index_ == __none;
    }
};

/* The object that an either holds. */
template <size_t _Ip, class _Ap, class _Bp>
constexpr auto& __fcxx_alt(__fcxx_either<_Ap, _Bp>& __v) noexcept {
    return __v.template __get<_Ip>();
}

template <size_t _Ip, class _Ap, class _Bp>
constexpr const auto& __fcxx_alt(const __fcxx_either<_Ap, _Bp>& __v) noexcept {
    return __v.template __get<_Ip>();
}

template <size_t _Ip, class _Ap, class _Bp>
constexpr auto&& __fcxx_alt(__fcxx_either<_Ap, _Bp>&& __v) noexcept {
    return std::move(__v).template __get<_Ip>();
}

#  if _FCXX_STD >= 2026
/* The alternative that a variant holds, as std::get gives it, but without
 * the exception std::get has for another one, which makes std::get deleted
 * in freestanding code. */
template <size_t _Ip, class... _Ts>
constexpr variant_alternative_t<_Ip, variant<_Ts...> >& __fcxx_alt(variant<_Ts...>& __v) noexcept {
    return *std::get_if<_Ip>(std::addressof(__v));
}

template <size_t _Ip, class... _Ts>
constexpr const variant_alternative_t<_Ip, variant<_Ts...> >& __fcxx_alt(const variant<_Ts...>& __v) noexcept {
    return *std::get_if<_Ip>(std::addressof(__v));
}

template <size_t _Ip, class... _Ts>
constexpr variant_alternative_t<_Ip, variant<_Ts...> >&& __fcxx_alt(variant<_Ts...>&& __v) noexcept {
    return std::move(*std::get_if<_Ip>(std::addressof(__v)));
}
#  endif

/* [range.concat.view], the concepts of the ranges that can be concatenated,
 * which join_with_view uses too, as LWG 4074 has it. */
template <class... _Rs>
using __fcxx_concat_reference_t = common_reference_t<range_reference_t<_Rs>...>;

template <class... _Rs>
using __fcxx_concat_value_t = common_type_t<range_value_t<_Rs>...>;

template <class... _Rs>
using __fcxx_concat_rvalue_reference_t = common_reference_t<range_rvalue_reference_t<_Rs>...>;

template <class _Ref, class _RRef, class _It>
concept __fcxx_concat_indirectly_readable_impl = requires(const _It __it) {
    { *__it } -> convertible_to<_Ref>;
    { ranges::iter_move(__it) } -> convertible_to<_RRef>;
};

template <class... _Rs>
concept __fcxx_concat_indirectly_readable =
    common_reference_with<__fcxx_concat_reference_t<_Rs...>&&, __fcxx_concat_value_t<_Rs...>&>
    && common_reference_with<__fcxx_concat_reference_t<_Rs...>&&, __fcxx_concat_rvalue_reference_t<_Rs...>&&>
    && common_reference_with<__fcxx_concat_rvalue_reference_t<_Rs...>&&, const __fcxx_concat_value_t<_Rs...>&>
    && (__fcxx_concat_indirectly_readable_impl<__fcxx_concat_reference_t<_Rs...>,
                                               __fcxx_concat_rvalue_reference_t<_Rs...>, iterator_t<_Rs> >
        && ...);

template <class... _Rs>
concept __fcxx_concatable = requires {
    typename __fcxx_concat_reference_t<_Rs...>;
    typename __fcxx_concat_value_t<_Rs...>;
    typename __fcxx_concat_rvalue_reference_t<_Rs...>;
} && __fcxx_concat_indirectly_readable<_Rs...>;

/* [range.join.with.iterator], the iterator category of the iterator of
 * join_with_view. */
template <class _Rp>
concept __fcxx_bidirectional_common = bidirectional_range<_Rp> && common_range<_Rp>;

template <class _Base, class _PatternBase, bool _RefIsGlvalue>
struct __fcxx_join_with_category {};

template <class _Base, class _PatternBase>
    requires forward_range<_Base> && forward_range<range_reference_t<_Base> >
struct __fcxx_join_with_category<_Base, _PatternBase, true> {
private:
    typedef range_reference_t<_Base> _InnerBase;
    typedef typename iterator_traits<iterator_t<_Base> >::iterator_category _OuterC;
    typedef typename iterator_traits<iterator_t<_InnerBase> >::iterator_category _InnerC;
    typedef typename iterator_traits<iterator_t<_PatternBase> >::iterator_category _PatternC;

public:
    using iterator_category = conditional_t<
        !is_reference_v<
            common_reference_t<iter_reference_t<iterator_t<_InnerBase>>, iter_reference_t<iterator_t<_PatternBase>>>>,
        input_iterator_tag,
        conditional_t<
            derived_from<_OuterC, bidirectional_iterator_tag> && derived_from<_InnerC, bidirectional_iterator_tag>
                && derived_from<_PatternC, bidirectional_iterator_tag> && common_range<_InnerBase>
                && common_range<_PatternBase>,
            bidirectional_iterator_tag,
            conditional_t<derived_from<_OuterC, forward_iterator_tag> && derived_from<_InnerC, forward_iterator_tag>
                              && derived_from<_PatternC, forward_iterator_tag>,
                          forward_iterator_tag, input_iterator_tag>>>;
};

/* [range.join.with.view], class template join_with_view.
 *
 * Workaround: Clang 12 to 15, all targets: the constraints of the members of
 * the const view are a concept of their own (see join_view). */
template <class _Vp, class _Pattern>
concept __fcxx_join_with_const_iterable = forward_range<const _Vp> && forward_range<const _Pattern>
                                       && is_reference_v<range_reference_t<const _Vp> >
                                       && input_range<range_reference_t<const _Vp> >
                                       && __fcxx_concatable<range_reference_t<const _Vp>, const _Pattern>;

template <input_range _Vp, forward_range _Pattern>
    requires view<_Vp> && input_range<range_reference_t<_Vp> > && view<_Pattern>
          && __fcxx_concatable<range_reference_t<_Vp>, _Pattern>
class join_with_view : public view_interface<join_with_view<_Vp, _Pattern> > {
    using _InnerRng = range_reference_t<_Vp>;

    template <bool _Const>
    class __sentinel;

    /* [range.join.with.iterator], class template join_with_view::iterator */
    template <bool _Const>
    class __iterator
        : public __fcxx_join_with_category<__fcxx_maybe_const<_Const, _Vp>, __fcxx_maybe_const<_Const, _Pattern>,
                                           is_reference_v<range_reference_t<__fcxx_maybe_const<_Const, _Vp> > > > {
        friend join_with_view;

        template <bool>
        friend class __iterator;

        using _Parent = __fcxx_maybe_const<_Const, join_with_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;
        using _InnerBase = range_reference_t<_Base>;
        using _PatternBase = __fcxx_maybe_const<_Const, _Pattern>;

        using _OuterIter = iterator_t<_Base>;
        using _InnerIter = iterator_t<_InnerBase>;
        using _PatternIter = iterator_t<_PatternBase>;

        static constexpr bool __ref_is_glvalue = is_reference_v<_InnerBase>;

        using _OuterStore = conditional_t<forward_range<_Base>, _OuterIter, __fcxx_no_cache>;

        _Parent* __parent_ = nullptr;
        _FCXX_NO_UNIQUE_ADDRESS _OuterStore __outer_it_ = _OuterStore();
        __fcxx_either<_PatternIter, _InnerIter> __inner_it_;

        constexpr _OuterIter& __outer() {
            if constexpr (forward_range<_Base>) {
                return __outer_it_;
            } else {
                return *__parent_->__outer_it_;
            }
        }

        constexpr auto& __update_inner() {
            if constexpr (__ref_is_glvalue) {
                return ranges::__fcxx_as_lvalue(*__outer());
            } else {
                return __parent_->__inner_.__emplace_deref(__outer());
            }
        }

        constexpr auto& __get_inner() {
            if constexpr (__ref_is_glvalue) {
                return ranges::__fcxx_as_lvalue(*__outer());
            } else {
                return *__parent_->__inner_;
            }
        }

        constexpr void __satisfy() {
            while (true) {
                if (__inner_it_.index() == 0) {
                    if (ranges::__fcxx_alt<0>(__inner_it_) != ranges::end(__parent_->__pattern_)) {
                        break;
                    }
                    __inner_it_.template emplace<1>(ranges::begin(__update_inner()));
                } else {
                    if (ranges::__fcxx_alt<1>(__inner_it_) != ranges::end(__get_inner())) {
                        break;
                    }
                    if (++__outer() == ranges::end(__parent_->__base_)) {
                        if constexpr (__ref_is_glvalue) {
                            __inner_it_.template emplace<0>();
                        }
                        break;
                    }
                    __inner_it_.template emplace<0>(ranges::begin(__parent_->__pattern_));
                }
            }
        }

        constexpr void __start() {
            if (__outer() != ranges::end(__parent_->__base_)) {
                __inner_it_.template emplace<1>(ranges::begin(__update_inner()));
                __satisfy();
            }
        }

        constexpr __iterator(_Parent& __parent, _OuterIter __outer)
            requires forward_range<_Base>
            : __parent_(std::addressof(__parent)), __outer_it_(std::move(__outer)) {
            __start();
        }

        constexpr explicit __iterator(_Parent& __parent)
            requires(!forward_range<_Base>)
            : __parent_(std::addressof(__parent)) {
            __start();
        }

        using __ref_t = common_reference_t<iter_reference_t<_InnerIter>, iter_reference_t<_PatternIter> >;
        using __rref_t =
            common_reference_t<iter_rvalue_reference_t<_InnerIter>, iter_rvalue_reference_t<_PatternIter> >;

        template <class _Var>
        static constexpr __fcxx_either<_PatternIter, _InnerIter> __convert(_Var&& __v) {
            if (__v.index() == 0) {
                return __fcxx_either<_PatternIter, _InnerIter>(in_place_index<0>,
                                                               ranges::__fcxx_alt<0>(std::move(__v)));
            } else {
                return __fcxx_either<_PatternIter, _InnerIter>(in_place_index<1>,
                                                               ranges::__fcxx_alt<1>(std::move(__v)));
            }
        }

    public:
        using iterator_concept =
            conditional_t<__ref_is_glvalue && bidirectional_range<_Base> && __fcxx_bidirectional_common<_InnerBase>
                              && __fcxx_bidirectional_common<_PatternBase>,
                          bidirectional_iterator_tag,
                          conditional_t<__ref_is_glvalue && forward_range<_Base> && forward_range<_InnerBase>,
                                        forward_iterator_tag, input_iterator_tag> >;
        using value_type = common_type_t<iter_value_t<_InnerIter>, iter_value_t<_PatternIter> >;
        using difference_type = common_type_t<iter_difference_t<_OuterIter>, iter_difference_t<_InnerIter>,
                                              iter_difference_t<_PatternIter>>;

        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, _OuterIter>
                  && convertible_to<iterator_t<_InnerRng>, _InnerIter>
                  && convertible_to<iterator_t<_Pattern>, _PatternIter>
            : __parent_(__i.__parent_), __outer_it_(std::move(__i.__outer_it_)),
              __inner_it_(__convert(std::move(__i.__inner_it_))) {}

        /* outer(), for the sentinel. */
        constexpr const _OuterIter& __fcxx_outer() const {
            if constexpr (forward_range<_Base>) {
                return __outer_it_;
            } else {
                return *__parent_->__outer_it_;
            }
        }

        /* The elements are converted implicitly, as the return of the
         * function visit calls. */
        constexpr __ref_t operator*() const {
            if (__inner_it_.index() == 0) {
                return *ranges::__fcxx_alt<0>(__inner_it_);
            }
            return *ranges::__fcxx_alt<1>(__inner_it_);
        }

        constexpr __iterator& operator++() {
            if (__inner_it_.index() == 0) {
                ++ranges::__fcxx_alt<0>(__inner_it_);
            } else {
                ++ranges::__fcxx_alt<1>(__inner_it_);
            }
            __satisfy();
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        constexpr __iterator operator++(int)
            requires __ref_is_glvalue && forward_iterator<_OuterIter> && forward_iterator<_InnerIter>
        {
            __iterator __tmp = *this;
            ++*this;
            return __tmp;
        }

        constexpr __iterator& operator--()
            requires __ref_is_glvalue && bidirectional_range<_Base> && __fcxx_bidirectional_common<_InnerBase>
                  && __fcxx_bidirectional_common<_PatternBase>
        {
            if (__outer_it_ == ranges::end(__parent_->__base_)) {
                auto&& __inner = *--__outer_it_;
                __inner_it_.template emplace<1>(ranges::end(__inner));
            }
            while (true) {
                if (__inner_it_.index() == 0) {
                    auto& __it = ranges::__fcxx_alt<0>(__inner_it_);
                    if (__it == ranges::begin(__parent_->__pattern_)) {
                        auto&& __inner = *--__outer_it_;
                        __inner_it_.template emplace<1>(ranges::end(__inner));
                    } else {
                        break;
                    }
                } else {
                    auto& __it = ranges::__fcxx_alt<1>(__inner_it_);
                    auto&& __inner = *__outer_it_;
                    if (__it == ranges::begin(__inner)) {
                        __inner_it_.template emplace<0>(ranges::end(__parent_->__pattern_));
                    } else {
                        break;
                    }
                }
            }
            if (__inner_it_.index() == 0) {
                --ranges::__fcxx_alt<0>(__inner_it_);
            } else {
                --ranges::__fcxx_alt<1>(__inner_it_);
            }
            return *this;
        }

        constexpr __iterator operator--(int)
            requires __ref_is_glvalue && bidirectional_range<_Base> && __fcxx_bidirectional_common<_InnerBase>
                  && __fcxx_bidirectional_common<_PatternBase>
        {
            __iterator __tmp = *this;
            --*this;
            return __tmp;
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires __ref_is_glvalue && forward_range<_Base> && equality_comparable<_InnerIter>
        {
            return __x.__outer_it_ == __y.__outer_it_ && __x.__inner_it_ == __y.__inner_it_;
        }

        friend constexpr __rref_t iter_move(const __iterator& __x) {
            if (__x.__inner_it_.index() == 0) {
                return ranges::iter_move(ranges::__fcxx_alt<0>(__x.__inner_it_));
            }
            return ranges::iter_move(ranges::__fcxx_alt<1>(__x.__inner_it_));
        }

        friend constexpr void iter_swap(const __iterator& __x, const __iterator& __y)
            requires indirectly_swappable<_InnerIter, _PatternIter>
        {
            if (__x.__inner_it_.index() == 0) {
                if (__y.__inner_it_.index() == 0) {
                    ranges::iter_swap(ranges::__fcxx_alt<0>(__x.__inner_it_), ranges::__fcxx_alt<0>(__y.__inner_it_));
                } else {
                    ranges::iter_swap(ranges::__fcxx_alt<0>(__x.__inner_it_), ranges::__fcxx_alt<1>(__y.__inner_it_));
                }
            } else {
                if (__y.__inner_it_.index() == 0) {
                    ranges::iter_swap(ranges::__fcxx_alt<1>(__x.__inner_it_), ranges::__fcxx_alt<0>(__y.__inner_it_));
                } else {
                    ranges::iter_swap(ranges::__fcxx_alt<1>(__x.__inner_it_), ranges::__fcxx_alt<1>(__y.__inner_it_));
                }
            }
        }
    };

    /* [range.join.with.sentinel], class template join_with_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend join_with_view;

        template <bool>
        friend class __sentinel;

        using _Parent = __fcxx_maybe_const<_Const, join_with_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        sentinel_t<_Base> __end_ = sentinel_t<_Base>();

        constexpr explicit __sentinel(_Parent& __parent) : __end_(ranges::end(__parent.__base_)) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __s)
            requires _Const && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __end_(std::move(__s.__end_)) {}

        template <bool _OtherConst>
            requires sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.__fcxx_outer() == __y.__end_;
        }
    };

    _Vp __base_ = _Vp();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<!forward_range<_Vp>, iterator_t<_Vp> > __outer_it_;
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<!is_reference_v<_InnerRng>, remove_cv_t<_InnerRng> > __inner_;
    _Pattern __pattern_ = _Pattern();

public:
    join_with_view()
        requires default_initializable<_Vp> && default_initializable<_Pattern>
    = default;

    constexpr explicit join_with_view(_Vp __base, _Pattern __pattern)
        : __base_(std::move(__base)), __pattern_(std::move(__pattern)) {}

    template <input_range _Rp>
        requires constructible_from<_Vp, views::all_t<_Rp> >
              && constructible_from<_Pattern, single_view<range_value_t<_InnerRng> > >
    constexpr explicit join_with_view(_Rp&& __r, range_value_t<_InnerRng> __e)
        : __base_(views::all(std::forward<_Rp>(__r))), __pattern_(views::single(std::move(__e))) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin() {
        if constexpr (forward_range<_Vp>) {
            constexpr bool __use_const =
                __fcxx_simple_view<_Vp> && is_reference_v<_InnerRng> && __fcxx_simple_view<_Pattern>;
            return __iterator<__use_const>(*this, ranges::begin(__base_));
        } else {
            __outer_it_ = ranges::begin(__base_);
            return __iterator<false>(*this);
        }
    }

    constexpr auto begin() const
        requires __fcxx_join_with_const_iterable<_Vp, _Pattern>
    {
        return __iterator<true>(*this, ranges::begin(__base_));
    }

    constexpr auto end() {
        if constexpr (forward_range<_Vp> && is_reference_v<_InnerRng> && forward_range<_InnerRng> && common_range<_Vp>
                      && common_range<_InnerRng>) {
            return __iterator<__fcxx_simple_view<_Vp> && __fcxx_simple_view<_Pattern> >(*this, ranges::end(__base_));
        } else {
            return __sentinel<__fcxx_simple_view<_Vp> && __fcxx_simple_view<_Pattern> >(*this);
        }
    }

    constexpr auto end() const
        requires __fcxx_join_with_const_iterable<_Vp, _Pattern>
    {
        using _InnerConstRng = range_reference_t<const _Vp>;
        if constexpr (forward_range<_InnerConstRng> && common_range<const _Vp> && common_range<_InnerConstRng>) {
            return __iterator<true>(*this, ranges::end(__base_));
        } else {
            return __sentinel<true>(*this);
        }
    }
};

template <class _Rp, class _Pp>
join_with_view(_Rp&&, _Pp&&) -> join_with_view<views::all_t<_Rp>, views::all_t<_Pp> >;

template <input_range _Rp>
join_with_view(_Rp&&, range_value_t<range_reference_t<_Rp> >)
    -> join_with_view<views::all_t<_Rp>, single_view<range_value_t<range_reference_t<_Rp> > > >;

namespace views {
struct __join_with_fn {
    template <class _Rp, class _Pp>
        requires requires { join_with_view(std::declval<_Rp>(), std::declval<_Pp>()); }
    constexpr auto operator()(_Rp&& __r, _Pp&& __p) const
        noexcept(noexcept(join_with_view(std::forward<_Rp>(__r), std::forward<_Pp>(__p)))) {
        return join_with_view(std::forward<_Rp>(__r), std::forward<_Pp>(__p));
    }

    template <class _Pp>
        requires __fcxx_can_bind<__join_with_fn, _Pp>
    constexpr auto operator()(_Pp&& __p) const noexcept(is_nothrow_constructible_v<decay_t<_Pp>, _Pp>) {
        return ranges::__fcxx_bind_adaptor<__join_with_fn>(std::forward<_Pp>(__p));
    }
};

inline constexpr __join_with_fn join_with{};
} /* namespace views */

/* [range.lazy.split.view], tiny-range */
template <auto>
struct __fcxx_require_constant;

template <class _Rp>
concept __fcxx_tiny_range = sized_range<_Rp>
                         && requires { typename __fcxx_require_constant<remove_reference_t<_Rp>::size()>; }
                         && (remove_reference_t<_Rp>::size() <= 1);

/* The iterator categories of the iterators of lazy_split_view. */
template <bool _Forward>
struct __fcxx_lazy_split_outer_category {};

template <>
struct __fcxx_lazy_split_outer_category<true> {
    using iterator_category = input_iterator_tag;
};

template <class _Base>
struct __fcxx_lazy_split_inner_category {};

template <forward_range _Base>
struct __fcxx_lazy_split_inner_category<_Base> {
private:
    typedef typename iterator_traits<iterator_t<_Base> >::iterator_category _Cp;

public:
    using iterator_category = conditional_t<derived_from<_Cp, forward_iterator_tag>, forward_iterator_tag, _Cp>;
};

/* [range.lazy.split.view], class template lazy_split_view. The outer
 * iterator's conversion copies trailing_empty_, as LWG 3904 has it, and
 * the value type of the outer iterator cannot be default-initialized, as
 * in C++26. */
template <input_range _Vp, forward_range _Pattern>
    requires view<_Vp> && view<_Pattern>
          && indirectly_comparable<iterator_t<_Vp>, iterator_t<_Pattern>, ranges::equal_to>
          && (forward_range<_Vp> || __fcxx_tiny_range<_Pattern>)
class lazy_split_view : public view_interface<lazy_split_view<_Vp, _Pattern> > {
    template <bool _Const>
    class __inner_iterator;

    /* [range.lazy.split.outer], class template
     * lazy_split_view::outer-iterator */
    template <bool _Const>
    class __outer_iterator : public __fcxx_lazy_split_outer_category<forward_range<__fcxx_maybe_const<_Const, _Vp> > > {
        friend lazy_split_view;

        template <bool>
        friend class __outer_iterator;

        using _Parent = __fcxx_maybe_const<_Const, lazy_split_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;
        using _CurrentStore = conditional_t<forward_range<_Vp>, iterator_t<_Base>, __fcxx_no_cache>;

        _Parent* __parent_ = nullptr;
        _FCXX_NO_UNIQUE_ADDRESS _CurrentStore __current_ = _CurrentStore();
        bool __trailing_empty_ = false;

        constexpr explicit __outer_iterator(_Parent& __parent)
            requires(!forward_range<_Base>)
            : __parent_(std::addressof(__parent)) {}

        constexpr __outer_iterator(_Parent& __parent, iterator_t<_Base> __current)
            requires forward_range<_Base>
            : __parent_(std::addressof(__parent)), __current_(std::move(__current)) {}

        static constexpr bool __at_end(const __outer_iterator& __x) {
            return __x.__fcxx_cur() == ranges::end(__x.__parent_->__base_) && !__x.__trailing_empty_;
        }

    public:
        using iterator_concept = conditional_t<forward_range<_Base>, forward_iterator_tag, input_iterator_tag>;

        /* [range.lazy.split.outer.value], class
         * lazy_split_view::outer-iterator::value_type */
        struct value_type : view_interface<value_type> {
        private:
            friend __outer_iterator;

            __outer_iterator __i_ = __outer_iterator();

            constexpr explicit value_type(__outer_iterator __i) : __i_(std::move(__i)) {}

        public:
            constexpr __inner_iterator<_Const> begin() const { return lazy_split_view::__fcxx_make_inner(__i_); }

            constexpr default_sentinel_t end() const noexcept { return default_sentinel; }
        };

        using difference_type = range_difference_t<_Base>;

        __outer_iterator() = default;

        constexpr __outer_iterator(__outer_iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base>>
            : __parent_(__i.__parent_), __current_(std::move(__i.__current_)),
              __trailing_empty_(__i.__trailing_empty_) {}

        /* current, and what the inner iterator needs. */
        constexpr iterator_t<_Base>& __fcxx_cur() noexcept {
            if constexpr (forward_range<_Vp>) {
                return __current_;
            } else {
                return *__parent_->__current_;
            }
        }

        constexpr const iterator_t<_Base>& __fcxx_cur() const noexcept {
            if constexpr (forward_range<_Vp>) {
                return __current_;
            } else {
                return *__parent_->__current_;
            }
        }

        constexpr _Parent* __fcxx_parent() const noexcept { return __parent_; }

        constexpr value_type operator*() const { return value_type{*this}; }

        constexpr __outer_iterator& operator++() {
            const auto __end = ranges::end(__parent_->__base_);
            if (__fcxx_cur() == __end) {
                __trailing_empty_ = false;
                return *this;
            }
            const auto __pbegin = ranges::begin(__parent_->__pattern_);
            const auto __pend = ranges::end(__parent_->__pattern_);
            if (__pbegin == __pend) {
                ++__fcxx_cur();
            } else if constexpr (__fcxx_tiny_range<_Pattern>) {
                __fcxx_cur() = ranges::__fcxx_find(std::move(__fcxx_cur()), __end, *__pbegin);
                if (__fcxx_cur() != __end) {
                    ++__fcxx_cur();
                    if (__fcxx_cur() == __end) {
                        __trailing_empty_ = true;
                    }
                }
            } else {
                do {
                    auto __r = ranges::__fcxx_mismatch(__fcxx_cur(), __end, __pbegin, __pend);
                    if (__r.__in2 == __pend) {
                        __fcxx_cur() = __r.__in1;
                        if (__fcxx_cur() == __end) {
                            __trailing_empty_ = true;
                        }
                        break;
                    }
                } while (++__fcxx_cur() != __end);
            }
            return *this;
        }

        constexpr decltype(auto) operator++(int) {
            if constexpr (forward_range<_Base>) {
                auto __tmp = *this;
                ++*this;
                return __tmp;
            } else {
                ++*this;
            }
        }

        friend constexpr bool operator==(const __outer_iterator& __x, const __outer_iterator& __y)
            requires forward_range<_Base>
        {
            return __x.__current_ == __y.__current_ && __x.__trailing_empty_ == __y.__trailing_empty_;
        }

        friend constexpr bool operator==(const __outer_iterator& __x, default_sentinel_t) { return __at_end(__x); }
    };

    /* [range.lazy.split.inner], class template
     * lazy_split_view::inner-iterator */
    template <bool _Const>
    class __inner_iterator : public __fcxx_lazy_split_inner_category<__fcxx_maybe_const<_Const, _Vp> > {
        friend lazy_split_view;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        __outer_iterator<_Const> __i_ = __outer_iterator<_Const>();
        bool __incremented_ = false;

        constexpr explicit __inner_iterator(__outer_iterator<_Const> __i) : __i_(std::move(__i)) {}

        static constexpr bool __at_end(const __inner_iterator& __x) {
            auto __pcur = ranges::begin(__x.__i_.__fcxx_parent()->__pattern_);
            auto __pend = ranges::end(__x.__i_.__fcxx_parent()->__pattern_);
            auto __end = ranges::end(__x.__i_.__fcxx_parent()->__base_);
            if constexpr (__fcxx_tiny_range<_Pattern>) {
                const auto& __cur = __x.__i_.__fcxx_cur();
                if (__cur == __end) {
                    return true;
                }
                if (__pcur == __pend) {
                    return __x.__incremented_;
                }
                return *__cur == *__pcur;
            } else {
                auto __cur = __x.__i_.__fcxx_cur();
                if (__cur == __end) {
                    return true;
                }
                if (__pcur == __pend) {
                    return __x.__incremented_;
                }
                do {
                    if (*__cur != *__pcur) {
                        return false;
                    }
                    if (++__pcur == __pend) {
                        return true;
                    }
                } while (++__cur != __end);
                return false;
            }
        }

    public:
        using iterator_concept = typename __outer_iterator<_Const>::iterator_concept;
        using value_type = range_value_t<_Base>;
        using difference_type = range_difference_t<_Base>;

        __inner_iterator() = default;

        constexpr const iterator_t<_Base>& base() const& noexcept { return __i_.__fcxx_cur(); }

        constexpr iterator_t<_Base> base() &&
            requires forward_range<_Vp>
        {
            return std::move(__i_.__fcxx_cur());
        }

        constexpr decltype(auto) operator*() const { return *__i_.__fcxx_cur(); }

        constexpr __inner_iterator& operator++() {
            __incremented_ = true;
            if constexpr (!forward_range<_Base>) {
                if constexpr (_Pattern::size() == 0) {
                    return *this;
                }
            }
            ++__i_.__fcxx_cur();
            return *this;
        }

        constexpr decltype(auto) operator++(int) {
            if constexpr (forward_range<_Base>) {
                auto __tmp = *this;
                ++*this;
                return __tmp;
            } else {
                ++*this;
            }
        }

        friend constexpr bool operator==(const __inner_iterator& __x, const __inner_iterator& __y)
            requires forward_range<_Base>
        {
            return __x.__i_.__fcxx_cur() == __y.__i_.__fcxx_cur();
        }

        friend constexpr bool operator==(const __inner_iterator& __x, default_sentinel_t) { return __at_end(__x); }

        friend constexpr decltype(auto) iter_move(const __inner_iterator& __i) noexcept(
            noexcept(ranges::iter_move(__i.__i_.__fcxx_cur()))) {
            return ranges::iter_move(__i.__i_.__fcxx_cur());
        }

        friend constexpr void iter_swap(const __inner_iterator& __x, const __inner_iterator& __y) noexcept(
            noexcept(ranges::iter_swap(__x.__i_.__fcxx_cur(), __y.__i_.__fcxx_cur())))
            requires indirectly_swappable<iterator_t<_Base> >
        {
            ranges::iter_swap(__x.__i_.__fcxx_cur(), __y.__i_.__fcxx_cur());
        }
    };

    template <bool _Const>
    static constexpr __inner_iterator<_Const> __fcxx_make_inner(const __outer_iterator<_Const>& __i) {
        return __inner_iterator<_Const>(__i);
    }

    _Vp __base_ = _Vp();
    _Pattern __pattern_ = _Pattern();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<!forward_range<_Vp>, iterator_t<_Vp> > __current_;

public:
    lazy_split_view()
        requires default_initializable<_Vp> && default_initializable<_Pattern>
    = default;

    constexpr explicit lazy_split_view(_Vp __base, _Pattern __pattern)
        : __base_(std::move(__base)), __pattern_(std::move(__pattern)) {}

    template <input_range _Rp>
        requires constructible_from<_Vp, views::all_t<_Rp>>
                     && constructible_from<_Pattern, single_view<range_value_t<_Rp>>>
    constexpr explicit lazy_split_view(_Rp&& __r, range_value_t<_Rp> __e)
        : __base_(views::all(std::forward<_Rp>(__r))), __pattern_(views::single(std::move(__e))) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin() {
        if constexpr (forward_range<_Vp>) {
            return __outer_iterator<__fcxx_simple_view<_Vp> && __fcxx_simple_view<_Pattern> >{*this,
                                                                                            ranges::begin(__base_)};
        } else {
            __current_ = ranges::begin(__base_);
            return __outer_iterator<false>{*this};
        }
    }

    constexpr auto begin() const
        requires forward_range<_Vp> && forward_range<const _Vp> && forward_range<const _Pattern>
    {
        return __outer_iterator<true>{*this, ranges::begin(__base_)};
    }

    constexpr auto end()
        requires forward_range<_Vp> && common_range<_Vp>
    {
        return __outer_iterator<__fcxx_simple_view<_Vp> && __fcxx_simple_view<_Pattern> >{*this, ranges::end(__base_)};
    }

    constexpr auto end() const {
        if constexpr (forward_range<_Vp> && forward_range<const _Vp> && common_range<const _Vp>
                      && forward_range<const _Pattern>) {
            return __outer_iterator<true>{*this, ranges::end(__base_)};
        } else {
            return default_sentinel;
        }
    }
};

template <class _Rp, class _Pp>
lazy_split_view(_Rp&&, _Pp&&) -> lazy_split_view<views::all_t<_Rp>, views::all_t<_Pp> >;

template <input_range _Rp>
lazy_split_view(_Rp&&, range_value_t<_Rp>) -> lazy_split_view<views::all_t<_Rp>, single_view<range_value_t<_Rp> > >;

/* [range.split.view], class template split_view. Its begin() caches where
 * the first subrange ends. */
template <forward_range _Vp, forward_range _Pattern>
    requires view<_Vp> && view<_Pattern>
          && indirectly_comparable<iterator_t<_Vp>, iterator_t<_Pattern>, ranges::equal_to>
class split_view : public view_interface<split_view<_Vp, _Pattern> > {
    /* [range.split.iterator], class split_view::iterator */
    class __iterator {
        friend split_view;

        split_view* __parent_ = nullptr;
        iterator_t<_Vp> __cur_ = iterator_t<_Vp>();
        subrange<iterator_t<_Vp> > __next_ = subrange<iterator_t<_Vp> >();
        bool __trailing_empty_ = false;

        constexpr __iterator(split_view& __parent, iterator_t<_Vp> __current, subrange<iterator_t<_Vp> > __next)
            : __parent_(std::addressof(__parent)), __cur_(std::move(__current)), __next_(std::move(__next)) {}

    public:
        using iterator_concept = forward_iterator_tag;
        using iterator_category = input_iterator_tag;
        using value_type = subrange<iterator_t<_Vp> >;
        using difference_type = range_difference_t<_Vp>;

        __iterator() = default;

        constexpr iterator_t<_Vp> base() const { return __cur_; }

        constexpr bool __fcxx_trailing_empty() const noexcept { return __trailing_empty_; }

        constexpr value_type operator*() const { return {__cur_, __next_.begin()}; }

        constexpr __iterator& operator++() {
            __cur_ = __next_.begin();
            if (__cur_ != ranges::end(__parent_->__base_)) {
                __cur_ = __next_.end();
                if (__cur_ == ranges::end(__parent_->__base_)) {
                    __trailing_empty_ = true;
                    __next_ = {__cur_, __cur_};
                } else {
                    __next_ = __parent_->__find_next(__cur_);
                }
            } else {
                __trailing_empty_ = false;
            }
            return *this;
        }

        constexpr __iterator operator++(int) {
            auto __tmp = *this;
            ++*this;
            return __tmp;
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y) {
            return __x.__cur_ == __y.__cur_ && __x.__trailing_empty_ == __y.__trailing_empty_;
        }
    };

    /* [range.split.sentinel], class split_view::sentinel */
    class __sentinel {
        friend split_view;

        sentinel_t<_Vp> __end_ = sentinel_t<_Vp>();

        constexpr explicit __sentinel(split_view& __parent) : __end_(ranges::end(__parent.__base_)) {}

    public:
        __sentinel() = default;

        friend constexpr bool operator==(const __iterator& __x, const __sentinel& __y) {
            return __x.base() == __y.__end_ && !__x.__fcxx_trailing_empty();
        }
    };

    _Vp __base_ = _Vp();
    _Pattern __pattern_ = _Pattern();
    __fcxx_non_propagating_cache<subrange<iterator_t<_Vp> > > __next_;

    /* find-next */
    constexpr subrange<iterator_t<_Vp> > __find_next(iterator_t<_Vp> __it) {
        auto __found = ranges::__fcxx_search(std::move(__it), ranges::end(__base_), ranges::begin(__pattern_),
                                             ranges::end(__pattern_));
        auto __b = __found.begin();
        auto __e = __found.end();
        if (__b != ranges::end(__base_) && ranges::empty(__pattern_)) {
            ++__b;
            ++__e;
        }
        return {__b, __e};
    }

public:
    split_view()
        requires default_initializable<_Vp> && default_initializable<_Pattern>
    = default;

    constexpr explicit split_view(_Vp __base, _Pattern __pattern)
        : __base_(std::move(__base)), __pattern_(std::move(__pattern)) {}

    template <forward_range _Rp>
        requires constructible_from<_Vp, views::all_t<_Rp>>
                     && constructible_from<_Pattern, single_view<range_value_t<_Rp>>>
    constexpr explicit split_view(_Rp&& __r, range_value_t<_Rp> __e)
        : __base_(views::all(std::forward<_Rp>(__r))), __pattern_(views::single(std::move(__e))) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr __iterator begin() {
        if (!__next_.has_value()) {
            __next_ = __find_next(ranges::begin(__base_));
        }
        return __iterator(*this, ranges::begin(__base_), *__next_);
    }

    constexpr auto end() {
        if constexpr (common_range<_Vp>) {
            return __iterator(*this, ranges::end(__base_), {});
        } else {
            return __sentinel(*this);
        }
    }
};

template <class _Rp, class _Pp>
split_view(_Rp&&, _Pp&&) -> split_view<views::all_t<_Rp>, views::all_t<_Pp> >;

template <forward_range _Rp>
split_view(_Rp&&, range_value_t<_Rp>) -> split_view<views::all_t<_Rp>, single_view<range_value_t<_Rp> > >;

namespace views {
struct __lazy_split_fn {
    template <class _Rp, class _Pp>
        requires requires { lazy_split_view(std::declval<_Rp>(), std::declval<_Pp>()); }
    constexpr auto operator()(_Rp&& __r, _Pp&& __p) const
        noexcept(noexcept(lazy_split_view(std::forward<_Rp>(__r), std::forward<_Pp>(__p)))) {
        return lazy_split_view(std::forward<_Rp>(__r), std::forward<_Pp>(__p));
    }

    template <class _Pp>
        requires __fcxx_can_bind<__lazy_split_fn, _Pp>
    constexpr auto operator()(_Pp&& __p) const noexcept(is_nothrow_constructible_v<decay_t<_Pp>, _Pp>) {
        return ranges::__fcxx_bind_adaptor<__lazy_split_fn>(std::forward<_Pp>(__p));
    }
};

inline constexpr __lazy_split_fn lazy_split{};

struct __split_fn {
    template <class _Rp, class _Pp>
        requires requires { split_view(std::declval<_Rp>(), std::declval<_Pp>()); }
    constexpr auto operator()(_Rp&& __r, _Pp&& __p) const
        noexcept(noexcept(split_view(std::forward<_Rp>(__r), std::forward<_Pp>(__p)))) {
        return split_view(std::forward<_Rp>(__r), std::forward<_Pp>(__p));
    }

    template <class _Pp>
        requires __fcxx_can_bind<__split_fn, _Pp>
    constexpr auto operator()(_Pp&& __p) const noexcept(is_nothrow_constructible_v<decay_t<_Pp>, _Pp>) {
        return ranges::__fcxx_bind_adaptor<__split_fn>(std::forward<_Pp>(__p));
    }
};

inline constexpr __split_fn split{};
} /* namespace views */

#  if _FCXX_STD >= 2026
/* [range.concat.view], the concepts of the iterators of concat_view, whose
 * ranges but the last have to be common ranges to go backwards. */
template <bool _Const, class... _Rs>
struct __fcxx_concat_all_but_last_common;

template <bool _Const, class _Rp>
struct __fcxx_concat_all_but_last_common<_Const, _Rp> : true_type {};

template <bool _Const, class _Rp, class... _Rest>
struct __fcxx_concat_all_but_last_common<_Const, _Rp, _Rest...>
    : bool_constant<common_range<__fcxx_maybe_const<_Const, _Rp> >
                    && __fcxx_concat_all_but_last_common<_Const, _Rest...>::value> {};

template <bool _Const, class... _Rs>
concept __fcxx_concat_is_random_access =
    __fcxx_all_random_access<_Const, _Rs...> && __fcxx_concat_all_but_last_common<_Const, _Rs...>::value;

template <bool _Const, class... _Rs>
concept __fcxx_concat_is_bidirectional =
    __fcxx_all_bidirectional<_Const, _Rs...> && __fcxx_concat_all_but_last_common<_Const, _Rs...>::value;

/* The ranges but the first are sized, for the distance to the end. */
template <bool _Const, class _First, class... _Rest>
struct __fcxx_concat_rest_sized_impl : bool_constant<(sized_range<__fcxx_maybe_const<_Const, _Rest> > && ...)> {};

template <bool _Const, class... _Rs>
concept __fcxx_concat_rest_sized = __fcxx_concat_rest_sized_impl<_Const, _Rs...>::value;

template <bool _Const, class... _Views>
struct __fcxx_concat_category {};

template <bool _Const, class... _Views>
    requires __fcxx_all_forward<_Const, _Views...>
struct __fcxx_concat_category<_Const, _Views...> {
private:
    template <class _Tag>
    static constexpr bool __all_derived =
        (derived_from<typename iterator_traits<iterator_t<__fcxx_maybe_const<_Const, _Views> > >::iterator_category,
                      _Tag>
         && ...);

public:
    using iterator_category = conditional_t<
        !is_reference_v<__fcxx_concat_reference_t<__fcxx_maybe_const<_Const, _Views>...> >, input_iterator_tag,
        conditional_t<__all_derived<random_access_iterator_tag> && __fcxx_concat_is_random_access<_Const, _Views...>,
                      random_access_iterator_tag,
                      conditional_t<__all_derived<bidirectional_iterator_tag>
                                        && __fcxx_concat_is_bidirectional<_Const, _Views...>,
                                    bidirectional_iterator_tag,
                                    conditional_t<__all_derived<forward_iterator_tag>, forward_iterator_tag,
                                                  input_iterator_tag> > > >;
};

/* [range.concat.view], class template concat_view */
template <input_range... _Views>
    requires(view<_Views> && ...) && (sizeof...(_Views) > 0) && __fcxx_concatable<_Views...>
class concat_view : public view_interface<concat_view<_Views...> > {
    static constexpr size_t _Np = sizeof...(_Views);

    template <size_t _Ip>
    using _View = __fcxx_type_at_t<_Ip, _Views...>;

    /* [range.concat.iterator], class concat_view::iterator */
    template <bool _Const>
    class __iterator : public __fcxx_concat_category<_Const, _Views...> {
        friend concat_view;

        template <bool>
        friend class __iterator;

    public:
        using iterator_concept =
            conditional_t<__fcxx_concat_is_random_access<_Const, _Views...>, random_access_iterator_tag,
                          conditional_t<__fcxx_concat_is_bidirectional<_Const, _Views...>, bidirectional_iterator_tag,
                                        conditional_t<__fcxx_all_forward<_Const, _Views...>, forward_iterator_tag,
                                                      input_iterator_tag> > >;
        using value_type = __fcxx_concat_value_t<__fcxx_maybe_const<_Const, _Views>...>;
        using difference_type = common_type_t<range_difference_t<__fcxx_maybe_const<_Const, _Views> >...>;

    private:
        using __base_iter = variant<iterator_t<__fcxx_maybe_const<_Const, _Views> >...>;
        using _Parent = __fcxx_maybe_const<_Const, concat_view>;
        using __ref_t = __fcxx_concat_reference_t<__fcxx_maybe_const<_Const, _Views>...>;
        using __rref_t = __fcxx_concat_rvalue_reference_t<__fcxx_maybe_const<_Const, _Views>...>;

        template <size_t _Ip>
        using _It = iterator_t<__fcxx_maybe_const<_Const, _View<_Ip> > >;

        _Parent* __parent_ = nullptr;
        __base_iter __it_;

        template <size_t _Ip>
        constexpr void __satisfy() {
            if constexpr (_Ip < _Np - 1) {
                if (ranges::__fcxx_alt<_Ip>(__it_) == ranges::end(std::get<_Ip>(__parent_->__views_))) {
                    __it_.template emplace<_Ip + 1>(ranges::begin(std::get<_Ip + 1>(__parent_->__views_)));
                    __satisfy<_Ip + 1>();
                }
            }
        }

        template <size_t _Ip>
        constexpr void __prev() {
            if constexpr (_Ip == 0) {
                --ranges::__fcxx_alt<0>(__it_);
            } else {
                if (ranges::__fcxx_alt<_Ip>(__it_) == ranges::begin(std::get<_Ip>(__parent_->__views_))) {
                    __it_.template emplace<_Ip - 1>(ranges::end(std::get<_Ip - 1>(__parent_->__views_)));
                    __prev<_Ip - 1>();
                } else {
                    --ranges::__fcxx_alt<_Ip>(__it_);
                }
            }
        }

        template <size_t _Ip>
        constexpr void __advance_fwd(difference_type __offset, difference_type __steps) {
            using _Ud = iter_difference_t<_It<_Ip> >;
            if constexpr (_Ip == _Np - 1) {
                ranges::__fcxx_alt<_Ip>(__it_) += static_cast<_Ud>(__steps);
            } else {
                auto __n_size = ranges::distance(std::get<_Ip>(__parent_->__views_));
                if (__offset + __steps < __n_size) {
                    ranges::__fcxx_alt<_Ip>(__it_) += static_cast<_Ud>(__steps);
                } else {
                    __it_.template emplace<_Ip + 1>(ranges::begin(std::get<_Ip + 1>(__parent_->__views_)));
                    __advance_fwd<_Ip + 1>(0, __offset + __steps - __n_size);
                }
            }
        }

        template <size_t _Ip>
        constexpr void __advance_bwd(difference_type __offset, difference_type __steps) {
            using _Ud = iter_difference_t<_It<_Ip> >;
            if constexpr (_Ip == 0) {
                ranges::__fcxx_alt<_Ip>(__it_) -= static_cast<_Ud>(__steps);
            } else {
                if (__offset >= __steps) {
                    ranges::__fcxx_alt<_Ip>(__it_) -= static_cast<_Ud>(__steps);
                } else {
                    auto __prev_size = ranges::distance(std::get<_Ip - 1>(__parent_->__views_));
                    __it_.template emplace<_Ip - 1>(ranges::end(std::get<_Ip - 1>(__parent_->__views_)));
                    __advance_bwd<_Ip - 1>(__prev_size, __steps - __offset);
                }
            }
        }

        template <class... _Args>
            requires constructible_from<__base_iter, _Args&&...>
        constexpr explicit __iterator(_Parent* __parent, _Args&&... __args)
            : __parent_(__parent), __it_(std::forward<_Args>(__args)...) {}

        template <class _Var>
        static constexpr __base_iter __convert(_Var&& __v) {
            return ranges::__fcxx_visit_index<0, _Np>(__v.index(), [&](auto __i) {
                return __base_iter(in_place_index<decltype(__i)::value>,
                                   ranges::__fcxx_alt<decltype(__i)::value>(std::move(__v)));
            });
        }

        /* The size of the ranges from the first to the last, before it. */
        template <size_t _First, size_t _Last>
        constexpr difference_type __sizes() const {
            if constexpr (_First >= _Last) {
                return 0;
            } else {
                return static_cast<difference_type>(ranges::size(std::get<_First>(__parent_->__views_)))
                     + __sizes<_First + 1, _Last>();
            }
        }

        static constexpr bool __at_end(const __iterator& __it) {
            constexpr size_t __last_idx = _Np - 1;
            return __it.__it_.index() == __last_idx
                && ranges::__fcxx_alt<__last_idx>(__it.__it_)
                == ranges::end(std::get<__last_idx>(__it.__parent_->__views_));
        }

        static constexpr difference_type __distance(const __iterator& __x, const __iterator& __y) {
            const size_t __ix = __x.__it_.index();
            const size_t __iy = __y.__it_.index();
            if (__ix > __iy) {
                return ranges::__fcxx_visit_index<0, _Np>(__iy, [&](auto __j) {
                    constexpr size_t __jy = decltype(__j)::value;
                    const difference_type __dy = ranges::distance(
                        ranges::__fcxx_alt<__jy>(__y.__it_), ranges::end(std::get<__jy>(__y.__parent_->__views_)));
                    return ranges::__fcxx_visit_index<0, _Np>(__ix, [&](auto __k) -> difference_type {
                        constexpr size_t __jx = decltype(__k)::value;
                        if constexpr (__jx > __jy) {
                            const difference_type __dx =
                                ranges::distance(ranges::begin(std::get<__jx>(__x.__parent_->__views_)),
                                                 ranges::__fcxx_alt<__jx>(__x.__it_));
                            return __dy + __x.template __sizes<__jy + 1, __jx>() + __dx;
                        } else {
                            _FCXX_UNREACHABLE();
                        }
                    });
                });
            } else if (__ix < __iy) {
                return -__distance(__y, __x);
            } else {
                return ranges::__fcxx_visit_index<0, _Np>(__ix, [&](auto __j) -> difference_type {
                    return ranges::__fcxx_alt<decltype(__j)::value>(__x.__it_)
                        - ranges::__fcxx_alt<decltype(__j)::value>(__y.__it_);
                });
            }
        }

        static constexpr difference_type __distance_to_end(const __iterator& __x) {
            return ranges::__fcxx_visit_index<0, _Np>(__x.__it_.index(), [&](auto __j) -> difference_type {
                constexpr size_t __jx = decltype(__j)::value;
                const difference_type __dx = ranges::distance(ranges::__fcxx_alt<__jx>(__x.__it_),
                                                              ranges::end(std::get<__jx>(__x.__parent_->__views_)));
                return -(__dx + __x.template __sizes<__jx + 1, _Np>());
            });
        }

    public:
        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __it)
            requires _Const
                         && (convertible_to<iterator_t<_Views>, iterator_t<__fcxx_maybe_const<_Const, _Views>>> && ...)
            : __parent_(__it.__parent_), __it_(__convert(std::move(__it.__it_))) {}

        constexpr decltype(auto) operator*() const {
            return ranges::__fcxx_visit_index<0, _Np>(__it_.index(), [&](auto __i) -> __ref_t {
                return *ranges::__fcxx_alt<decltype(__i)::value>(__it_);
            });
        }

        constexpr __iterator& operator++() {
            ranges::__fcxx_visit_index<0, _Np>(__it_.index(), [&](auto __i) {
                ++ranges::__fcxx_alt<decltype(__i)::value>(__it_);
                this->template __satisfy<decltype(__i)::value>();
            });
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        constexpr __iterator operator++(int)
            requires __fcxx_all_forward<_Const, _Views...>
        {
            auto __tmp = *this;
            ++*this;
            return __tmp;
        }

        constexpr __iterator& operator--()
            requires __fcxx_concat_is_bidirectional<_Const, _Views...>
        {
            ranges::__fcxx_visit_index<0, _Np>(__it_.index(), [&](auto __i) {
                this->template __prev<decltype(__i)::value>();
            });
            return *this;
        }

        constexpr __iterator operator--(int)
            requires __fcxx_concat_is_bidirectional<_Const, _Views...>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        constexpr __iterator& operator+=(difference_type __n)
            requires __fcxx_concat_is_random_access<_Const, _Views...>
        {
            ranges::__fcxx_visit_index<0, _Np>(__it_.index(), [&](auto __i) {
                constexpr size_t __j = decltype(__i)::value;
                const difference_type __offset =
                    ranges::__fcxx_alt<__j>(__it_) - ranges::begin(std::get<__j>(__parent_->__views_));
                if (__n > 0) {
                    this->template __advance_fwd<__j>(__offset, __n);
                } else if (__n < 0) {
                    this->template __advance_bwd<__j>(__offset, -__n);
                }
            });
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __n)
            requires __fcxx_concat_is_random_access<_Const, _Views...>
        {
            *this += -__n;
            return *this;
        }

        constexpr decltype(auto) operator[](difference_type __n) const
            requires __fcxx_concat_is_random_access<_Const, _Views...>
        {
            return *((*this) + __n);
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires(equality_comparable<iterator_t<__fcxx_maybe_const<_Const, _Views> > > && ...)
        {
            return __x.__it_ == __y.__it_;
        }

        friend constexpr bool operator==(const __iterator& __it, default_sentinel_t) { return __at_end(__it); }

        friend constexpr bool operator<(const __iterator& __x, const __iterator& __y)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            return __x.__it_ < __y.__it_;
        }

        friend constexpr bool operator>(const __iterator& __x, const __iterator& __y)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            return __x.__it_ > __y.__it_;
        }

        friend constexpr bool operator<=(const __iterator& __x, const __iterator& __y)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            return __x.__it_ <= __y.__it_;
        }

        friend constexpr bool operator>=(const __iterator& __x, const __iterator& __y)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            return __x.__it_ >= __y.__it_;
        }

        friend constexpr auto operator<=>(const __iterator& __x, const __iterator& __y)
            requires __fcxx_all_random_access<_Const, _Views...>
                  && (three_way_comparable<iterator_t<__fcxx_maybe_const<_Const, _Views> > > && ...)
        {
            return __x.__it_ <=> __y.__it_;
        }

        friend constexpr __iterator operator+(const __iterator& __it, difference_type __n)
            requires __fcxx_concat_is_random_access<_Const, _Views...>
        {
            auto __temp = __it;
            __temp += __n;
            return __temp;
        }

        friend constexpr __iterator operator+(difference_type __n, const __iterator& __it)
            requires __fcxx_concat_is_random_access<_Const, _Views...>
        {
            return __it + __n;
        }

        friend constexpr __iterator operator-(const __iterator& __it, difference_type __n)
            requires __fcxx_concat_is_random_access<_Const, _Views...>
        {
            auto __temp = __it;
            __temp -= __n;
            return __temp;
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y)
            requires __fcxx_concat_is_random_access<_Const, _Views...>
        {
            return __distance(__x, __y);
        }

        friend constexpr difference_type operator-(const __iterator& __x, default_sentinel_t)
            requires(sized_sentinel_for<sentinel_t<__fcxx_maybe_const<_Const, _Views> >,
                                        iterator_t<__fcxx_maybe_const<_Const, _Views> > >
                     && ...)
                 && __fcxx_concat_rest_sized<_Const, _Views...>
        {
            return __distance_to_end(__x);
        }

        friend constexpr difference_type operator-(default_sentinel_t, const __iterator& __x)
            requires(sized_sentinel_for<sentinel_t<__fcxx_maybe_const<_Const, _Views> >,
                                        iterator_t<__fcxx_maybe_const<_Const, _Views> > >
                     && ...)
                 && __fcxx_concat_rest_sized<_Const, _Views...>
        {
            return -__distance_to_end(__x);
        }

        friend constexpr decltype(auto) iter_move(const __iterator& __it) noexcept(
            ((is_nothrow_invocable_v<decltype(ranges::iter_move), const iterator_t<__fcxx_maybe_const<_Const, _Views>>&>
              && is_nothrow_convertible_v<range_rvalue_reference_t<__fcxx_maybe_const<_Const, _Views>>, __rref_t>)
             && ...)) {
            return ranges::__fcxx_visit_index<0, _Np>(__it.__it_.index(), [&](auto __i) -> __rref_t {
                return ranges::iter_move(ranges::__fcxx_alt<decltype(__i)::value>(__it.__it_));
            });
        }

        friend constexpr void iter_swap(const __iterator& __x, const __iterator& __y) noexcept(
            noexcept(ranges::swap(*__x, *__y))
            && (noexcept(ranges::iter_swap(std::declval<const iterator_t<__fcxx_maybe_const<_Const, _Views> >&>(),
                                           std::declval<const iterator_t<__fcxx_maybe_const<_Const, _Views> >&>()))
                && ...))
            requires swappable_with<__ref_t, __ref_t>
                  && (indirectly_swappable<iterator_t<__fcxx_maybe_const<_Const, _Views> > > && ...)
        {
            ranges::__fcxx_visit_index<0, _Np>(__x.__it_.index(), [&](auto __i) {
                ranges::__fcxx_visit_index<0, _Np>(__y.__it_.index(), [&](auto __j) {
                    const auto& __it1 = ranges::__fcxx_alt<decltype(__i)::value>(__x.__it_);
                    const auto& __it2 = ranges::__fcxx_alt<decltype(__j)::value>(__y.__it_);
                    if constexpr (is_same_v<decltype(__it1), decltype(__it2)>) {
                        ranges::iter_swap(__it1, __it2);
                    } else {
                        ranges::swap(*__x, *__y);
                    }
                });
            });
        }
    };

    tuple<_Views...> __views_;

    template <bool _Const, class _Self>
    static constexpr __iterator<_Const> __begin(_Self* __self) {
        __iterator<_Const> __it(__self, in_place_index<0>, ranges::begin(std::get<0>(__self->__views_)));
        __it.template __satisfy<0>();
        return __it;
    }

    template <bool _Const, class _Self>
    static constexpr auto __end(_Self* __self) {
        if constexpr (__fcxx_all_forward<_Const, _Views...>
                      && common_range<__fcxx_maybe_const<_Const, _View<_Np - 1>>>) {
            return __iterator<_Const>(__self, in_place_index<_Np - 1>,
                                      ranges::end(std::get<_Np - 1>(__self->__views_)));
        } else {
            return default_sentinel;
        }
    }

    template <class _Tuple, size_t... _Is>
    static constexpr auto __size(_Tuple& __views, index_sequence<_Is...>) {
        using _CT = __fcxx_make_unsigned_like_t<common_type_t<decltype(ranges::size(std::get<_Is>(__views)))...> >;
        return (_CT(ranges::size(std::get<_Is>(__views))) + ...);
    }

    template <class _Tuple, size_t... _Is>
    static constexpr auto __reserve_hint(_Tuple& __views, index_sequence<_Is...>) {
        using _CT =
            __fcxx_make_unsigned_like_t<common_type_t<decltype(ranges::reserve_hint(std::get<_Is>(__views)))...> >;
        return (_CT(ranges::reserve_hint(std::get<_Is>(__views))) + ...);
    }

public:
    constexpr concat_view() = default;

    constexpr explicit concat_view(_Views... __views) : __views_(std::move(__views)...) {}

    constexpr __iterator<false> begin()
        requires(!(__fcxx_simple_view<_Views> && ...))
    {
        return __begin<false>(this);
    }

    constexpr __iterator<true> begin() const
        requires(range<const _Views> && ...) && __fcxx_concatable<const _Views...>
    {
        return __begin<true>(this);
    }

    constexpr auto end()
        requires(!(__fcxx_simple_view<_Views> && ...))
    {
        return __end<false>(this);
    }

    constexpr auto end() const
        requires(range<const _Views> && ...) && __fcxx_concatable<const _Views...>
    {
        return __end<true>(this);
    }

    constexpr auto size()
        requires(sized_range<_Views> && ...)
    {
        return __size(__views_, index_sequence_for<_Views...>());
    }

    constexpr auto size() const
        requires(sized_range<const _Views> && ...)
    {
        return __size(__views_, index_sequence_for<_Views...>());
    }

    constexpr auto reserve_hint()
        requires(approximately_sized_range<_Views> && ...)
    {
        return __reserve_hint(__views_, index_sequence_for<_Views...>());
    }

    constexpr auto reserve_hint() const
        requires(approximately_sized_range<const _Views> && ...)
    {
        return __reserve_hint(__views_, index_sequence_for<_Views...>());
    }
};

template <class... _Rp>
concat_view(_Rp&&...) -> concat_view<views::all_t<_Rp>...>;

namespace views {
template <class... _Rs>
concept __fcxx_concat_single = sizeof...(_Rs) == 1 && (input_range<_Rs> && ...);

struct __concat_fn {
    template <class _Rp>
        requires input_range<_Rp> && requires { views::all(std::declval<_Rp>()); }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(views::all(std::forward<_Rp>(__r)))) {
        return views::all(std::forward<_Rp>(__r));
    }

    template <class... _Rs>
        requires(!__fcxx_concat_single<_Rs...>) && requires { concat_view(std::declval<_Rs>()...); }
    constexpr auto operator()(_Rs&&... __rs) const noexcept(noexcept(concat_view(std::forward<_Rs>(__rs)...))) {
        return concat_view(std::forward<_Rs>(__rs)...);
    }
};

inline constexpr __concat_fn concat{};
} /* namespace views */
#  endif

} /* namespace ranges */
} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
