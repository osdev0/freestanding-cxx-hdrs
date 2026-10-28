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


/* [range.factories], the range factories of <ranges>: empty_view,
 * single_view, iota_view and repeat_view. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_FACTORIES_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_FACTORIES_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

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
#include <__fcxx/ranges_util.h>

namespace std {
namespace ranges {

/* [range.empty], empty view */
template <class _Tp>
    requires is_object_v<_Tp>
class empty_view : public view_interface<empty_view<_Tp> > {
public:
    static constexpr _Tp* begin() noexcept { return nullptr; }
    static constexpr _Tp* end() noexcept { return nullptr; }
    static constexpr _Tp* data() noexcept { return nullptr; }
    static constexpr size_t size() noexcept { return 0; }
    static constexpr bool empty() noexcept { return true; }
};

template <class _Tp>
inline constexpr bool enable_borrowed_range<empty_view<_Tp> > = true;

namespace views {
template <class _Tp>
inline constexpr empty_view<_Tp> empty{};
}

/* [range.single], single view. It has empty(), as LWG 4035 has it. */
template <move_constructible _Tp>
    requires is_object_v<_Tp>
class single_view : public view_interface<single_view<_Tp> > {
    __fcxx_movable_box<_Tp> __value_;

public:
    single_view()
        requires default_initializable<_Tp>
    = default;

    constexpr explicit single_view(const _Tp& __t)
        requires copy_constructible<_Tp>
        : __value_(__t) {}

    constexpr explicit single_view(_Tp&& __t) : __value_(std::move(__t)) {}

    template <class... _Args>
        requires constructible_from<_Tp, _Args...>
    constexpr explicit single_view(in_place_t, _Args&&... __args)
        : __value_(in_place, std::forward<_Args>(__args)...) {}

    constexpr _Tp* begin() noexcept { return data(); }
    constexpr const _Tp* begin() const noexcept { return data(); }
    constexpr _Tp* end() noexcept { return data() + 1; }
    constexpr const _Tp* end() const noexcept { return data() + 1; }

    static constexpr bool empty() noexcept { return false; }
    static constexpr size_t size() noexcept { return 1; }

    constexpr _Tp* data() noexcept { return __value_.operator->(); }
    constexpr const _Tp* data() const noexcept { return __value_.operator->(); }
};

template <class _Tp>
single_view(_Tp) -> single_view<_Tp>;

namespace views {
struct __single_fn {
    template <class _Tp>
        requires requires { single_view<decay_t<_Tp> >(std::declval<_Tp>()); }
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(single_view<decay_t<_Tp> >(std::forward<_Tp>(__t)))) {
        return single_view<decay_t<_Tp> >(std::forward<_Tp>(__t));
    }
};

inline constexpr __single_fn single{};
} /* namespace views */

/* [range.iota.view], IOTA-DIFF-T: the difference type of W where it is
 * wider than W, and otherwise the narrowest signed integer type wider than
 * it, or the widest there is. */
template <class _Wp>
struct __fcxx_iota_diff {
    using type = iter_difference_t<_Wp>;
};

template <class _Wp>
    requires integral<_Wp> && (sizeof(iter_difference_t<_Wp>) <= sizeof(_Wp))
struct __fcxx_iota_diff<_Wp> {
#  ifdef __SIZEOF_INT128__
    typedef __fcxx_int128 __widest;
#  else
    typedef long long __widest;
#  endif
    using type = conditional_t<
        (sizeof(short) > sizeof(_Wp)), short,
        conditional_t<(sizeof(int) > sizeof(_Wp)), int,
                      conditional_t<(sizeof(long) > sizeof(_Wp)), long,
                                    conditional_t<(sizeof(long long) > sizeof(_Wp)), long long, __widest> > > >;
};

template <class _Wp>
using __fcxx_iota_diff_t = typename __fcxx_iota_diff<_Wp>::type;

template <class _Ip>
concept __fcxx_decrementable = incrementable<_Ip> && requires(_Ip __i) {
    { --__i } -> same_as<_Ip&>;
    { __i-- } -> same_as<_Ip>;
};

