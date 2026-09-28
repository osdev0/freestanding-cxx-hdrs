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


/* What of <ranges> <algorithm> and the containers need too: view_interface,
 * subrange, dangling, borrowed_iterator_t and borrowed_subrange_t, from_range
 * and the exposition-only concepts of [range.utility.helpers]. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_CORE_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_CORE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <concepts>
#include <type_traits>
#include <__fcxx/iterator_base.h>
#include <__fcxx/iterator_ops.h>
#include <__fcxx/pointer_traits.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_base.h>
#include <__fcxx/tuple_like.h>
/* The pair-like types of C++20 are those that std::get knows, which has to
 * be declared before, for pair and tuple. */

namespace std {

/* [ranges.syn], from_range */
struct from_range_t {
    explicit from_range_t() = default;
};

inline constexpr from_range_t from_range{};

namespace ranges {

/* [range.utility.helpers], helper concepts. different-from is in
 * <__fcxx/iterator_base.h>. has-arrow requires a const operator->, as LWG
 * 4112 has it. */
template <class _Rp>
concept __fcxx_simple_view = view<_Rp> && range<const _Rp> && same_as<iterator_t<_Rp>, iterator_t<const _Rp> >
                          && same_as<sentinel_t<_Rp>, sentinel_t<const _Rp> >;

template <class _It>
concept __fcxx_has_arrow = input_iterator<_It> && (is_pointer_v<_It> || requires(const _It __i) { __i.operator->(); });

template <class _Rp>
concept __fcxx_range_with_movable_references = input_range<_Rp> && move_constructible<range_reference_t<_Rp> >
                                            && move_constructible<range_rvalue_reference_t<_Rp> >;

/* Constraints of the members of view_interface, on the derived class. */
template <class _Rp>
concept __fcxx_has_contiguous_iterator = contiguous_iterator<iterator_t<_Rp> >;

template <class _Rp>
concept __fcxx_has_sized_sentinel = sized_sentinel_for<sentinel_t<_Rp>, iterator_t<_Rp> >;

template <class _Rp>
concept __fcxx_can_empty = requires(_Rp& __r) { ranges::empty(__r); };

/* [view.interface], view interface. It does not derive from view_base, as
 * LWG 3549 has it, and its empty() needs a forward or a sized range, and its
 * size() gives an unsigned type, as LWG 3715 and 3646 have them. */
template <class _Dp>
    requires is_class_v<_Dp> && same_as<_Dp, remove_cv_t<_Dp> >
class view_interface {
    /* The members are templates of the derived class, whose constraints are
     * only checked where they are used, as Clang before 16 checks those of
     * members along with the class, where the derived class is not complete
     * yet, and keeps the results. */
    template <class _D2>
    constexpr _D2& __derived() noexcept {
        return static_cast<_D2&>(*this);
    }

    template <class _D2>
    constexpr const _D2& __derived() const noexcept {
        return static_cast<const _D2&>(*this);
    }

public:
    template <class _D2 = _Dp>
        requires sized_range<_D2> || forward_range<_D2>
    constexpr bool empty() {
        if constexpr (sized_range<_D2>) {
            return ranges::size(__derived<_D2>()) == 0;
        } else {
            return ranges::begin(__derived<_D2>()) == ranges::end(__derived<_D2>());
        }
    }

    template <class _D2 = _Dp>
        requires sized_range<const _D2> || forward_range<const _D2>
    constexpr bool empty() const {
        if constexpr (sized_range<const _D2>) {
            return ranges::size(__derived<_D2>()) == 0;
        } else {
            return ranges::begin(__derived<_D2>()) == ranges::end(__derived<_D2>());
        }
    }

    template <class _D2 = _Dp>
        requires input_range<_D2>
    constexpr auto cbegin() {
        return ranges::cbegin(__derived<_D2>());
    }

    template <class _D2 = _Dp>
        requires input_range<const _D2>
    constexpr auto cbegin() const {
        return ranges::cbegin(__derived<_D2>());
    }

    template <class _D2 = _Dp>
        requires input_range<_D2>
    constexpr auto cend() {
        return ranges::cend(__derived<_D2>());
    }

    template <class _D2 = _Dp>
        requires input_range<const _D2>
    constexpr auto cend() const {
        return ranges::cend(__derived<_D2>());
    }

    template <class _D2 = _Dp>
        requires __fcxx_can_empty<_D2>
    constexpr explicit operator bool() {
        return !ranges::empty(__derived<_D2>());
    }

