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


/* The constant iterators and sentinels of <iterator> in C++23 and later,
 * which ranges::cbegin and the other range access objects need too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_CONST_ITERATOR_H
#define __FREESTANDING_CXX_HDRS_FCXX_CONST_ITERATOR_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <compare>
#include <concepts>
#include <type_traits>
#include <__fcxx/addressof.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/pointer_traits.h>

namespace std {

/* [const.iterators.alias], alias templates */
template <indirectly_readable _It>
using iter_const_reference_t = common_reference_t<const iter_value_t<_It>&&, iter_reference_t<_It> >;

template <class _It>
concept __fcxx_constant_iterator = input_iterator<_It> && same_as<iter_const_reference_t<_It>, iter_reference_t<_It> >;

template <input_iterator _It>
class basic_const_iterator;

template <input_iterator _It>
using const_iterator = conditional_t<__fcxx_constant_iterator<_It>, _It, basic_const_iterator<_It> >;

template <class _Sp>
struct __fcxx_const_sentinel {
    using type = _Sp;
};

template <input_iterator _Sp>
struct __fcxx_const_sentinel<_Sp> {
    using type = const_iterator<_Sp>;
};

template <semiregular _Sp>
using const_sentinel = typename __fcxx_const_sentinel<_Sp>::type;

/* [const.iterators.iterator], class template basic_const_iterator */
template <class _Tp>
inline constexpr bool __fcxx_is_basic_const_iterator = false;

template <class _It>
inline constexpr bool __fcxx_is_basic_const_iterator<basic_const_iterator<_It> > = true;

template <class _Tp>
concept __fcxx_not_a_const_iterator = !__fcxx_is_basic_const_iterator<_Tp>;

template <indirectly_readable _It>
using __fcxx_iter_const_rvalue_reference_t =
    common_reference_t<const iter_value_t<_It>&&, iter_rvalue_reference_t<_It>>;

/* The iterator category, which basic_const_iterator only has for forward
 * iterators. */
template <class _It>
struct __fcxx_const_iterator_category {};

template <forward_iterator _It>
struct __fcxx_const_iterator_category<_It> {
    using iterator_category = typename iterator_traits<_It>::iterator_category;
};

template <input_iterator _It>
class basic_const_iterator : public __fcxx_const_iterator_category<_It> {
    template <input_iterator>
    friend class basic_const_iterator;

    _It __current_ = _It();

    using __reference = iter_const_reference_t<_It>;
    using __rvalue_reference = __fcxx_iter_const_rvalue_reference_t<_It>;

public:
    using iterator_type = _It;
    using iterator_concept = conditional_t<
        contiguous_iterator<_It>, contiguous_iterator_tag,
        conditional_t<random_access_iterator<_It>, random_access_iterator_tag,
                      conditional_t<bidirectional_iterator<_It>, bidirectional_iterator_tag,
                                    conditional_t<forward_iterator<_It>, forward_iterator_tag, input_iterator_tag>>>>;
    using value_type = iter_value_t<_It>;
    using difference_type = iter_difference_t<_It>;

    basic_const_iterator()
        requires default_initializable<_It>
    = default;

    constexpr basic_const_iterator(_It __current) : __current_(std::move(__current)) {}

    template <convertible_to<_It> _Up>
    constexpr basic_const_iterator(basic_const_iterator<_Up> __current) : __current_(std::move(__current.__current_)) {}

    template <__fcxx_different_from<basic_const_iterator> _Tp>
        requires convertible_to<_Tp, _It>
    constexpr basic_const_iterator(_Tp&& __current) : __current_(std::forward<_Tp>(__current)) {}

    constexpr const _It& base() const& noexcept { return __current_; }

    constexpr _It base() && { return std::move(__current_); }

    constexpr __reference operator*() const { return static_cast<__reference>(*__current_); }

    constexpr const auto* operator->() const
        requires is_lvalue_reference_v<iter_reference_t<_It> >
              && same_as<remove_cvref_t<iter_reference_t<_It> >, value_type>
    {
        if constexpr (contiguous_iterator<_It>) {
            return std::to_address(__current_);
        } else {
            return std::addressof(*__current_);
        }
    }