template <class _Ip>
concept __fcxx_advanceable = __fcxx_decrementable<_Ip> && totally_ordered<_Ip>
                          && requires(_Ip __i, const _Ip __j, const __fcxx_iota_diff_t<_Ip> __n) {
                                 { __i += __n } -> same_as<_Ip&>;
                                 { __i -= __n } -> same_as<_Ip&>;
                                 _Ip(__j + __n);
                                 _Ip(__n + __j);
                                 _Ip(__j - __n);
                                 { __j - __j } -> convertible_to<__fcxx_iota_diff_t<_Ip> >;
                             };

/* The iterator category of the iterator of iota_view, which it only has
 * where W is incrementable and its difference type is an integer type, as
 * P2259 has it. */
template <class _Wp>
struct __fcxx_iota_category {};

template <class _Wp>
    requires incrementable<_Wp> && is_integral_v<__fcxx_iota_diff_t<_Wp> >
struct __fcxx_iota_category<_Wp> {
    using iterator_category = input_iterator_tag;
};

/* [range.iota.view], class template iota_view. It requires W to be
 * copyable, as P2325 has it, and has empty(), as LWG 4001 has it. The
 * constructors of its iterator and sentinel are private, as they are
 * exposition-only. */
template <weakly_incrementable _Wp, semiregular _Bound = unreachable_sentinel_t>
    requires __weakly_equality_comparable_with<_Wp, _Bound> && copyable<_Wp>
class iota_view : public view_interface<iota_view<_Wp, _Bound> > {
    /* [range.iota.iterator], class iota_view::iterator */
    class __iterator : public __fcxx_iota_category<_Wp> {
        friend iota_view;

        _Wp __value_ = _Wp();

        constexpr explicit __iterator(_Wp __value) : __value_(__value) {}

    public:
        using iterator_concept = conditional_t<
            __fcxx_advanceable<_Wp>, random_access_iterator_tag,
            conditional_t<__fcxx_decrementable<_Wp>, bidirectional_iterator_tag,
                          conditional_t<incrementable<_Wp>, forward_iterator_tag, input_iterator_tag> > >;
        using value_type = _Wp;
        using difference_type = __fcxx_iota_diff_t<_Wp>;

        __iterator()
            requires default_initializable<_Wp>
        = default;

        constexpr _Wp operator*() const noexcept(is_nothrow_copy_constructible_v<_Wp>) { return __value_; }

        constexpr const _Wp& __fcxx_value() const noexcept { return __value_; }

        constexpr __iterator& operator++() {
            ++__value_;
            return *this;
        }

        /* Workaround: GCC 11 to 13 and Clang 12 to 17, all targets: the two
         * postfix increments as one function, as their mangled names are
         * the same with these compilers (see ranges_util.h). */
        constexpr auto operator++(int) {
            if constexpr (incrementable<_Wp>) {
                auto __tmp = *this;
                ++*this;
                return __tmp;
            } else {
                ++*this;
            }
        }

        constexpr __iterator& operator--()
            requires __fcxx_decrementable<_Wp>
        {
            --__value_;
            return *this;
        }

        constexpr __iterator operator--(int)
            requires __fcxx_decrementable<_Wp>
        {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        constexpr __iterator& operator+=(difference_type __n)
            requires __fcxx_advanceable<_Wp>
        {
            if constexpr (__fcxx_is_integer_like<_Wp> && !__fcxx_is_signed_integer_like<_Wp>) {
                if (__n >= difference_type(0)) {
                    __value_ += static_cast<_Wp>(__n);
                } else {
                    __value_ -= static_cast<_Wp>(-__n);
                }
            } else {
                __value_ += __n;
            }
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __n)
            requires __fcxx_advanceable<_Wp>
        {
            if constexpr (__fcxx_is_integer_like<_Wp> && !__fcxx_is_signed_integer_like<_Wp>) {
                if (__n >= difference_type(0)) {
                    __value_ -= static_cast<_Wp>(__n);
                } else {
                    __value_ += static_cast<_Wp>(-__n);
                }
            } else {
                __value_ -= __n;
            }
            return *this;
        }

        constexpr _Wp operator[](difference_type __n) const
            requires __fcxx_advanceable<_Wp>
        {
            return _Wp(__value_ + __n);
        }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y)
            requires equality_comparable<_Wp>
        {
            return __x.__value_ == __y.__value_;
        }

