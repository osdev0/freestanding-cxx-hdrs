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


/* What the views of <ranges> are built with: the exposition-only helpers of
 * [range.adaptor.helpers], movable-box and non-propagating-cache, the few
 * algorithms the views are specified with, and the range adaptor closure
 * objects with their pipes. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_UTIL_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_UTIL_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <concepts>
#include <tuple>
#include <type_traits>
#include <utility>
#include <__fcxx/addressof.h>
#include <__fcxx/construct_at.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
#include <__fcxx/operations.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_base.h>
#include <__fcxx/ranges_core.h>

/* Workaround: GCC 11 to 13 and Clang 12 to 17, all targets: they give two
 * member functions of a class template that differ only in their
 * constraints the same mangled name, so that an explicit instantiation of
 * the class, which instantiates both where both constraints are satisfied,
 * fails to assemble ("symbol ... is already defined") or to compile
 * ("definition with same mangled name"); GCC 14 and Clang 18 mangle the
 * constraints. The views make such pairs of members, where the standard has
 * them, single functions that choose what to do with if constexpr, with the
 * same results. */

namespace std {
namespace ranges {

/* [ranges.syn], maybe-const */
template <bool _Const, class _Tp>
using __fcxx_maybe_const = conditional_t<_Const, const _Tp, _Tp>;

/* [range.adaptor.helpers], as-lvalue and the concepts of all the ranges of a
 * pack. */
template <class _Tp>
constexpr _Tp& __fcxx_as_lvalue(_Tp&& __t) noexcept {
    return static_cast<_Tp&>(__t);
}

template <bool _Const, class... _Views>
concept __fcxx_all_random_access = (random_access_range<__fcxx_maybe_const<_Const, _Views> > && ...);

template <bool _Const, class... _Views>
concept __fcxx_all_bidirectional = (bidirectional_range<__fcxx_maybe_const<_Const, _Views> > && ...);

template <bool _Const, class... _Views>
concept __fcxx_all_forward = (forward_range<__fcxx_maybe_const<_Const, _Views> > && ...);

/* tuple-transform and tuple-for-each, of the elements of a tuple, which are
 * passed to the function as lvalues of it. */
template <class _Fp, class _Tuple, size_t... _Is>
constexpr auto __fcxx_tuple_transform_impl(_Fp& __f, _Tuple&& __t, index_sequence<_Is...>) {
    return tuple<invoke_result_t<_Fp&, decltype(std::get<_Is>(std::forward<_Tuple>(__t)))>...>(
        std::__fcxx_invoke(__f, std::get<_Is>(std::forward<_Tuple>(__t)))...);
}

template <class _Fp, class _Tuple>
constexpr auto __fcxx_tuple_transform(_Fp&& __f, _Tuple&& __t) {
    return ranges::__fcxx_tuple_transform_impl(__f, std::forward<_Tuple>(__t),
                                               make_index_sequence<tuple_size_v<remove_cvref_t<_Tuple> > >());
}

template <class _Fp, class _Tuple, size_t... _Is>
constexpr void __fcxx_tuple_for_each_impl(_Fp& __f, _Tuple&& __t, index_sequence<_Is...>) {
    (static_cast<void>(std::__fcxx_invoke(__f, std::get<_Is>(std::forward<_Tuple>(__t)))), ...);
}

template <class _Fp, class _Tuple>
constexpr void __fcxx_tuple_for_each(_Fp&& __f, _Tuple&& __t) {
    ranges::__fcxx_tuple_for_each_impl(__f, std::forward<_Tuple>(__t),
                                       make_index_sequence<tuple_size_v<remove_cvref_t<_Tuple> > >());
}

/* [range.chunk.view.input], div-ceil */
template <class _Ip>
constexpr _Ip __fcxx_div_ceil(_Ip __num, _Ip __denom) {
    _Ip __r = __num / __denom;
    if (__num % __denom) {
        ++__r;
    }
    return __r;
}

/* make-unsigned-like-t */
template <class _Tp>
using __fcxx_make_unsigned_like_t = make_unsigned_t<_Tp>;

/* The smaller of two values, the first if they are equivalent, as
 * ranges::min has it. */
template <class _Tp>
constexpr const _Tp& __fcxx_min(const _Tp& __a, const _Tp& __b) noexcept(noexcept(__b < __a)) {
    return __b < __a ? __b : __a;
}

/* The algorithms the views are specified with, which <algorithm> has, for
 * iterators and sentinels, with the predicate called as an lvalue, which
 * the views pass with ref. */
template <class _It, class _Sp, class _Pred>
constexpr _It __fcxx_find_if(_It __first, const _Sp& __last, _Pred& __pred) {
    for (; __first != __last; ++__first) {
        if (std::__fcxx_invoke(__pred, *__first)) {
            break;
        }
    }
    return __first;
}

template <class _It, class _Sp, class _Pred>
constexpr _It __fcxx_find_if_not(_It __first, const _Sp& __last, _Pred& __pred) {
    for (; __first != __last; ++__first) {
        if (!std::__fcxx_invoke(__pred, *__first)) {
            break;
        }
    }
    return __first;
}

template <class _It, class _Sp, class _Tp>
constexpr _It __fcxx_find(_It __first, const _Sp& __last, const _Tp& __value) {
    for (; __first != __last; ++__first) {
        if (*__first == __value) {
            break;
        }
    }
    return __first;
}

/* The first positions of two ranges whose elements are not equal, or those
 * where either range ends. */
template <class _I1, class _I2>
struct __fcxx_mismatch_result {
    _I1 __in1;
    _I2 __in2;
};

template <class _I1, class _S1, class _I2, class _S2>
constexpr __fcxx_mismatch_result<_I1, _I2> __fcxx_mismatch(_I1 __first1, const _S1& __last1, _I2 __first2,
                                                           const _S2& __last2) {
    while (__first1 != __last1 && __first2 != __last2 && ranges::equal_to()(*__first1, *__first2)) {
        ++__first1;
        ++__first2;
    }
    return {std::move(__first1), std::move(__first2)};
}

/* The first occurrence of the second range in the first, as a subrange, or
 * an empty subrange at the end of the first. */
template <class _I1, class _S1, class _I2, class _S2>
constexpr subrange<_I1> __fcxx_search(_I1 __first1, const _S1& __last1, const _I2& __first2, const _S2& __last2) {
    for (;;) {
        _I1 __it1 = __first1;
        _I2 __it2 = __first2;
        for (;;) {
            if (__it2 == __last2) {
                return {std::move(__first1), std::move(__it1)};
            }
            if (__it1 == __last1) {
                return {__it1, __it1};
            }
            if (!ranges::equal_to()(*__it1, *__it2)) {
                break;
            }
            ++__it1;
            ++__it2;
        }
        ++__first1;
    }
}

/* The first element that is not in the relation with the next one, or the
 * end. */
template <class _It, class _Sp, class _Pred>
constexpr _It __fcxx_adjacent_find_not(_It __first, const _Sp& __last, _Pred& __pred) {
    if (__first == __last) {
        return __first;
    }
    _It __next = __first;
    while (++__next != __last) {
        if (!std::__fcxx_invoke(__pred, *__first, *__next)) {
            return __first;
        }
        __first = __next;
    }
    return __next;
}

/* [range.move.wrap] and [range.nonprop.cache], the storage of an object that
 * can be there or not, which movable-box, non-propagating-cache and the
 * optional of join_view share. The value can be initialized directly from
 * the result of dereferencing an iterator, as emplace-deref has it, by
 * constructing the whole union from that result, with no copy or move of
 * it. */
struct __fcxx_deref_tag {
    explicit __fcxx_deref_tag() = default;
};

template <class _Tp, bool = is_trivially_destructible_v<_Tp> >
union __fcxx_uninit {
    unsigned char __empty_;
    _Tp __value_;

