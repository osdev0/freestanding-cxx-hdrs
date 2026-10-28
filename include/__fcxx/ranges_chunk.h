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


/* [range.chunk], [range.slide], [range.chunk.by], [range.stride],
 * [range.cache.latest] and [range.as.input], the views of <ranges> of C++23
 * and C++26 that step over the elements of a range in other ways: chunk_view,
 * slide_view, chunk_by_view, stride_view, cache_latest_view and
 * as_input_view.
 *
 * Workaround: GCC 11 and 12, all targets: they do not give the hidden friends
 * of a class nested in another the access to the private members of the
 * enclosing class that the nested class has ("... is private within this
 * context"), which Clang and GCC 13 give them. The hidden friends of the
 * iterators do what needs the private members of the views in static member
 * functions of the iterators. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_CHUNK_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_CHUNK_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX
#include <__fcxx/integer_product.h>

#include <compare>
#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>
#include <__fcxx/addressof.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_adaptors.h>
#include <__fcxx/ranges_base.h>
#include <__fcxx/ranges_core.h>
#include <__fcxx/ranges_util.h>

namespace std {
namespace ranges {

/* [range.chunk.view.input], class template chunk_view for input ranges */
template <view _Vp>
    requires input_range<_Vp>
class chunk_view : public view_interface<chunk_view<_Vp> > {
    class __inner_iterator;

    /* [range.chunk.outer.iter], class chunk_view::outer-iterator */
    class __outer_iterator {
        friend chunk_view;

        chunk_view* __parent_;

        constexpr explicit __outer_iterator(chunk_view& __parent) : __parent_(std::addressof(__parent)) {}

        static constexpr bool __at_end(const __outer_iterator& __x) {
            return *__x.__parent_->__current_ == ranges::end(__x.__parent_->__base_)
                && __x.__parent_->__remainder_ != 0;
        }

        static constexpr range_difference_t<_Vp> __to_end(const __outer_iterator& __x)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            const auto __dist = ranges::end(__x.__parent_->__base_) - *__x.__parent_->__current_;
            if (__dist < __x.__parent_->__remainder_) {
                return __dist == 0 ? 0 : 1;
            }
            return ranges::__fcxx_div_ceil(__dist - __x.__parent_->__remainder_, __x.__parent_->__n_) + 1;
        }

    public:
        using iterator_concept = input_iterator_tag;
        using difference_type = range_difference_t<_Vp>;

        /* [range.chunk.outer.value], class
         * chunk_view::outer-iterator::value_type */
        struct value_type : view_interface<value_type> {
        private:
            friend __outer_iterator;

            chunk_view* __parent_;

            constexpr explicit value_type(chunk_view& __parent) : __parent_(std::addressof(__parent)) {}

        public:
            constexpr __inner_iterator begin() const noexcept { return chunk_view::__fcxx_make_inner(*__parent_); }

            constexpr default_sentinel_t end() const noexcept { return default_sentinel; }

            constexpr auto size() const
                requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
            {
                return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_min(
                    __parent_->__remainder_,
                    static_cast<range_difference_t<_Vp> >(ranges::end(__parent_->__base_) - *__parent_->__current_)));
            }

#  if _FCXX_STD >= 2026
            constexpr auto reserve_hint() const noexcept {
                return ranges::__fcxx_to_unsigned_like(__parent_->__remainder_);
            }
#  endif
        };

        __outer_iterator(__outer_iterator&&) = default;
        __outer_iterator& operator=(__outer_iterator&&) = default;

        constexpr value_type operator*() const { return value_type(*__parent_); }

        constexpr __outer_iterator& operator++() {
            ranges::advance(*__parent_->__current_, __parent_->__remainder_, ranges::end(__parent_->__base_));
            __parent_->__remainder_ = __parent_->__n_;
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        friend constexpr bool operator==(const __outer_iterator& __x, default_sentinel_t) { return __at_end(__x); }

        friend constexpr difference_type operator-(default_sentinel_t, const __outer_iterator& __x)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            return __to_end(__x);
        }

        friend constexpr difference_type operator-(const __outer_iterator& __x, default_sentinel_t)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            return -__to_end(__x);
        }
    };

    /* [range.chunk.inner.iter], class chunk_view::inner-iterator */
    class __inner_iterator {
        friend chunk_view;

        chunk_view* __parent_;

        constexpr explicit __inner_iterator(chunk_view& __parent) noexcept : __parent_(std::addressof(__parent)) {}

        static constexpr bool __at_end(const __inner_iterator& __x) { return __x.__parent_->__remainder_ == 0; }

        static constexpr range_difference_t<_Vp> __to_end(const __inner_iterator& __x)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            return ranges::__fcxx_min(
                __x.__parent_->__remainder_,
                static_cast<range_difference_t<_Vp>>(ranges::end(__x.__parent_->__base_) - *__x.__parent_->__current_));
        }

        static constexpr range_rvalue_reference_t<_Vp> __iter_move(const __inner_iterator& __i) {
            return ranges::iter_move(*__i.__parent_->__current_);
        }

        static constexpr void __iter_swap(const __inner_iterator& __x, const __inner_iterator& __y) {
            ranges::iter_swap(*__x.__parent_->__current_, *__y.__parent_->__current_);
        }

    public:
        using iterator_concept = input_iterator_tag;
        using difference_type = range_difference_t<_Vp>;
        using value_type = range_value_t<_Vp>;

        __inner_iterator(__inner_iterator&&) = default;
        __inner_iterator& operator=(__inner_iterator&&) = default;

        constexpr const iterator_t<_Vp>& base() const& { return *__parent_->__current_; }

        constexpr range_reference_t<_Vp> operator*() const { return **__parent_->__current_; }

        constexpr __inner_iterator& operator++() {
            ++*__parent_->__current_;
            if (*__parent_->__current_ == ranges::end(__parent_->__base_)) {
                __parent_->__remainder_ = 0;
            } else {
                --__parent_->__remainder_;
            }
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        friend constexpr bool operator==(const __inner_iterator& __x, default_sentinel_t) { return __at_end(__x); }

        friend constexpr difference_type operator-(default_sentinel_t, const __inner_iterator& __x)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            return __to_end(__x);
        }

        friend constexpr difference_type operator-(const __inner_iterator& __x, default_sentinel_t)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            return -__to_end(__x);
        }

        friend constexpr range_rvalue_reference_t<_Vp> iter_move(const __inner_iterator& __i) noexcept(
            noexcept(ranges::iter_move(std::declval<iterator_t<_Vp>&>()))) {
            return __iter_move(__i);
        }

        friend constexpr void iter_swap(const __inner_iterator& __x, const __inner_iterator& __y) noexcept(
            noexcept(ranges::iter_swap(std::declval<iterator_t<_Vp>&>(), std::declval<iterator_t<_Vp>&>())))
            requires indirectly_swappable<iterator_t<_Vp> >
        {
            __iter_swap(__x, __y);
        }
    };

    static constexpr __inner_iterator __fcxx_make_inner(chunk_view& __parent) noexcept {
        return __inner_iterator(__parent);
    }

    _Vp __base_;
    range_difference_t<_Vp> __n_;
    range_difference_t<_Vp> __remainder_ = 0;
    __fcxx_non_propagating_cache<iterator_t<_Vp> > __current_;