        friend constexpr bool operator<(const __iterator& __x, const __iterator& __y)
            requires totally_ordered<_Wp>
        {
            return __x.__value_ < __y.__value_;
        }

        friend constexpr bool operator>(const __iterator& __x, const __iterator& __y)
            requires totally_ordered<_Wp>
        {
            return __y < __x;
        }

        friend constexpr bool operator<=(const __iterator& __x, const __iterator& __y)
            requires totally_ordered<_Wp>
        {
            return !(__y < __x);
        }

        friend constexpr bool operator>=(const __iterator& __x, const __iterator& __y)
            requires totally_ordered<_Wp>
        {
            return !(__x < __y);
        }

        friend constexpr auto operator<=>(const __iterator& __x, const __iterator& __y)
            requires totally_ordered<_Wp> && three_way_comparable<_Wp>
        {
            return __x.__value_ <=> __y.__value_;
        }

        friend constexpr __iterator operator+(__iterator __i, difference_type __n)
            requires __fcxx_advanceable<_Wp>
        {
            __i += __n;
            return __i;
        }

        friend constexpr __iterator operator+(difference_type __n, __iterator __i)
            requires __fcxx_advanceable<_Wp>
        {
            return __i + __n;
        }

        friend constexpr __iterator operator-(__iterator __i, difference_type __n)
            requires __fcxx_advanceable<_Wp>
        {
            __i -= __n;
            return __i;
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y)
            requires __fcxx_advanceable<_Wp>
        {
            using _Dp = difference_type;
            if constexpr (__fcxx_is_integer_like<_Wp>) {
                if constexpr (__fcxx_is_signed_integer_like<_Wp>) {
                    return _Dp(_Dp(__x.__value_) - _Dp(__y.__value_));
                } else {
                    return (__y.__value_ > __x.__value_) ? _Dp(-_Dp(__y.__value_ - __x.__value_))
                                                         : _Dp(__x.__value_ - __y.__value_);
                }
            } else {
                return __x.__value_ - __y.__value_;
            }
        }
    };

    /* [range.iota.sentinel], class iota_view::sentinel */
    class __sentinel {
        friend iota_view;

        _Bound __bound_ = _Bound();

        constexpr explicit __sentinel(_Bound __bound) : __bound_(__bound) {}

    public:
        __sentinel() = default;

        friend constexpr bool operator==(const __iterator& __x, const __sentinel& __y) {
            return __x.__fcxx_value() == __y.__bound_;
        }

        friend constexpr iter_difference_t<_Wp> operator-(const __iterator& __x, const __sentinel& __y)
            requires sized_sentinel_for<_Bound, _Wp>
        {
            return __x.__fcxx_value() - __y.__bound_;
        }

        friend constexpr iter_difference_t<_Wp> operator-(const __sentinel& __x, const __iterator& __y)
            requires sized_sentinel_for<_Bound, _Wp>
        {
            return -(__y - __x);
        }
    };

    /* The type of the last argument of the constructor from an iterator and
     * a sentinel, and the bound it gives. */
    using __last_type = conditional_t<same_as<_Wp, _Bound>, __iterator,
                                      conditional_t<same_as<_Bound, unreachable_sentinel_t>, _Bound, __sentinel> >;

    static constexpr _Bound __bound_of(const __last_type& __last) {
        if constexpr (same_as<_Wp, _Bound>) {
            return __last.__value_;
        } else if constexpr (same_as<_Bound, unreachable_sentinel_t>) {
            return __last;
        } else {
            return __last.__bound_;
        }
    }

