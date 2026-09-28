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


/* The range access customization point objects, which <iterator> provides
 * as well as <ranges>, and the range concepts and ranges::distance, which
 * <iterator> needs, and which <ranges> builds on. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_ACCESS_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_ACCESS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
#include <__fcxx/pointer_traits.h>
#include <__fcxx/ranges_base.h>
#include <__fcxx/reverse_iterator.h>
#include <__fcxx/const_iterator.h>

namespace std {
namespace ranges {

/* The decay-copy of an expression, which auto(E) is in C++23. It is only
 * used where it is unevaluated, since it moves prvalues, which auto(E) does
 * not: the objects return their results by value instead, and are only
 * potentially-throwing where copying them is. */
template <class _Tp>
decay_t<_Tp> __fcxx_decay_copy(_Tp&& __t) noexcept(is_nothrow_convertible_v<_Tp, decay_t<_Tp> >);

template <class _Tp>
inline constexpr bool __fcxx_nothrow_decay_copy = !is_reference_v<_Tp> || is_nothrow_convertible_v<_Tp, decay_t<_Tp> >;

template <class _Tp>
concept __fcxx_class_or_enum = is_class_v<_Tp> || is_union_v<_Tp> || is_enum_v<_Tp>;

/* Whether ranges::begin and the other objects accept an expression of a
 * type, which is not the case of rvalues of ranges that are not borrowed. */
template <class _Tp>
concept __fcxx_maybe_borrowed = is_lvalue_reference_v<_Tp> || enable_borrowed_range<remove_cvref_t<_Tp> >;

template <class _Tp>
concept __fcxx_integer_like_type = __fcxx_is_integer_like<_Tp>;

template <class _Tp>
concept __fcxx_pointer_to_object = is_pointer_v<_Tp> && is_object_v<remove_pointer_t<_Tp> >;

template <class _Tp>
constexpr make_unsigned_t<_Tp> __fcxx_to_unsigned_like(_Tp __x) noexcept {
    return static_cast<make_unsigned_t<_Tp> >(__x);
}

/* [range.access.begin], ranges::begin. The objects find the functions of
 * their names by argument-dependent lookup only, as P2602 has it. */
namespace __begin {

void begin();

template <class _Tp>
concept __member = requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(__t.begin()) } -> input_or_output_iterator;
};

template <class _Tp>
concept __adl = !__member<_Tp> && __fcxx_class_or_enum<remove_cvref_t<_Tp> > && requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(begin(__t)) } -> input_or_output_iterator;
};

struct __fn {
    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && is_array_v<remove_reference_t<_Tp> >
    constexpr auto operator()(_Tp&& __t) const noexcept {
        return __t + 0;
    }

    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && (!is_array_v<remove_reference_t<_Tp> >) && __member<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(__t.begin()) && __fcxx_nothrow_decay_copy<decltype(__t.begin())>) {
        return __t.begin();
    }

    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && (!is_array_v<remove_reference_t<_Tp> >) && __adl<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(begin(__t)) && __fcxx_nothrow_decay_copy<decltype(begin(__t))>) {
        return begin(__t);
    }
};

} /* namespace __begin */

inline namespace __cpo {
inline constexpr __begin::__fn begin{};
}

/* [range.range] */
template <class _Tp>
using iterator_t = decltype(ranges::begin(std::declval<_Tp&>()));

/* [range.access.end], ranges::end */
namespace __end {

void end();

template <class _Tp>
concept __member = requires(_Tp& __t) {
    typename iterator_t<_Tp>;
    { ranges::__fcxx_decay_copy(__t.end()) } -> sentinel_for<iterator_t<_Tp> >;
};

template <class _Tp>
concept __adl = !__member<_Tp> && __fcxx_class_or_enum<remove_cvref_t<_Tp> > && requires(_Tp& __t) {
    typename iterator_t<_Tp>;
    { ranges::__fcxx_decay_copy(end(__t)) } -> sentinel_for<iterator_t<_Tp> >;
};

struct __fn {
    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && is_bounded_array_v<remove_reference_t<_Tp> >
    constexpr auto operator()(_Tp&& __t) const noexcept {
        return __t + extent_v<remove_reference_t<_Tp> >;
    }

    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && (!is_array_v<remove_reference_t<_Tp> >) && __member<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(__t.end()) && __fcxx_nothrow_decay_copy<decltype(__t.end())>) {
        return __t.end();
    }

    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && (!is_array_v<remove_reference_t<_Tp> >) && __adl<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(end(__t)) && __fcxx_nothrow_decay_copy<decltype(end(__t))>) {
        return end(__t);
    }
};

} /* namespace __end */