    template <class _D2 = _Dp>
        requires __fcxx_can_empty<const _D2>
    constexpr explicit operator bool() const {
        return !ranges::empty(__derived<_D2>());
    }

    template <class _D2 = _Dp>
        requires __fcxx_has_contiguous_iterator<_D2>
    constexpr auto data() {
        return std::to_address(ranges::begin(__derived<_D2>()));
    }

    template <class _D2 = _Dp>
        requires range<const _D2> && __fcxx_has_contiguous_iterator<const _D2>
    constexpr auto data() const {
        return std::to_address(ranges::begin(__derived<_D2>()));
    }

    template <class _D2 = _Dp>
        requires forward_range<_D2> && __fcxx_has_sized_sentinel<_D2>
    constexpr auto size() {
        return ranges::__fcxx_to_unsigned_like(ranges::end(__derived<_D2>()) - ranges::begin(__derived<_D2>()));
    }

    template <class _D2 = _Dp>
        requires forward_range<const _D2> && __fcxx_has_sized_sentinel<const _D2>
    constexpr auto size() const {
        return ranges::__fcxx_to_unsigned_like(ranges::end(__derived<_D2>()) - ranges::begin(__derived<_D2>()));
    }

    template <class _D2 = _Dp>
        requires forward_range<_D2>
    constexpr decltype(auto) front() {
        return *ranges::begin(__derived<_D2>());
    }

    template <class _D2 = _Dp>
        requires forward_range<const _D2>
    constexpr decltype(auto) front() const {
        return *ranges::begin(__derived<_D2>());
    }

    template <class _D2 = _Dp>
        requires bidirectional_range<_D2> && common_range<_D2>
    constexpr decltype(auto) back() {
        return *ranges::prev(ranges::end(__derived<_D2>()));
    }

    template <class _D2 = _Dp>
        requires bidirectional_range<const _D2> && common_range<const _D2>
    constexpr decltype(auto) back() const {
        return *ranges::prev(ranges::end(__derived<_D2>()));
    }

    template <class _Rp = _Dp>
        requires random_access_range<_Rp>
    constexpr decltype(auto) operator[](range_difference_t<_Rp> __n) {
        return ranges::begin(__derived<_Rp>())[__n];
    }

    template <class _Rp = const _Dp>
        requires random_access_range<_Rp>
    constexpr decltype(auto) operator[](range_difference_t<_Rp> __n) const {
        return ranges::begin(__derived<remove_const_t<_Rp> >())[__n];
    }
};

/* [range.subrange], sub-ranges */
enum class subrange_kind : bool { unsized, sized };

/* The conversions of pointers that are not only qualification conversions
 * are slicing, as LWG 3470 has it. */
template <class _From, class _To>
concept __fcxx_uses_nonqualification_pointer_conversion = is_pointer_v<_From> && is_pointer_v<_To>
    && !convertible_to<remove_pointer_t<_From> (*)[], remove_pointer_t<_To> (*)[]>;

template <class _From, class _To>
concept __fcxx_convertible_to_non_slicing =
    convertible_to<_From, _To> && !__fcxx_uses_nonqualification_pointer_conversion<decay_t<_From>, decay_t<_To> >;

/* pair-like is one of the tuple-like types of two elements from C++23 on,
 * and any type with the tuple protocol for two elements before. */
template <class _Tp>
concept __fcxx_subrange_pair_like = __fcxx_pair_like<_Tp>;

template <class _Tp, class _Up, class _Vp>
concept __fcxx_pair_like_convertible_from =
    !range<_Tp> && !is_reference_v<_Tp> && __fcxx_subrange_pair_like<_Tp> && constructible_from<_Tp, _Up, _Vp>
    && __fcxx_convertible_to_non_slicing<_Up, tuple_element_t<0, _Tp>> && convertible_to<_Vp, tuple_element_t<1, _Tp>>;

/* The size a subrange stores, where it is sized and its iterator and
 * sentinel do not tell its size. */
template <bool _Store, class _Size>
struct __fcxx_subrange_size {
    constexpr __fcxx_subrange_size() noexcept = default;
    constexpr __fcxx_subrange_size(_Size) noexcept {}
};

template <class _Size>
struct __fcxx_subrange_size<true, _Size> {
    _Size __value = 0;