    constexpr basic_const_iterator& operator++() {
        ++__current_;
        return *this;
    }

    constexpr void operator++(int) { ++__current_; }

    constexpr basic_const_iterator operator++(int)
        requires forward_iterator<_It>
    {
        auto __tmp = *this;
        ++*this;
        return __tmp;
    }

    constexpr basic_const_iterator& operator--()
        requires bidirectional_iterator<_It>
    {
        --__current_;
        return *this;
    }

    constexpr basic_const_iterator operator--(int)
        requires bidirectional_iterator<_It>
    {
        auto __tmp = *this;
        --*this;
        return __tmp;
    }

    constexpr basic_const_iterator& operator+=(difference_type __n)
        requires random_access_iterator<_It>
    {
        __current_ += __n;
        return *this;
    }

    constexpr basic_const_iterator& operator-=(difference_type __n)
        requires random_access_iterator<_It>
    {
        __current_ -= __n;
        return *this;
    }

    constexpr __reference operator[](difference_type __n) const
        requires random_access_iterator<_It>
    {
        return static_cast<__reference>(__current_[__n]);
    }

    template <sentinel_for<_It> _Sp>
    constexpr bool operator==(const _Sp& __s) const {
        return __current_ == __s;
    }

    /* The conversions to other constant iterators, as P2836 has them. */
    template <__fcxx_not_a_const_iterator _CI>
        requires __fcxx_constant_iterator<_CI> && convertible_to<const _It&, _CI>
    constexpr operator _CI() const& {
        return __current_;
    }

    template <__fcxx_not_a_const_iterator _CI>
        requires __fcxx_constant_iterator<_CI> && convertible_to<_It, _CI>
    constexpr operator _CI() && {
        return std::move(__current_);
    }

    /* The comparisons with other types leave out basic_const_iterator by
     * substitution failure, before their constraints, which Clang 12 to 15
     * substitute into all at once, and recurse on otherwise. */
    constexpr bool operator<(const basic_const_iterator& __y) const
        requires random_access_iterator<_It>
    {
        return __current_ < __y.__current_;
    }

    constexpr bool operator>(const basic_const_iterator& __y) const
        requires random_access_iterator<_It>
    {
        return __current_ > __y.__current_;
    }

    constexpr bool operator<=(const basic_const_iterator& __y) const
        requires random_access_iterator<_It>
    {
        return __current_ <= __y.__current_;
    }

    constexpr bool operator>=(const basic_const_iterator& __y) const
        requires random_access_iterator<_It>
    {
        return __current_ >= __y.__current_;
    }

    constexpr auto operator<=>(const basic_const_iterator& __y) const
        requires random_access_iterator<_It> && three_way_comparable<_It>
    {
        return __current_ <=> __y.__current_;
    }

    template <class _Ip, class = __fcxx_enable_if_t<__fcxx_different_from<_Ip, basic_const_iterator> > >
    constexpr bool operator<(const _Ip& __y) const
        requires random_access_iterator<_It> && totally_ordered_with<_It, _Ip>
    {
        return __current_ < __y;
    }

    template <class _Ip, class = __fcxx_enable_if_t<__fcxx_different_from<_Ip, basic_const_iterator> > >
    constexpr bool operator>(const _Ip& __y) const
        requires random_access_iterator<_It> && totally_ordered_with<_It, _Ip>
    {
        return __current_ > __y;
    }

    template <class _Ip, class = __fcxx_enable_if_t<__fcxx_different_from<_Ip, basic_const_iterator> > >
    constexpr bool operator<=(const _Ip& __y) const
        requires random_access_iterator<_It> && totally_ordered_with<_It, _Ip>
    {
        return __current_ <= __y;
    }

    template <class _Ip, class = __fcxx_enable_if_t<__fcxx_different_from<_Ip, basic_const_iterator> > >
    constexpr bool operator>=(const _Ip& __y) const
        requires random_access_iterator<_It> && totally_ordered_with<_It, _Ip>
    {
        return __current_ >= __y;
    }