inline namespace __cpo {
inline constexpr __end::__fn end{};
}

/* [range.access.rbegin], ranges::rbegin */
namespace __rbegin {

void rbegin();

template <class _Tp>
concept __member = requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(__t.rbegin()) } -> input_or_output_iterator;
};

template <class _Tp>
concept __adl = !__member<_Tp> && __fcxx_class_or_enum<remove_cvref_t<_Tp> > && requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(rbegin(__t)) } -> input_or_output_iterator;
};

template <class _Tp>
concept __reverse = !__member<_Tp> && !__adl<_Tp> && requires(_Tp& __t) {
    { ranges::begin(__t) } -> bidirectional_iterator;
    { ranges::end(__t) } -> same_as<decltype(ranges::begin(__t))>;
};

struct __fn {
    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && __member<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(__t.rbegin()) && __fcxx_nothrow_decay_copy<decltype(__t.rbegin())>) {
        return __t.rbegin();
    }

    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && __adl<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(rbegin(__t)) && __fcxx_nothrow_decay_copy<decltype(rbegin(__t))>) {
        return rbegin(__t);
    }

    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && __reverse<_Tp>
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(std::make_reverse_iterator(ranges::end(__t)))) {
        return std::make_reverse_iterator(ranges::end(__t));
    }
};

} /* namespace __rbegin */

inline namespace __cpo {
inline constexpr __rbegin::__fn rbegin{};
}

/* [range.access.rend], ranges::rend */
namespace __rend {

void rend();

template <class _Tp>
concept __member = requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(__t.rend()) } -> sentinel_for<decltype(ranges::rbegin(std::declval<_Tp>()))>;
};

template <class _Tp>
concept __adl = !__member<_Tp> && __fcxx_class_or_enum<remove_cvref_t<_Tp> > && requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(rend(__t)) } -> sentinel_for<decltype(ranges::rbegin(std::declval<_Tp>()))>;
};

template <class _Tp>
concept __reverse = !__member<_Tp> && !__adl<_Tp> && requires(_Tp& __t) {
    { ranges::begin(__t) } -> bidirectional_iterator;
    { ranges::end(__t) } -> same_as<decltype(ranges::begin(__t))>;
};

struct __fn {
    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && __member<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(__t.rend()) && __fcxx_nothrow_decay_copy<decltype(__t.rend())>) {
        return __t.rend();
    }

    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && __adl<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(rend(__t)) && __fcxx_nothrow_decay_copy<decltype(rend(__t))>) {
        return rend(__t);
    }

    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && __reverse<_Tp>
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(std::make_reverse_iterator(ranges::begin(__t)))) {
        return std::make_reverse_iterator(ranges::begin(__t));
    }
};

} /* namespace __rend */

inline namespace __cpo {
inline constexpr __rend::__fn rend{};
}

/* [range.prim.size], ranges::size */
template <class>
inline constexpr bool disable_sized_range = false;

namespace __size {

void size();

template <class _Tp>
concept __member = !disable_sized_range<remove_cvref_t<_Tp> > && requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(__t.size()) } -> __fcxx_integer_like_type;
};

template <class _Tp>
concept __adl = !__member<_Tp> && __fcxx_class_or_enum<remove_cvref_t<_Tp> >
             && !disable_sized_range<remove_cvref_t<_Tp> > && requires(_Tp& __t) {
                    { ranges::__fcxx_decay_copy(size(__t)) } -> __fcxx_integer_like_type;
                };