public:
    constexpr explicit chunk_view(_Vp __base, range_difference_t<_Vp> __n) : __base_(std::move(__base)), __n_(__n) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr __outer_iterator begin() {
        __current_ = ranges::begin(__base_);
        __remainder_ = __n_;
        return __outer_iterator(*this);
    }

    constexpr default_sentinel_t end() const noexcept { return default_sentinel; }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(ranges::distance(__base_), __n_));
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(ranges::distance(__base_), __n_));
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        auto __s = static_cast<range_difference_t<decltype((__base_))> >(ranges::reserve_hint(__base_));
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(__s, __n_));
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        auto __s = static_cast<range_difference_t<decltype((__base_))> >(ranges::reserve_hint(__base_));
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(__s, __n_));
    }
#  endif
};

/* [range.chunk.view.fwd], class template chunk_view for forward ranges */
template <view _Vp>
    requires forward_range<_Vp>
class chunk_view<_Vp> : public view_interface<chunk_view<_Vp> > {
    /* [range.chunk.fwd.iter], class template
     * chunk_view::iterator for forward ranges */
    template <bool _Const>
    class __iterator {
        friend chunk_view;

        template <bool>
        friend class __iterator;

        using _Parent = __fcxx_maybe_const<_Const, chunk_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        iterator_t<_Base> __current_ = iterator_t<_Base>();
        sentinel_t<_Base> __end_ = sentinel_t<_Base>();
        range_difference_t<_Base> __n_ = 0;
        range_difference_t<_Base> __missing_ = 0;

        constexpr __iterator(_Parent* __parent, iterator_t<_Base> __current, range_difference_t<_Base> __missing = 0)
            : __current_(__current), __end_(ranges::end(__parent->__base_)), __n_(__parent->__n_),
              __missing_(__missing) {}

    public:
        using iterator_category = input_iterator_tag;
        using iterator_concept =
            conditional_t<random_access_range<_Base>, random_access_iterator_tag,
                          conditional_t<bidirectional_range<_Base>, bidirectional_iterator_tag, forward_iterator_tag> >;
        using value_type = decltype(views::take(subrange(std::declval<iterator_t<_Base> >(),
                                                         std::declval<sentinel_t<_Base> >()),
                                                std::declval<range_difference_t<_Base> >()));
        using difference_type = range_difference_t<_Base>;

        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base> >
                  && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __current_(std::move(__i.__current_)), __end_(std::move(__i.__end_)), __n_(__i.__n_),
              __missing_(__i.__missing_) {}

        constexpr iterator_t<_Base> base() const { return __current_; }

        constexpr value_type operator*() const { return views::take(subrange(__current_, __end_), __n_); }

        constexpr __iterator& operator++() {
            __missing_ = ranges::advance(__current_, __n_, __end_);
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
            ranges::advance(__current_, __missing_ - __n_);
            __missing_ = 0;
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
            if (__x > 0) {
                ranges::advance(__current_, _FCXX_MUL(__n_, __x - 1));
                __missing_ = ranges::advance(__current_, __n_, __end_);
            } else if (__x < 0) {
                ranges::advance(__current_, _FCXX_MUL(__n_, __x) + __missing_);
                __missing_ = 0;
            }
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __x)
            requires random_access_range<_Base>
        {
            return *this += -__x;
        }

        constexpr value_type operator[](difference_type __n) const
            requires random_access_range<_Base>
        {
            return *(*this + __n);
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y) {
            return __x.__current_ == __y.__current_;
        }

        friend constexpr bool operator==(const __iterator& __x, default_sentinel_t) {
            return __x.__current_ == __x.__end_;
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
            return (__x.__current_ - __y.__current_ + __x.__missing_ - __y.__missing_) / __x.__n_;
        }

