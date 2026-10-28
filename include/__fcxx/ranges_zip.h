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


/* [range.zip], [range.zip.transform], [range.adjacent],
 * [range.adjacent.transform] and [range.cartesian], the views of <ranges>
 * of C++23 whose elements are tuples of elements of ranges, or made of
 * them: zip_view, zip_transform_view, adjacent_view,
 * adjacent_transform_view and cartesian_product_view. They need the tuples
 * of references of C++23, with their common reference types.
 *
 * Workaround: GCC 11 and 12, all targets: they do not give the hidden friends
 * of a class nested in another the access to the private members of the
 * enclosing class that the nested class has, which Clang and GCC 13 give
 * them. The hidden friends of the iterator of cartesian_product_view do what
 * needs the private members of the view in static member functions of the
 * iterator. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_ZIP_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_ZIP_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
#include <__fcxx/integer_product.h>

#include <compare>
#include <concepts>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <__fcxx/addressof.h>
#include <__fcxx/integer_sequence.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_adaptors.h>
#include <__fcxx/ranges_base.h>
#include <__fcxx/ranges_core.h>
#include <__fcxx/ranges_factories.h>
#include <__fcxx/ranges_util.h>

namespace std {
namespace ranges {

/* The value with the smallest absolute value among some differences, and
 * whether any of some comparisons are true, which zip_view gives for its
 * iterators and sentinels. */
template <class _Dp, class _Xs, class _Ys, size_t... _Is>
constexpr _Dp __fcxx_zip_min_distance(const _Xs& __x, const _Ys& __y, index_sequence<_Is...>) {
    _Dp __ds[] = {static_cast<_Dp>(std::get<_Is>(__x) - std::get<_Is>(__y))...};
    _Dp __best = __ds[0];
    for (size_t __i = 1; __i < sizeof...(_Is); ++__i) {
        const _Dp __abs_best = __best < 0 ? -__best : __best;
        const _Dp __abs_d = __ds[__i] < 0 ? -__ds[__i] : __ds[__i];
        if (__abs_d < __abs_best) {
            __best = __ds[__i];
        }
    }
    return __best;
}

template <class _Xs, class _Ys, size_t... _Is>
constexpr bool __fcxx_zip_any_equal(const _Xs& __x, const _Ys& __y, index_sequence<_Is...>) {
    return (static_cast<bool>(std::get<_Is>(__x) == std::get<_Is>(__y)) || ...);
}

/* [range.zip.view], zip-is-common */
template <class... _Rs>
concept __fcxx_zip_is_common = (sizeof...(_Rs) == 1 && (common_range<_Rs> && ...))
                            || (!(bidirectional_range<_Rs> && ...) && (common_range<_Rs> && ...))
                            || ((random_access_range<_Rs> && ...) && (sized_range<_Rs> && ...));

template <bool _Forward>
struct __fcxx_zip_category {};

template <>
struct __fcxx_zip_category<true> {
    using iterator_category = input_iterator_tag;
};

template <bool _Const, class... _Views>
using __fcxx_zip_concept =
    conditional_t<__fcxx_all_random_access<_Const, _Views...>, random_access_iterator_tag,
                  conditional_t<__fcxx_all_bidirectional<_Const, _Views...>, bidirectional_iterator_tag,
                                conditional_t<__fcxx_all_forward<_Const, _Views...>, forward_iterator_tag,
                                              input_iterator_tag> > >;

/* [range.zip.view], class template zip_view */
template <input_range... _Views>
    requires(view<_Views> && ...) && (sizeof...(_Views) > 0)
class zip_view : public view_interface<zip_view<_Views...> > {
    /* [range.zip.iterator], class template zip_view::iterator */
    template <bool _Const>
    class __iterator : public __fcxx_zip_category<__fcxx_all_forward<_Const, _Views...> > {
        friend zip_view;

        template <bool>
        friend class __iterator;

        typedef tuple<iterator_t<__fcxx_maybe_const<_Const, _Views> >...> _Current;
        typedef index_sequence_for<_Views...> _Seq;

        _Current __current_;

        constexpr explicit __iterator(_Current __current) : __current_(std::move(__current)) {}

        template <size_t... _Is>
        constexpr auto __deref(index_sequence<_Is...>) const {
            return tuple<iter_reference_t<iterator_t<__fcxx_maybe_const<_Const, _Views> > >...>(
                *std::get<_Is>(__current_)...);
        }

        template <class _Dp, size_t... _Is>
        constexpr auto __subscript(_Dp __n, index_sequence<_Is...>) const {
            return tuple<iter_reference_t<iterator_t<__fcxx_maybe_const<_Const, _Views> > >...>(
                std::get<_Is>(__current_)[iter_difference_t<iterator_t<__fcxx_maybe_const<_Const, _Views> > >(__n)]...);
        }

        template <class _Dp, size_t... _Is>
        constexpr void __advance(_Dp __n, index_sequence<_Is...>) {
            ((std::get<_Is>(__current_) += iter_difference_t<iterator_t<__fcxx_maybe_const<_Const, _Views>>>(__n)),
             ...);
        }

        template <class _Dp, size_t... _Is>
        constexpr void __retreat(_Dp __n, index_sequence<_Is...>) {
            ((std::get<_Is>(__current_) -= iter_difference_t<iterator_t<__fcxx_maybe_const<_Const, _Views>>>(__n)),
             ...);
        }

        template <size_t... _Is>
        static constexpr auto __iter_move(const _Current& __c, index_sequence<_Is...>) {
            return tuple<iter_rvalue_reference_t<iterator_t<__fcxx_maybe_const<_Const, _Views> > >...>(
                ranges::iter_move(std::get<_Is>(__c))...);
        }

        template <size_t... _Is>
        static constexpr void __iter_swap(const _Current& __l, const _Current& __r, index_sequence<_Is...>) {
            (ranges::iter_swap(std::get<_Is>(__l), std::get<_Is>(__r)), ...);
        }

    public:
        using iterator_concept = __fcxx_zip_concept<_Const, _Views...>;
        using value_type = tuple<range_value_t<__fcxx_maybe_const<_Const, _Views> >...>;
        using difference_type = common_type_t<range_difference_t<__fcxx_maybe_const<_Const, _Views> >...>;

        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const
            && (convertible_to<iterator_t<_Views>, iterator_t<__fcxx_maybe_const<_Const, _Views>>> && ...)
            : __current_(std::move(__i.__current_)) {}

        /* current_, for the sentinel and zip_transform_view. */
        constexpr const _Current& __fcxx_current() const noexcept { return __current_; }

        constexpr auto operator*() const { return __deref(_Seq()); }

        constexpr __iterator& operator++() {
            ranges::__fcxx_tuple_for_each([](auto& __i) { ++__i; }, __current_);
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
            requires __fcxx_all_bidirectional<_Const, _Views...>
        {
            ranges::__fcxx_tuple_for_each([](auto& __i) { --__i; }, __current_);
            return *this;
        }

        constexpr __iterator operator--(int)
            requires __fcxx_all_bidirectional<_Const, _Views...>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        constexpr __iterator& operator+=(difference_type __x)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            __advance(__x, _Seq());
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __x)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            __retreat(__x, _Seq());
            return *this;
        }

        constexpr auto operator[](difference_type __n) const
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            return __subscript(__n, _Seq());
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires(equality_comparable<iterator_t<__fcxx_maybe_const<_Const, _Views> > > && ...)
        {
            if constexpr (__fcxx_all_bidirectional<_Const, _Views...>) {
                return __x.__current_ == __y.__current_;
            } else {
                return ranges::__fcxx_zip_any_equal(__x.__current_, __y.__current_, _Seq());
            }
        }