template <class _Tp>
concept __difference = !__member<_Tp> && !__adl<_Tp> && requires(_Tp& __t) {
    { ranges::begin(__t) } -> forward_iterator;
    { ranges::end(__t) } -> sized_sentinel_for<decltype(ranges::begin(__t))>;
    ranges::__fcxx_to_unsigned_like(ranges::end(__t) - ranges::begin(__t));
};

struct __fn {
    template <class _Tp>
        requires is_bounded_array_v<remove_reference_t<_Tp> >
    constexpr size_t operator()(_Tp&&) const noexcept {
        return extent_v<remove_reference_t<_Tp> >;
    }

    template <class _Tp>
        requires(!is_array_v<remove_reference_t<_Tp> >) && __member<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(__t.size()) && __fcxx_nothrow_decay_copy<decltype(__t.size())>) {
        return __t.size();
    }

    template <class _Tp>
        requires(!is_array_v<remove_reference_t<_Tp> >) && __adl<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(size(__t)) && __fcxx_nothrow_decay_copy<decltype(size(__t))>) {
        return size(__t);
    }

    template <class _Tp>
        requires(!is_array_v<remove_reference_t<_Tp> >) && __difference<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(ranges::__fcxx_to_unsigned_like(ranges::end(__t) - ranges::begin(__t)))) {
        return ranges::__fcxx_to_unsigned_like(ranges::end(__t) - ranges::begin(__t));
    }
};

} /* namespace __size */

inline namespace __cpo {
inline constexpr __size::__fn size{};
}

/* [range.prim.ssize], ranges::ssize. The signed type is that of the size,
 * or ptrdiff_t if it is wider, as LWG 3403 has it. */
namespace __ssize {

struct __fn {
    template <class _Tp>
        requires requires(_Tp& __t) { ranges::size(__t); }
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(ranges::size(__t))) {
        using _Sp = make_signed_t<decltype(ranges::size(__t))>;
        using _Dp = conditional_t<(sizeof(ptrdiff_t) > sizeof(_Sp)), ptrdiff_t, _Sp>;
        return static_cast<_Dp>(ranges::size(__t));
    }
};

} /* namespace __ssize */

inline namespace __cpo {
inline constexpr __ssize::__fn ssize{};
}

#  if _FCXX_STD >= 2026
/* [range.prim.size.hint], ranges::reserve_hint */
namespace __reserve_hint {

void reserve_hint();

template <class _Tp>
concept __sized = requires(_Tp&& __t) { ranges::size(std::forward<_Tp>(__t)); };

template <class _Tp>
concept __member = !__sized<_Tp> && requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(__t.reserve_hint()) } -> __fcxx_integer_like_type;
};

template <class _Tp>
concept __adl = !__sized<_Tp> && !__member<_Tp> && __fcxx_class_or_enum<remove_cvref_t<_Tp> > && requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(reserve_hint(__t)) } -> __fcxx_integer_like_type;
};

struct __fn {
    template <class _Tp>
        requires __sized<_Tp>
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(ranges::size(std::forward<_Tp>(__t)))) {
        return ranges::size(std::forward<_Tp>(__t));
    }

    template <class _Tp>
        requires __member<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(__t.reserve_hint()) && __fcxx_nothrow_decay_copy<decltype(__t.reserve_hint())>) {
        return __t.reserve_hint();
    }

    template <class _Tp>
        requires __adl<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(reserve_hint(__t)) && __fcxx_nothrow_decay_copy<decltype(reserve_hint(__t))>) {
        return reserve_hint(__t);
    }
};

} /* namespace __reserve_hint */

inline namespace __cpo {
inline constexpr __reserve_hint::__fn reserve_hint{};
}
#  endif