    _Wp __value_ = _Wp();
    _Bound __bound_ = _Bound();

public:
    iota_view()
        requires default_initializable<_Wp>
    = default;

    constexpr explicit iota_view(_Wp __value) : __value_(__value) {}

    constexpr explicit iota_view(type_identity_t<_Wp> __value, type_identity_t<_Bound> __bound)
        : __value_(__value), __bound_(__bound) {}

    constexpr explicit iota_view(__iterator __first, __last_type __last)
        : iota_view(__first.__value_, __bound_of(__last)) {}

    constexpr __iterator begin() const { return __iterator(__value_); }

    /* Workaround: GCC 11 to 13 and Clang 12 to 17, all targets: the two
     * end() as one function, as their mangled names are the same with these
     * compilers (see ranges_util.h). */
    constexpr auto end() const {
        if constexpr (same_as<_Wp, _Bound>) {
            return __iterator(__bound_);
        } else if constexpr (same_as<_Bound, unreachable_sentinel_t>) {
            return unreachable_sentinel;
        } else {
            return __sentinel(__bound_);
        }
    }

    constexpr bool empty() const { return __value_ == __bound_; }

    constexpr auto size() const
        requires(same_as<_Wp, _Bound> && __fcxx_advanceable<_Wp>)
             || (__fcxx_is_integer_like<_Wp> && __fcxx_is_integer_like<_Bound>) || sized_sentinel_for<_Bound, _Wp>
    {
        if constexpr (__fcxx_is_integer_like<_Wp> && __fcxx_is_integer_like<_Bound>) {
            /* The type of the conditional expression the standard gives,
             * which is unsigned, and the size, the difference of the bound
             * and the value, computed in it: modulo a power of two that is
             * larger than the size, as the value and the bound convert to
             * it. The expressions of the standard would negate the least
             * value of a signed type, which is undefined. */
            using _Rp = decltype(true ? (true ? ranges::__fcxx_to_unsigned_like(-__value_)
                                                    - ranges::__fcxx_to_unsigned_like(-__bound_)
                                              : ranges::__fcxx_to_unsigned_like(__bound_)
                                                    + ranges::__fcxx_to_unsigned_like(-__value_))
                                       : ranges::__fcxx_to_unsigned_like(__bound_)
                                             - ranges::__fcxx_to_unsigned_like(__value_));
            return static_cast<_Rp>(static_cast<_Rp>(__bound_) - static_cast<_Rp>(__value_));
        } else {
            return ranges::__fcxx_to_unsigned_like(__bound_ - __value_);
        }
    }
};

template <class _Wp, class _Bound>
    requires(!__fcxx_is_integer_like<_Wp> || !__fcxx_is_integer_like<_Bound>
             || (__fcxx_is_signed_integer_like<_Wp> == __fcxx_is_signed_integer_like<_Bound>))
iota_view(_Wp, _Bound) -> iota_view<_Wp, _Bound>;

template <class _Wp, class _Bound>
inline constexpr bool enable_borrowed_range<iota_view<_Wp, _Bound> > = true;

namespace views {
/* views::iota of a single value names the view of its decayed type, which
 * leaves out the iota views themselves, as LWG 4096 has it. */
struct __iota_fn {
    template <class _Wp>
        requires requires { iota_view<decay_t<_Wp> >(std::declval<_Wp>()); }
    constexpr auto operator()(_Wp&& __value) const
        noexcept(noexcept(iota_view<decay_t<_Wp> >(std::forward<_Wp>(__value)))) {
        return iota_view<decay_t<_Wp> >(std::forward<_Wp>(__value));
    }