        friend constexpr auto operator<=>(const __iterator& __x, const __iterator& __y)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            return __x.__current_ <=> __y.__current_;
        }

        friend constexpr __iterator operator+(const __iterator& __i, difference_type __n)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            auto __r = __i;
            __r += __n;
            return __r;
        }

        friend constexpr __iterator operator+(difference_type __n, const __iterator& __i)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            auto __r = __i;
            __r += __n;
            return __r;
        }

        friend constexpr __iterator operator-(const __iterator& __i, difference_type __n)
            requires __fcxx_all_random_access<_Const, _Views...>
        {
            auto __r = __i;
            __r -= __n;
            return __r;
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y)
            requires(sized_sentinel_for<iterator_t<__fcxx_maybe_const<_Const, _Views> >,
                                        iterator_t<__fcxx_maybe_const<_Const, _Views> > >
                     && ...)
        {
            return ranges::__fcxx_zip_min_distance<difference_type>(__x.__current_, __y.__current_, _Seq());
        }

        friend constexpr auto iter_move(const __iterator& __i) noexcept(
            (noexcept(ranges::iter_move(std::declval<const iterator_t<__fcxx_maybe_const<_Const, _Views> >&>())) && ...)
            && (is_nothrow_move_constructible_v<range_rvalue_reference_t<__fcxx_maybe_const<_Const, _Views> > >
                && ...)) {
            return __iter_move(__i.__current_, _Seq());
        }

        friend constexpr void iter_swap(const __iterator& __l, const __iterator& __r) noexcept(
            (noexcept(ranges::iter_swap(std::declval<const iterator_t<__fcxx_maybe_const<_Const, _Views> >&>(),
                                        std::declval<const iterator_t<__fcxx_maybe_const<_Const, _Views> >&>()))
             && ...))
            requires(indirectly_swappable<iterator_t<__fcxx_maybe_const<_Const, _Views> > > && ...)
        {
            __iter_swap(__l.__current_, __r.__current_, _Seq());
        }
    };

    /* [range.zip.sentinel], class template zip_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend zip_view;

        template <bool>
        friend class __sentinel;

        typedef tuple<sentinel_t<__fcxx_maybe_const<_Const, _Views> >...> _End;

        _End __end_;

        constexpr explicit __sentinel(_End __end) : __end_(__end) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __i)
            requires _Const
            && (convertible_to<sentinel_t<_Views>, sentinel_t<__fcxx_maybe_const<_Const, _Views>>> && ...)
            : __end_(std::move(__i.__end_)) {}

        template <bool _OtherConst>
            requires(sentinel_for<sentinel_t<__fcxx_maybe_const<_Const, _Views> >,
                                  iterator_t<__fcxx_maybe_const<_OtherConst, _Views> > >
                     && ...)
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return ranges::__fcxx_zip_any_equal(__x.__fcxx_current(), __y.__end_, index_sequence_for<_Views...>());
        }

        template <bool _OtherConst>
            requires(sized_sentinel_for<sentinel_t<__fcxx_maybe_const<_Const, _Views> >,
                                        iterator_t<__fcxx_maybe_const<_OtherConst, _Views> > >
                     && ...)
        friend constexpr common_type_t<range_difference_t<__fcxx_maybe_const<_OtherConst, _Views> >...> operator-(
            const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return ranges::__fcxx_zip_min_distance<
                common_type_t<range_difference_t<__fcxx_maybe_const<_OtherConst, _Views> >...> >(
                __x.__fcxx_current(), __y.__end_, index_sequence_for<_Views...>());
        }

        template <bool _OtherConst>
            requires(sized_sentinel_for<sentinel_t<__fcxx_maybe_const<_Const, _Views> >,
                                        iterator_t<__fcxx_maybe_const<_OtherConst, _Views> > >
                     && ...)
        friend constexpr common_type_t<range_difference_t<__fcxx_maybe_const<_OtherConst, _Views> >...> operator-(
            const __sentinel& __y, const __iterator<_OtherConst>& __x) {
            return -(__x - __y);
        }
    };

    tuple<_Views...> __views_;

    template <class _Tuple, size_t... _Is>
    static constexpr auto __size(_Tuple& __views, index_sequence<_Is...>) {
        using _CT = __fcxx_make_unsigned_like_t<common_type_t<decltype(ranges::size(std::get<_Is>(__views)))...> >;
        _CT __sizes[] = {_CT(ranges::size(std::get<_Is>(__views)))...};
        _CT __m = __sizes[0];
        for (size_t __i = 1; __i < sizeof...(_Is); ++__i) {
            if (__sizes[__i] < __m) {
                __m = __sizes[__i];
            }
        }
        return __m;
    }

public:
    zip_view() = default;

    constexpr explicit zip_view(_Views... __views) : __views_(std::move(__views)...) {}

    constexpr auto begin()
        requires(!(__fcxx_simple_view<_Views> && ...))
    {
        return __iterator<false>(ranges::__fcxx_tuple_transform(ranges::begin, __views_));
    }

    constexpr auto begin() const
        requires(range<const _Views> && ...)
    {
        return __iterator<true>(ranges::__fcxx_tuple_transform(ranges::begin, __views_));
    }

    constexpr auto end()
        requires(!(__fcxx_simple_view<_Views> && ...))
    {
        if constexpr (!__fcxx_zip_is_common<_Views...>) {
            return __sentinel<false>(ranges::__fcxx_tuple_transform(ranges::end, __views_));
        } else if constexpr ((random_access_range<_Views> && ...)) {
            return begin() + iter_difference_t<__iterator<false> >(size());
        } else {
            return __iterator<false>(ranges::__fcxx_tuple_transform(ranges::end, __views_));
        }
    }

    constexpr auto end() const
        requires(range<const _Views> && ...)
    {
        if constexpr (!__fcxx_zip_is_common<const _Views...>) {
            return __sentinel<true>(ranges::__fcxx_tuple_transform(ranges::end, __views_));
        } else if constexpr ((random_access_range<const _Views> && ...)) {
            return begin() + iter_difference_t<__iterator<true> >(size());
        } else {
            return __iterator<true>(ranges::__fcxx_tuple_transform(ranges::end, __views_));
        }
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
};

template <class... _Rs>
zip_view(_Rs&&...) -> zip_view<views::all_t<_Rs>...>;

template <class... _Views>
inline constexpr bool enable_borrowed_range<zip_view<_Views...> > = (enable_borrowed_range<_Views> && ...);

namespace views {
struct __zip_fn {
    constexpr auto operator()() const noexcept { return empty_view<tuple<> >(); }

    template <class... _Rs>
        requires(sizeof...(_Rs) > 0) && requires { zip_view<all_t<_Rs>...>(std::declval<_Rs>()...); }
    constexpr auto operator()(_Rs&&... __rs) const
        noexcept(noexcept(zip_view<all_t<_Rs>...>(std::forward<_Rs>(__rs)...))) {
        return zip_view<all_t<_Rs>...>(std::forward<_Rs>(__rs)...);
    }
};

inline constexpr __zip_fn zip{};
} /* namespace views */

/* [range.zip.transform.iterator], the iterator category of the iterator of
 * zip_transform_view. */
template <class _Fn, class _Base, class... _Bases>
struct __fcxx_zip_transform_category {};

template <class _Fn, class _Base, class... _Bases>
    requires forward_range<_Base>
struct __fcxx_zip_transform_category<_Fn, _Base, _Bases...> {
private:
    template <class _Tag>
    static constexpr bool __all_derived =
        (derived_from<typename iterator_traits<iterator_t<_Bases> >::iterator_category, _Tag> && ...);

public:
    using iterator_category = conditional_t<
        !is_reference_v<invoke_result_t<_Fn&, range_reference_t<_Bases>...> >, input_iterator_tag,
        conditional_t<__all_derived<random_access_iterator_tag>, random_access_iterator_tag,
                      conditional_t<__all_derived<bidirectional_iterator_tag>, bidirectional_iterator_tag,
                                    conditional_t<__all_derived<forward_iterator_tag>, forward_iterator_tag,
                                                  input_iterator_tag> > > >;
};

/* [range.zip.transform.view], class template zip_transform_view.
 * Workaround: Clang 12 to 15, all targets: the constraints of its members
 * that name the types of the const views are a concept of their own, and
 * likewise for adjacent_transform_view, as these compilers substitute into
 * the whole requires-clause of a non-template member of a class template
 * when they instantiate the class, where the types would not be valid (see
 * filter_view in ranges_adaptors.h). */
template <class _Fp, class... _Views>
concept __fcxx_zip_transform_const_iterable = range<const zip_view<_Views...> >
                                           && regular_invocable<const _Fp&, range_reference_t<const _Views>...>;

template <move_constructible _Fp, input_range... _Views>
    requires(view<_Views> && ...) && (sizeof...(_Views) > 0) && is_object_v<_Fp>
         && regular_invocable<_Fp&, range_reference_t<_Views>...>
         && __fcxx_can_reference<invoke_result_t<_Fp&, range_reference_t<_Views>...> >
class zip_transform_view : public view_interface<zip_transform_view<_Fp, _Views...> > {
    using _InnerView = zip_view<_Views...>;

    template <bool _Const>
    using __ziperator = iterator_t<__fcxx_maybe_const<_Const, _InnerView> >;

    template <bool _Const>
    using __zentinel = sentinel_t<__fcxx_maybe_const<_Const, _InnerView> >;