/* [range.prim.empty], ranges::empty */
namespace __empty {

template <class _Tp>
concept __member = requires(_Tp& __t) { bool(__t.empty()); };

template <class _Tp>
concept __size = !__member<_Tp> && requires(_Tp& __t) { (ranges::size(__t) == 0); };

template <class _Tp>
concept __iterators = !__member<_Tp> && !__size<_Tp> && requires(_Tp& __t) {
    bool(ranges::begin(__t) == ranges::end(__t));
    { ranges::begin(__t) } -> forward_iterator;
};

struct __fn {
    template <class _Tp>
        requires __member<_Tp>
    constexpr bool operator()(_Tp&& __t) const noexcept(noexcept(bool(__t.empty()))) {
        return bool(__t.empty());
    }

    template <class _Tp>
        requires __size<_Tp>
    constexpr bool operator()(_Tp&& __t) const noexcept(noexcept(ranges::size(__t) == 0)) {
        return ranges::size(__t) == 0;
    }

    template <class _Tp>
        requires __iterators<_Tp>
    constexpr bool operator()(_Tp&& __t) const noexcept(noexcept(bool(ranges::begin(__t) == ranges::end(__t)))) {
        return bool(ranges::begin(__t) == ranges::end(__t));
    }
};

} /* namespace __empty */

inline namespace __cpo {
inline constexpr __empty::__fn empty{};
}

/* [range.prim.data], ranges::data */
namespace __data {

template <class _Tp>
concept __member = requires(_Tp& __t) {
    { ranges::__fcxx_decay_copy(__t.data()) } -> __fcxx_pointer_to_object;
};

template <class _Tp>
concept __begin = !__member<_Tp> && requires(_Tp& __t) {
    { ranges::begin(__t) } -> contiguous_iterator;
};

struct __fn {
    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && __member<_Tp>
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(__t.data()) && __fcxx_nothrow_decay_copy<decltype(__t.data())>) {
        return __t.data();
    }

    template <class _Tp>
        requires __fcxx_maybe_borrowed<_Tp> && __begin<_Tp>
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(std::to_address(ranges::begin(__t)))) {
        return std::to_address(ranges::begin(__t));
    }
};

} /* namespace __data */

inline namespace __cpo {
inline constexpr __data::__fn data{};
}

/* [range.range], ranges */
template <class _Tp>
concept range = requires(_Tp& __t) {
    ranges::begin(__t);
    ranges::end(__t);
};

template <class _Tp>
concept borrowed_range = range<_Tp> && (is_lvalue_reference_v<_Tp> || enable_borrowed_range<remove_cvref_t<_Tp> >);

template <range _Rp>
using sentinel_t = decltype(ranges::end(std::declval<_Rp&>()));

template <range _Rp>
using range_difference_t = iter_difference_t<iterator_t<_Rp> >;

template <range _Rp>
using range_value_t = iter_value_t<iterator_t<_Rp> >;

template <range _Rp>
using range_reference_t = iter_reference_t<iterator_t<_Rp> >;

template <range _Rp>
using range_rvalue_reference_t = iter_rvalue_reference_t<iterator_t<_Rp> >;

template <range _Rp>
using range_const_reference_t = iter_const_reference_t<iterator_t<_Rp> >;

template <range _Rp>
using range_common_reference_t = iter_common_reference_t<iterator_t<_Rp> >;

/* [range.sized], sized ranges */
#  if _FCXX_STD >= 2026
template <class _Tp>
concept approximately_sized_range = range<_Tp> && requires(_Tp& __t) { ranges::reserve_hint(__t); };

template <class _Tp>
concept sized_range = approximately_sized_range<_Tp> && requires(_Tp& __t) { ranges::size(__t); };
#  else
template <class _Tp>
concept sized_range = range<_Tp> && requires(_Tp& __t) { ranges::size(__t); };
#  endif

template <sized_range _Rp>
using range_size_t = decltype(ranges::size(std::declval<_Rp&>()));

/* [range.view], views. They need not be default constructible, as P2325
 * has it. */
template <class _Tp>
concept view = range<_Tp> && movable<_Tp> && enable_view<_Tp>;