        friend constexpr difference_type operator-(default_sentinel_t, const __iterator& __x)
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<_Base> >
        {
            return ranges::__fcxx_div_ceil(static_cast<difference_type>(__x.__end_ - __x.__current_), __x.__n_);
        }

        friend constexpr difference_type operator-(const __iterator& __x, default_sentinel_t __y)
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<_Base> >
        {
            return -(__y - __x);
        }
    };

    _Vp __base_;
    range_difference_t<_Vp> __n_;

public:
    constexpr explicit chunk_view(_Vp __base, range_difference_t<_Vp> __n) : __base_(std::move(__base)), __n_(__n) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return __iterator<false>(this, ranges::begin(__base_));
    }

    constexpr auto begin() const
        requires forward_range<const _Vp>
    {
        return __iterator<true>(this, ranges::begin(__base_));
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        if constexpr (common_range<_Vp> && sized_range<_Vp>) {
            auto __missing = (__n_ - ranges::distance(__base_) % __n_) % __n_;
            return __iterator<false>(this, ranges::end(__base_), __missing);
        } else if constexpr (common_range<_Vp> && !bidirectional_range<_Vp>) {
            return __iterator<false>(this, ranges::end(__base_));
        } else {
            return default_sentinel;
        }
    }

    constexpr auto end() const
        requires forward_range<const _Vp>
    {
        if constexpr (common_range<const _Vp> && sized_range<const _Vp>) {
            auto __missing = (__n_ - ranges::distance(__base_) % __n_) % __n_;
            return __iterator<true>(this, ranges::end(__base_), __missing);
        } else if constexpr (common_range<const _Vp> && !bidirectional_range<const _Vp>) {
            return __iterator<true>(this, ranges::end(__base_));
        } else {
            return default_sentinel;
        }
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(ranges::distance(__base_), __n_));
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(ranges::distance(__base_), __n_));
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        auto __s = static_cast<range_difference_t<decltype((__base_))> >(ranges::reserve_hint(__base_));
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(__s, __n_));
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        auto __s = static_cast<range_difference_t<decltype((__base_))> >(ranges::reserve_hint(__base_));
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(__s, __n_));
    }
#  endif
};

template <class _Rp>
chunk_view(_Rp&&, range_difference_t<_Rp>) -> chunk_view<views::all_t<_Rp> >;

template <class _Vp>
inline constexpr bool enable_borrowed_range<chunk_view<_Vp> > = forward_range<_Vp> && enable_borrowed_range<_Vp>;

namespace views {
struct __chunk_fn {
    template <class _Rp, class _Np>
        requires requires { chunk_view(std::declval<_Rp>(), std::declval<_Np>()); }
    constexpr auto operator()(_Rp&& __r, _Np&& __n) const
        noexcept(noexcept(chunk_view(std::forward<_Rp>(__r), std::forward<_Np>(__n)))) {
        return chunk_view(std::forward<_Rp>(__r), std::forward<_Np>(__n));
    }

    template <class _Np>
        requires __fcxx_can_bind<__chunk_fn, _Np>
    constexpr auto operator()(_Np&& __n) const noexcept(is_nothrow_constructible_v<decay_t<_Np>, _Np>) {
        return ranges::__fcxx_bind_adaptor<__chunk_fn>(std::forward<_Np>(__n));
    }
};

inline constexpr __chunk_fn chunk{};
} /* namespace views */

/* [range.slide.view], what slide_view caches */
template <class _Vp>
concept __fcxx_slide_caches_nothing = random_access_range<_Vp> && sized_range<_Vp>;

template <class _Vp>
concept __fcxx_slide_caches_last = !__fcxx_slide_caches_nothing<_Vp> && bidirectional_range<_Vp> && common_range<_Vp>;

template <class _Vp>
concept __fcxx_slide_caches_first = !__fcxx_slide_caches_nothing<_Vp> && !__fcxx_slide_caches_last<_Vp>;

/* [range.slide.view], class template slide_view */
template <forward_range _Vp>
    requires view<_Vp>
class slide_view : public view_interface<slide_view<_Vp> > {
    class __sentinel;

    /* [range.slide.iterator], class template slide_view::iterator */
    template <bool _Const>
    class __iterator {
        friend slide_view;

        template <bool>
        friend class __iterator;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;
        using _LastStore = conditional_t<__fcxx_slide_caches_first<_Base>, iterator_t<_Base>, __fcxx_no_cache>;

        iterator_t<_Base> __current_ = iterator_t<_Base>();
        _FCXX_NO_UNIQUE_ADDRESS _LastStore __last_ele_ = _LastStore();
        range_difference_t<_Base> __n_ = 0;

        constexpr __iterator(iterator_t<_Base> __current, range_difference_t<_Base> __n)
            requires(!__fcxx_slide_caches_first<_Base>)
            : __current_(__current), __n_(__n) {}

        constexpr __iterator(iterator_t<_Base> __current, iterator_t<_Base> __last_ele, range_difference_t<_Base> __n)
            requires __fcxx_slide_caches_first<_Base>
            : __current_(__current), __last_ele_(__last_ele), __n_(__n) {}

    public:
        using iterator_category = input_iterator_tag;
        using iterator_concept =
            conditional_t<random_access_range<_Base>, random_access_iterator_tag,
                          conditional_t<bidirectional_range<_Base>, bidirectional_iterator_tag, forward_iterator_tag> >;
        using value_type = decltype(views::counted(std::declval<iterator_t<_Base>&>(),
                                                   std::declval<range_difference_t<_Base>&>()));
        using difference_type = range_difference_t<_Base>;