    template <class _Ip, class = __fcxx_enable_if_t<__fcxx_different_from<_Ip, basic_const_iterator> > >
    constexpr auto operator<=>(const _Ip& __y) const
        requires random_access_iterator<_It> && totally_ordered_with<_It, _Ip> && three_way_comparable_with<_It, _Ip>
    {
        return __current_ <=> __y;
    }

    template <class _Ip, class = __fcxx_enable_if_t<__fcxx_not_a_const_iterator<_Ip> > >
    friend constexpr bool operator<(const _Ip& __x, const basic_const_iterator& __y)
        requires random_access_iterator<_It> && totally_ordered_with<_It, _Ip>
    {
        return __x < __y.__current_;
    }

    template <class _Ip, class = __fcxx_enable_if_t<__fcxx_not_a_const_iterator<_Ip> > >
    friend constexpr bool operator>(const _Ip& __x, const basic_const_iterator& __y)
        requires random_access_iterator<_It> && totally_ordered_with<_It, _Ip>
    {
        return __x > __y.__current_;
    }

    template <class _Ip, class = __fcxx_enable_if_t<__fcxx_not_a_const_iterator<_Ip> > >
    friend constexpr bool operator<=(const _Ip& __x, const basic_const_iterator& __y)
        requires random_access_iterator<_It> && totally_ordered_with<_It, _Ip>
    {
        return __x <= __y.__current_;
    }

    template <class _Ip, class = __fcxx_enable_if_t<__fcxx_not_a_const_iterator<_Ip> > >
    friend constexpr bool operator>=(const _Ip& __x, const basic_const_iterator& __y)
        requires random_access_iterator<_It> && totally_ordered_with<_It, _Ip>
    {
        return __x >= __y.__current_;
    }

    friend constexpr basic_const_iterator operator+(const basic_const_iterator& __i, difference_type __n)
        requires random_access_iterator<_It>
    {
        return basic_const_iterator(__i.__current_ + __n);
    }

    friend constexpr basic_const_iterator operator+(difference_type __n, const basic_const_iterator& __i)
        requires random_access_iterator<_It>
    {
        return basic_const_iterator(__i.__current_ + __n);
    }

    friend constexpr basic_const_iterator operator-(const basic_const_iterator& __i, difference_type __n)
        requires random_access_iterator<_It>
    {
        return basic_const_iterator(__i.__current_ - __n);
    }

    template <sized_sentinel_for<_It> _Sp>
    constexpr difference_type operator-(const _Sp& __y) const {
        return __current_ - __y;
    }

    template <class _Sp, class = __fcxx_enable_if_t<__fcxx_not_a_const_iterator<_Sp> > >
        requires sized_sentinel_for<_Sp, _It>
    friend constexpr difference_type operator-(const _Sp& __x, const basic_const_iterator& __y) {
        return __x - __y.__current_;
    }

    friend constexpr __rvalue_reference iter_move(const basic_const_iterator& __i) noexcept(
        noexcept(static_cast<__rvalue_reference>(ranges::iter_move(__i.__current_)))) {
        return static_cast<__rvalue_reference>(ranges::iter_move(__i.__current_));
    }
};

template <class _Tp, common_with<_Tp> _Up>
    requires input_iterator<common_type_t<_Tp, _Up> >
struct common_type<basic_const_iterator<_Tp>, _Up> {
    using type = basic_const_iterator<common_type_t<_Tp, _Up> >;
};

template <class _Tp, common_with<_Tp> _Up>
    requires input_iterator<common_type_t<_Tp, _Up> >
struct common_type<_Up, basic_const_iterator<_Tp> > {
    using type = basic_const_iterator<common_type_t<_Tp, _Up> >;
};

template <class _Tp, common_with<_Tp> _Up>
    requires input_iterator<common_type_t<_Tp, _Up> >
struct common_type<basic_const_iterator<_Tp>, basic_const_iterator<_Up> > {
    using type = basic_const_iterator<common_type_t<_Tp, _Up> >;
};

template <input_iterator _It>
constexpr const_iterator<_It> make_const_iterator(_It __it) {
    return __it;
}

template <semiregular _Sp>
constexpr const_sentinel<_Sp> make_const_sentinel(_Sp __s) {
    return __s;
}

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
