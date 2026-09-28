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


/* [range.utility.conv], ranges::to, of <ranges> from C++23 on. The elements
 * are appended with emplace_back, push_back, emplace or insert, as LWG 4016
 * has it, and the ranges of ranges are converted through a ref_view, as LWG
 * 3984 has it. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_RANGES_TO_H
#define __FREESTANDING_CXX_HDRS_FCXX_RANGES_TO_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>
#include <__fcxx/iterator_base.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_adaptors.h>
#include <__fcxx/ranges_core.h>
#include <__fcxx/ranges_util.h>

namespace std {
namespace ranges {

/* [range.utility.conv.general], reservable-container, container-appendable
 * and container-append */
template <class _Container>
concept __fcxx_reservable_container =
    sized_range<_Container> && requires(_Container& __c, range_size_t<_Container> __n) {
        __c.reserve(__n);
        { __c.capacity() } -> same_as<decltype(__n)>;
        { __c.max_size() } -> same_as<decltype(__n)>;
    };

template <class _Container, class _Ref>
concept __fcxx_can_emplace_back =
    requires(_Container& __c, _Ref&& __ref) { __c.emplace_back(std::forward<_Ref>(__ref)); };

template <class _Container, class _Ref>
concept __fcxx_can_push_back = requires(_Container& __c, _Ref&& __ref) { __c.push_back(std::forward<_Ref>(__ref)); };

template <class _Container, class _Ref>
concept __fcxx_can_emplace_end = requires(_Container& __c, _Ref&& __ref) {
    __c.emplace(__c.end(), std::forward<_Ref>(__ref));
};

template <class _Container, class _Ref>
concept __fcxx_can_insert_end = requires(_Container& __c, _Ref&& __ref) {
    __c.insert(__c.end(), std::forward<_Ref>(__ref));
};

template <class _Container, class _Ref>
concept __fcxx_container_appendable =
    __fcxx_can_emplace_back<_Container, _Ref> || __fcxx_can_push_back<_Container, _Ref>
    || __fcxx_can_emplace_end<_Container, _Ref> || __fcxx_can_insert_end<_Container, _Ref>;

template <class _Container, class _Ref>
constexpr void __fcxx_container_append(_Container& __c, _Ref&& __ref) {
    if constexpr (__fcxx_can_emplace_back<_Container, _Ref>) {
        __c.emplace_back(std::forward<_Ref>(__ref));
    } else if constexpr (__fcxx_can_push_back<_Container, _Ref>) {
        __c.push_back(std::forward<_Ref>(__ref));
    } else if constexpr (__fcxx_can_emplace_end<_Container, _Ref>) {
        __c.emplace(__c.end(), std::forward<_Ref>(__ref));
    } else {
        __c.insert(__c.end(), std::forward<_Ref>(__ref));
    }
}

/* Whether the iterators of a range have a category of the requirements of
 * C++17 input iterators. */
template <class _Rp>
concept __fcxx_legacy_input_range = requires {
    typename iterator_traits<iterator_t<_Rp> >::iterator_category;
    requires derived_from<typename iterator_traits<iterator_t<_Rp> >::iterator_category, input_iterator_tag>;
};

template <class>
inline constexpr bool __fcxx_to_false = false;

/* Whether the elements are appended as they are: a concept, so that the
 * value type of C is only named where C is an input range. */
template <class _Cp, class _Rp>
concept __fcxx_to_direct = !input_range<_Cp> || convertible_to<range_reference_t<_Rp>, range_value_t<_Cp> >;

/* A cv-unqualified class type, which unions are. */
template <class _Cp>
inline constexpr bool __fcxx_to_class = (is_class_v<_Cp> || is_union_v<_Cp>) && !is_const_v<_Cp> && !is_volatile_v<_Cp>;

/* [range.utility.conv.to], ranges::to */
template <class _Cp, input_range _Rp, class... _Args>
    requires(!view<_Cp>)