    constexpr __fcxx_uninit() noexcept : __empty_() {}

    template <class... _Args>
    constexpr explicit __fcxx_uninit(in_place_t, _Args&&... __args) : __value_(std::forward<_Args>(__args)...) {}

    template <class _It>
    constexpr __fcxx_uninit(__fcxx_deref_tag, const _It& __i) : __value_(*__i) {}
};

template <class _Tp>
union __fcxx_uninit<_Tp, false> {
    unsigned char __empty_;
    _Tp __value_;

    constexpr __fcxx_uninit() noexcept : __empty_() {}

    template <class... _Args>
    constexpr explicit __fcxx_uninit(in_place_t, _Args&&... __args) : __value_(std::forward<_Args>(__args)...) {}

    template <class _It>
    constexpr __fcxx_uninit(__fcxx_deref_tag, const _It& __i) : __value_(*__i) {}

    constexpr ~__fcxx_uninit() {}
};

/* The value and whether it is there, with a destructor that is trivial
 * where that of the value is. The classes deriving from it define what
 * copying and moving it does. */
template <class _Tp>
struct __fcxx_opt_payload {
    __fcxx_uninit<_Tp> __u_;
    bool __engaged_;

    constexpr __fcxx_opt_payload() noexcept : __u_(), __engaged_(false) {}

    template <class... _Args>
    constexpr explicit __fcxx_opt_payload(in_place_t __tag, _Args&&... __args)
        : __u_(__tag, std::forward<_Args>(__args)...), __engaged_(true) {}

