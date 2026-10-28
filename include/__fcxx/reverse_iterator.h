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


/* reverse_iterator, of <iterator>, which the headers of the containers and
 * views with reverse iterators need too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_REVERSE_ITERATOR_H
#define __FREESTANDING_CXX_HDRS_FCXX_REVERSE_ITERATOR_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
#include <__fcxx/addressof.h>
#include <compare>

namespace std {

/* [reverse.iterator], class template reverse_iterator */
template <class _It>
class reverse_iterator {
    template <class _Up>
    friend class reverse_iterator;

public:
    typedef _It iterator_type;
    using iterator_concept =
        conditional_t<random_access_iterator<_It>, random_access_iterator_tag, bidirectional_iterator_tag>;
    using iterator_category =
        conditional_t<derived_from<typename iterator_traits<_It>::iterator_category, random_access_iterator_tag>,
                      random_access_iterator_tag, typename iterator_traits<_It>::iterator_category>;
    using value_type = iter_value_t<_It>;
    using difference_type = iter_difference_t<_It>;
    using pointer = typename iterator_traits<_It>::pointer;
    using reference = iter_reference_t<_It>;

    constexpr reverse_iterator() : current() {}

    constexpr explicit reverse_iterator(_It __x) : current(__x) {}

    /* The conversions from other specializations are constrained, as LWG
     * 3435 has it, where that can be written. */
    template <class _Up>
        requires(!is_same_v<_Up, _It>) && convertible_to<const _Up&, _It>
    constexpr reverse_iterator(const reverse_iterator<_Up>& __u) : current(__u.current) {}

    template <class _Up>
        requires(!is_same_v<_Up, _It>) && convertible_to<const _Up&, _It> && assignable_from<_It&, const _Up&>
    constexpr reverse_iterator& operator=(const reverse_iterator<_Up>& __u) {
        current = __u.current;
        return *this;
    }

    constexpr _It base() const { return current; }

    constexpr reference operator*() const {
        _It __tmp = current;
        return *--__tmp;
    }

    constexpr pointer operator->() const
        requires(is_pointer_v<_It> || __fcxx_member_arrow<_It>)
    {
        if constexpr (is_pointer_v<_It>) {
            return std::prev(current);
        } else {
            return std::prev(current).operator->();
        }
    }

    constexpr reverse_iterator& operator++() {
        --current;
        return *this;
    }

    constexpr reverse_iterator operator++(int) {
        reverse_iterator __tmp = *this;
        --current;
        return __tmp;
    }

    constexpr reverse_iterator& operator--() {
        ++current;
        return *this;
    }

    constexpr reverse_iterator operator--(int) {
        reverse_iterator __tmp = *this;
        ++current;
        return __tmp;
    }

    constexpr reverse_iterator operator+(difference_type __n) const { return reverse_iterator(current - __n); }

    constexpr reverse_iterator& operator+=(difference_type __n) {
        current -= __n;
        return *this;
    }

    constexpr reverse_iterator operator-(difference_type __n) const { return reverse_iterator(current + __n); }

    constexpr reverse_iterator& operator-=(difference_type __n) {
        current += __n;
        return *this;
    }

    constexpr reference operator[](difference_type __n) const { return current[-__n - 1]; }

    friend constexpr iter_rvalue_reference_t<_It> iter_move(const reverse_iterator& __i) noexcept(
        is_nothrow_copy_constructible_v<_It> && noexcept(ranges::iter_move(--std::declval<_It&>()))) {
        _It __tmp = __i.base();
        return ranges::iter_move(--__tmp);
    }

    template <indirectly_swappable<_It> _It2>
    friend constexpr void iter_swap(const reverse_iterator& __x, const reverse_iterator<_It2>& __y) noexcept(
        is_nothrow_copy_constructible_v<_It> && is_nothrow_copy_constructible_v<_It2>
        && noexcept(ranges::iter_swap(--std::declval<_It&>(), --std::declval<_It2&>()))) {
        _It __xtmp = __x.base();
        _It2 __ytmp = __y.base();
        ranges::iter_swap(--__xtmp, --__ytmp);
    }

protected:
    _It current;
};

/* [reverse.iter.cmp], comparisons */

template <class _I1, class _I2>
    requires requires(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
        { __x.base() == __y.base() } -> __fcxx_convertible_to_bool;
    }
constexpr bool operator==(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
    return __x.base() == __y.base();
}

template <class _I1, class _I2>
    requires requires(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
        { __x.base() != __y.base() } -> __fcxx_convertible_to_bool;
    }
constexpr bool operator!=(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
    return __x.base() != __y.base();
}

template <class _I1, class _I2>
    requires requires(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
        { __x.base() > __y.base() } -> __fcxx_convertible_to_bool;
    }
constexpr bool operator<(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
    return __x.base() > __y.base();
}

template <class _I1, class _I2>
    requires requires(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
        { __x.base() < __y.base() } -> __fcxx_convertible_to_bool;
    }
constexpr bool operator>(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
    return __x.base() < __y.base();
}

template <class _I1, class _I2>
    requires requires(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
        { __x.base() >= __y.base() } -> __fcxx_convertible_to_bool;
    }
constexpr bool operator<=(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
    return __x.base() >= __y.base();
}

template <class _I1, class _I2>
    requires requires(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
        { __x.base() <= __y.base() } -> __fcxx_convertible_to_bool;
    }
constexpr bool operator>=(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y) {
    return __x.base() <= __y.base();
}

template <class _I1, three_way_comparable_with<_I1> _I2>
constexpr compare_three_way_result_t<_I1, _I2> operator<=>(const reverse_iterator<_I1>& __x,
                                                           const reverse_iterator<_I2>& __y) {
    return __y.base() <=> __x.base();
}

/* [reverse.iter.nonmember], non-member functions */
template <class _I1, class _I2>
constexpr auto operator-(const reverse_iterator<_I1>& __x, const reverse_iterator<_I2>& __y)
    -> decltype(__y.base() - __x.base()) {
    return __y.base() - __x.base();
}

template <class _It>
constexpr reverse_iterator<_It> operator+(typename reverse_iterator<_It>::difference_type __n,
                                                  const reverse_iterator<_It>& __x) {
    return reverse_iterator<_It>(__x.base() - __n);
}

template <class _It>
constexpr reverse_iterator<_It> make_reverse_iterator(_It __i) {
    return reverse_iterator<_It>(__i);
}

template <class _I1, class _I2>
    requires(!sized_sentinel_for<_I1, _I2>)
inline constexpr bool disable_sized_sentinel_for<reverse_iterator<_I1>, reverse_iterator<_I2> > = true;

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