    /* [range.zip.transform.iterator], class template
     * zip_transform_view::iterator */
    template <bool _Const>
    class __iterator : public __fcxx_zip_transform_category<__fcxx_maybe_const<_Const, _Fp>,
                                                            __fcxx_maybe_const<_Const, _InnerView>,
                                                            __fcxx_maybe_const<_Const, _Views>...> {
        friend zip_transform_view;

        template <bool>
        friend class __iterator;

        using _Parent = __fcxx_maybe_const<_Const, zip_transform_view>;
        using _Base = __fcxx_maybe_const<_Const, _InnerView>;
        using _Fn = __fcxx_maybe_const<_Const, _Fp>;
        typedef index_sequence_for<_Views...> _Seq;

        _Parent* __parent_ = nullptr;
        __ziperator<_Const> __inner_;

        constexpr __iterator(_Parent& __parent, __ziperator<_Const> __inner)
            : __parent_(std::addressof(__parent)), __inner_(std::move(__inner)) {}

        template <size_t... _Is>
        static constexpr bool __nothrow_deref(index_sequence<_Is...>) {
            return noexcept(std::__fcxx_invoke(
                std::declval<_Fn&>(), *std::declval<const iterator_t<__fcxx_maybe_const<_Const, _Views> >&>()...));
        }

        template <size_t... _Is>
        constexpr decltype(auto) __deref(index_sequence<_Is...>) const {
            return std::__fcxx_invoke(*__parent_->__fun_, *std::get<_Is>(__inner_.__fcxx_current())...);
        }

        template <size_t... _Is>
        constexpr decltype(auto) __subscript(range_difference_t<_Base> __n, index_sequence<_Is...>) const {
            return std::__fcxx_invoke(
                *__parent_->__fun_,
                std::get<_Is>(
                    __inner_
                        .__fcxx_current())[iter_difference_t<iterator_t<__fcxx_maybe_const<_Const, _Views>>>(__n)]...);
        }

    public:
        using iterator_concept = typename __ziperator<_Const>::iterator_concept;
        using value_type =
            remove_cvref_t<invoke_result_t<_Fn&, range_reference_t<__fcxx_maybe_const<_Const, _Views> >...> >;
        using difference_type = range_difference_t<_Base>;

        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<__ziperator<false>, __ziperator<_Const> >
            : __parent_(__i.__parent_), __inner_(std::move(__i.__inner_)) {}

        /* inner_, for the sentinel. */
        constexpr const __ziperator<_Const>& __fcxx_inner() const noexcept { return __inner_; }

        constexpr decltype(auto) operator*() const noexcept(__nothrow_deref(_Seq())) { return __deref(_Seq()); }

        constexpr __iterator& operator++() {
            ++__inner_;
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        constexpr __iterator operator++(int)
            requires forward_range<_Base>
        {
            auto __tmp = *this;
            ++*this;
            return __tmp;
        }

        constexpr __iterator& operator--()
            requires bidirectional_range<_Base>
        {
            --__inner_;
            return *this;
        }

        constexpr __iterator operator--(int)
            requires bidirectional_range<_Base>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        constexpr __iterator& operator+=(difference_type __x)
            requires random_access_range<_Base>
        {
            __inner_ += __x;
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __x)
            requires random_access_range<_Base>
        {
            __inner_ -= __x;
            return *this;
        }

        constexpr decltype(auto) operator[](difference_type __n) const
            requires random_access_range<_Base>
        {
            return __subscript(__n, _Seq());
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires equality_comparable<__ziperator<_Const> >
        {
            return __x.__inner_ == __y.__inner_;
        }

        friend constexpr auto operator<=>(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __x.__inner_ <=> __y.__inner_;
        }

        friend constexpr __iterator operator+(const __iterator& __i, difference_type __n)
            requires random_access_range<_Base>
        {
            return __iterator(*__i.__parent_, __i.__inner_ + __n);
        }

        friend constexpr __iterator operator+(difference_type __n, const __iterator& __i)
            requires random_access_range<_Base>
        {
            return __iterator(*__i.__parent_, __i.__inner_ + __n);
        }

        friend constexpr __iterator operator-(const __iterator& __i, difference_type __n)
            requires random_access_range<_Base>
        {
            return __iterator(*__i.__parent_, __i.__inner_ - __n);
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y)
            requires sized_sentinel_for<__ziperator<_Const>, __ziperator<_Const> >
        {
            return __x.__inner_ - __y.__inner_;
        }
    };

    /* [range.zip.transform.sentinel], class template
     * zip_transform_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend zip_transform_view;

        template <bool>
        friend class __sentinel;

        __zentinel<_Const> __inner_;

        constexpr explicit __sentinel(__zentinel<_Const> __inner) : __inner_(__inner) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __i)
            requires _Const && convertible_to<__zentinel<false>, __zentinel<_Const> >
            : __inner_(std::move(__i.__inner_)) {}

        template <bool _OtherConst>
            requires sentinel_for<__zentinel<_Const>, __ziperator<_OtherConst> >
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.__fcxx_inner() == __y.__inner_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<__zentinel<_Const>, __ziperator<_OtherConst> >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _InnerView> > operator-(
            const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.__fcxx_inner() - __y.__inner_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<__zentinel<_Const>, __ziperator<_OtherConst> >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _InnerView> > operator-(
            const __sentinel& __x, const __iterator<_OtherConst>& __y) {
            return __x.__inner_ - __y.__fcxx_inner();
        }
    };

    _FCXX_NO_UNIQUE_ADDRESS __fcxx_movable_box<_Fp> __fun_;
    zip_view<_Views...> __zip_;

public:
    zip_transform_view() = default;

    constexpr explicit zip_transform_view(_Fp __fun, _Views... __views)
        : __fun_(std::move(__fun)), __zip_(std::move(__views)...) {}

    constexpr auto begin() { return __iterator<false>(*this, __zip_.begin()); }

    constexpr auto begin() const
        requires __fcxx_zip_transform_const_iterable<_Fp, _Views...>
    {
        return __iterator<true>(*this, __zip_.begin());
    }

    constexpr auto end() {
        if constexpr (common_range<_InnerView>) {
            return __iterator<false>(*this, __zip_.end());
        } else {
            return __sentinel<false>(__zip_.end());
        }
    }

    constexpr auto end() const
        requires __fcxx_zip_transform_const_iterable<_Fp, _Views...>
    {
        if constexpr (common_range<const _InnerView>) {
            return __iterator<true>(*this, __zip_.end());
        } else {
            return __sentinel<true>(__zip_.end());
        }
    }

    constexpr auto size()
        requires sized_range<_InnerView>
    {
        return __zip_.size();
    }

    constexpr auto size() const
        requires sized_range<const _InnerView>
    {
        return __zip_.size();
    }
};

template <class _Fp, class... _Rs>
zip_transform_view(_Fp, _Rs&&...) -> zip_transform_view<_Fp, views::all_t<_Rs>...>;

namespace views {
template <class _Fp>
concept __fcxx_zip_transform_nullary = move_constructible<decay_t<_Fp> > && regular_invocable<decay_t<_Fp>&>
                                    && is_object_v<decay_t<invoke_result_t<decay_t<_Fp>&> > >;

struct __zip_transform_fn {
    template <class _Fp>
        requires __fcxx_zip_transform_nullary<_Fp>
    constexpr auto operator()(_Fp&& __f) const noexcept {
        (void)__f;
        return empty_view<decay_t<invoke_result_t<decay_t<_Fp>&> > >();
    }

    template <class _Fp, class... _Rs>
        requires(sizeof...(_Rs) > 0) && requires { zip_transform_view(std::declval<_Fp>(), std::declval<_Rs>()...); }
    constexpr auto operator()(_Fp&& __f, _Rs&&... __rs) const
        noexcept(noexcept(zip_transform_view(std::forward<_Fp>(__f), std::forward<_Rs>(__rs)...))) {
        return zip_transform_view(std::forward<_Fp>(__f), std::forward<_Rs>(__rs)...);
    }
};

inline constexpr __zip_transform_fn zip_transform{};
} /* namespace views */

/* REPEAT(T, N), as the element types of a tuple and as the arguments of a
 * call. */
template <class _Tp, size_t>
using __fcxx_repeat_t = _Tp;

template <class _Tp, class _Seq>
struct __fcxx_repeat_tuple;

template <class _Tp, size_t... _Is>
struct __fcxx_repeat_tuple<_Tp, index_sequence<_Is...> > {
    using type = tuple<__fcxx_repeat_t<_Tp, _Is>...>;
};

template <class _Tp, size_t _Np>
using __fcxx_repeat_tuple_t = typename __fcxx_repeat_tuple<_Tp, make_index_sequence<_Np> >::type;

template <class _Fp, class _Tp, class _Seq>
struct __fcxx_repeat_invoke {
    static constexpr bool __regular = false;
};

template <class _Fp, class _Tp, size_t... _Is>
struct __fcxx_repeat_invoke<_Fp, _Tp, index_sequence<_Is...> > {
    static constexpr bool __regular = regular_invocable<_Fp, __fcxx_repeat_t<_Tp, _Is>...>;
};

template <class _Fp, class _Tp, class _Seq>
struct __fcxx_repeat_invoke_result {};

template <class _Fp, class _Tp, size_t... _Is>
    requires regular_invocable<_Fp, __fcxx_repeat_t<_Tp, _Is>...>
struct __fcxx_repeat_invoke_result<_Fp, _Tp, index_sequence<_Is...> > {
    using type = invoke_result_t<_Fp, __fcxx_repeat_t<_Tp, _Is>...>;
};

template <class _Fp, class _Tp, size_t _Np>
using __fcxx_repeat_invoke_result_t = typename __fcxx_repeat_invoke_result<_Fp, _Tp, make_index_sequence<_Np> >::type;

template <class _Fp, class _Tp, size_t _Np>
concept __fcxx_regular_invocable_repeat = __fcxx_repeat_invoke<_Fp, _Tp, make_index_sequence<_Np> >::__regular;

/* An array of iterators, which is value-initialized as a whole. */
template <class _Tp, size_t _Np>
struct __fcxx_iterator_array {
    _Tp __elems_[_Np];
};

/* [range.adjacent.view], class template adjacent_view */
template <forward_range _Vp, size_t _Np>
    requires view<_Vp> && (_Np > 0)
class adjacent_view : public view_interface<adjacent_view<_Vp, _Np> > {
    struct __as_sentinel {};

    /* [range.adjacent.iterator], class template adjacent_view::iterator */
    template <bool _Const>
    class __iterator {
        friend adjacent_view;

        template <bool>
        friend class __iterator;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;
        typedef __fcxx_iterator_array<iterator_t<_Base>, _Np> _Current;
        typedef make_index_sequence<_Np> _Seq;

        _Current __current_ = _Current();

        constexpr __iterator(iterator_t<_Base> __first, sentinel_t<_Base> __last) {
            __current_.__elems_[0] = __first;
            for (size_t __i = 1; __i < _Np; ++__i) {
                __current_.__elems_[__i] = ranges::next(__current_.__elems_[__i - 1], 1, __last);
            }
        }

        constexpr __iterator(__as_sentinel, iterator_t<_Base> __first, iterator_t<_Base> __last) {
            if constexpr (!bidirectional_range<_Base>) {
                for (size_t __i = 0; __i < _Np; ++__i) {
                    __current_.__elems_[__i] = __last;
                }
            } else {
                __current_.__elems_[_Np - 1] = __last;
                for (size_t __i = _Np - 1; __i > 0; --__i) {
                    __current_.__elems_[__i - 1] = ranges::prev(__current_.__elems_[__i], 1, __first);
                }
            }
        }

        template <size_t... _Is>
        constexpr auto __deref(index_sequence<_Is...>) const {
            return __fcxx_repeat_tuple_t<iter_reference_t<iterator_t<_Base> >, _Np>(*__current_.__elems_[_Is]...);
        }

        template <size_t... _Is>
        constexpr auto __subscript(range_difference_t<_Base> __n, index_sequence<_Is...>) const {
            return __fcxx_repeat_tuple_t<iter_reference_t<iterator_t<_Base> >, _Np>(__current_.__elems_[_Is][__n]...);
        }

        template <size_t... _Is>
        static constexpr auto __iter_move(const _Current& __c, index_sequence<_Is...>) {
            return __fcxx_repeat_tuple_t<iter_rvalue_reference_t<iterator_t<_Base> >, _Np>(
                ranges::iter_move(__c.__elems_[_Is])...);
        }

        /* The iterators of the other iterator, each initializing its own. */
        template <size_t... _Is>
        static constexpr _Current __convert(__fcxx_iterator_array<iterator_t<_Vp>, _Np>& __a, index_sequence<_Is...>) {
            return _Current{{std::move(__a.__elems_[_Is])...}};
        }

    public:
        using iterator_category = input_iterator_tag;
        using iterator_concept =
            conditional_t<random_access_range<_Base>, random_access_iterator_tag,
                          conditional_t<bidirectional_range<_Base>, bidirectional_iterator_tag, forward_iterator_tag> >;
        using value_type = __fcxx_repeat_tuple_t<range_value_t<_Base>, _Np>;
        using difference_type = range_difference_t<_Base>;

        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base> >
            : __current_(__convert(__i.__current_, _Seq())) {}

        /* current_, for the sentinel and adjacent_transform_view. */
        constexpr const _Current& __fcxx_current() const noexcept { return __current_; }

        constexpr auto operator*() const { return __deref(_Seq()); }

        constexpr __iterator& operator++() {
            for (size_t __k = 0; __k < _Np; ++__k) {
                ++__current_.__elems_[__k];
            }
            return *this;
        }

        constexpr __iterator operator++(int) {
            auto __tmp = *this;
            ++*this;
            return __tmp;
        }

        constexpr __iterator& operator--()
            requires bidirectional_range<_Base>
        {
            for (size_t __k = 0; __k < _Np; ++__k) {
                --__current_.__elems_[__k];
            }
            return *this;
        }

        constexpr __iterator operator--(int)
            requires bidirectional_range<_Base>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        constexpr __iterator& operator+=(difference_type __x)
            requires random_access_range<_Base>
        {
            for (size_t __k = 0; __k < _Np; ++__k) {
                __current_.__elems_[__k] += __x;
            }
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __x)
            requires random_access_range<_Base>
        {
            for (size_t __k = 0; __k < _Np; ++__k) {
                __current_.__elems_[__k] -= __x;
            }
            return *this;
        }

        constexpr auto operator[](difference_type __n) const
            requires random_access_range<_Base>
        {
            return __subscript(__n, _Seq());
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y) {
            return __x.__current_.__elems_[_Np - 1] == __y.__current_.__elems_[_Np - 1];
        }

        friend constexpr bool operator<(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __x.__current_.__elems_[_Np - 1] < __y.__current_.__elems_[_Np - 1];
        }

        friend constexpr bool operator>(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __y < __x;
        }

        friend constexpr bool operator<=(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return !(__y < __x);
        }

        friend constexpr bool operator>=(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return !(__x < __y);
        }

        friend constexpr auto operator<=>(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base> && three_way_comparable<iterator_t<_Base> >
        {
            return __x.__current_.__elems_[_Np - 1] <=> __y.__current_.__elems_[_Np - 1];
        }

        friend constexpr __iterator operator+(const __iterator& __i, difference_type __n)
            requires random_access_range<_Base>
        {
            auto __r = __i;
            __r += __n;
            return __r;
        }

        friend constexpr __iterator operator+(difference_type __n, const __iterator& __i)
            requires random_access_range<_Base>
        {
            auto __r = __i;
            __r += __n;
            return __r;
        }

        friend constexpr __iterator operator-(const __iterator& __i, difference_type __n)
            requires random_access_range<_Base>
        {
            auto __r = __i;
            __r -= __n;
            return __r;
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y)
            requires sized_sentinel_for<iterator_t<_Base>, iterator_t<_Base> >
        {
            return __x.__current_.__elems_[_Np - 1] - __y.__current_.__elems_[_Np - 1];
        }

        friend constexpr auto iter_move(const __iterator& __i) noexcept(
            noexcept(ranges::iter_move(std::declval<const iterator_t<_Base>&>()))
            && is_nothrow_move_constructible_v<range_rvalue_reference_t<_Base> >) {
            return __iter_move(__i.__current_, _Seq());
        }

        friend constexpr void iter_swap(const __iterator& __l, const __iterator& __r) noexcept(
            noexcept(ranges::iter_swap(std::declval<iterator_t<_Base> >(), std::declval<iterator_t<_Base> >())))
            requires indirectly_swappable<iterator_t<_Base> >
        {
            for (size_t __k = 0; __k < _Np; ++__k) {
                ranges::iter_swap(__l.__current_.__elems_[__k], __r.__current_.__elems_[__k]);
            }
        }
    };

    /* [range.adjacent.sentinel], class template adjacent_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend adjacent_view;

        template <bool>
        friend class __sentinel;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        sentinel_t<_Base> __end_ = sentinel_t<_Base>();

        constexpr explicit __sentinel(sentinel_t<_Base> __end) : __end_(__end) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __i)
            requires _Const && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __end_(std::move(__i.__end_)) {}

        template <bool _OtherConst>
            requires sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.__fcxx_current().__elems_[_Np - 1] == __y.__end_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _Vp> > operator-(
            const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.__fcxx_current().__elems_[_Np - 1] - __y.__end_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _Vp> > operator-(
            const __sentinel& __y, const __iterator<_OtherConst>& __x) {
            return __y.__end_ - __x.__fcxx_current().__elems_[_Np - 1];
        }
    };

    _Vp __base_ = _Vp();

public:
    adjacent_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit adjacent_view(_Vp __base) : __base_(std::move(__base)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return __iterator<false>(ranges::begin(__base_), ranges::end(__base_));
    }

    constexpr auto begin() const
        requires range<const _Vp>
    {
        return __iterator<true>(ranges::begin(__base_), ranges::end(__base_));
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        if constexpr (common_range<_Vp>) {
            return __iterator<false>(__as_sentinel{}, ranges::begin(__base_), ranges::end(__base_));
        } else {
            return __sentinel<false>(ranges::end(__base_));
        }
    }

    constexpr auto end() const
        requires range<const _Vp>
    {
        if constexpr (common_range<const _Vp>) {
            return __iterator<true>(__as_sentinel{}, ranges::begin(__base_), ranges::end(__base_));
        } else {
            return __sentinel<true>(ranges::end(__base_));
        }
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        using _ST = decltype(ranges::size(__base_));
        using _CT = common_type_t<_ST, size_t>;
        auto __sz = static_cast<_CT>(ranges::size(__base_));
        __sz -= ranges::__fcxx_min<_CT>(__sz, _Np - 1);
        return static_cast<_ST>(__sz);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        using _ST = decltype(ranges::size(__base_));
        using _CT = common_type_t<_ST, size_t>;
        auto __sz = static_cast<_CT>(ranges::size(__base_));
        __sz -= ranges::__fcxx_min<_CT>(__sz, _Np - 1);
        return static_cast<_ST>(__sz);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        using _DT = range_difference_t<decltype((__base_))>;
        using _CT = common_type_t<_DT, size_t>;
        auto __sz = static_cast<_CT>(ranges::reserve_hint(__base_));
        __sz -= ranges::__fcxx_min<_CT>(__sz, _Np - 1);
        return ranges::__fcxx_to_unsigned_like(__sz);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        using _DT = range_difference_t<decltype((__base_))>;
        using _CT = common_type_t<_DT, size_t>;
        auto __sz = static_cast<_CT>(ranges::reserve_hint(__base_));
        __sz -= ranges::__fcxx_min<_CT>(__sz, _Np - 1);
        return ranges::__fcxx_to_unsigned_like(__sz);
    }
#  endif
};

template <class _Vp, size_t _Np>
inline constexpr bool enable_borrowed_range<adjacent_view<_Vp, _Np> > = enable_borrowed_range<_Vp>;

namespace views {
/* views::adjacent<0> of a forward range is an empty view of empty tuples,
 * as LWG 4098 has it. */
template <size_t _Np>
struct __adjacent_fn : __fcxx_adaptor_closure<__adjacent_fn<_Np> > {
    template <class _Rp>
        requires(_Np == 0) && forward_range<_Rp>
    constexpr auto operator()(_Rp&& __r) const noexcept {
        (void)__r;
        return empty_view<tuple<> >();
    }

    template <class _Rp>
        requires(!(_Np == 0 && forward_range<_Rp>)) && requires { adjacent_view<all_t<_Rp>, _Np>(std::declval<_Rp>()); }
    constexpr auto operator()(_Rp&& __r) const
        noexcept(noexcept(adjacent_view<all_t<_Rp>, _Np>(std::forward<_Rp>(__r)))) {
        return adjacent_view<all_t<_Rp>, _Np>(std::forward<_Rp>(__r));
    }
};

template <size_t _Np>
inline constexpr __adjacent_fn<_Np> adjacent{};

inline constexpr auto pairwise = adjacent<2>;
} /* namespace views */

/* [range.adjacent.transform.iterator], the iterator category of the iterator
 * of adjacent_transform_view. */
template <class _Fn, class _Base, size_t _Np>
struct __fcxx_adjacent_transform_category {
private:
    typedef typename iterator_traits<iterator_t<_Base> >::iterator_category _Cp;

public:
    using iterator_category =
        conditional_t<!is_reference_v<__fcxx_repeat_invoke_result_t<_Fn&, range_reference_t<_Base>, _Np> >,
                      input_iterator_tag,
                      conditional_t<derived_from<_Cp, random_access_iterator_tag>, random_access_iterator_tag,
                                    conditional_t<derived_from<_Cp, bidirectional_iterator_tag>,
                                                  bidirectional_iterator_tag,
                                                  conditional_t<derived_from<_Cp, forward_iterator_tag>,
                                                                forward_iterator_tag, input_iterator_tag> > > >;
};

/* [range.adjacent.transform.view], class template adjacent_transform_view.
 * Workaround: Clang 12 to 15, all targets: the constraints of the members of
 * the const view are a concept of their own (see zip_transform_view). */
template <class _Vp, class _Fp, size_t _Np>
concept __fcxx_adjacent_transform_const_iterable = range<const adjacent_view<_Vp, _Np>>
    && __fcxx_regular_invocable_repeat<const _Fp&, range_reference_t<const _Vp>, _Np>;

template <forward_range _Vp, move_constructible _Fp, size_t _Np>
    requires view<_Vp> && (_Np > 0)
    && is_object_v<_Fp> && __fcxx_regular_invocable_repeat<_Fp&, range_reference_t<_Vp>, _Np>
    && __fcxx_can_reference<__fcxx_repeat_invoke_result_t<_Fp&, range_reference_t<_Vp>, _Np>>
class adjacent_transform_view : public view_interface<adjacent_transform_view<_Vp, _Fp, _Np>> {
    using _InnerView = adjacent_view<_Vp, _Np>;

    template <bool _Const>
    using __inner_iterator = iterator_t<__fcxx_maybe_const<_Const, _InnerView> >;

    template <bool _Const>
    using __inner_sentinel = sentinel_t<__fcxx_maybe_const<_Const, _InnerView> >;

    /* [range.adjacent.transform.iterator], class template
     * adjacent_transform_view::iterator */
    template <bool _Const>
    class __iterator : public __fcxx_adjacent_transform_category<__fcxx_maybe_const<_Const, _Fp>,
                                                                 __fcxx_maybe_const<_Const, _Vp>, _Np> {
        friend adjacent_transform_view;

        template <bool>
        friend class __iterator;

        using _Parent = __fcxx_maybe_const<_Const, adjacent_transform_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;
        using _Fn = __fcxx_maybe_const<_Const, _Fp>;
        typedef make_index_sequence<_Np> _Seq;

        _Parent* __parent_ = nullptr;
        __inner_iterator<_Const> __inner_;

        constexpr __iterator(_Parent& __parent, __inner_iterator<_Const> __inner)
            : __parent_(std::addressof(__parent)), __inner_(std::move(__inner)) {}

        template <size_t... _Is>
        static constexpr bool __nothrow_deref(index_sequence<_Is...>) {
            return noexcept(std::__fcxx_invoke(std::declval<_Fn&>(),
                                               *std::declval<const __fcxx_repeat_t<iterator_t<_Base>, _Is>&>()...));
        }

        template <size_t... _Is>
        constexpr decltype(auto) __deref(index_sequence<_Is...>) const {
            return std::__fcxx_invoke(*__parent_->__fun_, *__inner_.__fcxx_current().__elems_[_Is]...);
        }

        template <size_t... _Is>
        constexpr decltype(auto) __subscript(range_difference_t<_Base> __n, index_sequence<_Is...>) const {
            return std::__fcxx_invoke(*__parent_->__fun_, __inner_.__fcxx_current().__elems_[_Is][__n]...);
        }

    public:
        using iterator_concept = typename __inner_iterator<_Const>::iterator_concept;
        using value_type = remove_cvref_t<__fcxx_repeat_invoke_result_t<_Fn&, range_reference_t<_Base>, _Np> >;
        using difference_type = range_difference_t<_Base>;

        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<__inner_iterator<false>, __inner_iterator<_Const> >
            : __parent_(__i.__parent_), __inner_(std::move(__i.__inner_)) {}

        /* inner_, for the sentinel. */
        constexpr const __inner_iterator<_Const>& __fcxx_inner() const noexcept { return __inner_; }

        constexpr decltype(auto) operator*() const noexcept(__nothrow_deref(_Seq())) { return __deref(_Seq()); }

        constexpr __iterator& operator++() {
            ++__inner_;
            return *this;
        }

        constexpr __iterator operator++(int) {
            auto __tmp = *this;
            ++*this;
            return __tmp;
        }

        constexpr __iterator& operator--()
            requires bidirectional_range<_Base>
        {
            --__inner_;
            return *this;
        }

        constexpr __iterator operator--(int)
            requires bidirectional_range<_Base>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        constexpr __iterator& operator+=(difference_type __x)
            requires random_access_range<_Base>
        {
            __inner_ += __x;
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __x)
            requires random_access_range<_Base>
        {
            __inner_ -= __x;
            return *this;
        }

        constexpr decltype(auto) operator[](difference_type __n) const
            requires random_access_range<_Base>
        {
            return __subscript(__n, _Seq());
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y) {
            return __x.__inner_ == __y.__inner_;
        }

        friend constexpr bool operator<(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __x.__inner_ < __y.__inner_;
        }

        friend constexpr bool operator>(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __x.__inner_ > __y.__inner_;
        }

        friend constexpr bool operator<=(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __x.__inner_ <= __y.__inner_;
        }

        friend constexpr bool operator>=(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __x.__inner_ >= __y.__inner_;
        }

        friend constexpr auto operator<=>(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base> && three_way_comparable<__inner_iterator<_Const> >
        {
            return __x.__inner_ <=> __y.__inner_;
        }

        friend constexpr __iterator operator+(const __iterator& __i, difference_type __n)
            requires random_access_range<_Base>
        {
            return __iterator(*__i.__parent_, __i.__inner_ + __n);
        }

        friend constexpr __iterator operator+(difference_type __n, const __iterator& __i)
            requires random_access_range<_Base>
        {
            return __iterator(*__i.__parent_, __i.__inner_ + __n);
        }

        friend constexpr __iterator operator-(const __iterator& __i, difference_type __n)
            requires random_access_range<_Base>
        {
            return __iterator(*__i.__parent_, __i.__inner_ - __n);
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y)
            requires sized_sentinel_for<__inner_iterator<_Const>, __inner_iterator<_Const> >
        {
            return __x.__inner_ - __y.__inner_;
        }
    };

    /* [range.adjacent.transform.sentinel], class template
     * adjacent_transform_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend adjacent_transform_view;

        template <bool>
        friend class __sentinel;

        __inner_sentinel<_Const> __inner_;

        constexpr explicit __sentinel(__inner_sentinel<_Const> __inner) : __inner_(__inner) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __i)
            requires _Const && convertible_to<__inner_sentinel<false>, __inner_sentinel<_Const> >
            : __inner_(std::move(__i.__inner_)) {}

        template <bool _OtherConst>
            requires sentinel_for<__inner_sentinel<_Const>, __inner_iterator<_OtherConst> >
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.__fcxx_inner() == __y.__inner_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<__inner_sentinel<_Const>, __inner_iterator<_OtherConst> >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _InnerView> > operator-(
            const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.__fcxx_inner() - __y.__inner_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<__inner_sentinel<_Const>, __inner_iterator<_OtherConst> >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _InnerView> > operator-(
            const __sentinel& __x, const __iterator<_OtherConst>& __y) {
            return __x.__inner_ - __y.__fcxx_inner();
        }
    };

    _FCXX_NO_UNIQUE_ADDRESS __fcxx_movable_box<_Fp> __fun_;
    adjacent_view<_Vp, _Np> __inner_;

public:
    adjacent_transform_view() = default;

    constexpr explicit adjacent_transform_view(_Vp __base, _Fp __fun)
        : __fun_(std::move(__fun)), __inner_(std::move(__base)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __inner_.base();
    }

    constexpr _Vp base() && { return std::move(__inner_).base(); }

    constexpr auto begin() { return __iterator<false>(*this, __inner_.begin()); }

    constexpr auto begin() const
        requires __fcxx_adjacent_transform_const_iterable<_Vp, _Fp, _Np>
    {
        return __iterator<true>(*this, __inner_.begin());
    }

    constexpr auto end() {
        if constexpr (common_range<_InnerView>) {
            return __iterator<false>(*this, __inner_.end());
        } else {
            return __sentinel<false>(__inner_.end());
        }
    }

    constexpr auto end() const
        requires __fcxx_adjacent_transform_const_iterable<_Vp, _Fp, _Np>
    {
        if constexpr (common_range<const _InnerView>) {
            return __iterator<true>(*this, __inner_.end());
        } else {
            return __sentinel<true>(__inner_.end());
        }
    }

    constexpr auto size()
        requires sized_range<_InnerView>
    {
        return __inner_.size();
    }

    constexpr auto size() const
        requires sized_range<const _InnerView>
    {
        return __inner_.size();
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_InnerView>
    {
        return __inner_.reserve_hint();
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _InnerView>
    {
        return __inner_.reserve_hint();
    }
#  endif
};

namespace views {
/* views::adjacent_transform<0> of a forward range is views::zip_transform
 * of the function alone, as LWG 4098 has it. */
template <size_t _Np>
struct __adjacent_transform_fn {
    template <class _Rp, class _Fp>
        requires(_Np == 0) && forward_range<_Rp> && requires { views::zip_transform(std::declval<_Fp>()); }
    constexpr auto operator()(_Rp&& __r, _Fp&& __f) const
        noexcept(noexcept(views::zip_transform(std::forward<_Fp>(__f)))) {
        (void)__r;
        return views::zip_transform(std::forward<_Fp>(__f));
    }

    template <class _Rp, class _Fp>
        requires(!(_Np == 0 && forward_range<_Rp>)) && requires {
            adjacent_transform_view<all_t<_Rp>, decay_t<_Fp>, _Np>(std::declval<_Rp>(), std::declval<_Fp>());
        }
    constexpr auto operator()(_Rp&& __r, _Fp&& __f) const
        noexcept(noexcept(adjacent_transform_view<all_t<_Rp>, decay_t<_Fp>, _Np>(std::forward<_Rp>(__r),
                                                                                  std::forward<_Fp>(__f)))) {
        return adjacent_transform_view<all_t<_Rp>, decay_t<_Fp>, _Np>(std::forward<_Rp>(__r), std::forward<_Fp>(__f));
    }

    template <class _Fp>
        requires __fcxx_can_bind<__adjacent_transform_fn, _Fp>
    constexpr auto operator()(_Fp&& __f) const noexcept(is_nothrow_constructible_v<decay_t<_Fp>, _Fp>) {
        return ranges::__fcxx_bind_adaptor<__adjacent_transform_fn>(std::forward<_Fp>(__f));
    }
};

template <size_t _Np>
inline constexpr __adjacent_transform_fn<_Np> adjacent_transform{};

inline constexpr auto pairwise_transform = adjacent_transform<2>;
} /* namespace views */

/* [range.cartesian.view], the concepts of cartesian_product_view */
template <bool _Const, class _First, class... _Vs>
concept __fcxx_cartesian_product_is_random_access =
    (random_access_range<__fcxx_maybe_const<_Const, _First> > && ...
     && (random_access_range<__fcxx_maybe_const<_Const, _Vs> > && sized_range<__fcxx_maybe_const<_Const, _Vs> >));

template <class _Rp>
concept __fcxx_cartesian_product_common_arg = common_range<_Rp> || (sized_range<_Rp> && random_access_range<_Rp>);

template <bool _Const, class _First, class... _Vs>
concept __fcxx_cartesian_product_is_bidirectional =
    (bidirectional_range<__fcxx_maybe_const<_Const, _First> > && ...
     && (bidirectional_range<__fcxx_maybe_const<_Const, _Vs> >
         && __fcxx_cartesian_product_common_arg<__fcxx_maybe_const<_Const, _Vs> >));

template <class _First, class...>
concept __fcxx_cartesian_product_is_common = __fcxx_cartesian_product_common_arg<_First>;

template <class... _Vs>
concept __fcxx_cartesian_product_is_sized = (sized_range<_Vs> && ...);

/* cartesian-is-sized-sentinel, of the iterators, and of the sentinel of
 * the first range. */
template <bool _Const, class _First, class... _Vs>
concept __fcxx_cartesian_is_sized_iterator =
    (sized_sentinel_for<iterator_t<__fcxx_maybe_const<_Const, _First>>, iterator_t<__fcxx_maybe_const<_Const, _First>>>
     && ...
     && (sized_range<__fcxx_maybe_const<_Const, _Vs>>
         && sized_sentinel_for<iterator_t<__fcxx_maybe_const<_Const, _Vs>>,
                               iterator_t<__fcxx_maybe_const<_Const, _Vs>>>));

template <bool _Const, class _First, class... _Vs>
concept __fcxx_cartesian_is_sized_sentinel =
    (sized_sentinel_for<sentinel_t<__fcxx_maybe_const<_Const, _First>>, iterator_t<__fcxx_maybe_const<_Const, _First>>>
     && ...
     && (sized_range<__fcxx_maybe_const<_Const, _Vs>>
         && sized_sentinel_for<iterator_t<__fcxx_maybe_const<_Const, _Vs>>,
                               iterator_t<__fcxx_maybe_const<_Const, _Vs>>>));

template <__fcxx_cartesian_product_common_arg _Rp>
constexpr auto __fcxx_cartesian_common_arg_end(_Rp& __r) {
    if constexpr (common_range<_Rp>) {
        return ranges::end(__r);
    } else {
        return ranges::begin(__r) + ranges::distance(__r);
    }
}

/* The number of bits a product of the sizes of ranges can need, and the
 * narrowest unsigned and signed integer types with that many, or the widest
 * where there is none, which the size and difference types of
 * cartesian_product_view are, as the recommended practice has it. */
template <class... _Rs>
inline constexpr size_t __fcxx_cartesian_bits = ((sizeof(range_difference_t<_Rs>) * __CHAR_BIT__ - 1) + ...);

template <size_t _Bits>
struct __fcxx_cartesian_integers {
#  ifdef __SIZEOF_INT128__
    typedef __fcxx_uint128 __widest_unsigned;
    typedef __fcxx_int128 __widest_signed;
#  else
    typedef unsigned long long __widest_unsigned;
    typedef long long __widest_signed;
#  endif
    using __unsigned_type = conditional_t<
        (sizeof(unsigned int) * __CHAR_BIT__ >= _Bits), unsigned int,
        conditional_t<(sizeof(unsigned long) * __CHAR_BIT__ >= _Bits), unsigned long,
                      conditional_t<(sizeof(unsigned long long) * __CHAR_BIT__ >= _Bits), unsigned long long,
                                    __widest_unsigned> > >;
    using __signed_type = conditional_t<
        (sizeof(int) * __CHAR_BIT__ - 1 >= _Bits), int,
        conditional_t<(sizeof(long) * __CHAR_BIT__ - 1 >= _Bits), long,
                      conditional_t<(sizeof(long long) * __CHAR_BIT__ - 1 >= _Bits), long long, __widest_signed> > >;
};

/* [range.cartesian.view], class template cartesian_product_view */
template <input_range _First, forward_range... _Vs>
    requires(view<_First> && ... && view<_Vs>)
class cartesian_product_view : public view_interface<cartesian_product_view<_First, _Vs...> > {
    static constexpr size_t _Nv = sizeof...(_Vs);

    typedef __fcxx_cartesian_integers<__fcxx_cartesian_bits<_First, _Vs...> > _Integers;

    /* [range.cartesian.iterator], class template
     * cartesian_product_view::iterator */
    template <bool _Const>
    class __iterator {
        friend cartesian_product_view;

        template <bool>
        friend class __iterator;

    public:
        using iterator_category = input_iterator_tag;
        using iterator_concept = conditional_t<
            __fcxx_cartesian_product_is_random_access<_Const, _First, _Vs...>, random_access_iterator_tag,
            conditional_t<__fcxx_cartesian_product_is_bidirectional<_Const, _First, _Vs...>, bidirectional_iterator_tag,
                          conditional_t<forward_range<__fcxx_maybe_const<_Const, _First> >, forward_iterator_tag,
                                        input_iterator_tag> > >;
        using value_type =
            tuple<range_value_t<__fcxx_maybe_const<_Const, _First>>, range_value_t<__fcxx_maybe_const<_Const, _Vs>>...>;
        using reference = tuple<range_reference_t<__fcxx_maybe_const<_Const, _First> >,
                                range_reference_t<__fcxx_maybe_const<_Const, _Vs> >...>;
        using difference_type = typename _Integers::__signed_type;

    private:
        using _Parent = __fcxx_maybe_const<_Const, cartesian_product_view>;
        typedef tuple<iterator_t<__fcxx_maybe_const<_Const, _First> >, iterator_t<__fcxx_maybe_const<_Const, _Vs> >...>
            _Current;
        typedef index_sequence_for<_First, _Vs...> _Seq;

        _Parent* __parent_ = nullptr;
        _Current __current_;

        template <size_t _Np = _Nv>
        constexpr void __next() {
            auto& __it = std::get<_Np>(__current_);
            ++__it;
            if constexpr (_Np > 0) {
                if (__it == ranges::end(std::get<_Np>(__parent_->__bases_))) {
                    __it = ranges::begin(std::get<_Np>(__parent_->__bases_));
                    __next<_Np - 1>();
                }
            }
        }

        template <size_t _Np = _Nv>
        constexpr void __prev() {
            auto& __it = std::get<_Np>(__current_);
            if constexpr (_Np > 0) {
                if (__it == ranges::begin(std::get<_Np>(__parent_->__bases_))) {
                    __it = ranges::__fcxx_cartesian_common_arg_end(std::get<_Np>(__parent_->__bases_));
                    __prev<_Np - 1>();
                }
            }
            --__it;
        }

        /* Advances the iterator of the range N by some steps, carrying to
         * the iterators of the ranges before it. */
        template <size_t _Np = _Nv>
        constexpr void __advance(difference_type __x) {
            if (__x == 0) {
                return;
            }
            auto& __it = std::get<_Np>(__current_);
            using _Dp = iter_difference_t<remove_reference_t<decltype(__it)> >;
            if constexpr (_Np == 0) {
                __it += static_cast<_Dp>(__x);
            } else {
                auto& __base = std::get<_Np>(__parent_->__bases_);
                const auto __first = ranges::begin(__base);
                const difference_type __size = static_cast<difference_type>(ranges::size(__base));
                const difference_type __total = static_cast<difference_type>(__it - __first) + __x;
                difference_type __carry = __total / __size;
                difference_type __pos = __total % __size;
                if (__pos < 0) {
                    __pos += __size;
                    --__carry;
                }
                __it = __first + static_cast<_Dp>(__pos);
                __advance<_Np - 1>(__carry);
            }
        }

        /* scaled-size(N) */
        template <size_t _Np>
        constexpr difference_type __scaled_size() const {
            if constexpr (_Np <= _Nv) {
                return _FCXX_MUL(static_cast<difference_type>(ranges::size(std::get<_Np>(__parent_->__bases_))),
                                 __scaled_size<_Np + 1>());
            } else {
                return static_cast<difference_type>(1);
            }
        }

        template <class _Tuple, size_t... _Is>
        constexpr difference_type __distance_from(const _Tuple& __t, index_sequence<_Is...>) const {
            return (_FCXX_MUL(static_cast<difference_type>(std::get<_Is>(__current_) - std::get<_Is>(__t)),
                              __scaled_size<_Is + 1>())
                    + ...);
        }

        template <size_t... _Is>
        constexpr difference_type __distance_to_end(index_sequence<_Is...>) const {
            auto __end_tuple =
                tuple<sentinel_t<__fcxx_maybe_const<_Const, _First>>, iterator_t<__fcxx_maybe_const<_Const, _Vs>>...>(
                    ranges::end(std::get<0>(__parent_->__bases_)),
                    ranges::begin(std::get<_Is + 1>(__parent_->__bases_))...);
            return __distance_from(__end_tuple, _Seq());
        }

        template <size_t... _Is>
        constexpr bool __at_end(index_sequence<_Is...>) const {
            return (static_cast<bool>(std::get<_Is>(__current_) == ranges::end(std::get<_Is>(__parent_->__bases_)))
                    || ...);
        }

        static constexpr bool __at_end(const __iterator& __x) { return __x.__at_end(_Seq()); }

        static constexpr difference_type __to_end(const __iterator& __i) {
            return __i.__distance_to_end(make_index_sequence<_Nv>());
        }

        template <size_t... _Is>
        static constexpr auto __iter_move(const _Current& __c, index_sequence<_Is...>) {
            return tuple<iter_rvalue_reference_t<iterator_t<__fcxx_maybe_const<_Const, _First> > >,
                         iter_rvalue_reference_t<iterator_t<__fcxx_maybe_const<_Const, _Vs> > >...>(
                ranges::iter_move(std::get<_Is>(__c))...);
        }

        template <size_t... _Is>
        static constexpr void __iter_swap(const _Current& __l, const _Current& __r, index_sequence<_Is...>) {
            (ranges::iter_swap(std::get<_Is>(__l), std::get<_Is>(__r)), ...);
        }

        template <size_t... _Is>
        static constexpr bool __nothrow_swap(index_sequence<_Is...>) {
            return (noexcept(ranges::iter_swap(std::declval<const tuple_element_t<_Is, _Current>&>(),
                                               std::declval<const tuple_element_t<_Is, _Current>&>()))
                    && ...);
        }

        template <size_t... _Is>
        constexpr reference __deref(index_sequence<_Is...>) const {
            return reference(*std::get<_Is>(__current_)...);
        }

        constexpr __iterator(_Parent& __parent, _Current __current)
            : __parent_(std::addressof(__parent)), __current_(std::move(__current)) {}

    public:
        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const
                         && (convertible_to<iterator_t<_First>, iterator_t<__fcxx_maybe_const<_Const, _First>>> && ...
                             && convertible_to<iterator_t<_Vs>, iterator_t<__fcxx_maybe_const<_Const, _Vs>>>)
            : __parent_(__i.__parent_), __current_(std::move(__i.__current_)) {}

        constexpr auto operator*() const { return __deref(_Seq()); }

        constexpr __iterator& operator++() {
            __next();
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        constexpr __iterator operator++(int)
            requires forward_range<__fcxx_maybe_const<_Const, _First> >
        {
            auto __tmp = *this;
            ++*this;
            return __tmp;
        }

        constexpr __iterator& operator--()
            requires __fcxx_cartesian_product_is_bidirectional<_Const, _First, _Vs...>
        {
            __prev();
            return *this;
        }

        constexpr __iterator operator--(int)
            requires __fcxx_cartesian_product_is_bidirectional<_Const, _First, _Vs...>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        constexpr __iterator& operator+=(difference_type __x)
            requires __fcxx_cartesian_product_is_random_access<_Const, _First, _Vs...>
        {
            __advance(__x);
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __x)
            requires __fcxx_cartesian_product_is_random_access<_Const, _First, _Vs...>
        {
            *this += -__x;
            return *this;
        }

        constexpr reference operator[](difference_type __n) const
            requires __fcxx_cartesian_product_is_random_access<_Const, _First, _Vs...>
        {
            return *((*this) + __n);
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires equality_comparable<iterator_t<__fcxx_maybe_const<_Const, _First> > >
        {
            return __x.__current_ == __y.__current_;
        }

        friend constexpr bool operator==(const __iterator& __x, default_sentinel_t) { return __at_end(__x); }

        friend constexpr auto operator<=>(const __iterator& __x, const __iterator& __y)
            requires __fcxx_all_random_access<_Const, _First, _Vs...>
        {
            return __x.__current_ <=> __y.__current_;
        }

        friend constexpr __iterator operator+(const __iterator& __x, difference_type __y)
            requires __fcxx_cartesian_product_is_random_access<_Const, _First, _Vs...>
        {
            return __iterator(__x) += __y;
        }

        friend constexpr __iterator operator+(difference_type __x, const __iterator& __y)
            requires __fcxx_cartesian_product_is_random_access<_Const, _First, _Vs...>
        {
            return __y + __x;
        }

        friend constexpr __iterator operator-(const __iterator& __x, difference_type __y)
            requires __fcxx_cartesian_product_is_random_access<_Const, _First, _Vs...>
        {
            return __iterator(__x) -= __y;
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y)
            requires __fcxx_cartesian_is_sized_iterator<_Const, _First, _Vs...>
        {
            return __x.__distance_from(__y.__current_, _Seq());
        }

        friend constexpr difference_type operator-(const __iterator& __i, default_sentinel_t)
            requires __fcxx_cartesian_is_sized_sentinel<_Const, _First, _Vs...>
        {
            return __to_end(__i);
        }

        friend constexpr difference_type operator-(default_sentinel_t, const __iterator& __i)
            requires __fcxx_cartesian_is_sized_sentinel<_Const, _First, _Vs...>
        {
            return -__to_end(__i);
        }

        friend constexpr auto iter_move(const __iterator& __i) noexcept(
            (noexcept(ranges::iter_move(std::declval<const iterator_t<__fcxx_maybe_const<_Const, _First> >&>()))
             && ... && noexcept(ranges::iter_move(std::declval<const iterator_t<__fcxx_maybe_const<_Const, _Vs> >&>())))
            && (is_nothrow_move_constructible_v<range_rvalue_reference_t<__fcxx_maybe_const<_Const, _First> > > && ...
                && is_nothrow_move_constructible_v<range_rvalue_reference_t<__fcxx_maybe_const<_Const, _Vs> > >)) {
            return __iter_move(__i.__current_, _Seq());
        }

        friend constexpr void iter_swap(const __iterator& __l, const __iterator& __r) noexcept(__nothrow_swap(_Seq()))
            requires(indirectly_swappable<iterator_t<__fcxx_maybe_const<_Const, _First> > > && ...
                     && indirectly_swappable<iterator_t<__fcxx_maybe_const<_Const, _Vs> > >)
        {
            __iter_swap(__l.__current_, __r.__current_, _Seq());
        }
    };

    tuple<_First, _Vs...> __bases_;

    template <bool _Const, class _Self, size_t... _Is>
    static constexpr __iterator<_Const> __end(_Self& __self, index_sequence<_Is...>) {
        const bool __any_empty = (static_cast<bool>(ranges::empty(std::get<_Is + 1>(__self.__bases_))) || ... || false);
        auto& __first = std::get<0>(__self.__bases_);
        return __iterator<_Const>(
            __self, typename __iterator<_Const>::_Current(
                        __any_empty ? ranges::begin(__first) : ranges::__fcxx_cartesian_common_arg_end(__first),
                        ranges::begin(std::get<_Is + 1>(__self.__bases_))...));
    }

    template <class _Tuple, size_t... _Is>
    static constexpr auto __size(_Tuple& __tuple, index_sequence<_Is...>) {
        using _Size = typename _Integers::__unsigned_type;
        _Size __p = 1;
        ((__p = static_cast<_Size>(_FCXX_MUL(__p, static_cast<_Size>(ranges::size(std::get<_Is>(__tuple)))))), ...);
        return __p;
    }

public:
    constexpr cartesian_product_view() = default;

    constexpr explicit cartesian_product_view(_First __first_base, _Vs... __rest_bases)
        : __bases_(std::move(__first_base), std::move(__rest_bases)...) {}

    constexpr __iterator<false> begin()
        requires(!__fcxx_simple_view<_First> || ... || !__fcxx_simple_view<_Vs>)
    {
        return __iterator<false>(*this, ranges::__fcxx_tuple_transform(ranges::begin, __bases_));
    }

    constexpr __iterator<true> begin() const
        requires(range<const _First> && ... && range<const _Vs>)
    {
        return __iterator<true>(*this, ranges::__fcxx_tuple_transform(ranges::begin, __bases_));
    }

    constexpr __iterator<false> end()
        requires((!__fcxx_simple_view<_First> || ... || !__fcxx_simple_view<_Vs>)
                 && __fcxx_cartesian_product_is_common<_First, _Vs...>)
    {
        return __end<false>(*this, make_index_sequence<_Nv>());
    }

    /* Workaround: GCC 11 to 13 and Clang 12 to 17, all targets: the two
     * end() const as one function, as their mangled names are the same with
     * these compilers (see ranges_util.h). */
    constexpr auto end() const noexcept(!__fcxx_cartesian_product_is_common<const _First, const _Vs...>) {
        if constexpr (__fcxx_cartesian_product_is_common<const _First, const _Vs...>) {
            return __end<true>(*this, make_index_sequence<_Nv>());
        } else {
            return default_sentinel;
        }
    }

    constexpr auto size()
        requires __fcxx_cartesian_product_is_sized<_First, _Vs...>
    {
        return __size(__bases_, index_sequence_for<_First, _Vs...>());
    }

    constexpr auto size() const
        requires __fcxx_cartesian_product_is_sized<const _First, const _Vs...>
    {
        return __size(__bases_, index_sequence_for<_First, _Vs...>());
    }
};

template <class... _Vs>
cartesian_product_view(_Vs&&...) -> cartesian_product_view<views::all_t<_Vs>...>;

namespace views {
struct __cartesian_product_fn {
    constexpr auto operator()() const { return views::single(tuple<>()); }

    template <class... _Rs>
        requires(sizeof...(_Rs) > 0) && requires { cartesian_product_view<all_t<_Rs>...>(std::declval<_Rs>()...); }
    constexpr auto operator()(_Rs&&... __rs) const
        noexcept(noexcept(cartesian_product_view<all_t<_Rs>...>(std::forward<_Rs>(__rs)...))) {
        return cartesian_product_view<all_t<_Rs>...>(std::forward<_Rs>(__rs)...);
    }
};

inline constexpr __cartesian_product_fn cartesian_product{};
} /* namespace views */

} /* namespace ranges */
} /* namespace std */

#endif