    constexpr _Tp& __get() noexcept { return __u_.__value_; }
    constexpr const _Tp& __get() const noexcept { return __u_.__value_; }

    constexpr void __reset() noexcept {
        if (__engaged_) {
            if constexpr (!is_trivially_destructible_v<_Tp>) {
                __u_.__value_.~_Tp();
            }
            __engaged_ = false;
        }
    }

    /* Constructs the value, which is not there. */
    template <class... _Args>
    constexpr _Tp& __construct(_Args&&... __args) {
        std::construct_at(std::addressof(__u_.__value_), std::forward<_Args>(__args)...);
        __engaged_ = true;
        return __u_.__value_;
    }

    template <class _It>
    constexpr _Tp& __construct_deref(const _It& __i) {
        std::construct_at(std::addressof(__u_), __fcxx_deref_tag(), __i);
        __engaged_ = true;
        return __u_.__value_;
    }
};

template <class _Tp, bool = is_trivially_destructible_v<_Tp> >
struct __fcxx_opt_storage : __fcxx_opt_payload<_Tp> {
    using __fcxx_opt_payload<_Tp>::__fcxx_opt_payload;
};

template <class _Tp>
struct __fcxx_opt_storage<_Tp, false> : __fcxx_opt_payload<_Tp> {
    using __fcxx_opt_payload<_Tp>::__fcxx_opt_payload;

    __fcxx_opt_storage() = default;
    __fcxx_opt_storage(const __fcxx_opt_storage&) = default;
    __fcxx_opt_storage(__fcxx_opt_storage&&) = default;
    __fcxx_opt_storage& operator=(const __fcxx_opt_storage&) = default;
    __fcxx_opt_storage& operator=(__fcxx_opt_storage&&) = default;

    constexpr ~__fcxx_opt_storage() { this->__reset(); }
};

/* [range.nonprop.cache], non-propagating-cache: copying it gives an empty
 * one, and moving from it empties it. */
template <class _Tp>
class __fcxx_non_propagating_cache : private __fcxx_opt_storage<_Tp> {
    typedef __fcxx_opt_storage<_Tp> _Base;

public:
    constexpr __fcxx_non_propagating_cache() noexcept = default;

    constexpr __fcxx_non_propagating_cache(const __fcxx_non_propagating_cache&) noexcept : _Base() {}

    constexpr __fcxx_non_propagating_cache(__fcxx_non_propagating_cache&& __other) noexcept : _Base() {
        __other.__reset();
    }

    constexpr __fcxx_non_propagating_cache& operator=(const __fcxx_non_propagating_cache& __other) noexcept {
        if (std::addressof(__other) != this) {
            this->__reset();
        }
        return *this;
    }

    constexpr __fcxx_non_propagating_cache& operator=(__fcxx_non_propagating_cache&& __other) noexcept {
        this->__reset();
        __other.__reset();
        return *this;
    }

    /* What optional has, of which the views use these. */
    template <class _Up>
        requires constructible_from<_Tp, _Up> && assignable_from<_Tp&, _Up>
    constexpr __fcxx_non_propagating_cache& operator=(_Up&& __v) {
        if (this->__engaged_) {
            this->__get() = std::forward<_Up>(__v);
        } else {
            this->__construct(std::forward<_Up>(__v));
        }
        return *this;
    }

    constexpr bool has_value() const noexcept { return this->__engaged_; }
    constexpr explicit operator bool() const noexcept { return this->__engaged_; }

    constexpr _Tp& operator*() noexcept { return this->__get(); }

    constexpr void reset() noexcept { this->__reset(); }

    /* emplace-deref, whose value is the result of *i itself. */
    template <class _It>
    constexpr _Tp& __emplace_deref(const _It& __i) {
        this->__reset();
        return this->__construct_deref(__i);
    }
};

/* A cache that is only there where the view needs one, which is empty
 * otherwise. */
struct __fcxx_no_cache {};

template <bool _Present, class _Tp>
using __fcxx_cache_if = conditional_t<_Present, __fcxx_non_propagating_cache<_Tp>, __fcxx_no_cache>;

/* An optional value, which copies and moves along with its owner, as the
 * inner iterator of join_view is. */
template <class _Tp>
class __fcxx_optional_value : private __fcxx_opt_storage<_Tp> {
    typedef __fcxx_opt_storage<_Tp> _Base;

    template <class>
    friend class __fcxx_optional_value;

public:
    constexpr __fcxx_optional_value() noexcept = default;