        __iterator() = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base> >
            : __current_(std::move(__i.__current_)), __n_(__i.__n_) {}

        /* last_ele_, for the sentinel. */
        constexpr const _LastStore& __fcxx_last_ele() const noexcept { return __last_ele_; }

        constexpr auto operator*() const { return views::counted(__current_, __n_); }

        constexpr __iterator& operator++() {
            __current_ = ranges::next(__current_);
            if constexpr (__fcxx_slide_caches_first<_Base>) {
                __last_ele_ = ranges::next(__last_ele_);
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
            __current_ = ranges::prev(__current_);
            if constexpr (__fcxx_slide_caches_first<_Base>) {
                __last_ele_ = ranges::prev(__last_ele_);
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
            __current_ = __current_ + __x;
            if constexpr (__fcxx_slide_caches_first<_Base>) {
                __last_ele_ = __last_ele_ + __x;
            }
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __x)
            requires random_access_range<_Base>
        {
            __current_ = __current_ - __x;
            if constexpr (__fcxx_slide_caches_first<_Base>) {
                __last_ele_ = __last_ele_ - __x;
            }
            return *this;
        }

        constexpr auto operator[](difference_type __n) const
            requires random_access_range<_Base>
        {
            return views::counted(__current_ + __n, __n_);
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y) {
            if constexpr (__fcxx_slide_caches_first<_Base>) {
                return __x.__last_ele_ == __y.__last_ele_;
            } else {
                return __x.__current_ == __y.__current_;
            }
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
            if constexpr (__fcxx_slide_caches_first<_Base>) {
                return __x.__last_ele_ - __y.__last_ele_;
            } else {
                return __x.__current_ - __y.__current_;
            }
        }
    };

    /* [range.slide.sentinel], class slide_view::sentinel */
    class __sentinel {
        friend slide_view;

        sentinel_t<_Vp> __end_ = sentinel_t<_Vp>();

        constexpr explicit __sentinel(sentinel_t<_Vp> __end) : __end_(__end) {}

    public:
        __sentinel() = default;

        friend constexpr bool operator==(const __iterator<false>& __x, const __sentinel& __y) {
            return __x.__fcxx_last_ele() == __y.__end_;
        }

        friend constexpr range_difference_t<_Vp> operator-(const __iterator<false>& __x, const __sentinel& __y)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            return __x.__fcxx_last_ele() - __y.__end_;
        }

        friend constexpr range_difference_t<_Vp> operator-(const __sentinel& __y, const __iterator<false>& __x)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            return __y.__end_ - __x.__fcxx_last_ele();
        }
    };

    _Vp __base_;
    range_difference_t<_Vp> __n_;
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<__fcxx_slide_caches_first<_Vp>, iterator_t<_Vp> > __begin_;
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_cache_if<__fcxx_slide_caches_last<_Vp>, iterator_t<_Vp> > __end_;

public:
    constexpr explicit slide_view(_Vp __base, range_difference_t<_Vp> __n) : __base_(std::move(__base)), __n_(__n) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin()
        requires(!(__fcxx_simple_view<_Vp> && __fcxx_slide_caches_nothing<const _Vp>))
    {
        if constexpr (__fcxx_slide_caches_first<_Vp>) {
            if (!__begin_.has_value()) {
                __begin_ = ranges::next(ranges::begin(__base_), __n_ - 1, ranges::end(__base_));
            }
            return __iterator<false>(ranges::begin(__base_), *__begin_, __n_);
        } else {
            return __iterator<false>(ranges::begin(__base_), __n_);
        }
    }

    constexpr auto begin() const
        requires __fcxx_slide_caches_nothing<const _Vp>
    {
        return __iterator<true>(ranges::begin(__base_), __n_);
    }

    constexpr auto end()
        requires(!(__fcxx_simple_view<_Vp> && __fcxx_slide_caches_nothing<const _Vp>))
    {
        if constexpr (__fcxx_slide_caches_nothing<_Vp>) {
            return __iterator<false>(ranges::begin(__base_) + range_difference_t<_Vp>(size()), __n_);
        } else if constexpr (__fcxx_slide_caches_last<_Vp>) {
            if (!__end_.has_value()) {
                __end_ = ranges::prev(ranges::end(__base_), __n_ - 1, ranges::begin(__base_));
            }
            return __iterator<false>(*__end_, __n_);
        } else if constexpr (common_range<_Vp>) {
            return __iterator<false>(ranges::end(__base_), ranges::end(__base_), __n_);
        } else {
            return __sentinel(ranges::end(__base_));
        }
    }

    constexpr auto end() const
        requires __fcxx_slide_caches_nothing<const _Vp>
    {
        return begin() + range_difference_t<const _Vp>(size());
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        auto __sz = ranges::distance(__base_) - __n_ + 1;
        if (__sz < 0) {
            __sz = 0;
        }
        return ranges::__fcxx_to_unsigned_like(__sz);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        auto __sz = ranges::distance(__base_) - __n_ + 1;
        if (__sz < 0) {
            __sz = 0;
        }
        return ranges::__fcxx_to_unsigned_like(__sz);
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        auto __sz = static_cast<range_difference_t<decltype((__base_))> >(ranges::reserve_hint(__base_)) - __n_ + 1;
        if (__sz < 0) {
            __sz = 0;
        }
        return ranges::__fcxx_to_unsigned_like(__sz);
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        auto __sz = static_cast<range_difference_t<decltype((__base_))> >(ranges::reserve_hint(__base_)) - __n_ + 1;
        if (__sz < 0) {
            __sz = 0;
        }
        return ranges::__fcxx_to_unsigned_like(__sz);
    }
#  endif
};