/* [range.refinements], other range refinements */
template <class _Rp, class _Tp>
concept output_range = range<_Rp> && output_iterator<iterator_t<_Rp>, _Tp>;

template <class _Tp>
concept input_range = range<_Tp> && input_iterator<iterator_t<_Tp> >;

template <class _Tp>
concept forward_range = input_range<_Tp> && forward_iterator<iterator_t<_Tp> >;

template <class _Tp>
concept bidirectional_range = forward_range<_Tp> && bidirectional_iterator<iterator_t<_Tp> >;

template <class _Tp>
concept random_access_range = bidirectional_range<_Tp> && random_access_iterator<iterator_t<_Tp> >;

template <class _Tp>
concept contiguous_range = random_access_range<_Tp> && contiguous_iterator<iterator_t<_Tp> > && requires(_Tp& __t) {
    { ranges::data(__t) } -> same_as<add_pointer_t<range_reference_t<_Tp> > >;
};

template <class _Tp>
concept common_range = range<_Tp> && same_as<iterator_t<_Tp>, sentinel_t<_Tp> >;

template <class _Tp>
inline constexpr bool __fcxx_is_initializer_list = false;

template <class _Ep>
inline constexpr bool __fcxx_is_initializer_list<initializer_list<_Ep> > = true;

/* Lvalues of ranges that are not views, and rvalues of those that are
 * movable, are viewable, as P2415 has it. */
template <class _Tp>
concept viewable_range =
    range<_Tp>
    && ((view<remove_cvref_t<_Tp> > && constructible_from<remove_cvref_t<_Tp>, _Tp>)
        || (!view<remove_cvref_t<_Tp> >
            && (is_lvalue_reference_v<_Tp>
                || (movable<remove_reference_t<_Tp> > && !__fcxx_is_initializer_list<remove_cvref_t<_Tp> >))));

template <class _Tp>
concept constant_range = input_range<_Tp> && __fcxx_constant_iterator<iterator_t<_Tp> >;

#  if _FCXX_STD >= 2026
template <class _Tp>
concept __fcxx_sized_random_access_range = random_access_range<_Tp> && sized_range<_Tp>;
#  endif

/* [range.access.cbegin], [range.access.cend], [range.access.crbegin],
 * [range.access.crend], [range.prim.cdata]. Since C++23, they give constant
 * iterators, sentinels and pointers, of the range made constant where it
 * can be, which possibly-const-range makes it where it stays an input
 * range, as LWG 4027 has it. Before, they call the other objects on the
 * range made constant. */

template <input_range _Rp>
constexpr auto& __fcxx_possibly_const_range(_Rp& __r) noexcept {
    if constexpr (input_range<const _Rp>) {
        return const_cast<const _Rp&>(__r);
    } else {
        return __r;
    }
}

template <class _Tp>
constexpr auto __fcxx_as_const_pointer(const _Tp* __p) noexcept {
    return __p;
}

namespace __cbegin {
struct __fn {
    template <__fcxx_maybe_borrowed _Tp>
        requires requires(_Tp& __t) { ranges::begin(ranges::__fcxx_possibly_const_range(__t)); }
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(const_iterator<decltype(ranges::begin(ranges::__fcxx_possibly_const_range(__t)))>(
            ranges::begin(ranges::__fcxx_possibly_const_range(__t))))) {
        using _Up = decltype(ranges::begin(ranges::__fcxx_possibly_const_range(__t)));
        return const_iterator<_Up>(ranges::begin(ranges::__fcxx_possibly_const_range(__t)));
    }
};
} /* namespace __cbegin */

namespace __cend {
struct __fn {
    template <__fcxx_maybe_borrowed _Tp>
        requires requires(_Tp& __t) { ranges::end(ranges::__fcxx_possibly_const_range(__t)); }
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(const_sentinel<decltype(ranges::end(ranges::__fcxx_possibly_const_range(__t)))>(
            ranges::end(ranges::__fcxx_possibly_const_range(__t))))) {
        using _Up = decltype(ranges::end(ranges::__fcxx_possibly_const_range(__t)));
        return const_sentinel<_Up>(ranges::end(ranges::__fcxx_possibly_const_range(__t)));
    }
};
} /* namespace __cend */