    constexpr __fcxx_optional_value(const __fcxx_optional_value& __other) noexcept(
        is_nothrow_copy_constructible_v<_Tp>)
        : _Base() {
        if (__other.__engaged_) {
            this->__construct(__other.__get());
        }
    }

    constexpr __fcxx_optional_value(__fcxx_optional_value&& __other) noexcept(is_nothrow_move_constructible_v<_Tp>)
        : _Base() {
        if (__other.__engaged_) {
            this->__construct(std::move(__other.__get()));
        }
    }

    /* The conversion of that of the iterators of the other constness. */
    template <class _Up>
    constexpr explicit __fcxx_optional_value(__fcxx_optional_value<_Up>&& __other) : _Base() {
        if (__other.__engaged_) {
            this->__construct(std::move(__other.__get()));
        }
    }

    constexpr __fcxx_optional_value& operator=(const __fcxx_optional_value& __other) noexcept(
        is_nothrow_copy_constructible_v<_Tp> && is_nothrow_copy_assignable_v<_Tp>) {
        if (this->__engaged_ && __other.__engaged_) {
            this->__get() = __other.__get();
        } else if (__other.__engaged_) {
            this->__construct(__other.__get());
        } else {
            this->__reset();
        }
        return *this;
    }

    constexpr __fcxx_optional_value& operator=(__fcxx_optional_value&& __other) noexcept(
        is_nothrow_move_constructible_v<_Tp> && is_nothrow_move_assignable_v<_Tp>) {
        if (this->__engaged_ && __other.__engaged_) {
            this->__get() = std::move(__other.__get());
        } else if (__other.__engaged_) {
            this->__construct(std::move(__other.__get()));
        } else {
            this->__reset();
        }
        return *this;
    }

    template <class _Up>
        requires constructible_from<_Tp, _Up> && assignable_from<_Tp&, _Up>
    constexpr __fcxx_optional_value& operator=(_Up&& __v) {
        if (this->__engaged_) {
            this->__get() = std::forward<_Up>(__v);
        } else {
            this->__construct(std::forward<_Up>(__v));
        }
        return *this;
    }

    constexpr _Tp& operator*() noexcept { return this->__get(); }
    constexpr const _Tp& operator*() const noexcept { return this->__get(); }

    constexpr void reset() noexcept { this->__reset(); }

    /* Equal when neither has a value, or both have equal ones. */
    friend constexpr bool operator==(const __fcxx_optional_value& __x, const __fcxx_optional_value& __y) {
        if (__x.__engaged_ != __y.__engaged_) {
            return false;
        }
        return !__x.__engaged_ || static_cast<bool>(__x.__get() == __y.__get());
    }
};

/* [range.move.wrap], movable-box. It holds only the object where assigning
 * it cannot leave it without one, as the recommended practice has it: where
 * the type is copyable, or its copies cannot throw, and likewise for the
 * types that are only movable. Otherwise, it is like optional. Assigning it
 * copies or moves the value of the other into it anew where the value
 * cannot be assigned. */
template <class _Tp>
concept __fcxx_boxable = move_constructible<_Tp> && is_object_v<_Tp>;

/* 0: the value alone, assigned as such; 1: the value alone, copied anew;
 * 2: the value alone, copied and moved anew; 3: the value alone, moved
 * anew; 4: like optional, copyable; 5: like optional, movable only. */
template <class _Tp>
inline constexpr int __fcxx_box_kind =
    copy_constructible<_Tp>
        ? (copyable<_Tp> ? 0
                         : (is_nothrow_move_constructible_v<_Tp> && is_nothrow_copy_constructible_v<_Tp>
                                ? (movable<_Tp> ? 1 : 2)
                                : 4))
        : (movable<_Tp> ? 0 : (is_nothrow_move_constructible_v<_Tp> ? 3 : 5));

/* The value alone, which is empty where the type is, and then takes no
 * room, for the types whose assignments the box uses. */
template <class _Tp, int = __fcxx_box_kind<_Tp> >
class __fcxx_box_impl {
    _FCXX_NO_UNIQUE_ADDRESS _Tp __value_;

public:
    constexpr __fcxx_box_impl() noexcept(is_nothrow_default_constructible_v<_Tp>)
        requires default_initializable<_Tp>
        : __value_() {}

    template <class... _Args>
    constexpr explicit __fcxx_box_impl(in_place_t, _Args&&... __args) noexcept(
        is_nothrow_constructible_v<_Tp, _Args...>)
        : __value_(std::forward<_Args>(__args)...) {}

    constexpr _Tp& operator*() & noexcept { return __value_; }
    constexpr const _Tp& operator*() const& noexcept { return __value_; }