constexpr _Cp to(_Rp&& __r, _Args&&... __args) {
    static_assert(__fcxx_to_class<_Cp>, "ranges::to of a type that is not a cv-unqualified class type");
    if constexpr (__fcxx_to_direct<_Cp, _Rp>) {
        if constexpr (constructible_from<_Cp, _Rp, _Args...>) {
            return _Cp(std::forward<_Rp>(__r), std::forward<_Args>(__args)...);
        } else if constexpr (constructible_from<_Cp, from_range_t, _Rp, _Args...>) {
            return _Cp(from_range, std::forward<_Rp>(__r), std::forward<_Args>(__args)...);
        } else if constexpr (common_range<_Rp> && __fcxx_legacy_input_range<_Rp>
                             && constructible_from<_Cp, iterator_t<_Rp>, sentinel_t<_Rp>, _Args...>) {
            return _Cp(ranges::begin(__r), ranges::end(__r), std::forward<_Args>(__args)...);
        } else if constexpr (constructible_from<_Cp, _Args...>
                             && __fcxx_container_appendable<_Cp, range_reference_t<_Rp> >) {
            _Cp __c(std::forward<_Args>(__args)...);
#  if _FCXX_STD >= 2026
            if constexpr (approximately_sized_range<_Rp> && __fcxx_reservable_container<_Cp>) {
                __c.reserve(static_cast<range_size_t<_Cp> >(ranges::reserve_hint(__r)));
            }
#  else
            if constexpr (sized_range<_Rp> && __fcxx_reservable_container<_Cp>) {
                __c.reserve(static_cast<range_size_t<_Cp> >(ranges::size(__r)));
            }
#  endif
            auto __last = ranges::end(__r);
            for (auto __it = ranges::begin(__r); __it != __last; ++__it) {
                ranges::__fcxx_container_append(__c, *__it);
            }
            return __c;
        } else {
            static_assert(__fcxx_to_false<_Cp>, "ranges::to of a type that cannot be made from the range");
        }
    } else if constexpr (input_range<range_reference_t<_Rp> >) {
        return ranges::to<_Cp>(ref_view(__r) | views::transform([](auto&& __elem) {
                                   return ranges::to<range_value_t<_Cp> >(std::forward<decltype(__elem)>(__elem));
                               }),
                               std::forward<_Args>(__args)...);
    } else {
        static_assert(__fcxx_to_false<_Cp>,
                      "ranges::to of a type whose elements cannot be made from those of the range");
    }
}

/* The input iterator of the deduction of the type of the container of a
 * template, whose members are only declared. */
template <class _Rp>
struct __fcxx_to_input_iterator {
    using iterator_category = input_iterator_tag;
    using value_type = range_value_t<_Rp>;
    using difference_type = ptrdiff_t;
    using pointer = add_pointer_t<range_reference_t<_Rp> >;
    using reference = range_reference_t<_Rp>;

    reference operator*() const;
    pointer operator->() const;
    __fcxx_to_input_iterator& operator++();
    __fcxx_to_input_iterator operator++(int);
    bool operator==(const __fcxx_to_input_iterator&) const;
};

/* DEDUCE_EXPR */
template <template <class...> class _Cp, class _Rp, class... _Args>
struct __fcxx_to_deduce {
    static auto __deduce() {
        if constexpr (requires { _Cp(std::declval<_Rp>(), std::declval<_Args>()...); }) {
            return type_identity<decltype(_Cp(std::declval<_Rp>(), std::declval<_Args>()...))>();
        } else if constexpr (requires { _Cp(from_range, std::declval<_Rp>(), std::declval<_Args>()...); }) {
            return type_identity<decltype(_Cp(from_range, std::declval<_Rp>(), std::declval<_Args>()...))>();
        } else if constexpr (requires {
                                 _Cp(std::declval<__fcxx_to_input_iterator<_Rp> >(),
                                     std::declval<__fcxx_to_input_iterator<_Rp> >(), std::declval<_Args>()...);
                             }) {
            return type_identity<decltype(_Cp(std::declval<__fcxx_to_input_iterator<_Rp> >(),
                                              std::declval<__fcxx_to_input_iterator<_Rp> >(),
                                              std::declval<_Args>()...))>();
        } else {
            static_assert(__fcxx_to_false<_Rp>, "ranges::to of a template whose arguments cannot be deduced");
            return type_identity<void>();
        }
    }

    using type = typename decltype(__deduce())::type;
};

template <template <class...> class _Cp, input_range _Rp, class... _Args>
constexpr auto to(_Rp&& __r, _Args&&... __args) {
    return ranges::to<typename __fcxx_to_deduce<_Cp, _Rp, _Args...>::type>(std::forward<_Rp>(__r),
                                                                           std::forward<_Args>(__args)...);
}

/* [range.utility.conv.adaptors], ranges::to adaptors, which have no target
 * object, but call ranges::to. */
template <class _Cp>
struct __fcxx_to_fn {
    template <input_range _Rp, class... _Args>
    constexpr _Cp operator()(_Rp&& __r, _Args&&... __args) const {
        return ranges::to<_Cp>(std::forward<_Rp>(__r), std::forward<_Args>(__args)...);
    }
};

template <template <class...> class _Cp>
struct __fcxx_to_template_fn {
    template <input_range _Rp, class... _Args>
    constexpr auto operator()(_Rp&& __r, _Args&&... __args) const {
        return ranges::to<_Cp>(std::forward<_Rp>(__r), std::forward<_Args>(__args)...);
    }
};

template <class _Cp, class... _Args>
    requires(!view<_Cp>)
constexpr auto to(_Args&&... __args) {
    static_assert(__fcxx_to_class<_Cp>, "ranges::to of a type that is not a cv-unqualified class type");
    return __fcxx_partial<__fcxx_to_fn<_Cp>, decay_t<_Args>...>(__fcxx_partial_tag(), std::forward<_Args>(__args)...);
}

template <template <class...> class _Cp, class... _Args>
constexpr auto to(_Args&&... __args) {
    return __fcxx_partial<__fcxx_to_template_fn<_Cp>, decay_t<_Args>...>(__fcxx_partial_tag(),
                                                                        std::forward<_Args>(__args)...);
}

} /* namespace ranges */
} /* namespace std */

#endif