template <class _Rp>
slide_view(_Rp&&, range_difference_t<_Rp>) -> slide_view<views::all_t<_Rp> >;

template <class _Vp>
inline constexpr bool enable_borrowed_range<slide_view<_Vp> > = enable_borrowed_range<_Vp>;

namespace views {
struct __slide_fn {
    template <class _Rp, class _Np>
        requires requires { slide_view(std::declval<_Rp>(), std::declval<_Np>()); }
    constexpr auto operator()(_Rp&& __r, _Np&& __n) const
        noexcept(noexcept(slide_view(std::forward<_Rp>(__r), std::forward<_Np>(__n)))) {
        return slide_view(std::forward<_Rp>(__r), std::forward<_Np>(__n));
    }

    template <class _Np>
        requires __fcxx_can_bind<__slide_fn, _Np>
    constexpr auto operator()(_Np&& __n) const noexcept(is_nothrow_constructible_v<decay_t<_Np>, _Np>) {
        return ranges::__fcxx_bind_adaptor<__slide_fn>(std::forward<_Np>(__n));
    }
};

inline constexpr __slide_fn slide{};
} /* namespace views */

/* [range.chunk.by.view], class template chunk_by_view. Its begin() caches
 * where the first chunk ends. */
template <forward_range _Vp, indirect_binary_predicate<iterator_t<_Vp>, iterator_t<_Vp> > _Pred>
    requires view<_Vp> && is_object_v<_Pred>
class chunk_by_view : public view_interface<chunk_by_view<_Vp, _Pred> > {
    /* [range.chunk.by.iter], class chunk_by_view::iterator */
    class __iterator {
        friend chunk_by_view;

        chunk_by_view* __parent_ = nullptr;
        iterator_t<_Vp> __current_ = iterator_t<_Vp>();
        iterator_t<_Vp> __next_ = iterator_t<_Vp>();

        constexpr __iterator(chunk_by_view& __parent, iterator_t<_Vp> __current, iterator_t<_Vp> __next)
            : __parent_(std::addressof(__parent)), __current_(__current), __next_(__next) {}

    public:
        using value_type = subrange<iterator_t<_Vp> >;
        using difference_type = range_difference_t<_Vp>;
        using iterator_category = input_iterator_tag;
        using iterator_concept =
            conditional_t<bidirectional_range<_Vp>, bidirectional_iterator_tag, forward_iterator_tag>;

        __iterator() = default;

        constexpr value_type operator*() const { return subrange(__current_, __next_); }

        constexpr __iterator& operator++() {
            __current_ = __next_;
            __next_ = __parent_->__find_next(__current_);
            return *this;
        }

        constexpr __iterator operator++(int) {
            auto __tmp = *this;
            ++*this;
            return __tmp;
        }

        constexpr __iterator& operator--()
            requires bidirectional_range<_Vp>
        {
            __next_ = __current_;
            __current_ = __parent_->__find_prev(__next_);
            return *this;
        }

        constexpr __iterator operator--(int)
            requires bidirectional_range<_Vp>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y) {
            return __x.__current_ == __y.__current_;
        }

        friend constexpr bool operator==(const __iterator& __x, default_sentinel_t) {
            return __x.__current_ == __x.__next_;
        }
    };

    _Vp __base_ = _Vp();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_movable_box<_Pred> __pred_;
    __fcxx_non_propagating_cache<iterator_t<_Vp> > __begin_next_;

    /* find-next and find-prev */
    constexpr iterator_t<_Vp> __find_next(iterator_t<_Vp> __current) {
        return ranges::next(ranges::__fcxx_adjacent_find_not(__current, ranges::end(__base_), *__pred_), 1,
                            ranges::end(__base_));
    }

    constexpr iterator_t<_Vp> __find_prev(iterator_t<_Vp> __current)
        requires bidirectional_range<_Vp>
    {
        const iterator_t<_Vp> __first = ranges::begin(__base_);
        iterator_t<_Vp> __i = ranges::prev(__current);
        while (__i != __first) {
            iterator_t<_Vp> __before = ranges::prev(__i);
            if (!static_cast<bool>(std::__fcxx_invoke(*__pred_, *__before, *__i))) {
                break;
            }
            __i = __before;
        }
        return __i;
    }

public:
    chunk_by_view()
        requires default_initializable<_Vp> && default_initializable<_Pred>
    = default;

    constexpr explicit chunk_by_view(_Vp __base, _Pred __pred)
        : __base_(std::move(__base)), __pred_(std::move(__pred)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr const _Pred& pred() const { return *__pred_; }

    constexpr __iterator begin() {
        if (!__begin_next_.has_value()) {
            __begin_next_ = __find_next(ranges::begin(__base_));
        }
        return __iterator(*this, ranges::begin(__base_), *__begin_next_);
    }

    constexpr auto end() {
        if constexpr (common_range<_Vp>) {
            return __iterator(*this, ranges::end(__base_), ranges::end(__base_));
        } else {
            return default_sentinel;
        }
    }
};

template <class _Rp, class _Pred>
chunk_by_view(_Rp&&, _Pred) -> chunk_by_view<views::all_t<_Rp>, _Pred>;