    constexpr _Tp* operator->() noexcept { return std::addressof(__value_); }
    constexpr const _Tp* operator->() const noexcept { return std::addressof(__value_); }
};

/* The value alone, which is copied or moved anew. It is not a potentially-
 * overlapping subobject, so that the new one replaces it. */
template <class _Tp>
class __fcxx_box_value {
protected:
    _Tp __value_;

    template <class _Up>
    constexpr void __renew(_Up&& __v) noexcept {
        std::destroy_at(std::addressof(__value_));
        std::construct_at(std::addressof(__value_), std::forward<_Up>(__v));
    }

public:
    constexpr __fcxx_box_value() noexcept(is_nothrow_default_constructible_v<_Tp>)
        requires default_initializable<_Tp>
        : __value_() {}

    template <class... _Args>
    constexpr explicit __fcxx_box_value(in_place_t, _Args&&... __args) noexcept(
        is_nothrow_constructible_v<_Tp, _Args...>)
        : __value_(std::forward<_Args>(__args)...) {}

    constexpr _Tp& operator*() & noexcept { return __value_; }
    constexpr const _Tp& operator*() const& noexcept { return __value_; }

    constexpr _Tp* operator->() noexcept { return std::addressof(__value_); }
    constexpr const _Tp* operator->() const noexcept { return std::addressof(__value_); }
};

/* Copies that cannot throw, of a type that is movable but not copyable. */
template <class _Tp>
class __fcxx_box_impl<_Tp, 1> : public __fcxx_box_value<_Tp> {
public:
    using __fcxx_box_value<_Tp>::__fcxx_box_value;

    __fcxx_box_impl() = default;
    __fcxx_box_impl(const __fcxx_box_impl&) = default;
    __fcxx_box_impl(__fcxx_box_impl&&) = default;
    __fcxx_box_impl& operator=(__fcxx_box_impl&&) = default;

    constexpr __fcxx_box_impl& operator=(const __fcxx_box_impl& __that) noexcept {
        if (this != std::addressof(__that)) {
            this->__renew(__that.__value_);
        }
        return *this;
    }
};

/* Copies and moves that cannot throw, of a type that cannot be assigned. */
template <class _Tp>
class __fcxx_box_impl<_Tp, 2> : public __fcxx_box_value<_Tp> {
public:
    using __fcxx_box_value<_Tp>::__fcxx_box_value;

    __fcxx_box_impl() = default;
    __fcxx_box_impl(const __fcxx_box_impl&) = default;
    __fcxx_box_impl(__fcxx_box_impl&&) = default;

    constexpr __fcxx_box_impl& operator=(const __fcxx_box_impl& __that) noexcept {
        if (this != std::addressof(__that)) {
            this->__renew(__that.__value_);
        }
        return *this;
    }

    constexpr __fcxx_box_impl& operator=(__fcxx_box_impl&& __that) noexcept {
        if (this != std::addressof(__that)) {
            this->__renew(std::move(__that.__value_));
        }
        return *this;
    }
};

/* Moves that cannot throw, of a type that can only be moved, and cannot be
 * assigned. */
template <class _Tp>
class __fcxx_box_impl<_Tp, 3> : public __fcxx_box_value<_Tp> {
public:
    using __fcxx_box_value<_Tp>::__fcxx_box_value;

    __fcxx_box_impl() = default;
    __fcxx_box_impl(__fcxx_box_impl&&) = default;

    constexpr __fcxx_box_impl& operator=(__fcxx_box_impl&& __that) noexcept {
        if (this != std::addressof(__that)) {
            this->__renew(std::move(__that.__value_));
        }
        return *this;
    }
};

/* The others, which are like optional, and can be left without a value by
 * an assignment that throws. */
template <class _Tp>
class __fcxx_box_opt : protected __fcxx_opt_storage<_Tp> {
    typedef __fcxx_opt_storage<_Tp> _Base;

protected:
    /* Without a value, for the copies to construct theirs into. */
    constexpr explicit __fcxx_box_opt(__fcxx_deref_tag) noexcept : _Base() {}

public:
    constexpr __fcxx_box_opt() noexcept(is_nothrow_default_constructible_v<_Tp>)
        requires default_initializable<_Tp>
        : _Base(in_place) {}

    template <class... _Args>
    constexpr explicit __fcxx_box_opt(in_place_t, _Args&&... __args) noexcept(
        is_nothrow_constructible_v<_Tp, _Args...>)
        : _Base(in_place, std::forward<_Args>(__args)...) {}

    constexpr __fcxx_box_opt(__fcxx_box_opt&& __other) noexcept(is_nothrow_move_constructible_v<_Tp>) : _Base() {
        if (__other.__engaged_) {
            this->__construct(std::move(__other.__get()));
        }
    }

