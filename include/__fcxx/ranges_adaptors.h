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


/* [range.adaptors], the range adaptors of <ranges> of a single range, but
 * for those that join and split ranges: views::all with ref_view and
 * owning_view, as_rvalue_view, filter_view, transform_view, take_view,
 * take_while_view, drop_view, drop_while_view, views::counted,
 * common_view, reverse_view, as_const_view, elements_view and
 * enumerate_view. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_ADAPTORS_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_ADAPTORS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <compare>
#include <concepts>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <__fcxx/addressof.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_base.h>
#include <__fcxx/ranges_core.h>
#include <__fcxx/ranges_factories.h>
#include <__fcxx/ranges_util.h>
#include <__fcxx/tuple_like.h>
/* views::counted gives spans, which are there wherever <ranges> is. */
#include <__fcxx/span.h>

namespace std {

/* The views of the other headers that views::take, views::drop and others
 * make views of their own kind from, which are only freestanding from C++26
 * on. */
#  if _FCXX_STD >= 2026
template <class _CharT, class _Traits>
class basic_string_view;
#  endif

#  if _FCXX_STD >= 2026
template <class _Tp>
class optional;
#  endif

namespace ranges {

template <class _Tp>
inline constexpr bool __fcxx_is_span = false;

template <class _Tp, size_t _Extent>
inline constexpr bool __fcxx_is_span<span<_Tp, _Extent> > = true;

template <class _Tp>
inline constexpr bool __fcxx_is_string_view = false;

#  if _FCXX_STD >= 2026
template <class _CharT, class _Traits>
inline constexpr bool __fcxx_is_string_view<basic_string_view<_CharT, _Traits> > = true;
#  endif

template <class _Tp>
inline constexpr bool __fcxx_is_subrange = false;

template <class _It, class _Sp, subrange_kind _Kp>
inline constexpr bool __fcxx_is_subrange<subrange<_It, _Sp, _Kp> > = true;

/* Whether a subrange stores its size, as StoreSize has it. */
template <class _Tp>
inline constexpr bool __fcxx_subrange_stores_size = false;

template <class _It, class _Sp, subrange_kind _Kp>
inline constexpr bool __fcxx_subrange_stores_size<subrange<_It, _Sp, _Kp> > =
    _Kp == subrange_kind::sized && !sized_sentinel_for<_Sp, _It>;

template <class _Tp>
inline constexpr bool __fcxx_is_empty_view = false;

template <class _Tp>
inline constexpr bool __fcxx_is_empty_view<empty_view<_Tp> > = true;

template <class _Tp>
inline constexpr bool __fcxx_is_iota_view = false;

template <class _Wp, class _Bound>
inline constexpr bool __fcxx_is_iota_view<iota_view<_Wp, _Bound> > = true;

#  if _FCXX_STD >= 2026
/* optional, which is a view from C++26 on. */
template <class _Tp>
inline constexpr bool __fcxx_is_optional = false;

template <class _Tp>
inline constexpr bool __fcxx_is_optional<optional<_Tp> > = true;

template <class _Tp>
concept __fcxx_optional_view = __fcxx_is_optional<_Tp> && view<_Tp>;
#  else
template <class _Tp>
concept __fcxx_optional_view = false;
#  endif

/* [range.ref.view], class template ref_view */
template <range _Rp>
    requires is_object_v<_Rp>
class ref_view : public view_interface<ref_view<_Rp> > {
    _Rp* __r_;

    static void __fun(_Rp&);
    static void __fun(_Rp&&) = delete;

public:
    template <__fcxx_different_from<ref_view> _Tp>
        requires convertible_to<_Tp, _Rp&> && requires { __fun(std::declval<_Tp>()); }
    constexpr ref_view(_Tp&& __t) noexcept(noexcept(static_cast<_Rp&>(std::declval<_Tp>())))
        : __r_(std::addressof(static_cast<_Rp&>(std::forward<_Tp>(__t)))) {}

    constexpr _Rp& base() const { return *__r_; }

    constexpr iterator_t<_Rp> begin() const { return ranges::begin(*__r_); }
    constexpr sentinel_t<_Rp> end() const { return ranges::end(*__r_); }

    constexpr bool empty() const
        requires requires(_Rp& __r) { ranges::empty(__r); }
    {
        return ranges::empty(*__r_);
    }

    constexpr auto size() const
        requires sized_range<_Rp>
    {
        return ranges::size(*__r_);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint() const
        requires approximately_sized_range<_Rp>
    {
        return ranges::reserve_hint(*__r_);
    }
#  endif

    constexpr auto data() const
        requires contiguous_range<_Rp>
    {
        return ranges::data(*__r_);
    }
};

template <class _Rp>
ref_view(_Rp&) -> ref_view<_Rp>;

template <class _Tp>
inline constexpr bool enable_borrowed_range<ref_view<_Tp> > = true;

/* [range.owning.view], class template owning_view */
template <range _Rp>
    requires movable<_Rp> && (!__fcxx_is_initializer_list<_Rp>)
class owning_view : public view_interface<owning_view<_Rp> > {
    _Rp __r_ = _Rp();

public:
    owning_view()
        requires default_initializable<_Rp>
    = default;

    constexpr owning_view(_Rp&& __t) noexcept(is_nothrow_move_constructible_v<_Rp>) : __r_(std::move(__t)) {}

    owning_view(owning_view&&) = default;
    owning_view& operator=(owning_view&&) = default;

    constexpr _Rp& base() & noexcept { return __r_; }
    constexpr const _Rp& base() const& noexcept { return __r_; }
    constexpr _Rp&& base() && noexcept { return std::move(__r_); }
    constexpr const _Rp&& base() const&& noexcept { return std::move(__r_); }

    constexpr iterator_t<_Rp> begin() { return ranges::begin(__r_); }
    constexpr sentinel_t<_Rp> end() { return ranges::end(__r_); }

    constexpr auto begin() const
        requires range<const _Rp>
    {
        return ranges::begin(__r_);
    }

    constexpr auto end() const
        requires range<const _Rp>
    {
        return ranges::end(__r_);
    }

    constexpr bool empty()
        requires requires(_Rp& __r) { ranges::empty(__r); }
    {
        return ranges::empty(__r_);
    }

    constexpr bool empty() const
        requires requires(const _Rp& __r) { ranges::empty(__r); }
    {
        return ranges::empty(__r_);
    }

    constexpr auto size()
        requires sized_range<_Rp>
    {
        return ranges::size(__r_);
    }

    constexpr auto size() const
        requires sized_range<const _Rp>
    {
        return ranges::size(__r_);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Rp>
    {
        return ranges::reserve_hint(__r_);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Rp>
    {
        return ranges::reserve_hint(__r_);
    }
#  endif

    constexpr auto data()
        requires contiguous_range<_Rp>
    {
        return ranges::data(__r_);
    }

    constexpr auto data() const
        requires contiguous_range<const _Rp>
    {
        return ranges::data(__r_);
    }
};

template <class _Tp>
inline constexpr bool enable_borrowed_range<owning_view<_Tp> > = enable_borrowed_range<_Tp>;

/* [range.all], all view */
namespace views {
struct __all_fn : __fcxx_adaptor_closure<__all_fn> {
    /* 0: a view, decay-copied; 1: a ref_view; 2: an owning_view. */
    template <class _Rp>
    static constexpr int __kind() noexcept {
        if constexpr (view<decay_t<_Rp> >) {
            return 0;
        } else if constexpr (requires { ref_view{std::declval<_Rp>()}; }) {
            return 1;
        } else {
            return 2;
        }
    }

    template <class _Rp>
    static constexpr bool __nothrow() noexcept {
        if constexpr (__kind<_Rp>() == 0) {
            return is_nothrow_constructible_v<decay_t<_Rp>, _Rp>;
        } else if constexpr (__kind<_Rp>() == 1) {
            return noexcept(ref_view{std::declval<_Rp>()});
        } else {
            return noexcept(owning_view{std::declval<_Rp>()});
        }
    }

    template <viewable_range _Rp>
    constexpr auto operator()(_Rp&& __r) const noexcept(__nothrow<_Rp>()) {
        if constexpr (__kind<_Rp>() == 0) {
            return static_cast<decay_t<_Rp> >(std::forward<_Rp>(__r));
        } else if constexpr (__kind<_Rp>() == 1) {
            return ref_view{std::forward<_Rp>(__r)};
        } else {
            return owning_view{std::forward<_Rp>(__r)};
        }
    }
};

inline constexpr __all_fn all{};

template <viewable_range _Rp>
using all_t = decltype(views::all(std::declval<_Rp>()));
} /* namespace views */

/* [range.as.rvalue.view], class template as_rvalue_view */
template <view _Vp>
    requires input_range<_Vp>
class as_rvalue_view : public view_interface<as_rvalue_view<_Vp> > {
    _Vp __base_ = _Vp();

public:
    as_rvalue_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit as_rvalue_view(_Vp __base) : __base_(std::move(__base)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return move_iterator(ranges::begin(__base_));
    }

    constexpr auto begin() const
        requires range<const _Vp>
    {
        return move_iterator(ranges::begin(__base_));
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        if constexpr (common_range<_Vp>) {
            return move_iterator(ranges::end(__base_));
        } else {
            return move_sentinel(ranges::end(__base_));
        }
    }

    constexpr auto end() const
        requires range<const _Vp>
    {
        if constexpr (common_range<const _Vp>) {
            return move_iterator(ranges::end(__base_));
        } else {
            return move_sentinel(ranges::end(__base_));
        }
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::size(__base_);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::size(__base_);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        return ranges::reserve_hint(__base_);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        return ranges::reserve_hint(__base_);
    }
#  endif
};

template <class _Rp>
as_rvalue_view(_Rp&&) -> as_rvalue_view<views::all_t<_Rp> >;

template <class _Tp>
inline constexpr bool enable_borrowed_range<as_rvalue_view<_Tp> > = enable_borrowed_range<_Tp>;

namespace views {
/* views::as_rvalue is views::all for the input ranges whose elements are
 * rvalues already, as LWG 4083 has it. */
template <class _Rp>
concept __fcxx_rvalue_elements = input_range<_Rp> && same_as<range_rvalue_reference_t<_Rp>, range_reference_t<_Rp> >;

struct __as_rvalue_fn : __fcxx_adaptor_closure<__as_rvalue_fn> {
    template <class _Rp>
        requires __fcxx_rvalue_elements<_Rp> && requires { views::all(std::declval<_Rp>()); }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(views::all(std::forward<_Rp>(__r)))) {
        return views::all(std::forward<_Rp>(__r));
    }

    template <class _Rp>
        requires(!__fcxx_rvalue_elements<_Rp>) && requires { as_rvalue_view(std::declval<_Rp>()); }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(as_rvalue_view(std::forward<_Rp>(__r)))) {
        return as_rvalue_view(std::forward<_Rp>(__r));
    }
};

inline constexpr __as_rvalue_fn as_rvalue{};
} /* namespace views */

/* The iterator category of the iterators that have one only where the view
 * they adapt is a forward range, as P2259 has it: that of filter_view. */
template <class _Base>
struct __fcxx_filter_category {};

template <forward_range _Base>
struct __fcxx_filter_category<_Base> {
private:
    typedef typename iterator_traits<iterator_t<_Base> >::iterator_category _Cp;

public:
    using iterator_category =
        conditional_t<derived_from<_Cp, bidirectional_iterator_tag>, bidirectional_iterator_tag,
                      conditional_t<derived_from<_Cp, forward_iterator_tag>, forward_iterator_tag, _Cp> >;
};

/* [range.filter.view], class template filter_view. Its begin() caches its
 * result for forward ranges. A const view of an input range that is not a
 * forward one, and whose predicate is const-invocable, can be iterated, as
 * P3725, a defect report, has it.
 *
 * Workaround: Clang 12 to 15, all targets: it substitutes into the whole
 * requires-clause of a non-template member of a class template when it
 * instantiates the class, and so fails, rather than leave the member out,
 * where a conjunct after one that is false names a type that is not valid
 * for the class ("no type named ..."); Clang 16 does not. The constraints of
 * the members that name the types of the const view are concepts of their
 * own, which Clang substitutes into as a whole, here and in the views below
 * (the concepts named __fcxx_*_const_iterable). */
template <class _Vp, class _Pred>
concept __fcxx_filter_const_iterable = input_range<const _Vp> && !forward_range<const _Vp>
                                    && indirect_unary_predicate<const _Pred, iterator_t<const _Vp> >;

template <input_range _Vp, indirect_unary_predicate<iterator_t<_Vp> > _Pred>
    requires view<_Vp> && is_object_v<_Pred>
class filter_view : public view_interface<filter_view<_Vp, _Pred> > {
    /* [range.filter.iterator], class template filter_view::iterator */
    template <bool _Const>
    class __iterator : public __fcxx_filter_category<__fcxx_maybe_const<_Const, _Vp> > {
        friend filter_view;