namespace views {
struct __chunk_by_fn {
    template <class _Rp, class _Pred>
        requires requires { chunk_by_view(std::declval<_Rp>(), std::declval<_Pred>()); }
    constexpr auto operator()(_Rp&& __r, _Pred&& __pred) const
        noexcept(noexcept(chunk_by_view(std::forward<_Rp>(__r), std::forward<_Pred>(__pred)))) {
        return chunk_by_view(std::forward<_Rp>(__r), std::forward<_Pred>(__pred));
    }

    template <class _Pred>
        requires __fcxx_can_bind<__chunk_by_fn, _Pred>
    constexpr auto operator()(_Pred&& __pred) const noexcept(is_nothrow_constructible_v<decay_t<_Pred>, _Pred>) {
        return ranges::__fcxx_bind_adaptor<__chunk_by_fn>(std::forward<_Pred>(__pred));
    }
};

inline constexpr __chunk_by_fn chunk_by{};
} /* namespace views */

/* [range.stride.iterator], the iterator category of the iterator of
 * stride_view. */
template <class _Base>
struct __fcxx_stride_category {};

template <forward_range _Base>
struct __fcxx_stride_category<_Base> {
private:
    typedef typename iterator_traits<iterator_t<_Base> >::iterator_category _Cp;

public:
    using iterator_category =
        conditional_t<derived_from<_Cp, random_access_iterator_tag>, random_access_iterator_tag, _Cp>;
};

/* [range.stride.view], class template stride_view */
template <input_range _Vp>
    requires view<_Vp>
class stride_view : public view_interface<stride_view<_Vp> > {
    /* [range.stride.iterator], class template stride_view::iterator */
    template <bool _Const>
    class __iterator : public __fcxx_stride_category<__fcxx_maybe_const<_Const, _Vp> > {
        friend stride_view;

        template <bool>
        friend class __iterator;

        using _Parent = __fcxx_maybe_const<_Const, stride_view>;
        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        iterator_t<_Base> __current_ = iterator_t<_Base>();
        sentinel_t<_Base> __end_ = sentinel_t<_Base>();
        range_difference_t<_Base> __stride_ = 0;
        range_difference_t<_Base> __missing_ = 0;

        constexpr __iterator(_Parent* __parent, iterator_t<_Base> __current, range_difference_t<_Base> __missing = 0)
            : __current_(std::move(__current)), __end_(ranges::end(__parent->__base_)), __stride_(__parent->__stride_),
              __missing_(__missing) {}

    public:
        using difference_type = range_difference_t<_Base>;
        using value_type = range_value_t<_Base>;
        using iterator_concept = __fcxx_iterator_concept_of<_Base>;

        __iterator()
            requires default_initializable<iterator_t<_Base> >
        = default;

        constexpr __iterator(__iterator<!_Const> __other)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base> >
                  && convertible_to<sentinel_t<_Vp>, sentinel_t<_Base> >
            : __current_(std::move(__other.__current_)), __end_(std::move(__other.__end_)),
              __stride_(__other.__stride_), __missing_(__other.__missing_) {}

        constexpr iterator_t<_Base> base() && { return std::move(__current_); }
        constexpr const iterator_t<_Base>& base() const& noexcept { return __current_; }

        constexpr decltype(auto) operator*() const { return *__current_; }

        constexpr __iterator& operator++() {
            __missing_ = ranges::advance(__current_, __stride_, __end_);
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
            ranges::advance(__current_, __missing_ - __stride_);
            __missing_ = 0;
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
            if (__n > 0) {
                ranges::advance(__current_, _FCXX_MUL(__stride_, __n - 1));
                __missing_ = ranges::advance(__current_, __stride_, __end_);
            } else if (__n < 0) {
                ranges::advance(__current_, _FCXX_MUL(__stride_, __n) + __missing_);
                __missing_ = 0;
            }
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __x)
            requires random_access_range<_Base>
        {
            return *this += -__x;
        }

        constexpr decltype(auto) operator[](difference_type __n) const
            requires random_access_range<_Base>
        {
            return *(*this + __n);
        }

        friend constexpr bool operator==(const __iterator& __x, default_sentinel_t) {
            return __x.__current_ == __x.__end_;
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
            const difference_type __n = __x.__current_ - __y.__current_;
            if constexpr (forward_range<_Base>) {
                return (__n + __x.__missing_ - __y.__missing_) / __x.__stride_;
            } else if (__n < 0) {
                return -ranges::__fcxx_div_ceil(-__n, __x.__stride_);
            } else {
                return ranges::__fcxx_div_ceil(__n, __x.__stride_);
            }
        }

        friend constexpr difference_type operator-(default_sentinel_t, const __iterator& __x)
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<_Base> >
        {
            return ranges::__fcxx_div_ceil(static_cast<difference_type>(__x.__end_ - __x.__current_), __x.__stride_);
        }