    constexpr __fcxx_box_opt& operator=(__fcxx_box_opt&& __that) noexcept(
        is_nothrow_move_constructible_v<_Tp> && (!movable<_Tp> || is_nothrow_move_assignable_v<_Tp>)) {
        if constexpr (movable<_Tp>) {
            if (this->__engaged_ && __that.__engaged_) {
                this->__get() = std::move(__that.__get());
                return *this;
            }
        }
        if (this != std::addressof(__that)) {
            this->__reset();
            if (__that.__engaged_) {
                this->__construct(std::move(__that.__get()));
            }
        }
        return *this;
    }

    constexpr _Tp& operator*() & noexcept { return this->__get(); }
    constexpr const _Tp& operator*() const& noexcept { return this->__get(); }

    constexpr _Tp* operator->() noexcept { return std::addressof(this->__get()); }
    constexpr const _Tp* operator->() const noexcept { return std::addressof(this->__get()); }
};

template <class _Tp>
class __fcxx_box_impl<_Tp, 4> : public __fcxx_box_opt<_Tp> {
public:
    using __fcxx_box_opt<_Tp>::__fcxx_box_opt;

    __fcxx_box_impl() = default;
    __fcxx_box_impl(__fcxx_box_impl&&) = default;
    __fcxx_box_impl& operator=(__fcxx_box_impl&&) = default;

    constexpr __fcxx_box_impl(const __fcxx_box_impl& __other) noexcept(is_nothrow_copy_constructible_v<_Tp>)
        : __fcxx_box_opt<_Tp>(__fcxx_deref_tag()) {
        if (__other.__engaged_) {
            this->__construct(__other.__get());
        }
    }

    constexpr __fcxx_box_impl& operator=(const __fcxx_box_impl& __that) noexcept(is_nothrow_copy_constructible_v<_Tp>) {
        if (this != std::addressof(__that)) {
            this->__reset();
            if (__that.__engaged_) {
                this->__construct(__that.__get());
            }
        }
        return *this;
    }
};

template <class _Tp>
class __fcxx_box_impl<_Tp, 5> : public __fcxx_box_opt<_Tp> {
public:
    using __fcxx_box_opt<_Tp>::__fcxx_box_opt;

    __fcxx_box_impl() = default;
    __fcxx_box_impl(__fcxx_box_impl&&) = default;
    __fcxx_box_impl& operator=(__fcxx_box_impl&&) = default;
};

template <__fcxx_boxable _Tp>
class __fcxx_movable_box : public __fcxx_box_impl<_Tp> {
public:
    using __fcxx_box_impl<_Tp>::__fcxx_box_impl;

    __fcxx_movable_box() = default;

    constexpr explicit __fcxx_movable_box(const _Tp& __t) noexcept(is_nothrow_copy_constructible_v<_Tp>)
        requires copy_constructible<_Tp>
        : __fcxx_box_impl<_Tp>(in_place, __t) {}

    constexpr explicit __fcxx_movable_box(_Tp&& __t) noexcept(is_nothrow_move_constructible_v<_Tp>)
        : __fcxx_box_impl<_Tp>(in_place, std::move(__t)) {}
};

/* [range.adaptor.object], range adaptor closure objects. The pipes are
 * hidden friends of the base class of the closure objects, which argument-
 * dependent lookup finds for the class deriving from it, and which take the
 * closure object as their right operand, of the type deriving from it. The
 * left operand is a range, which the closure object is called with, or
 * another closure object, which the result calls first. */
template <class _Dp>
class __fcxx_closure_base;

template <class _Up>
void __fcxx_closure_test(const __fcxx_closure_base<_Up>&);

template <class _Tp>
concept __fcxx_is_closure = requires(const remove_cvref_t<_Tp>& __t) { ranges::__fcxx_closure_test(__t); }
                         && !range<remove_cvref_t<_Tp> >;

template <class _Rp, class _Cp>
concept __fcxx_pipe_range = !__fcxx_is_closure<_Rp> && range<_Rp> && invocable<_Cp, _Rp>;

template <class _Lhs, class _Rhs>
concept __fcxx_pipe_closures = __fcxx_is_closure<_Lhs> && constructible_from<decay_t<_Lhs>, _Lhs>
                            && constructible_from<decay_t<_Rhs>, _Rhs>;

template <class _C1, class _C2>
class __fcxx_pipe;

template <class _Dp>
class __fcxx_closure_base {
    /* One template for each operation, whose right operand is the closure
     * object with the value category and constness it has, so that the pipe
     * is valid exactly where the call it makes is, with no other overload to
     * fall back on. */
    template <class _Rp, class _Self>
        requires same_as<remove_cvref_t<_Self>, _Dp> && __fcxx_pipe_range<_Rp, _Self>
    friend constexpr auto operator|(_Rp&& __r, _Self&& __c) noexcept(is_nothrow_invocable_v<_Self, _Rp>)
        -> decltype(std::declval<_Self>()(std::declval<_Rp>())) {
        return std::forward<_Self>(__c)(std::forward<_Rp>(__r));
    }