        template <bool>
        friend class __iterator;

        using _Parent = __fcxx_maybe_const<_Const, filter_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        iterator_t<_Base> __current_ = iterator_t<_Base>();
        _Parent* __parent_ = nullptr;

        constexpr __iterator(_Parent& __parent, iterator_t<_Base> __current)
            : __current_(std::move(__current)), __parent_(std::addressof(__parent)) {}

    public:
        using iterator_concept =
            conditional_t<_Const, input_iterator_tag,
                          conditional_t<bidirectional_range<_Vp>, bidirectional_iterator_tag,
                                        conditional_t<forward_range<_Vp>, forward_iterator_tag, input_iterator_tag> > >;
        using value_type = range_value_t<_Base>;
        using difference_type = range_difference_t<_Base>;

        __iterator()
            requires default_initializable<iterator_t<_Base> >
        = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base> >
            : __current_(std::move(__i.__current_)), __parent_(__i.__parent_) {}

        constexpr const iterator_t<_Base>& base() const& noexcept { return __current_; }
        constexpr iterator_t<_Base> base() && { return std::move(__current_); }

        constexpr range_reference_t<_Base> operator*() const { return *__current_; }

        constexpr iterator_t<_Base> operator->() const
            requires __fcxx_has_arrow<iterator_t<_Base> > && copyable<iterator_t<_Base> >
        {
            return __current_;
        }

        constexpr __iterator& operator++() {
            __current_ = ranges::__fcxx_find_if(std::move(++__current_), ranges::end(__parent_->__base_),
                                                *__parent_->__pred_);
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
            do {
                --__current_;
            } while (!std::__fcxx_invoke(*__parent_->__pred_, *__current_));
            return *this;
        }

        constexpr __iterator operator--(int)
            requires bidirectional_range<_Base>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires equality_comparable<iterator_t<_Base> >
        {
            return __x.__current_ == __y.__current_;
        }

        friend constexpr range_rvalue_reference_t<_Base> iter_move(const __iterator& __i) noexcept(
            noexcept(ranges::iter_move(__i.__current_))) {
            return ranges::iter_move(__i.__current_);
        }

        friend constexpr void iter_swap(const __iterator& __x, const __iterator& __y) noexcept(
            noexcept(ranges::iter_swap(__x.__current_, __y.__current_)))
            requires indirectly_swappable<iterator_t<_Base> >
        {
            ranges::iter_swap(__x.__current_, __y.__current_);
        }
    };

    /* [range.filter.sentinel], class template filter_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend filter_view;

        template <bool>
        friend class __sentinel;

        using _Parent = __fcxx_maybe_const<_Const, filter_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        sentinel_t<_Base> __end_ = sentinel_t<_Base>();

        constexpr explicit __sentinel(_Parent& __parent) : __end_(ranges::end(__parent.__base_)) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __other)
            requires _Const && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __end_(std::move(__other.__end_)) {}

        constexpr sentinel_t<_Base> base() const { return __end_; }

        template <bool _OtherConst>
            requires sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.base() == __y.__end_;
        }
    };

    _Vp __base_ = _Vp();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_movable_box<_Pred> __pred_;
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<forward_range<_Vp>, iterator_t<_Vp> > __begin_;

public:
    filter_view()
        requires default_initializable<_Vp> && default_initializable<_Pred>
    = default;

    constexpr explicit filter_view(_Vp __base, _Pred __pred) : __base_(std::move(__base)), __pred_(std::move(__pred)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr const _Pred& pred() const { return *__pred_; }

    constexpr __iterator<false> begin() {
        if constexpr (forward_range<_Vp>) {
            if (!__begin_.has_value()) {
                __begin_ = ranges::__fcxx_find_if(ranges::begin(__base_), ranges::end(__base_), *__pred_);
            }
            return __iterator<false>(*this, *__begin_);
        } else {
            return __iterator<false>(*this,
                                     ranges::__fcxx_find_if(ranges::begin(__base_), ranges::end(__base_), *__pred_));
        }
    }

    constexpr __iterator<true> begin() const
        requires __fcxx_filter_const_iterable<_Vp, _Pred>
    {
        return __iterator<true>(*this, ranges::__fcxx_find_if(ranges::begin(__base_), ranges::end(__base_), *__pred_));
    }

    constexpr auto end() {
        if constexpr (common_range<_Vp>) {
            return __iterator<false>(*this, ranges::end(__base_));
        } else {
            return __sentinel<false>(*this);
        }
    }

    constexpr __sentinel<true> end() const
        requires __fcxx_filter_const_iterable<_Vp, _Pred>
    {
        return __sentinel<true>(*this);
    }
};

template <class _Rp, class _Pred>
filter_view(_Rp&&, _Pred) -> filter_view<views::all_t<_Rp>, _Pred>;

namespace views {
struct __filter_fn {
    template <class _Rp, class _Pred>
        requires requires { filter_view(std::declval<_Rp>(), std::declval<_Pred>()); }
    constexpr auto operator()(_Rp&& __r, _Pred&& __pred) const
        noexcept(noexcept(filter_view(std::forward<_Rp>(__r), std::forward<_Pred>(__pred)))) {
        return filter_view(std::forward<_Rp>(__r), std::forward<_Pred>(__pred));
    }

    template <class _Pred>
        requires __fcxx_can_bind<__filter_fn, _Pred>
    constexpr auto operator()(_Pred&& __pred) const noexcept(is_nothrow_constructible_v<decay_t<_Pred>, _Pred>) {
        return ranges::__fcxx_bind_adaptor<__filter_fn>(std::forward<_Pred>(__pred));
    }
};

inline constexpr __filter_fn filter{};
} /* namespace views */

/* [range.transform.iterator], the iterator category of the iterator of
 * transform_view. */
template <class _Base, class _Fn>
struct __fcxx_transform_category {};

template <forward_range _Base, class _Fn>
struct __fcxx_transform_category<_Base, _Fn> {
private:
    typedef typename iterator_traits<iterator_t<_Base> >::iterator_category _Cp;

public:
    using iterator_category =
        conditional_t<is_reference_v<invoke_result_t<_Fn&, range_reference_t<_Base> > >,
                      conditional_t<derived_from<_Cp, contiguous_iterator_tag>, random_access_iterator_tag, _Cp>,
                      input_iterator_tag>;
};

/* The iterator concept of the iterators of the views whose iterators are
 * as strong as those of the view they adapt. */
template <class _Base>
using __fcxx_iterator_concept_of =
    conditional_t<random_access_range<_Base>, random_access_iterator_tag,
                  conditional_t<bidirectional_range<_Base>, bidirectional_iterator_tag,
                                conditional_t<forward_range<_Base>, forward_iterator_tag, input_iterator_tag> > >;

/* [range.transform.view], class template transform_view.
 *
 * Workaround: Clang 12 to 15, all targets: the constraints of the members of
 * the const view are a concept of their own (see filter_view). */
template <class _Vp, class _Fp>
concept __fcxx_transform_const_iterable =
    range<const _Vp> && regular_invocable<const _Fp&, range_reference_t<const _Vp>>;

template <input_range _Vp, move_constructible _Fp>
    requires view<_Vp> && is_object_v<_Fp> && regular_invocable<_Fp&, range_reference_t<_Vp> >
          && __fcxx_can_reference<invoke_result_t<_Fp&, range_reference_t<_Vp> > >
class transform_view : public view_interface<transform_view<_Vp, _Fp> > {
    /* [range.transform.iterator], class template transform_view::iterator */
    template <bool _Const>
    class __iterator
        : public __fcxx_transform_category<__fcxx_maybe_const<_Const, _Vp>, __fcxx_maybe_const<_Const, _Fp> > {
        friend transform_view;

        template <bool>
        friend class __iterator;

        using _Parent = __fcxx_maybe_const<_Const, transform_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;
        using _Fn = __fcxx_maybe_const<_Const, _Fp>;

        iterator_t<_Base> __current_ = iterator_t<_Base>();
        _Parent* __parent_ = nullptr;

        constexpr __iterator(_Parent& __parent, iterator_t<_Base> __current)
            : __current_(std::move(__current)), __parent_(std::addressof(__parent)) {}

    public:
        using iterator_concept = __fcxx_iterator_concept_of<_Base>;
        using value_type = remove_cvref_t<invoke_result_t<_Fn&, range_reference_t<_Base> > >;
        using difference_type = range_difference_t<_Base>;

        __iterator()
            requires default_initializable<iterator_t<_Base> >
        = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base> >
            : __current_(std::move(__i.__current_)), __parent_(__i.__parent_) {}

        constexpr const iterator_t<_Base>& base() const& noexcept { return __current_; }
        constexpr iterator_t<_Base> base() && { return std::move(__current_); }

        constexpr decltype(auto) operator*() const noexcept(noexcept(std::__fcxx_invoke(
            std::declval<_Fn&>(), *std::declval<const iterator_t<_Base>&>()))) {
            return std::__fcxx_invoke(*__parent_->__fun_, *__current_);
        }

        constexpr __iterator& operator++() {
            ++__current_;
            return *this;
        }

        constexpr void operator++(int) { ++__current_; }

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
            --__current_;
            return *this;
        }