    template <class _Wp, class _Bound>
        requires requires { iota_view(std::declval<_Wp>(), std::declval<_Bound>()); }
    constexpr auto operator()(_Wp&& __value, _Bound&& __bound) const
        noexcept(noexcept(iota_view(std::forward<_Wp>(__value), std::forward<_Bound>(__bound)))) {
        return iota_view(std::forward<_Wp>(__value), std::forward<_Bound>(__bound));
    }
};

inline constexpr __iota_fn iota{};

#  if _FCXX_STD >= 2026
struct __indices_fn {
    template <class _Tp>
        requires __fcxx_is_integer_like<remove_cvref_t<_Tp> >
    constexpr auto operator()(_Tp&& __e) const
        noexcept(noexcept(views::iota(remove_cvref_t<_Tp>(0), std::forward<_Tp>(__e)))) {
        return views::iota(remove_cvref_t<_Tp>(0), std::forward<_Tp>(__e));
    }
};

inline constexpr __indices_fn indices{};
#  endif
} /* namespace views */

/* [range.repeat.view], class template repeat_view */
template <class _Tp>
concept __fcxx_integer_like_with_usable_difference_type =
    __fcxx_is_signed_integer_like<_Tp> || (__fcxx_is_integer_like<_Tp> && weakly_incrementable<_Tp>);

template <move_constructible _Tp, semiregular _Bound = unreachable_sentinel_t>
    requires(is_object_v<_Tp> && same_as<_Tp, remove_cv_t<_Tp> >
             && (__fcxx_integer_like_with_usable_difference_type<_Bound> || same_as<_Bound, unreachable_sentinel_t>))
class repeat_view : public view_interface<repeat_view<_Tp, _Bound> > {
    /* [range.repeat.iterator], class repeat_view::iterator */
    class __iterator {
        friend repeat_view;

        using __index_type = conditional_t<same_as<_Bound, unreachable_sentinel_t>, ptrdiff_t, _Bound>;

        const _Tp* __value_ = nullptr;
        __index_type __current_ = __index_type();

        constexpr explicit __iterator(const _Tp* __value, __index_type __b = __index_type())
            : __value_(__value), __current_(__b) {}

    public:
        using iterator_concept = random_access_iterator_tag;
        using iterator_category = random_access_iterator_tag;
        using value_type = _Tp;
        using difference_type = conditional_t<__fcxx_is_signed_integer_like<__index_type>, __index_type,
                                              __fcxx_iota_diff_t<__index_type> >;

        __iterator() = default;

        constexpr const _Tp& operator*() const noexcept { return *__value_; }

        constexpr __iterator& operator++() {
            ++__current_;
            return *this;
        }

        constexpr __iterator operator++(int) {
            auto __tmp = *this;
            ++*this;
            return __tmp;
        }

        constexpr __iterator& operator--() {
            --__current_;
            return *this;
        }

        constexpr __iterator operator--(int) {
            auto __tmp = *this;
            --*this;
            return __tmp;
        }

        constexpr __iterator& operator+=(difference_type __n) {
            __current_ += __n;
            return *this;
        }

        constexpr __iterator& operator-=(difference_type __n) {
            __current_ -= __n;
            return *this;
        }

        constexpr const _Tp& operator[](difference_type __n) const noexcept { return *(*this + __n); }

        friend constexpr bool operator==(const __iterator& __x, const __iterator& __y) {
            return __x.__current_ == __y.__current_;
        }

        friend constexpr auto operator<=>(const __iterator& __x, const __iterator& __y) {
            return __x.__current_ <=> __y.__current_;
        }

        friend constexpr __iterator operator+(__iterator __i, difference_type __n) {
            __i += __n;
            return __i;
        }

        friend constexpr __iterator operator+(difference_type __n, __iterator __i) {
            __i += __n;
            return __i;
        }

        friend constexpr __iterator operator-(__iterator __i, difference_type __n) {
            __i -= __n;
            return __i;
        }

        friend constexpr difference_type operator-(const __iterator& __x, const __iterator& __y) {
            return static_cast<difference_type>(__x.__current_) - static_cast<difference_type>(__y.__current_);
        }
    };

    __fcxx_movable_box<_Tp> __value_;
    _Bound __bound_ = _Bound();