namespace __crbegin {
struct __fn {
    template <__fcxx_maybe_borrowed _Tp>
        requires requires(_Tp& __t) { ranges::rbegin(ranges::__fcxx_possibly_const_range(__t)); }
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(const_iterator<decltype(ranges::rbegin(ranges::__fcxx_possibly_const_range(__t)))>(
            ranges::rbegin(ranges::__fcxx_possibly_const_range(__t))))) {
        using _Up = decltype(ranges::rbegin(ranges::__fcxx_possibly_const_range(__t)));
        return const_iterator<_Up>(ranges::rbegin(ranges::__fcxx_possibly_const_range(__t)));
    }
};
} /* namespace __crbegin */

namespace __crend {
struct __fn {
    template <__fcxx_maybe_borrowed _Tp>
        requires requires(_Tp& __t) { ranges::rend(ranges::__fcxx_possibly_const_range(__t)); }
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(const_sentinel<decltype(ranges::rend(ranges::__fcxx_possibly_const_range(__t)))>(
            ranges::rend(ranges::__fcxx_possibly_const_range(__t))))) {
        using _Up = decltype(ranges::rend(ranges::__fcxx_possibly_const_range(__t)));
        return const_sentinel<_Up>(ranges::rend(ranges::__fcxx_possibly_const_range(__t)));
    }
};
} /* namespace __crend */

namespace __cdata {
struct __fn {
    template <__fcxx_maybe_borrowed _Tp>
        requires requires(_Tp& __t) { ranges::data(ranges::__fcxx_possibly_const_range(__t)); }
    constexpr auto operator()(_Tp&& __t) const
        noexcept(noexcept(ranges::data(ranges::__fcxx_possibly_const_range(__t)))) {
        return ranges::__fcxx_as_const_pointer(ranges::data(ranges::__fcxx_possibly_const_range(__t)));
    }
};
} /* namespace __cdata */

inline namespace __cpo {
inline constexpr __cbegin::__fn cbegin{};
inline constexpr __cend::__fn cend{};
inline constexpr __crbegin::__fn crbegin{};
inline constexpr __crend::__fn crend{};
inline constexpr __cdata::__fn cdata{};
}

/* const_iterator_t is the type of ranges::cbegin, as LWG 3946 has it. */
template <range _Rp>
using const_iterator_t = decltype(ranges::cbegin(std::declval<_Rp&>()));

template <range _Rp>
using const_sentinel_t = decltype(ranges::cend(std::declval<_Rp&>()));

/* [range.iter.op.distance], ranges::distance. It takes iterators and
 * sentinels that are not sized, and those that are, by reference, as LWG
 * 3664 and LWG 4242 have it. */
struct __distance_fn {
    template <class _It, sentinel_for<_It> _Sp>
        requires(!sized_sentinel_for<_Sp, _It>)
    constexpr iter_difference_t<_It> operator()(_It __first, _Sp __last) const {
        iter_difference_t<_It> __n = 0;
        while (static_cast<bool>(__first != __last)) {
            ++__first;
            ++__n;
        }
        return __n;
    }

    template <class _It, sized_sentinel_for<decay_t<_It> > _Sp>
    constexpr iter_difference_t<decay_t<_It> > operator()(_It&& __first, _Sp __last) const {
        if constexpr (!is_array_v<remove_reference_t<_It> >) {
            return __last - __first;
        } else {
            return __last - static_cast<decay_t<_It> >(__first);
        }
    }

    template <range _Rp>
    constexpr range_difference_t<_Rp> operator()(_Rp&& __r) const {
        if constexpr (sized_range<_Rp>) {
            return static_cast<range_difference_t<_Rp> >(ranges::size(__r));
        } else {
            return (*this)(ranges::begin(__r), ranges::end(__r));
        }
    }
};

inline constexpr __distance_fn distance{};

} /* namespace ranges */
} /* namespace std */

#endif