        constexpr __iterator operator--(int)
            requires bidirectional_range<_Base>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        constexpr __iterator& operator+=(difference_type __n)
            requires random_access_range<_Base>
        {
            __current_ += __n;
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __n)
            requires random_access_range<_Base>
        {
            __current_ -= __n;
            return *this;
        }

        constexpr decltype(auto) operator[](difference_type __n) const
            requires random_access_range<_Base>
        {
            return std::__fcxx_invoke(*__parent_->__fun_, __current_[__n]);
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires equality_comparable<iterator_t<_Base> >
        {
            return __x.__current_ == __y.__current_;
        }

        friend constexpr bool operator<(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __x.__current_ < __y.__current_;
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
            return __x.__current_ <=> __y.__current_;
        }

        friend constexpr __iterator operator+(__iterator __i, difference_type __n)
            requires random_access_range<_Base>
        {
            return __iterator(*__i.__parent_, __i.__current_ + __n);
        }

        friend constexpr __iterator operator+(difference_type __n, __iterator __i)
            requires random_access_range<_Base>
        {
            return __iterator(*__i.__parent_, __i.__current_ + __n);
        }

        friend constexpr __iterator operator-(__iterator __i, difference_type __n)
            requires random_access_range<_Base>
        {
            return __iterator(*__i.__parent_, __i.__current_ - __n);
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y)
            requires sized_sentinel_for<iterator_t<_Base>, iterator_t<_Base> >
        {
            return __x.__current_ - __y.__current_;
        }
    };

    /* [range.transform.sentinel], class template transform_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend transform_view;

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

        constexpr sentinel_t<_Base> base() const { return __end_; }

        template <bool _OtherConst>
            requires sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.base() == __y.__end_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _Vp> > operator-(
            const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.base() - __y.__end_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _Vp> > operator-(
            const __sentinel& __y, const __iterator<_OtherConst>& __x) {
            return __y.__end_ - __x.base();
        }
    };

    _Vp __base_ = _Vp();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_movable_box<_Fp> __fun_;

public:
    transform_view()
        requires default_initializable<_Vp> && default_initializable<_Fp>
    = default;

    constexpr explicit transform_view(_Vp __base, _Fp __fun) : __base_(std::move(__base)), __fun_(std::move(__fun)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr __iterator<false> begin() { return __iterator<false>(*this, ranges::begin(__base_)); }

    constexpr __iterator<true> begin() const
        requires __fcxx_transform_const_iterable<_Vp, _Fp>
    {
        return __iterator<true>(*this, ranges::begin(__base_));
    }

#  if _FCXX_RANGES_ONE_OVERLOAD
    /* Workaround: GCC 11 to 13 and Clang 12 to 17, all targets: each pair of
     * end() as one function, as their mangled names are the same with these
     * compilers (see ranges_util.h). */
    constexpr auto end() {
        if constexpr (common_range<_Vp>) {
            return __iterator<false>(*this, ranges::end(__base_));
        } else {
            return __sentinel<false>(ranges::end(__base_));
        }
    }

    constexpr auto end() const
        requires __fcxx_transform_const_iterable<_Vp, _Fp>
    {
        if constexpr (common_range<const _Vp>) {
            return __iterator<true>(*this, ranges::end(__base_));
        } else {
            return __sentinel<true>(ranges::end(__base_));
        }
    }
#  else
    constexpr __sentinel<false> end() { return __sentinel<false>(ranges::end(__base_)); }

    constexpr __iterator<false> end()
        requires common_range<_Vp>
    {
        return __iterator<false>(*this, ranges::end(__base_));
    }

    constexpr __sentinel<true> end() const
        requires __fcxx_transform_const_iterable<_Vp, _Fp>
    {
        return __sentinel<true>(ranges::end(__base_));
    }

    constexpr __iterator<true> end() const
        requires common_range<const _Vp> && __fcxx_transform_const_iterable<_Vp, _Fp>
    {
        return __iterator<true>(*this, ranges::end(__base_));
    }
#  endif

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::size(__base_);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::size(__base_);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        return ranges::reserve_hint(__base_);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        return ranges::reserve_hint(__base_);
    }
#  endif
};

template <class _Rp, class _Fp>
transform_view(_Rp&&, _Fp) -> transform_view<views::all_t<_Rp>, _Fp>;

namespace views {
struct __transform_fn {
    template <class _Rp, class _Fp>
        requires requires { transform_view(std::declval<_Rp>(), std::declval<_Fp>()); }
    constexpr auto operator()(_Rp&& __r, _Fp&& __f) const
        noexcept(noexcept(transform_view(std::forward<_Rp>(__r), std::forward<_Fp>(__f)))) {
        return transform_view(std::forward<_Rp>(__r), std::forward<_Fp>(__f));
    }

    template <class _Fp>
        requires __fcxx_can_bind<__transform_fn, _Fp>
    constexpr auto operator()(_Fp&& __f) const noexcept(is_nothrow_constructible_v<decay_t<_Fp>, _Fp>) {
        return ranges::__fcxx_bind_adaptor<__transform_fn>(std::forward<_Fp>(__f));
    }
};

inline constexpr __transform_fn transform{};
} /* namespace views */

/* [range.take.view], class template take_view */
template <view _Vp>
class take_view : public view_interface<take_view<_Vp> > {
    /* [range.take.sentinel], class template take_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend take_view;

        template <bool>
        friend class __sentinel;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        template <bool _OtherConst>
        using _CI = counted_iterator<iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >;

        sentinel_t<_Base> __end_ = sentinel_t<_Base>();

        constexpr explicit __sentinel(sentinel_t<_Base> __end) : __end_(__end) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __s)
            requires _Const && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __end_(std::move(__s.__end_)) {}

        constexpr sentinel_t<_Base> base() const { return __end_; }

        friend constexpr bool operator==(const _CI<_Const>& __y, const __sentinel& __x) {
            return __y.count() == 0 || __y.base() == __x.__end_;
        }

        template <bool _OtherConst = !_Const>
            requires sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr bool operator==(const _CI<_OtherConst>& __y, const __sentinel& __x) {
            return __y.count() == 0 || __y.base() == __x.__end_;
        }
    };

    _Vp __base_ = _Vp();
    range_difference_t<_Vp> __count_ = 0;

public:
    take_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit take_view(_Vp __base, range_difference_t<_Vp> __count)
        : __base_(std::move(__base)), __count_(__count) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        if constexpr (sized_range<_Vp>) {
            if constexpr (random_access_range<_Vp>) {
                return ranges::begin(__base_);
            } else {
                auto __sz = range_difference_t<_Vp>(size());
                return counted_iterator(ranges::begin(__base_), __sz);
            }
        } else if constexpr (sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >) {
            auto __it = ranges::begin(__base_);
            auto __sz = ranges::__fcxx_min(__count_, range_difference_t<_Vp>(ranges::end(__base_) - __it));
            return counted_iterator(std::move(__it), __sz);
        } else {
            return counted_iterator(ranges::begin(__base_), __count_);
        }
    }

    constexpr auto begin() const
        requires range<const _Vp>
    {
        if constexpr (sized_range<const _Vp>) {
            if constexpr (random_access_range<const _Vp>) {
                return ranges::begin(__base_);
            } else {
                auto __sz = range_difference_t<const _Vp>(size());
                return counted_iterator(ranges::begin(__base_), __sz);
            }
        } else if constexpr (sized_sentinel_for<sentinel_t<const _Vp>, iterator_t<const _Vp> >) {
            auto __it = ranges::begin(__base_);
            auto __sz = ranges::__fcxx_min(__count_, range_difference_t<const _Vp>(ranges::end(__base_) - __it));
            return counted_iterator(std::move(__it), __sz);
        } else {
            return counted_iterator(ranges::begin(__base_), __count_);
        }
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        if constexpr (sized_range<_Vp>) {
            if constexpr (random_access_range<_Vp>) {
                return ranges::begin(__base_) + range_difference_t<_Vp>(size());
            } else {
                return default_sentinel;
            }
        } else if constexpr (sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >) {
            return default_sentinel;
        } else {
            return __sentinel<false>{ranges::end(__base_)};
        }
    }

    constexpr auto end() const
        requires range<const _Vp>
    {
        if constexpr (sized_range<const _Vp>) {
            if constexpr (random_access_range<const _Vp>) {
                return ranges::begin(__base_) + range_difference_t<const _Vp>(size());
            } else {
                return default_sentinel;
            }
        } else if constexpr (sized_sentinel_for<sentinel_t<const _Vp>, iterator_t<const _Vp> >) {
            return default_sentinel;
        } else {
            return __sentinel<true>{ranges::end(__base_)};
        }
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        auto __n = ranges::size(__base_);
        return ranges::__fcxx_min(__n, static_cast<decltype(__n)>(__count_));
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        auto __n = ranges::size(__base_);
        return ranges::__fcxx_min(__n, static_cast<decltype(__n)>(__count_));
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint() {
        if constexpr (approximately_sized_range<_Vp>) {
            auto __n = static_cast<range_difference_t<_Vp> >(ranges::reserve_hint(__base_));
            return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_min(__n, __count_));
        } else {
            return ranges::__fcxx_to_unsigned_like(__count_);
        }
    }

    constexpr auto reserve_hint() const {
        if constexpr (approximately_sized_range<const _Vp>) {
            auto __n = static_cast<range_difference_t<const _Vp> >(ranges::reserve_hint(__base_));
            return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_min(__n, __count_));
        } else {
            return ranges::__fcxx_to_unsigned_like(__count_);
        }
    }
#  endif
};

template <class _Rp>
take_view(_Rp&&, range_difference_t<_Rp>) -> take_view<views::all_t<_Rp> >;

template <class _Tp>
inline constexpr bool enable_borrowed_range<take_view<_Tp> > = enable_borrowed_range<_Tp>;

namespace views {
/* What views::take and views::drop give for a view of a type, which is a
 * view of that kind itself for some of them. */
enum __fcxx_slice_kind { __slice_empty, __slice_optional, __slice_same, __slice_sized_subrange, __slice_iota,
                         __slice_repeat_sized, __slice_repeat, __slice_view };

template <class _Tp, bool _Drop>
constexpr __fcxx_slice_kind __fcxx_slice_kind_of() noexcept {
    if constexpr (__fcxx_is_empty_view<_Tp>) {
        return __slice_empty;
    } else if constexpr (__fcxx_optional_view<_Tp>) {
        return __slice_optional;
    } else if constexpr (random_access_range<_Tp> && sized_range<_Tp>
                         && (__fcxx_is_span<_Tp> || __fcxx_is_string_view<_Tp>
                             || (_Drop && __fcxx_is_iota_view<_Tp>)
                             || (__fcxx_is_subrange<_Tp> && !(_Drop && __fcxx_subrange_stores_size<_Tp>)))) {
        return __slice_same;
    } else if constexpr (_Drop && __fcxx_is_subrange<_Tp> && random_access_range<_Tp> && sized_range<_Tp>) {
        return __slice_sized_subrange;
    } else if constexpr (!_Drop && __fcxx_is_iota_view<_Tp> && random_access_range<_Tp> && sized_range<_Tp>) {
        return __slice_iota;
    } else if constexpr (__fcxx_is_repeat_view<_Tp> && sized_range<_Tp>) {
        return __slice_repeat_sized;
    } else if constexpr (__fcxx_is_repeat_view<_Tp>) {
        return __slice_repeat;
    } else {
        return __slice_view;
    }
}

/* The type of the view of the same kind: a span of the elements of a span,
 * and otherwise the type itself, or a subrange of the iterator of a
 * subrange for views::take. */
template <class _Tp, bool _Drop>
struct __fcxx_slice_type {
    using type = _Tp;
};

template <class _Tp, size_t _Extent, bool _Drop>
struct __fcxx_slice_type<span<_Tp, _Extent>, _Drop> {
    using type = span<_Tp>;
};

template <class _It, class _Sp, subrange_kind _Kp>
struct __fcxx_slice_type<subrange<_It, _Sp, _Kp>, false> {
    using type = subrange<_It>;
};

template <class _Rp, class _Np>
concept __fcxx_takes_count = range<_Rp> && convertible_to<_Np, range_difference_t<_Rp> >;

struct __take_fn {
    template <class _Rp, class _Np, __fcxx_slice_kind _Kind = __fcxx_slice_kind_of<remove_cvref_t<_Rp>, false>()>
    static constexpr bool __valid() noexcept {
        if constexpr (_Kind == __slice_empty || _Kind == __slice_optional) {
            return true;
        } else if constexpr (_Kind == __slice_view) {
            return requires { take_view(std::declval<_Rp>(), std::declval<_Np>()); };
        } else {
            return true;
        }
    }