    template <class... _TArgs, class... _BoundArgs, size_t... _Is, size_t... _Js>
    constexpr repeat_view(tuple<_TArgs...>& __value_args, tuple<_BoundArgs...>& __bound_args, index_sequence<_Is...>,
                          index_sequence<_Js...>)
        : __value_(in_place, std::get<_Is>(std::move(__value_args))...),
          __bound_(std::get<_Js>(std::move(__bound_args))...) {}

public:
    repeat_view()
        requires default_initializable<_Tp>
    = default;

    constexpr explicit repeat_view(const _Tp& __value, _Bound __bound = _Bound())
        requires copy_constructible<_Tp>
        : __value_(__value), __bound_(__bound) {}

    constexpr explicit repeat_view(_Tp&& __value, _Bound __bound = _Bound())
        : __value_(std::move(__value)), __bound_(__bound) {}

    template <class... _TArgs, class... _BoundArgs>
        requires constructible_from<_Tp, _TArgs...> && constructible_from<_Bound, _BoundArgs...>
    constexpr explicit repeat_view(piecewise_construct_t, tuple<_TArgs...> __value_args,
                                   tuple<_BoundArgs...> __bound_args = tuple<>{})
        : repeat_view(__value_args, __bound_args, index_sequence_for<_TArgs...>(),
                      index_sequence_for<_BoundArgs...>()) {}

    constexpr __iterator begin() const { return __iterator(std::addressof(*__value_)); }

    /* Workaround: GCC 11 to 13 and Clang 12 to 17, all targets: the two
     * end() as one function, as their mangled names are the same with these
     * compilers (see ranges_util.h). */
    constexpr auto end() const noexcept(same_as<_Bound, unreachable_sentinel_t>) {
        if constexpr (same_as<_Bound, unreachable_sentinel_t>) {
            return unreachable_sentinel;
        } else {
            return __iterator(std::addressof(*__value_), __bound_);
        }
    }

    constexpr auto size() const
        requires(!same_as<_Bound, unreachable_sentinel_t>)
    {
        return ranges::__fcxx_to_unsigned_like(__bound_);
    }

    /* The value, for views::take and views::drop, of the value category of
     * the view. */
    constexpr _Tp& __fcxx_value() & noexcept { return *__value_; }
    constexpr const _Tp& __fcxx_value() const& noexcept { return *__value_; }
    constexpr _Tp&& __fcxx_value() && noexcept { return std::move(*__value_); }
    constexpr const _Tp&& __fcxx_value() const&& noexcept { return std::move(*__value_); }
};

template <class _Tp, class _Bound = unreachable_sentinel_t>
repeat_view(_Tp, _Bound = _Bound()) -> repeat_view<_Tp, _Bound>;

template <class _Tp>
inline constexpr bool __fcxx_is_repeat_view = false;

template <class _Tp, class _Bound>
inline constexpr bool __fcxx_is_repeat_view<repeat_view<_Tp, _Bound> > = true;

namespace views {
/* views::repeat of a single value names the view of its decayed type, as
 * LWG 4053 has it. */
struct __repeat_fn {
    template <class _Tp>
        requires requires { repeat_view<decay_t<_Tp> >(std::declval<_Tp>()); }
    constexpr auto operator()(_Tp&& __value) const
        noexcept(noexcept(repeat_view<decay_t<_Tp> >(std::forward<_Tp>(__value)))) {
        return repeat_view<decay_t<_Tp> >(std::forward<_Tp>(__value));
    }

    template <class _Tp, class _Bound>
        requires requires { repeat_view(std::declval<_Tp>(), std::declval<_Bound>()); }
    constexpr auto operator()(_Tp&& __value, _Bound&& __bound) const
        noexcept(noexcept(repeat_view(std::forward<_Tp>(__value), std::forward<_Bound>(__bound)))) {
        return repeat_view(std::forward<_Tp>(__value), std::forward<_Bound>(__bound));
    }
};

inline constexpr __repeat_fn repeat{};
} /* namespace views */

} /* namespace ranges */
} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