    template <class _Cp, class _Self>
        requires same_as<remove_cvref_t<_Self>, _Dp> && __fcxx_pipe_closures<_Cp, _Self>
    friend constexpr __fcxx_pipe<decay_t<_Cp>, _Dp> operator|(_Cp&& __c, _Self&& __d) noexcept(
        is_nothrow_constructible_v<decay_t<_Cp>, _Cp> && is_nothrow_constructible_v<_Dp, _Self>) {
        return __fcxx_pipe<decay_t<_Cp>, _Dp>(std::forward<_Cp>(__c), std::forward<_Self>(__d));
    }
};

/* The base class of the closure objects of the program, from C++23 on,
 * which the closure objects of the library derive from as well. */
template <class _Dp>
    requires is_class_v<_Dp> && same_as<_Dp, remove_cv_t<_Dp> >
class range_adaptor_closure : public __fcxx_closure_base<_Dp> {};

template <class _Dp>
using __fcxx_adaptor_closure = range_adaptor_closure<_Dp>;

/* The closure object that C | D gives, a perfect forwarding call wrapper
 * calling D with the result of calling C. The call operators of each
 * qualification are deleted where the call they make is not valid, so that
 * those of the others are not used instead. */
template <class _C1, class _C2>
class __fcxx_pipe : public __fcxx_adaptor_closure<__fcxx_pipe<_C1, _C2> > {
    _FCXX_NO_UNIQUE_ADDRESS _C1 __c1_;
    _FCXX_NO_UNIQUE_ADDRESS _C2 __c2_;

    template <class _Fp, class _Gp, class _Rp>
    static constexpr bool __callable = requires(_Fp&& __f, _Gp&& __g, _Rp&& __r) {
        std::forward<_Gp>(__g)(std::forward<_Fp>(__f)(std::forward<_Rp>(__r)));
    };

    template <class _Fp, class _Gp, class _Rp>
    static constexpr bool __nothrow = noexcept(std::declval<_Gp>()(std::declval<_Fp>()(std::declval<_Rp>())));

    /* The result, named so that the calls need not be instantiated to know
     * it. */
    template <class _Fp, class _Gp, class _Rp>
    using __result = decltype(std::declval<_Gp>()(std::declval<_Fp>()(std::declval<_Rp>())));

public:
    template <class _A1, class _A2>
    constexpr __fcxx_pipe(_A1&& __a1, _A2&& __a2) noexcept(is_nothrow_constructible_v<_C1, _A1>
                                                           && is_nothrow_constructible_v<_C2, _A2>)
        : __c1_(std::forward<_A1>(__a1)), __c2_(std::forward<_A2>(__a2)) {}

    template <class _Rp>
        requires __callable<_C1&, _C2&, _Rp>
    constexpr auto operator()(_Rp&& __r) & noexcept(__nothrow<_C1&, _C2&, _Rp>) -> __result<_C1&, _C2&, _Rp> {
        return __c2_(__c1_(std::forward<_Rp>(__r)));
    }

    template <class _Rp>
        requires(!__callable<_C1&, _C2&, _Rp>)
    void operator()(_Rp&&) & = delete;

    template <class _Rp>
        requires __callable<const _C1&, const _C2&, _Rp>
    constexpr auto operator()(_Rp&& __r) const& noexcept(__nothrow<const _C1&, const _C2&, _Rp>)
        -> __result<const _C1&, const _C2&, _Rp> {
        return __c2_(__c1_(std::forward<_Rp>(__r)));
    }

    template <class _Rp>
        requires(!__callable<const _C1&, const _C2&, _Rp>)
    void operator()(_Rp&&) const& = delete;

    template <class _Rp>
        requires __callable<_C1, _C2, _Rp>
    constexpr auto operator()(_Rp&& __r) && noexcept(__nothrow<_C1, _C2, _Rp>) -> __result<_C1, _C2, _Rp> {
        return std::move(__c2_)(std::move(__c1_)(std::forward<_Rp>(__r)));
    }

    template <class _Rp>
        requires(!__callable<_C1, _C2, _Rp>)
    void operator()(_Rp&&) && = delete;