    /* Whether the expression each kind gives cannot throw, as the call is
     * expression-equivalent to it. */
    template <class _Rp, class _Np>
    static constexpr bool __nothrow() noexcept {
        using _Tp = remove_cvref_t<_Rp>;
        using _Dp = range_difference_t<_Rp>;
        constexpr __fcxx_slice_kind _Kind = __fcxx_slice_kind_of<_Tp, false>();
        if constexpr (_Kind == __slice_empty) {
            return is_nothrow_constructible_v<_Tp, _Rp>;
        } else if constexpr (_Kind == __slice_optional) {
            return noexcept(static_cast<_Dp>(std::declval<_Np>()) == _Dp()) && is_nothrow_default_constructible_v<_Tp>
                && is_nothrow_constructible_v<_Tp, _Rp>;
        } else if constexpr (_Kind == __slice_same) {
            using _Up = typename __fcxx_slice_type<_Tp, false>::type;
            return noexcept(_Up(ranges::begin(std::declval<_Rp&>()),
                                ranges::begin(std::declval<_Rp&>())
                                    + ranges::__fcxx_min<_Dp>(ranges::distance(std::declval<_Rp&>()),
                                                              static_cast<_Dp>(std::declval<_Np>()))));
        } else if constexpr (_Kind == __slice_iota) {
            return noexcept(iota_view(*ranges::begin(std::declval<_Rp&>()),
                                      *(ranges::begin(std::declval<_Rp&>())
                                        + ranges::__fcxx_min<_Dp>(ranges::distance(std::declval<_Rp&>()),
                                                                  static_cast<_Dp>(std::declval<_Np>())))));
        } else if constexpr (_Kind == __slice_repeat_sized) {
            return noexcept(views::repeat(std::declval<_Rp>().__fcxx_value(),
                                          ranges::__fcxx_min<_Dp>(ranges::distance(std::declval<_Rp&>()),
                                                                  static_cast<_Dp>(std::declval<_Np>()))));
        } else if constexpr (_Kind == __slice_repeat) {
            return noexcept(views::repeat(std::declval<_Rp>().__fcxx_value(), static_cast<_Dp>(std::declval<_Np>())));
        } else {
            return noexcept(take_view(std::declval<_Rp>(), std::declval<_Np>()));
        }
    }

    template <class _Rp, class _Np>
        requires __fcxx_takes_count<_Rp, _Np> && (__valid<_Rp, _Np>())
    constexpr auto operator()(_Rp&& __e, _Np&& __f) const noexcept(__nothrow<_Rp, _Np>()) {
        using _Tp = remove_cvref_t<_Rp>;
        using _Dp = range_difference_t<_Rp>;
        constexpr __fcxx_slice_kind _Kind = __fcxx_slice_kind_of<_Tp, false>();
        if constexpr (_Kind == __slice_empty) {
            (void)__f;
            return static_cast<_Tp>(std::forward<_Rp>(__e));
        } else if constexpr (_Kind == __slice_optional) {
            return static_cast<_Dp>(std::forward<_Np>(__f)) == _Dp() ? ((void)__e, _Tp())
                                                                    : static_cast<_Tp>(std::forward<_Rp>(__e));
        } else if constexpr (_Kind == __slice_same) {
            using _Up = typename __fcxx_slice_type<_Tp, false>::type;
            return _Up(ranges::begin(__e),
                       ranges::begin(__e)
                           + ranges::__fcxx_min<_Dp>(ranges::distance(__e), static_cast<_Dp>(std::forward<_Np>(__f))));
        } else if constexpr (_Kind == __slice_iota) {
            return iota_view(
                *ranges::begin(__e),
                *(ranges::begin(__e)
                  + ranges::__fcxx_min<_Dp>(ranges::distance(__e), static_cast<_Dp>(std::forward<_Np>(__f)))));
        } else if constexpr (_Kind == __slice_repeat_sized) {
            auto __n = ranges::__fcxx_min<_Dp>(ranges::distance(__e), static_cast<_Dp>(std::forward<_Np>(__f)));
            return views::repeat(std::forward<_Rp>(__e).__fcxx_value(), __n);
        } else if constexpr (_Kind == __slice_repeat) {
            return views::repeat(std::forward<_Rp>(__e).__fcxx_value(), static_cast<_Dp>(std::forward<_Np>(__f)));
        } else {
            return take_view(std::forward<_Rp>(__e), std::forward<_Np>(__f));
        }
    }

    template <class _Np>
        requires __fcxx_can_bind<__take_fn, _Np>
    constexpr auto operator()(_Np&& __n) const noexcept(is_nothrow_constructible_v<decay_t<_Np>, _Np>) {
        return ranges::__fcxx_bind_adaptor<__take_fn>(std::forward<_Np>(__n));
    }
};

inline constexpr __take_fn take{};
} /* namespace views */

/* [range.take.while.view], class template take_while_view.
 *
 * Workaround: Clang 12 to 15, all targets: the constraints of the members of
 * the const view are a concept of their own (see filter_view). */
template <class _Vp, class _Pred>
concept __fcxx_take_while_const_iterable =
    range<const _Vp> && indirect_unary_predicate<const _Pred, iterator_t<const _Vp>>;

template <view _Vp, class _Pred>
    requires input_range<_Vp> && is_object_v<_Pred> && indirect_unary_predicate<const _Pred, iterator_t<_Vp> >
class take_while_view : public view_interface<take_while_view<_Vp, _Pred> > {
    /* [range.take.while.sentinel], class template take_while_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend take_while_view;

        template <bool>
        friend class __sentinel;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        sentinel_t<_Base> __end_ = sentinel_t<_Base>();
        const _Pred* __pred_ = nullptr;

        constexpr explicit __sentinel(sentinel_t<_Base> __end, const _Pred* __pred) : __end_(__end), __pred_(__pred) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __s)
            requires _Const && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __end_(std::move(__s.__end_)), __pred_(__s.__pred_) {}

        constexpr sentinel_t<_Base> base() const { return __end_; }

        friend constexpr bool operator==(const iterator_t<_Base>& __x, const __sentinel& __y) {
            return __y.__end_ == __x || !std::__fcxx_invoke(*__y.__pred_, *__x);
        }

        template <bool _OtherConst = !_Const>
            requires sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr bool operator==(const iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> >& __x,
                                         const __sentinel& __y) {
            return __y.__end_ == __x || !std::__fcxx_invoke(*__y.__pred_, *__x);
        }
    };

    _Vp __base_ = _Vp();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_movable_box<_Pred> __pred_;

public:
    take_while_view()
        requires default_initializable<_Vp> && default_initializable<_Pred>
    = default;

    constexpr explicit take_while_view(_Vp __base, _Pred __pred)
        : __base_(std::move(__base)), __pred_(std::move(__pred)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr const _Pred& pred() const { return *__pred_; }

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return ranges::begin(__base_);
    }

    constexpr auto begin() const
        requires __fcxx_take_while_const_iterable<_Vp, _Pred>
    {
        return ranges::begin(__base_);
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return __sentinel<false>(ranges::end(__base_), std::addressof(*__pred_));
    }

    constexpr auto end() const
        requires __fcxx_take_while_const_iterable<_Vp, _Pred>
    {
        return __sentinel<true>(ranges::end(__base_), std::addressof(*__pred_));
    }
};

template <class _Rp, class _Pred>
take_while_view(_Rp&&, _Pred) -> take_while_view<views::all_t<_Rp>, _Pred>;

namespace views {
struct __take_while_fn {
    template <class _Rp, class _Pred>
        requires requires { take_while_view(std::declval<_Rp>(), std::declval<_Pred>()); }
    constexpr auto operator()(_Rp&& __r, _Pred&& __pred) const
        noexcept(noexcept(take_while_view(std::forward<_Rp>(__r), std::forward<_Pred>(__pred)))) {
        return take_while_view(std::forward<_Rp>(__r), std::forward<_Pred>(__pred));
    }

    template <class _Pred>
        requires __fcxx_can_bind<__take_while_fn, _Pred>
    constexpr auto operator()(_Pred&& __pred) const noexcept(is_nothrow_constructible_v<decay_t<_Pred>, _Pred>) {
        return ranges::__fcxx_bind_adaptor<__take_while_fn>(std::forward<_Pred>(__pred));
    }
};

inline constexpr __take_while_fn take_while{};
} /* namespace views */

/* [range.drop.view], class template drop_view. Its begin() caches its
 * result for the forward ranges that cannot find it in constant time. */
template <view _Vp>
class drop_view : public view_interface<drop_view<_Vp> > {
    static constexpr bool __caches = forward_range<_Vp> && !(random_access_range<_Vp> && sized_range<_Vp>);