    constexpr __fcxx_subrange_size() noexcept = default;
    constexpr __fcxx_subrange_size(_Size __n) noexcept : __value(__n) {}
};

template <input_or_output_iterator _It, sentinel_for<_It> _Sp = _It,
          subrange_kind _Kp = sized_sentinel_for<_Sp, _It> ? subrange_kind::sized : subrange_kind::unsized>
    requires(_Kp == subrange_kind::sized || !sized_sentinel_for<_Sp, _It>)
class subrange : public view_interface<subrange<_It, _Sp, _Kp> > {
    static constexpr bool _StoreSize = _Kp == subrange_kind::sized && !sized_sentinel_for<_Sp, _It>;

    using __size_type = make_unsigned_t<iter_difference_t<_It> >;

    _It __begin_ = _It();
    _Sp __end_ = _Sp();
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_subrange_size<_StoreSize, __size_type> __size_;

public:
    subrange()
        requires default_initializable<_It>
    = default;

    template <__fcxx_convertible_to_non_slicing<_It> _Ip>
    constexpr subrange(_Ip __i, _Sp __s)
        requires(!_StoreSize)
        : __begin_(std::move(__i)), __end_(__s) {}

    template <__fcxx_convertible_to_non_slicing<_It> _Ip>
    constexpr subrange(_Ip __i, _Sp __s, __size_type __n)
        requires(_Kp == subrange_kind::sized)
        : __begin_(std::move(__i)), __end_(__s), __size_(__n) {}

    template <__fcxx_different_from<subrange> _Rp>
        requires borrowed_range<_Rp> && __fcxx_convertible_to_non_slicing<iterator_t<_Rp>, _It>
              && convertible_to<sentinel_t<_Rp>, _Sp> && (!_StoreSize)
    constexpr subrange(_Rp&& __r) : subrange(ranges::begin(__r), ranges::end(__r)) {}

    template <__fcxx_different_from<subrange> _Rp>
        requires borrowed_range<_Rp> && __fcxx_convertible_to_non_slicing<iterator_t<_Rp>, _It>
              && convertible_to<sentinel_t<_Rp>, _Sp> && _StoreSize && sized_range<_Rp>
    constexpr subrange(_Rp&& __r) : subrange(__r, static_cast<__size_type>(ranges::size(__r))) {}

    template <borrowed_range _Rp>
        requires __fcxx_convertible_to_non_slicing<iterator_t<_Rp>, _It> && convertible_to<sentinel_t<_Rp>, _Sp>
    constexpr subrange(_Rp&& __r, __size_type __n)
        requires(_Kp == subrange_kind::sized)
        : subrange{ranges::begin(__r), ranges::end(__r), __n} {}

    template <__fcxx_different_from<subrange> _PairLike>
        requires __fcxx_pair_like_convertible_from<_PairLike, const _It&, const _Sp&>
    constexpr operator _PairLike() const {
        return _PairLike(__begin_, __end_);
    }

    constexpr _It begin() const
        requires copyable<_It>
    {
        return __begin_;
    }

    constexpr _It begin()
        requires(!copyable<_It>)
    {
        return std::move(__begin_);
    }

    constexpr _Sp end() const { return __end_; }

    constexpr bool empty() const { return __begin_ == __end_; }

    constexpr __size_type size() const
        requires(_Kp == subrange_kind::sized)
    {
        if constexpr (_StoreSize) {
            return __size_.__value;
        } else {
            return ranges::__fcxx_to_unsigned_like(__end_ - __begin_);
        }
    }

    constexpr subrange next(iter_difference_t<_It> __n = 1) const&
        requires forward_iterator<_It>
    {
        auto __tmp = *this;
        __tmp.advance(__n);
        return __tmp;
    }

    constexpr subrange next(iter_difference_t<_It> __n = 1) && {
        advance(__n);
        return std::move(*this);
    }

    constexpr subrange prev(iter_difference_t<_It> __n = 1) const
        requires bidirectional_iterator<_It>
    {
        auto __tmp = *this;
        __tmp.advance(-__n);
        return __tmp;
    }