    template <class _Rp>
        requires __callable<const _C1, const _C2, _Rp>
    constexpr auto operator()(_Rp&& __r) const&& noexcept(__nothrow<const _C1, const _C2, _Rp>)
        -> __result<const _C1, const _C2, _Rp> {
        return std::move(__c2_)(std::move(__c1_)(std::forward<_Rp>(__r)));
    }

    template <class _Rp>
        requires(!__callable<const _C1, const _C2, _Rp>)
    void operator()(_Rp&&) const&& = delete;
};

/* The closure object that a range adaptor object called without its range
 * gives, which calls it with the range and the arguments it was given. The
 * adaptor objects are empty, and called as const lvalues. */
struct __fcxx_partial_tag {
    explicit __fcxx_partial_tag() = default;
};

template <class _Fn, class... _Bound>
class __fcxx_partial : public __fcxx_adaptor_closure<__fcxx_partial<_Fn, _Bound...> > {
    tuple<_Bound...> __bound_;

    template <class _Tuple, class _Rp, size_t... _Is>
    static constexpr decltype(auto) __call(_Tuple&& __t, _Rp&& __r, index_sequence<_Is...>) noexcept(
        noexcept(_Fn()(std::forward<_Rp>(__r), std::get<_Is>(std::forward<_Tuple>(__t))...))) {
        return _Fn()(std::forward<_Rp>(__r), std::get<_Is>(std::forward<_Tuple>(__t))...);
    }

    typedef index_sequence_for<_Bound...> _Seq;

public:
    template <class... _Args>
    constexpr explicit __fcxx_partial(__fcxx_partial_tag, _Args&&... __args) noexcept(
        (is_nothrow_constructible_v<_Bound, _Args> && ...))
        : __bound_(std::forward<_Args>(__args)...) {}

    template <class _Rp>
        requires invocable<const _Fn&, _Rp, _Bound&...>
    constexpr auto operator()(_Rp&& __r) & noexcept(is_nothrow_invocable_v<const _Fn&, _Rp, _Bound&...>)
        -> invoke_result_t<const _Fn&, _Rp, _Bound&...> {
        return __call(__bound_, std::forward<_Rp>(__r), _Seq());
    }

    template <class _Rp>
        requires(!invocable<const _Fn&, _Rp, _Bound&...>)
    void operator()(_Rp&&) & = delete;

    template <class _Rp>
        requires invocable<const _Fn&, _Rp, const _Bound&...>
    constexpr auto operator()(_Rp&& __r) const& noexcept(is_nothrow_invocable_v<const _Fn&, _Rp, const _Bound&...>)
        -> invoke_result_t<const _Fn&, _Rp, const _Bound&...> {
        return __call(__bound_, std::forward<_Rp>(__r), _Seq());
    }

    template <class _Rp>
        requires(!invocable<const _Fn&, _Rp, const _Bound&...>)
    void operator()(_Rp&&) const& = delete;

    template <class _Rp>
        requires invocable<const _Fn&, _Rp, _Bound...>
    constexpr auto operator()(_Rp&& __r) && noexcept(is_nothrow_invocable_v<const _Fn&, _Rp, _Bound...>)
        -> invoke_result_t<const _Fn&, _Rp, _Bound...> {
        return __call(std::move(__bound_), std::forward<_Rp>(__r), _Seq());
    }

    template <class _Rp>
        requires(!invocable<const _Fn&, _Rp, _Bound...>)
    void operator()(_Rp&&) && = delete;

    template <class _Rp>
        requires invocable<const _Fn&, _Rp, const _Bound...>
    constexpr auto operator()(_Rp&& __r) const&& noexcept(is_nothrow_invocable_v<const _Fn&, _Rp, const _Bound...>)
        -> invoke_result_t<const _Fn&, _Rp, const _Bound...> {
        return __call(std::move(__bound_), std::forward<_Rp>(__r), _Seq());
    }

    template <class _Rp>
        requires(!invocable<const _Fn&, _Rp, const _Bound...>)
    void operator()(_Rp&&) const&& = delete;
};

/* The closure object of a range adaptor object and the arguments after the
 * range, which is only there where they can all be decay-copied. */
template <class _Fn, class... _Args>
concept __fcxx_can_bind = (constructible_from<decay_t<_Args>, _Args> && ...);

template <class _Fn, class... _Args>
constexpr __fcxx_partial<_Fn, decay_t<_Args>...> __fcxx_bind_adaptor(_Args&&... __args) noexcept(
    (is_nothrow_constructible_v<decay_t<_Args>, _Args> && ...)) {
    return __fcxx_partial<_Fn, decay_t<_Args>...>(__fcxx_partial_tag(), std::forward<_Args>(__args)...);
}

namespace views {}

} /* namespace ranges */
} /* namespace std */

#endif