    _Vp __base_ = _Vp();
    range_difference_t<_Vp> __count_ = 0;
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<__caches, iterator_t<_Vp> > __begin_;

public:
    drop_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit drop_view(_Vp __base, range_difference_t<_Vp> __count)
        : __base_(std::move(__base)), __count_(__count) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin()
        requires(!(__fcxx_simple_view<_Vp> && random_access_range<const _Vp> && sized_range<const _Vp>))
    {
        if constexpr (__caches) {
            if (!__begin_.has_value()) {
                __begin_ = ranges::next(ranges::begin(__base_), __count_, ranges::end(__base_));
            }
            return *__begin_;
        } else if constexpr (random_access_range<_Vp> && sized_range<_Vp>) {
            /* The same iterator, in constant time, where the sentinel may
             * not be one that can be subtracted from it. */
            return ranges::begin(__base_) + ranges::__fcxx_min(__count_, ranges::distance(__base_));
        } else {
            return ranges::next(ranges::begin(__base_), __count_, ranges::end(__base_));
        }
    }

    constexpr auto begin() const
        requires random_access_range<const _Vp> && sized_range<const _Vp>
    {
        using _Dp = range_difference_t<const _Vp>;
        return ranges::begin(__base_) + ranges::__fcxx_min(static_cast<_Dp>(__count_), ranges::distance(__base_));
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return ranges::end(__base_);
    }

    constexpr auto end() const
        requires range<const _Vp>
    {
        return ranges::end(__base_);
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        const auto __s = ranges::size(__base_);
        const auto __c = static_cast<remove_cv_t<decltype(__s)> >(__count_);
        return __s < __c ? 0 : __s - __c;
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        const auto __s = ranges::size(__base_);
        const auto __c = static_cast<remove_cv_t<decltype(__s)> >(__count_);
        return __s < __c ? 0 : __s - __c;
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        const auto __s = static_cast<range_difference_t<_Vp> >(ranges::reserve_hint(__base_));
        return ranges::__fcxx_to_unsigned_like(__s < __count_ ? 0 : __s - __count_);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        const auto __s = static_cast<range_difference_t<const _Vp> >(ranges::reserve_hint(__base_));
        return ranges::__fcxx_to_unsigned_like(__s < __count_ ? 0 : __s - __count_);
    }
#  endif
};

template <class _Rp>
drop_view(_Rp&&, range_difference_t<_Rp>) -> drop_view<views::all_t<_Rp> >;

template <class _Tp>
inline constexpr bool enable_borrowed_range<drop_view<_Tp> > = enable_borrowed_range<_Tp>;

namespace views {
struct __drop_fn {
    template <class _Rp, class _Np, __fcxx_slice_kind _Kind = __fcxx_slice_kind_of<remove_cvref_t<_Rp>, true>()>
    static constexpr bool __valid() noexcept {
        if constexpr (_Kind == __slice_view) {
            return requires { drop_view(std::declval<_Rp>(), std::declval<_Np>()); };
        } else {
            return true;
        }
    }

    /* Whether the expression each kind gives cannot throw, as the call is
     * expression-equivalent to it. */
    template <class _Rp, class _Np>
    static constexpr bool __nothrow() noexcept {
        using _Tp = remove_cvref_t<_Rp>;
        using _Dp = range_difference_t<_Rp>;
        constexpr __fcxx_slice_kind _Kind = __fcxx_slice_kind_of<_Tp, true>();
        if constexpr (_Kind == __slice_empty || _Kind == __slice_repeat) {
            return is_nothrow_constructible_v<_Tp, _Rp>;
        } else if constexpr (_Kind == __slice_optional) {
            return noexcept(static_cast<_Dp>(std::declval<_Np>()) == _Dp()) && is_nothrow_default_constructible_v<_Tp>
                && is_nothrow_constructible_v<_Tp, _Rp>;
        } else if constexpr (_Kind == __slice_same) {
            using _Up = typename __fcxx_slice_type<_Tp, true>::type;
            return noexcept(_Up(ranges::begin(std::declval<_Rp&>())
                                    + ranges::__fcxx_min<_Dp>(ranges::distance(std::declval<_Rp&>()),
                                                              static_cast<_Dp>(std::declval<_Np>())),
                                ranges::end(std::declval<_Rp&>())));
        } else if constexpr (_Kind == __slice_sized_subrange) {
            return noexcept(_Tp(ranges::begin(std::declval<_Rp&>()) + std::declval<_Dp&>(),
                                ranges::end(std::declval<_Rp&>()),
                                ranges::__fcxx_to_unsigned_like(ranges::distance(std::declval<_Rp&>())
                                                                - ranges::__fcxx_min<_Dp>(
                                                                    ranges::distance(std::declval<_Rp&>()),
                                                                    static_cast<_Dp>(std::declval<_Np>())))));
        } else if constexpr (_Kind == __slice_repeat_sized) {
            return noexcept(views::repeat(std::declval<_Rp>().__fcxx_value(),
                                          ranges::distance(std::declval<_Rp&>())
                                              - ranges::__fcxx_min<_Dp>(ranges::distance(std::declval<_Rp&>()),
                                                                        static_cast<_Dp>(std::declval<_Np>()))));
        } else {
            return noexcept(drop_view(std::declval<_Rp>(), std::declval<_Np>()));
        }
    }

    template <class _Rp, class _Np>
        requires __fcxx_takes_count<_Rp, _Np> && (__valid<_Rp, _Np>())
    constexpr auto operator()(_Rp&& __e, _Np&& __f) const noexcept(__nothrow<_Rp, _Np>()) {
        using _Tp = remove_cvref_t<_Rp>;
        using _Dp = range_difference_t<_Rp>;
        constexpr __fcxx_slice_kind _Kind = __fcxx_slice_kind_of<_Tp, true>();
        if constexpr (_Kind == __slice_empty || _Kind == __slice_repeat) {
            (void)__f;
            return static_cast<_Tp>(std::forward<_Rp>(__e));
        } else if constexpr (_Kind == __slice_optional) {
            return static_cast<_Dp>(std::forward<_Np>(__f)) == _Dp() ? static_cast<_Tp>(std::forward<_Rp>(__e))
                                                                    : ((void)__e, _Tp());
        } else if constexpr (_Kind == __slice_same) {
            using _Up = typename __fcxx_slice_type<_Tp, true>::type;
            return _Up(ranges::begin(__e)
                           + ranges::__fcxx_min<_Dp>(ranges::distance(__e), static_cast<_Dp>(std::forward<_Np>(__f))),
                       ranges::end(__e));
        } else if constexpr (_Kind == __slice_sized_subrange) {
            auto __dist = ranges::distance(__e);
            auto __n = ranges::__fcxx_min<_Dp>(__dist, static_cast<_Dp>(std::forward<_Np>(__f)));
            return _Tp(ranges::begin(__e) + __n, ranges::end(__e), ranges::__fcxx_to_unsigned_like(__dist - __n));
        } else if constexpr (_Kind == __slice_repeat_sized) {
            auto __dist = ranges::distance(__e);
            auto __n = __dist - ranges::__fcxx_min<_Dp>(__dist, static_cast<_Dp>(std::forward<_Np>(__f)));
            return views::repeat(std::forward<_Rp>(__e).__fcxx_value(), __n);
        } else {
            return drop_view(std::forward<_Rp>(__e), std::forward<_Np>(__f));
        }
    }

    template <class _Np>
        requires __fcxx_can_bind<__drop_fn, _Np>
    constexpr auto operator()(_Np&& __n) const noexcept(is_nothrow_constructible_v<decay_t<_Np>, _Np>) {
        return ranges::__fcxx_bind_adaptor<__drop_fn>(std::forward<_Np>(__n));
    }
};

inline constexpr __drop_fn drop{};
} /* namespace views */

/* [range.drop.while.view], class template drop_while_view. Its begin()
 * caches its result for forward ranges. */
template <view _Vp, class _Pred>
    requires input_range<_Vp> && is_object_v<_Pred> && indirect_unary_predicate<const _Pred, iterator_t<_Vp> >
class drop_while_view : public view_interface<drop_while_view<_Vp, _Pred> > {
    _Vp __base_ = _Vp();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_movable_box<_Pred> __pred_;
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<forward_range<_Vp>, iterator_t<_Vp> > __begin_;

public:
    drop_while_view()
        requires default_initializable<_Vp> && default_initializable<_Pred>
    = default;