        friend constexpr difference_type operator-(const __iterator& __x, default_sentinel_t __y)
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<_Base> >
        {
            return -(__y - __x);
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

    _Vp __base_;
    range_difference_t<_Vp> __stride_;

public:
    constexpr explicit stride_view(_Vp __base, range_difference_t<_Vp> __stride)
        : __base_(std::move(__base)), __stride_(__stride) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr range_difference_t<_Vp> stride() const noexcept { return __stride_; }

    constexpr auto begin()
        requires(!__fcxx_simple_view<_Vp>)
    {
        return __iterator<false>(this, ranges::begin(__base_));
    }

    constexpr auto begin() const
        requires range<const _Vp>
    {
        return __iterator<true>(this, ranges::begin(__base_));
    }

    constexpr auto end()
        requires(!__fcxx_simple_view<_Vp>)
    {
        if constexpr (common_range<_Vp> && sized_range<_Vp> && forward_range<_Vp>) {
            auto __missing = (__stride_ - ranges::distance(__base_) % __stride_) % __stride_;
            return __iterator<false>(this, ranges::end(__base_), __missing);
        } else if constexpr (common_range<_Vp> && !bidirectional_range<_Vp>) {
            return __iterator<false>(this, ranges::end(__base_));
        } else {
            return default_sentinel;
        }
    }

    constexpr auto end() const
        requires range<const _Vp>
    {
        if constexpr (common_range<const _Vp> && sized_range<const _Vp> && forward_range<const _Vp>) {
            auto __missing = (__stride_ - ranges::distance(__base_) % __stride_) % __stride_;
            return __iterator<true>(this, ranges::end(__base_), __missing);
        } else if constexpr (common_range<const _Vp> && !bidirectional_range<const _Vp>) {
            return __iterator<true>(this, ranges::end(__base_));
        } else {
            return default_sentinel;
        }
    }

    constexpr auto size()
        requires sized_range<_Vp>
    {
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(ranges::distance(__base_), __stride_));
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(ranges::distance(__base_), __stride_));
    }

#  if _FCXX_STD >= 2026
    constexpr auto reserve_hint()
        requires approximately_sized_range<_Vp>
    {
        auto __s = static_cast<range_difference_t<decltype((__base_))> >(ranges::reserve_hint(__base_));
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(__s, __stride_));
    }

    constexpr auto reserve_hint() const
        requires approximately_sized_range<const _Vp>
    {
        auto __s = static_cast<range_difference_t<decltype((__base_))> >(ranges::reserve_hint(__base_));
        return ranges::__fcxx_to_unsigned_like(ranges::__fcxx_div_ceil(__s, __stride_));
    }
#  endif
};

template <class _Rp>
stride_view(_Rp&&, range_difference_t<_Rp>) -> stride_view<views::all_t<_Rp> >;

template <class _Vp>
inline constexpr bool enable_borrowed_range<stride_view<_Vp> > = enable_borrowed_range<_Vp>;

namespace views {
struct __stride_fn {
    template <class _Rp, class _Np>
        requires requires { stride_view(std::declval<_Rp>(), std::declval<_Np>()); }
    constexpr auto operator()(_Rp&& __r, _Np&& __n) const
        noexcept(noexcept(stride_view(std::forward<_Rp>(__r), std::forward<_Np>(__n)))) {
        return stride_view(std::forward<_Rp>(__r), std::forward<_Np>(__n));
    }

    template <class _Np>
        requires __fcxx_can_bind<__stride_fn, _Np>
    constexpr auto operator()(_Np&& __n) const noexcept(is_nothrow_constructible_v<decay_t<_Np>, _Np>) {
        return ranges::__fcxx_bind_adaptor<__stride_fn>(std::forward<_Np>(__n));
    }
};

inline constexpr __stride_fn stride{};
} /* namespace views */

#  if _FCXX_STD >= 2026
/* [range.cache.latest.view], class template cache_latest_view */
template <input_range _Vp>
    requires view<_Vp>
class cache_latest_view : public view_interface<cache_latest_view<_Vp> > {
    using __cache_t = conditional_t<is_reference_v<range_reference_t<_Vp> >, add_pointer_t<range_reference_t<_Vp> >,
                                    range_reference_t<_Vp> >;

    class __sentinel;

    /* [range.cache.latest.iterator], class cache_latest_view::iterator */
    class __iterator {
        friend cache_latest_view;

        cache_latest_view* __parent_;
        iterator_t<_Vp> __current_;

        constexpr explicit __iterator(cache_latest_view& __parent)
            : __parent_(std::addressof(__parent)), __current_(ranges::begin(__parent.__base_)) {}

    public:
        using difference_type = range_difference_t<_Vp>;
        using value_type = range_value_t<_Vp>;
        using iterator_concept = input_iterator_tag;

        __iterator(__iterator&&) = default;
        __iterator& operator=(__iterator&&) = default;

        constexpr iterator_t<_Vp> base() && { return std::move(__current_); }
        constexpr const iterator_t<_Vp>& base() const& noexcept { return __current_; }

        constexpr range_reference_t<_Vp>& operator*() const {
            if constexpr (is_reference_v<range_reference_t<_Vp> >) {
                if (!__parent_->__cache_) {
                    __parent_->__cache_ = std::addressof(ranges::__fcxx_as_lvalue(*__current_));
                }
                return **__parent_->__cache_;
            } else {
                if (!__parent_->__cache_) {
                    __parent_->__cache_.__emplace_deref(__current_);
                }
                return *__parent_->__cache_;
            }
        }

        constexpr __iterator& operator++() {
            __parent_->__cache_.reset();
            ++__current_;
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        friend constexpr range_rvalue_reference_t<_Vp> iter_move(const __iterator& __i) noexcept(
            noexcept(ranges::iter_move(__i.__current_))) {
            return ranges::iter_move(__i.__current_);
        }

        friend constexpr void iter_swap(const __iterator& __x, const __iterator& __y) noexcept(
            noexcept(ranges::iter_swap(__x.__current_, __y.__current_)))
            requires indirectly_swappable<iterator_t<_Vp> >
        {
            ranges::iter_swap(__x.__current_, __y.__current_);
        }
    };

    /* [range.cache.latest.sentinel], class cache_latest_view::sentinel */
    class __sentinel {
        friend cache_latest_view;