    constexpr subrange& advance(iter_difference_t<_It> __n) {
        if constexpr (bidirectional_iterator<_It>) {
            if (__n < 0) {
                ranges::advance(__begin_, __n);
                if constexpr (_StoreSize) {
                    __size_.__value += ranges::__fcxx_to_unsigned_like(-__n);
                }
                return *this;
            }
        }
        auto __d = __n - ranges::advance(__begin_, __n, __end_);
        if constexpr (_StoreSize) {
            __size_.__value -= ranges::__fcxx_to_unsigned_like(__d);
        }
        return *this;
    }
};

template <input_or_output_iterator _It, sentinel_for<_It> _Sp>
subrange(_It, _Sp) -> subrange<_It, _Sp>;

template <input_or_output_iterator _It, sentinel_for<_It> _Sp>
subrange(_It, _Sp, make_unsigned_t<iter_difference_t<_It> >) -> subrange<_It, _Sp, subrange_kind::sized>;

template <borrowed_range _Rp>
subrange(_Rp&&)
    -> subrange<iterator_t<_Rp>, sentinel_t<_Rp>,
                (sized_range<_Rp> || sized_sentinel_for<sentinel_t<_Rp>, iterator_t<_Rp> >) ? subrange_kind::sized
                                                                                           : subrange_kind::unsized>;

template <borrowed_range _Rp>
subrange(_Rp&&, make_unsigned_t<range_difference_t<_Rp> >)
    -> subrange<iterator_t<_Rp>, sentinel_t<_Rp>, subrange_kind::sized>;

template <class _It, class _Sp, subrange_kind _Kp>
inline constexpr bool enable_borrowed_range<subrange<_It, _Sp, _Kp> > = true;

/* get of the begin requires a copyable iterator, as LWG 3589 has it. */
template <size_t _Np, class _It, class _Sp, subrange_kind _Kp>
    requires((_Np == 0 && copyable<_It>) || _Np == 1)
constexpr auto get(const subrange<_It, _Sp, _Kp>& __r) {
    if constexpr (_Np == 0) {
        return __r.begin();
    } else {
        return __r.end();
    }
}

template <size_t _Np, class _It, class _Sp, subrange_kind _Kp>
    requires(_Np < 2)
constexpr auto get(subrange<_It, _Sp, _Kp>&& __r) {
    if constexpr (_Np == 0) {
        return __r.begin();
    } else {
        return __r.end();
    }
}

/* [range.dangling], dangling iterator handling */
struct dangling {
    constexpr dangling() noexcept = default;

    template <class... _Args>
    constexpr dangling(_Args&&...) noexcept {}
};

/* As the standard defines them: borrowed_subrange_t names subrange of the
 * iterator of the range either way, so that where no subrange can be made
 * of it, which its iterators being only movable causes, substituting into it
 * fails in the immediate context, as Clang before 21 needs of the return
 * types of the algorithms, which it substitutes into before it checks the
 * constraints, as C++20 had it before CWG 2369. */
template <range _Rp>
using borrowed_iterator_t = conditional_t<borrowed_range<_Rp>, iterator_t<_Rp>, dangling>;

template <range _Rp>
using borrowed_subrange_t = conditional_t<borrowed_range<_Rp>, subrange<iterator_t<_Rp> >, dangling>;

} /* namespace ranges */

using ranges::get;

/* The tuple protocol of subrange, which is tuple-like from C++23 on. */
template <class _It, class _Sp, ranges::subrange_kind _Kp>
struct tuple_size<ranges::subrange<_It, _Sp, _Kp> > : integral_constant<size_t, 2> {};

template <class _It, class _Sp, ranges::subrange_kind _Kp>
struct tuple_element<0, ranges::subrange<_It, _Sp, _Kp> > {
    using type = _It;
};

template <class _It, class _Sp, ranges::subrange_kind _Kp>
struct tuple_element<1, ranges::subrange<_It, _Sp, _Kp> > {
    using type = _Sp;
};

template <class _It, class _Sp, ranges::subrange_kind _Kp>
struct tuple_element<0, const ranges::subrange<_It, _Sp, _Kp> > {
    using type = _It;
};

template <class _It, class _Sp, ranges::subrange_kind _Kp>
struct tuple_element<1, const ranges::subrange<_It, _Sp, _Kp> > {
    using type = _Sp;
};

template <class _It, class _Sp, ranges::subrange_kind _Kp>
struct __fcxx_is_subrange<ranges::subrange<_It, _Sp, _Kp> > : true_type {};

template <class _It, class _Sp, ranges::subrange_kind _Kp>
struct __fcxx_is_tuple_like_impl<ranges::subrange<_It, _Sp, _Kp> > : true_type {};

} /* namespace std */

#endif