    constexpr explicit drop_while_view(_Vp __base, _Pred __pred)
        : __base_(std::move(__base)), __pred_(std::move(__pred)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr const _Pred& pred() const { return *__pred_; }

    constexpr auto begin() {
        const _Pred& __pred = *__pred_;
        if constexpr (forward_range<_Vp>) {
            if (!__begin_.has_value()) {
                __begin_ = ranges::__fcxx_find_if_not(ranges::begin(__base_), ranges::end(__base_), __pred);
            }
            return *__begin_;
        } else {
            return ranges::__fcxx_find_if_not(ranges::begin(__base_), ranges::end(__base_), __pred);
        }
    }

    constexpr auto end() { return ranges::end(__base_); }
};

template <class _Rp, class _Pred>
drop_while_view(_Rp&&, _Pred) -> drop_while_view<views::all_t<_Rp>, _Pred>;

template <class _Tp, class _Pred>
inline constexpr bool enable_borrowed_range<drop_while_view<_Tp, _Pred> > = enable_borrowed_range<_Tp>;

namespace views {
struct __drop_while_fn {
    template <class _Rp, class _Pred>
        requires requires { drop_while_view(std::declval<_Rp>(), std::declval<_Pred>()); }
    constexpr auto operator()(_Rp&& __r, _Pred&& __pred) const
        noexcept(noexcept(drop_while_view(std::forward<_Rp>(__r), std::forward<_Pred>(__pred)))) {
        return drop_while_view(std::forward<_Rp>(__r), std::forward<_Pred>(__pred));
    }

    template <class _Pred>
        requires __fcxx_can_bind<__drop_while_fn, _Pred>
    constexpr auto operator()(_Pred&& __pred) const noexcept(is_nothrow_constructible_v<decay_t<_Pred>, _Pred>) {
        return ranges::__fcxx_bind_adaptor<__drop_while_fn>(std::forward<_Pred>(__pred));
    }
};

inline constexpr __drop_while_fn drop_while{};

/* [range.counted], views::counted */
struct __counted_fn {
    template <class _It, class _Np>
        requires input_or_output_iterator<decay_t<_It> > && convertible_to<_Np, iter_difference_t<decay_t<_It> > >
    constexpr auto operator()(_It&& __e, _Np&& __f) const {
        using _Tp = decay_t<_It>;
        using _Dp = iter_difference_t<_Tp>;
        if constexpr (contiguous_iterator<_Tp>) {
            return span(std::to_address(__e), static_cast<size_t>(static_cast<_Dp>(std::forward<_Np>(__f))));
        } else if constexpr (random_access_iterator<_Tp>) {
            return subrange(__e, __e + static_cast<_Dp>(std::forward<_Np>(__f)));
        } else {
            return subrange(counted_iterator(std::forward<_It>(__e), std::forward<_Np>(__f)), default_sentinel);
        }
    }
};

inline constexpr __counted_fn counted{};
} /* namespace views */

/* [range.common.view], class template common_view. Its non-const begin()
 * and end() are left out for simple views, as LWG 4012 has it. */
template <view _Vp>
    requires(!common_range<_Vp> && copyable<iterator_t<_Vp> >)
class common_view : public view_interface<common_view<_Vp> > {
    _Vp __base_ = _Vp();

public:
    common_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit common_view(_Vp __r) : __base_(std::move(__r)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        if constexpr (random_access_range<_Vp> && sized_range<_Vp>) {
            return ranges::begin(__base_);
        } else {
            return common_iterator<iterator_t<_Vp>, sentinel_t<_Vp> >(ranges::begin(__base_));
        }
    }

    constexpr auto begin() const
        requires range<const _Vp>
    {
        if constexpr (random_access_range<const _Vp> && sized_range<const _Vp>) {
            return ranges::begin(__base_);
        } else {
            return common_iterator<iterator_t<const _Vp>, sentinel_t<const _Vp> >(ranges::begin(__base_));
        }
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        if constexpr (random_access_range<_Vp> && sized_range<_Vp>) {
            return ranges::begin(__base_) + ranges::distance(__base_);
        } else {
            return common_iterator<iterator_t<_Vp>, sentinel_t<_Vp> >(ranges::end(__base_));
        }
    }

    constexpr auto end() const
        requires range<const _Vp>
    {
        if constexpr (random_access_range<const _Vp> && sized_range<const _Vp>) {
            return ranges::begin(__base_) + ranges::distance(__base_);
        } else {
            return common_iterator<iterator_t<const _Vp>, sentinel_t<const _Vp> >(ranges::end(__base_));
        }
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::size(__base_);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::size(__base_);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        return ranges::reserve_hint(__base_);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        return ranges::reserve_hint(__base_);
    }
#  endif
};

template <class _Rp>
common_view(_Rp&&) -> common_view<views::all_t<_Rp> >;

template <class _Tp>
inline constexpr bool enable_borrowed_range<common_view<_Tp> > = enable_borrowed_range<_Tp>;

namespace views {
template <class _Rp>
concept __fcxx_common_as_all = common_range<_Rp> && requires { views::all(std::declval<_Rp>()); };

struct __common_fn : __fcxx_adaptor_closure<__common_fn> {
    template <class _Rp>
        requires __fcxx_common_as_all<_Rp>
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(views::all(std::forward<_Rp>(__r)))) {
        return views::all(std::forward<_Rp>(__r));
    }

    template <class _Rp>
        requires(!__fcxx_common_as_all<_Rp>) && requires { common_view{std::declval<_Rp>()}; }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(common_view{std::forward<_Rp>(__r)})) {
        return common_view{std::forward<_Rp>(__r)};
    }
};

inline constexpr __common_fn common{};
} /* namespace views */

/* [range.reverse.view], class template reverse_view. Its begin() caches its
 * result for the views that are not common ranges. */
template <view _Vp>
    requires bidirectional_range<_Vp>
class reverse_view : public view_interface<reverse_view<_Vp> > {
    _Vp __base_ = _Vp();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<!common_range<_Vp>, iterator_t<_Vp> > __end_;

public:
    reverse_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit reverse_view(_Vp __r) : __base_(std::move(__r)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    /* It is only chosen where V is not common, but is there for the common
     * ones too, which have no cache. */
    constexpr reverse_iterator<iterator_t<_Vp> > begin() {
        if constexpr (common_range<_Vp>) {
            return std::make_reverse_iterator(ranges::end(__base_));
        } else {
            if (!__end_.has_value()) {
                __end_ = ranges::next(ranges::begin(__base_), ranges::end(__base_));
            }
            return std::make_reverse_iterator(*__end_);
        }
    }

#  if !_FCXX_RANGES_ONE_OVERLOAD
    /* Workaround: GCC 11 to 13 and Clang 12 to 17, all targets: left out, as
     * its mangled name is that of the one above with these compilers, which
     * does what it does (see ranges_util.h). */
    constexpr reverse_iterator<iterator_t<_Vp> > begin()
        requires common_range<_Vp>
    {
        return std::make_reverse_iterator(ranges::end(__base_));
    }
#  endif

    constexpr auto begin() const
        requires common_range<const _Vp>
    {
        return std::make_reverse_iterator(ranges::end(__base_));
    }

    constexpr reverse_iterator<iterator_t<_Vp> > end() { return std::make_reverse_iterator(ranges::begin(__base_)); }

    constexpr auto end() const
        requires common_range<const _Vp>
    {
        return std::make_reverse_iterator(ranges::begin(__base_));
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::size(__base_);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::size(__base_);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        return ranges::reserve_hint(__base_);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        return ranges::reserve_hint(__base_);
    }
#  endif
};

template <class _Rp>
reverse_view(_Rp&&) -> reverse_view<views::all_t<_Rp> >;

template <class _Tp>
inline constexpr bool enable_borrowed_range<reverse_view<_Tp> > = enable_borrowed_range<_Tp>;

template <class _Tp>
inline constexpr bool __fcxx_is_reverse_view = false;

template <class _Vp>
inline constexpr bool __fcxx_is_reverse_view<reverse_view<_Vp> > = true;

/* Whether a type is a subrange of reverse iterators of the same type. */
template <class _Tp>
inline constexpr bool __fcxx_is_reversed_subrange = false;

template <class _It, subrange_kind _Kp>
inline constexpr bool __fcxx_is_reversed_subrange<subrange<reverse_iterator<_It>, reverse_iterator<_It>, _Kp> > = true;

template <class _Tp>
struct __fcxx_unreversed_subrange {};

template <class _It, subrange_kind _Kp>
struct __fcxx_unreversed_subrange<subrange<reverse_iterator<_It>, reverse_iterator<_It>, _Kp> > {
    using type = subrange<_It, _It, _Kp>;
    static constexpr bool __sized = _Kp == subrange_kind::sized;
};

namespace views {
struct __reverse_fn : __fcxx_adaptor_closure<__reverse_fn> {
    template <class _Rp>
        requires __fcxx_is_reverse_view<remove_cvref_t<_Rp> > && requires { std::declval<_Rp>().base(); }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(std::forward<_Rp>(__r).base())) {
        return std::forward<_Rp>(__r).base();
    }

    template <class _Rp>
        requires(!__fcxx_is_reverse_view<remove_cvref_t<_Rp> >) && __fcxx_optional_view<remove_cvref_t<_Rp> >
    constexpr auto operator()(_Rp&& __r) const
        noexcept(is_nothrow_constructible_v<remove_cvref_t<_Rp>, _Rp>) {
        return static_cast<remove_cvref_t<_Rp> >(std::forward<_Rp>(__r));
    }

    template <class _Rp>
        requires __fcxx_is_reversed_subrange<remove_cvref_t<_Rp> >
    constexpr auto operator()(_Rp&& __r) const {
        using _Sub = __fcxx_unreversed_subrange<remove_cvref_t<_Rp> >;
        if constexpr (_Sub::__sized) {
            return typename _Sub::type(__r.end().base(), __r.begin().base(), __r.size());
        } else {
            return typename _Sub::type(__r.end().base(), __r.begin().base());
        }
    }

    template <class _Rp>
        requires(!__fcxx_is_reverse_view<remove_cvref_t<_Rp> >) && (!__fcxx_optional_view<remove_cvref_t<_Rp> >)
             && (!__fcxx_is_reversed_subrange<remove_cvref_t<_Rp> >) && requires { reverse_view{std::declval<_Rp>()}; }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(reverse_view{std::forward<_Rp>(__r)})) {
        return reverse_view{std::forward<_Rp>(__r)};
    }
};

inline constexpr __reverse_fn reverse{};
} /* namespace views */

/* [range.as.const.view], class template as_const_view */
template <view _Vp>
    requires input_range<_Vp>
class as_const_view : public view_interface<as_const_view<_Vp> > {
    _Vp __base_ = _Vp();

public:
    as_const_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit as_const_view(_Vp __base) : __base_(std::move(__base)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return ranges::cbegin(__base_);
    }

    constexpr auto begin() const
        requires range<const _Vp>
    {
        return ranges::cbegin(__base_);
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return ranges::cend(__base_);
    }