        sentinel_t<_Vp> __end_ = sentinel_t<_Vp>();

        constexpr explicit __sentinel(cache_latest_view& __parent) : __end_(ranges::end(__parent.__base_)) {}

    public:
        __sentinel() = default;

        constexpr sentinel_t<_Vp> base() const { return __end_; }

        friend constexpr bool operator==(const __iterator& __x, const __sentinel& __y) {
            return __x.base() == __y.__end_;
        }

        friend constexpr range_difference_t<_Vp> operator-(const __iterator& __x, const __sentinel& __y)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            return __x.base() - __y.__end_;
        }

        friend constexpr range_difference_t<_Vp> operator-(const __sentinel& __x, const __iterator& __y)
            requires sized_sentinel_for<sentinel_t<_Vp>, iterator_t<_Vp> >
        {
            return __x.__end_ - __y.base();
        }
    };

    _Vp __base_ = _Vp();
    __fcxx_non_propagating_cache<__cache_t> __cache_;

public:
    cache_latest_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit cache_latest_view(_Vp __base) : __base_(std::move(__base)) {}

    constexpr _Vp base() const&
        requires copy_constructible<_Vp>
    {
        return __base_;
    }

    constexpr _Vp base() && { return std::move(__base_); }

    constexpr auto begin() { return __iterator(*this); }

    constexpr auto end() { return __sentinel(*this); }

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
};

template <class _Rp>
cache_latest_view(_Rp&&) -> cache_latest_view<views::all_t<_Rp> >;

namespace views {
struct __cache_latest_fn : __fcxx_adaptor_closure<__cache_latest_fn> {
    template <class _Rp>
        requires requires { cache_latest_view(std::declval<_Rp>()); }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(cache_latest_view(std::forward<_Rp>(__r)))) {
        return cache_latest_view(std::forward<_Rp>(__r));
    }
};

inline constexpr __cache_latest_fn cache_latest{};
} /* namespace views */

/* [range.as.input.view], class template as_input_view */
template <input_range _Vp>
    requires view<_Vp>
class as_input_view : public view_interface<as_input_view<_Vp> > {
    /* [range.as.input.iterator], class template as_input_view::iterator */
    template <bool _Const>
    class __iterator {
        friend as_input_view;

        template <bool>
        friend class __iterator;

        using _Base = __fcxx_maybe_const<_Const, _Vp>;

        iterator_t<_Base> __current_ = iterator_t<_Base>();

        constexpr explicit __iterator(iterator_t<_Base> __current) : __current_(std::move(__current)) {}

    public:
        using difference_type = range_difference_t<_Base>;
        using value_type = range_value_t<_Base>;
        using iterator_concept = input_iterator_tag;

        __iterator()
            requires default_initializable<iterator_t<_Base> >
        = default;

        __iterator(__iterator&&) = default;
        __iterator& operator=(__iterator&&) = default;

        constexpr __iterator(__iterator<!_Const> __i)
            requires _Const && convertible_to<iterator_t<_Vp>, iterator_t<_Base> >
            : __current_(std::move(__i.__current_)) {}

        constexpr iterator_t<_Base> base() && { return std::move(__current_); }
        constexpr const iterator_t<_Base>& base() const& noexcept { return __current_; }

        constexpr decltype(auto) operator*() const { return *__current_; }

        constexpr __iterator& operator++() {
            ++__current_;
            return *this;
        }

        constexpr void operator++(int) { ++*this; }

        friend constexpr bool operator==(const __iterator& __x, const sentinel_t<_Base>& __y) {
            return __x.__current_ == __y;
        }

        friend constexpr difference_type operator-(const sentinel_t<_Base>& __y, const __iterator& __x)
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<_Base> >
        {
            return __y - __x.__current_;
        }

        friend constexpr difference_type operator-(const __iterator& __x, const sentinel_t<_Base>& __y)
            requires sized_sentinel_for<sentinel_t<_Base>, iterator_t<_Base> >
        {
            return __x.__current_ - __y;
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

    _Vp __base_ = _Vp();

public:
    as_input_view()
        requires default_initializable<_Vp>
    = default;

    constexpr explicit as_input_view(_Vp __base) : __base_(std::move(__base)) {}

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
        return ranges::size(__base_);
    }

    constexpr auto size() const
        requires sized_range<const _Vp>
    {
        return ranges::size(__base_);
    }

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
};

template <class _Rp>
as_input_view(_Rp&&) -> as_input_view<views::all_t<_Rp> >;

template <class _Vp>
inline constexpr bool enable_borrowed_range<as_input_view<_Vp> > = enable_borrowed_range<_Vp>;

namespace views {
template <class _Rp>
concept __fcxx_input_only = input_range<_Rp> && !common_range<_Rp> && !forward_range<_Rp>;

struct __as_input_fn : __fcxx_adaptor_closure<__as_input_fn> {
    template <class _Rp>
        requires __fcxx_input_only<_Rp> && requires { views::all(std::declval<_Rp>()); }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(views::all(std::forward<_Rp>(__r)))) {
        return views::all(std::forward<_Rp>(__r));
    }

    template <class _Rp>
        requires(!__fcxx_input_only<_Rp>) && requires { as_input_view(std::declval<_Rp>()); }
    constexpr auto operator()(_Rp&& __r) const noexcept(noexcept(as_input_view(std::forward<_Rp>(__r)))) {
        return as_input_view(std::forward<_Rp>(__r));
    }
};

inline constexpr __as_input_fn as_input{};
} /* namespace views */
#  endif

} /* namespace ranges */
} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