    constexpr auto end() const
        requires range<const _Vp>
    {
        return ranges::cend(__base_);
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::size(__base_);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::size(__base_);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        return ranges::reserve_hint(__base_);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        return ranges::reserve_hint(__base_);
    }
#  endif
};

template <class _Rp>
as_const_view(_Rp&&) -> as_const_view<views::all_t<_Rp> >;

template <class _Tp>
inline constexpr bool enable_borrowed_range<as_const_view<_Tp> > = enable_borrowed_range<_Tp>;

namespace views {
/* What views::as_const gives for an expression of a type. */
enum __fcxx_as_const_kind { __as_const_all, __as_const_empty, __as_const_optional_ref, __as_const_span,
                            __as_const_ref_view, __as_const_ref, __as_const_view };

template <class _Tp>
struct __fcxx_as_const_types {};

template <class _Xp>
struct __fcxx_as_const_types<empty_view<_Xp> > {
    using type = empty_view<const _Xp>;
};

template <class _Xp, size_t _Extent>
struct __fcxx_as_const_types<span<_Xp, _Extent> > {
    using type = span<const _Xp, _Extent>;
};

template <class _Xp>
struct __fcxx_as_const_types<ref_view<_Xp> > {
    using type = const _Xp;
};

#  if _FCXX_STD >= 2026
template <class _Xp>
struct __fcxx_as_const_types<optional<_Xp&> > {
    using type = optional<const _Xp&>;
};
#  endif

template <class _Tp>
inline constexpr bool __fcxx_is_ref_view = false;

template <class _Xp>
inline constexpr bool __fcxx_is_ref_view<ref_view<_Xp> > = true;

template <class _Tp>
inline constexpr bool __fcxx_is_optional_ref = false;

#  if _FCXX_STD >= 2026
template <class _Xp>
inline constexpr bool __fcxx_is_optional_ref<optional<_Xp&> > = true;
#  endif

template <class _Rp>
concept __fcxx_all_constant = constant_range<all_t<_Rp> >;

template <class _Up>
concept __fcxx_ref_view_of_constant =
    __fcxx_is_ref_view<_Up> && constant_range<typename __fcxx_as_const_types<_Up>::type>;

template <class _Rp, class _Up = remove_cvref_t<_Rp> >
constexpr __fcxx_as_const_kind __fcxx_as_const_kind_of() noexcept {
    if constexpr (__fcxx_all_constant<_Rp>) {
        return __as_const_all;
    } else if constexpr (__fcxx_is_empty_view<_Up>) {
        return __as_const_empty;
    } else if constexpr (__fcxx_is_optional_ref<_Up>) {
        return __as_const_optional_ref;
    } else if constexpr (__fcxx_is_span<_Up>) {
        return __as_const_span;
    } else if constexpr (__fcxx_ref_view_of_constant<_Up>) {
        return __as_const_ref_view;
    } else if constexpr (is_lvalue_reference_v<_Rp> && constant_range<const _Up> && !view<_Up>) {
        return __as_const_ref;
    } else {
        return __as_const_view;
    }
}

struct __as_const_fn : __fcxx_adaptor_closure<__as_const_fn> {
    template <class _Rp>
    static constexpr bool __valid() noexcept {
        constexpr __fcxx_as_const_kind _Kind = __fcxx_as_const_kind_of<_Rp>();
        if constexpr (_Kind == __as_const_all) {
            return true;
        } else if constexpr (_Kind == __as_const_view) {
            return requires { as_const_view(std::declval<_Rp>()); };
        } else {
            return true;
        }
    }

    /* Whether the expression each kind gives cannot throw, as the call is
     * expression-equivalent to it. */
    template <class _Rp>
    static constexpr bool __nothrow() noexcept {
        using _Up = remove_cvref_t<_Rp>;
        constexpr __fcxx_as_const_kind _Kind = __fcxx_as_const_kind_of<_Rp>();
        if constexpr (_Kind == __as_const_all) {
            return noexcept(views::all(std::declval<_Rp>()));
        } else if constexpr (_Kind == __as_const_empty) {
            return true;
        } else if constexpr (_Kind == __as_const_optional_ref || _Kind == __as_const_span) {
            return noexcept(typename __fcxx_as_const_types<_Up>::type(std::declval<_Rp>()));
        } else if constexpr (_Kind == __as_const_ref_view) {
            return noexcept(
                ref_view(static_cast<typename __fcxx_as_const_types<_Up>::type&>(std::declval<_Rp>().base())));
        } else if constexpr (_Kind == __as_const_ref) {
            return noexcept(ref_view(static_cast<const _Up&>(std::declval<_Rp>())));
        } else {
            return noexcept(as_const_view(std::declval<_Rp>()));
        }
    }

    template <class _Rp>
        requires(__valid<_Rp>())
    constexpr auto operator()(_Rp&& __r) const noexcept(__nothrow<_Rp>()) {
        using _Up = remove_cvref_t<_Rp>;
        constexpr __fcxx_as_const_kind _Kind = __fcxx_as_const_kind_of<_Rp>();
        if constexpr (_Kind == __as_const_all) {
            return views::all(std::forward<_Rp>(__r));
        } else if constexpr (_Kind == __as_const_empty) {
            return typename __fcxx_as_const_types<_Up>::type();
        } else if constexpr (_Kind == __as_const_optional_ref || _Kind == __as_const_span) {
            return typename __fcxx_as_const_types<_Up>::type(std::forward<_Rp>(__r));
        } else if constexpr (_Kind == __as_const_ref_view) {
            return ref_view(static_cast<typename __fcxx_as_const_types<_Up>::type&>(std::forward<_Rp>(__r).base()));
        } else if constexpr (_Kind == __as_const_ref) {
            return ref_view(static_cast<const _Up&>(__r));
        } else {
            return as_const_view(std::forward<_Rp>(__r));
        }
    }
};

inline constexpr __as_const_fn as_const{};
} /* namespace views */

/* [range.elements.view], has-tuple-element and returnable-element. The
 * elements are those of the tuple-like types from C++23 on, as P2165 has
 * it, and of any type with the tuple protocol before. */
template <class _Tp, size_t _Np>
concept __fcxx_has_tuple_element = __fcxx_tuple_like<_Tp> && (_Np < tuple_size_v<_Tp>) && requires(_Tp __t) {
    { std::get<_Np>(__t) } -> convertible_to<const tuple_element_t<_Np, _Tp>&>;
};

template <class _Tp, size_t _Np>
concept __fcxx_returnable_element = is_reference_v<_Tp> || move_constructible<tuple_element_t<_Np, _Tp> >;

/* The iterator category of the iterator of elements_view. */
template <class _Base, size_t _Np>
struct __fcxx_elements_category {};

template <forward_range _Base, size_t _Np>
struct __fcxx_elements_category<_Base, _Np> {
private:
    typedef typename iterator_traits<iterator_t<_Base> >::iterator_category _Cp;

public:
    using iterator_category =
        conditional_t<!is_lvalue_reference_v<decltype(std::get<_Np>(*std::declval<iterator_t<_Base>&>()))>,
                      input_iterator_tag,
                      conditional_t<derived_from<_Cp, random_access_iterator_tag>, random_access_iterator_tag, _Cp> >;
};

/* [range.elements.view], class template elements_view */
template <input_range _Vp, size_t _Np>
    requires view<_Vp> && __fcxx_has_tuple_element<range_value_t<_Vp>, _Np>
          && __fcxx_has_tuple_element<remove_reference_t<range_reference_t<_Vp> >, _Np>
          && __fcxx_returnable_element<range_reference_t<_Vp>, _Np>
class elements_view : public view_interface<elements_view<_Vp, _Np> > {
    /* [range.elements.iterator], class template elements_view::iterator */
    template <bool _Const>
    class __iterator : public __fcxx_elements_category<__fcxx_maybe_const<_Const, _Vp>, _Np> {
        friend elements_view;

        template <bool>
        friend class __iterator;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        iterator_t<_Base> __current_ = iterator_t<_Base>();

        static constexpr decltype(auto) __get_element(const iterator_t<_Base>& __i) {
            if constexpr (is_reference_v<range_reference_t<_Base> >) {
                return std::get<_Np>(*__i);
            } else {
                using _Ep = remove_cv_t<tuple_element_t<_Np, range_reference_t<_Base> > >;
                return static_cast<_Ep>(std::get<_Np>(*__i));
            }
        }

        constexpr explicit __iterator(iterator_t<_Base> __current) : __current_(std::move(__current)) {}

    public:
        using iterator_concept = __fcxx_iterator_concept_of<_Base>;
        using value_type = remove_cvref_t<tuple_element_t<_Np, range_value_t<_Base> > >;
        using difference_type = range_difference_t<_Base>;

        __iterator()
            requires default_initializable<iterator_t<_Base> >
        = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base> >
            : __current_(std::move(__i.__current_)) {}

        constexpr const iterator_t<_Base>& base() const& noexcept { return __current_; }
        constexpr iterator_t<_Base> base() && { return std::move(__current_); }

        constexpr decltype(auto) operator*() const { return __get_element(__current_); }

        constexpr __iterator& operator++() {
            ++__current_;
            return *this;
        }

        constexpr void operator++(int) { ++__current_; }

        constexpr __iterator operator++(int)
            requires forward_range<_Base>
        {
            auto __temp = *this;
            ++__current_;
            return __temp;
        }

        constexpr __iterator& operator--()
            requires bidirectional_range<_Base>
        {
            --__current_;
            return *this;
        }

        constexpr __iterator operator--(int)
            requires bidirectional_range<_Base>
        {
            auto __temp = *this;
            --__current_;
            return __temp;
        }

        constexpr __iterator& operator+=(difference_type __n)
            requires random_access_range<_Base>
        {
            __current_ += __n;
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __n)
            requires random_access_range<_Base>
        {
            __current_ -= __n;
            return *this;
        }

        constexpr decltype(auto) operator[](difference_type __n) const
            requires random_access_range<_Base>
        {
            return __get_element(__current_ + __n);
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires equality_comparable<iterator_t<_Base> >
        {
            return __x.__current_ == __y.__current_;
        }

        friend constexpr bool operator<(const __iterator& __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __x.__current_ < __y.__current_;
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
            return __x.__current_ <=> __y.__current_;
        }

        friend constexpr __iterator operator+(const __iterator& __x, difference_type __y)
            requires random_access_range<_Base>
        {
            return __iterator{__x} += __y;
        }

        friend constexpr __iterator operator+(difference_type __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __y + __x;
        }

        friend constexpr __iterator operator-(const __iterator& __x, difference_type __y)
            requires random_access_range<_Base>
        {
            return __iterator{__x} -= __y;
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y)
            requires sized_sentinel_for<iterator_t<_Base>, iterator_t<_Base> >
        {
            return __x.__current_ - __y.__current_;
        }
    };

    /* [range.elements.sentinel], class template elements_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend elements_view;

        template <bool>
        friend class __sentinel;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        sentinel_t<_Base> __end_ = sentinel_t<_Base>();

        constexpr explicit __sentinel(sentinel_t<_Base> __end) : __end_(__end) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __other)
            requires _Const && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __end_(std::move(__other.__end_)) {}

        constexpr sentinel_t<_Base> base() const { return __end_; }

        template <bool _OtherConst>
            requires sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.base() == __y.__end_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _Vp> > operator-(
            const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.base() - __y.__end_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _Vp> > operator-(
            const __sentinel& __x, const __iterator<_OtherConst>& __y) {
            return __x.__end_ - __y.base();
        }
    };

    _Vp __base_ = _Vp();

public:
    elements_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit elements_view(_Vp __base) : __base_(std::move(__base)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return __iterator<false>(ranges::begin(__base_));
    }

    constexpr auto begin() const
        requires range<const _Vp>
    {
        return __iterator<true>(ranges::begin(__base_));
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp> && !common_range<_Vp>)
    {
        return __sentinel<false>{ranges::end(__base_)};
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp> && common_range<_Vp>)
    {
        return __iterator<false>{ranges::end(__base_)};
    }

#  if _FCXX_RANGES_ONE_OVERLOAD
    /* Workaround: GCC 11 to 13 and Clang 12 to 17, all targets: the two
     * end() const as one function, as their mangled names are the same with
     * these compilers (see ranges_util.h). */
    constexpr auto end() const
        requires range<const _Vp>
    {
        if constexpr (common_range<const _Vp>) {
            return __iterator<true>{ranges::end(__base_)};
        } else {
            return __sentinel<true>{ranges::end(__base_)};
        }
    }
#  else
    constexpr auto end() const
        requires range<const _Vp>
    {
        return __sentinel<true>{ranges::end(__base_)};
    }

    constexpr auto end() const
        requires common_range<const _Vp>
    {
        return __iterator<true>{ranges::end(__base_)};
    }
#  endif

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::size(__base_);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::size(__base_);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        return ranges::reserve_hint(__base_);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        return ranges::reserve_hint(__base_);
    }
#  endif
};

template <class _Tp, size_t _Np>
inline constexpr bool enable_borrowed_range<elements_view<_Tp, _Np> > = enable_borrowed_range<_Tp>;

/* keys_view and values_view name elements_view of the view itself, as LWG
 * 3563 has it. */
template <class _Rp>
using keys_view = elements_view<_Rp, 0>;

template <class _Rp>
using values_view = elements_view<_Rp, 1>;

namespace views {
template <size_t _Np>
struct __elements_fn : __fcxx_adaptor_closure<__elements_fn<_Np> > {
    template <class _Rp>
        requires requires { elements_view<all_t<_Rp>, _Np>{std::declval<_Rp>()}; }
    constexpr auto operator()(_Rp&& __r) const
        noexcept(noexcept(elements_view<all_t<_Rp>, _Np>{std::forward<_Rp>(__r)})) {
        return elements_view<all_t<_Rp>, _Np>{std::forward<_Rp>(__r)};
    }
};

template <size_t _Np>
inline constexpr __elements_fn<_Np> elements{};

inline constexpr auto keys = elements<0>;
inline constexpr auto values = elements<1>;
} /* namespace views */

/* [range.enumerate.view], class template enumerate_view */
template <view _Vp>
    requires __fcxx_range_with_movable_references<_Vp>
class enumerate_view : public view_interface<enumerate_view<_Vp> > {
    /* [range.enumerate.iterator], class template enumerate_view::iterator */
    template <bool _Const>
    class __iterator {
        friend enumerate_view;

        template <bool>
        friend class __iterator;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;

    public:
        using iterator_category = input_iterator_tag;
        using iterator_concept = __fcxx_iterator_concept_of<_Base>;
        using difference_type = range_difference_t<_Base>;
        using value_type = tuple<difference_type, range_value_t<_Base> >;

    private:
        using __ref_tuple = tuple<difference_type, range_reference_t<_Base> >;

        iterator_t<_Base> __current_ = iterator_t<_Base>();
        difference_type __pos_ = 0;

        constexpr explicit __iterator(iterator_t<_Base> __current, difference_type __pos)
            : __current_(std::move(__current)), __pos_(__pos) {}

    public:
        __iterator()
            requires default_initializable<iterator_t<_Base> >
        = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base> >
            : __current_(std::move(__i.__current_)), __pos_(__i.__pos_) {}

        constexpr const iterator_t<_Base>& base() const& noexcept { return __current_; }
        constexpr iterator_t<_Base> base() && { return std::move(__current_); }

        constexpr difference_type index() const noexcept { return __pos_; }

        constexpr auto operator*() const { return __ref_tuple(__pos_, *__current_); }

        constexpr __iterator& operator++() {
            ++__current_;
            ++__pos_;
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        constexpr __iterator operator++(int)
            requires forward_range<_Base>
        {
            auto __temp = *this;
            ++*this;
            return __temp;
        }

        constexpr __iterator& operator--()
            requires bidirectional_range<_Base>
        {
            --__current_;
            --__pos_;
            return *this;
        }

        constexpr __iterator operator--(int)
            requires bidirectional_range<_Base>
        {
            auto __temp = *this;
            --*this;
            return __temp;
        }

        constexpr __iterator& operator+=(difference_type __n)
            requires random_access_range<_Base>
        {
            __current_ += __n;
            __pos_ += __n;
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __n)
            requires random_access_range<_Base>
        {
            __current_ -= __n;
            __pos_ -= __n;
            return *this;
        }

        constexpr auto operator[](difference_type __n) const
            requires random_access_range<_Base>
        {
            return __ref_tuple(__pos_ + __n, __current_[__n]);
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y) noexcept {
            return __x.__pos_ == __y.__pos_;
        }

        friend constexpr strong_ordering operator<=>(const __iterator& __x, const __iterator& __y) noexcept {
            return __x.__pos_ <=> __y.__pos_;
        }

        friend constexpr __iterator operator+(const __iterator& __x, difference_type __y)
            requires random_access_range<_Base>
        {
            auto __temp = __x;
            __temp += __y;
            return __temp;
        }

        friend constexpr __iterator operator+(difference_type __x, const __iterator& __y)
            requires random_access_range<_Base>
        {
            return __y + __x;
        }

        friend constexpr __iterator operator-(const __iterator& __x, difference_type __y)
            requires random_access_range<_Base>
        {
            auto __temp = __x;
            __temp -= __y;
            return __temp;
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y) noexcept {
            return __x.__pos_ - __y.__pos_;
        }

        friend constexpr auto iter_move(const __iterator& __i) noexcept(
            noexcept(ranges::iter_move(__i.__current_))
            && is_nothrow_move_constructible_v<range_rvalue_reference_t<_Base> >) {
            return tuple<difference_type, range_rvalue_reference_t<_Base> >(__i.__pos_,
                                                                            ranges::iter_move(__i.__current_));
        }
    };

    /* [range.enumerate.sentinel], class template enumerate_view::sentinel */
    template <bool _Const>
    class __sentinel {
        friend enumerate_view;

        template <bool>
        friend class __sentinel;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        sentinel_t<_Base> __end_ = sentinel_t<_Base>();

        constexpr explicit __sentinel(sentinel_t<_Base> __end) : __end_(std::move(__end)) {}

    public:
        __sentinel() = default;

        constexpr __sentinel(__sentinel<!_Const> __other)
            requires _Const && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __end_(std::move(__other.__end_)) {}

        constexpr sentinel_t<_Base> base() const { return __end_; }

        template <bool _OtherConst>
            requires sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr bool operator==(const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.base() == __y.__end_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _Vp> > operator-(
            const __iterator<_OtherConst>& __x, const __sentinel& __y) {
            return __x.base() - __y.__end_;
        }

        template <bool _OtherConst>
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<__fcxx_maybe_const<_OtherConst, _Vp> > >
        friend constexpr range_difference_t<__fcxx_maybe_const<_OtherConst, _Vp> > operator-(
            const __sentinel& __x, const __iterator<_OtherConst>& __y) {
            return __x.__end_ - __y.base();
        }
    };

    _Vp __base_ = _Vp();

public:
    constexpr enumerate_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit enumerate_view(_Vp __base) : __base_(std::move(__base)) {}

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return __iterator<false>(ranges::begin(__base_), 0);
    }

    constexpr auto begin() const
        requires __fcxx_range_with_movable_references<const _Vp>
    {
        return __iterator<true>(ranges::begin(__base_), 0);
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        if constexpr (forward_range<_Vp> && common_range<_Vp> && sized_range<_Vp>) {
            return __iterator<false>(ranges::end(__base_), ranges::distance(__base_));
        } else {
            return __sentinel<false>(ranges::end(__base_));
        }
    }

    constexpr auto end() const
        requires __fcxx_range_with_movable_references<const _Vp>
    {
        if constexpr (forward_range<const _Vp> && common_range<const _Vp> && sized_range<const _Vp>) {
            return __iterator<true>(ranges::end(__base_), ranges::distance(__base_));
        } else {
            return __sentinel<true>(ranges::end(__base_));
        }
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::size(__base_);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::size(__base_);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        return ranges::reserve_hint(__base_);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        return ranges::reserve_hint(__base_);
    }
#  endif

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }
};

template <class _Rp>
enumerate_view(_Rp&&) -> enumerate_view<views::all_t<_Rp> >;

template <class _View>
inline constexpr bool enable_borrowed_range<enumerate_view<_View> > = enable_borrowed_range<_View>;

namespace views {
struct __enumerate_fn : __fcxx_adaptor_closure<__enumerate_fn> {
    template <class _Rp>
        requires requires { enumerate_view<all_t<_Rp> >(std::declval<_Rp>()); }
    constexpr auto operator()(_Rp&& __r) const
        noexcept(noexcept(enumerate_view<all_t<_Rp> >(std::forward<_Rp>(__r)))) {
        return enumerate_view<all_t<_Rp> >(std::forward<_Rp>(__r));
    }
};

inline constexpr __enumerate_fn enumerate{};
} /* namespace views */

} /* namespace ranges */
} /* namespace std */

#endif
