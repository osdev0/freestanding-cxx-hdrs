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


/* The iterator tags, iterator_traits, the associated types of iterators,
 * ranges::iter_move and ranges::iter_swap, the iterator concepts and the
 * concepts of the requirements of algorithms, of <iterator>, which the other
 * headers with iterators need too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_ITERATOR_BASE_H
#define __FREESTANDING_CXX_HDRS_FCXX_ITERATOR_BASE_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <type_traits>
#include <concepts>
#include <__fcxx/operations.h>
#include <__fcxx/pointer_traits.h>

namespace std {

/* [std.iterator.tags], standard iterator tags */
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
struct contiguous_iterator_tag : public random_access_iterator_tag {};

template <class _Tp>
struct __fcxx_iter_remove_cv {
    typedef _Tp type;
};

template <class _Tp>
struct __fcxx_iter_remove_cv<const _Tp> {
    typedef _Tp type;
};

template <class _Tp>
struct __fcxx_iter_remove_cv<volatile _Tp> {
    typedef _Tp type;
};

template <class _Tp>
struct __fcxx_iter_remove_cv<const volatile _Tp> {
    typedef _Tp type;
};

/* [iterator.synopsis] */
template <class _Tp>
using __fcxx_with_reference = _Tp&;

template <class _Tp>
concept __fcxx_can_reference = requires { typename __fcxx_with_reference<_Tp>; };

/* Clang before 16 takes *p for a p of type void* to be an expression of
 * type void&, which is left out. */
template <class _Tp>
concept __fcxx_not_void_pointer = !is_void_v<remove_pointer_t<remove_cvref_t<_Tp> > >;

template <class _Tp>
concept __fcxx_dereferenceable = __fcxx_not_void_pointer<_Tp> && requires(_Tp& __t) {
    { *__t } -> __fcxx_can_reference;
};

/* [incrementable.traits], incrementable traits */
template <class>
struct incrementable_traits {};

template <class _Tp>
    requires is_object_v<_Tp>
struct incrementable_traits<_Tp*> {
    using difference_type = ptrdiff_t;
};

template <class _It>
struct incrementable_traits<const _It> : incrementable_traits<_It> {};

template <class _Tp>
    requires requires { typename _Tp::difference_type; }
struct incrementable_traits<_Tp> {
    using difference_type = typename _Tp::difference_type;
};

template <class _Tp>
    requires(!requires { typename _Tp::difference_type; }) && requires(const _Tp& __a, const _Tp& __b) {
        { __a - __b } -> integral;
    }
struct incrementable_traits<_Tp> {
    using difference_type = make_signed_t<decltype(std::declval<_Tp>() - std::declval<_Tp>())>;
};

/* [readable.traits], indirectly readable traits */
template <class>
struct __fcxx_cond_value_type {};

template <class _Tp>
    requires is_object_v<_Tp>
struct __fcxx_cond_value_type<_Tp> {
    using value_type = remove_cv_t<_Tp>;
};

template <class _Tp>
concept __fcxx_has_member_value_type = requires { typename _Tp::value_type; };

template <class _Tp>
concept __fcxx_has_member_element_type = requires { typename _Tp::element_type; };

template <class>
struct indirectly_readable_traits {};

template <class _Tp>
struct indirectly_readable_traits<_Tp*> : __fcxx_cond_value_type<_Tp> {};

template <class _It>
    requires is_array_v<_It>
struct indirectly_readable_traits<_It> {
    using value_type = remove_cv_t<remove_extent_t<_It> >;
};

template <class _It>
struct indirectly_readable_traits<const _It> : indirectly_readable_traits<_It> {};

template <__fcxx_has_member_value_type _Tp>
struct indirectly_readable_traits<_Tp> : __fcxx_cond_value_type<typename _Tp::value_type> {};

template <__fcxx_has_member_element_type _Tp>
struct indirectly_readable_traits<_Tp> : __fcxx_cond_value_type<typename _Tp::element_type> {};

template <__fcxx_has_member_value_type _Tp>
    requires __fcxx_has_member_element_type<_Tp>
struct indirectly_readable_traits<_Tp> {};

template <__fcxx_has_member_value_type _Tp>
    requires __fcxx_has_member_element_type<_Tp>
          && same_as<remove_cv_t<typename _Tp::element_type>, remove_cv_t<typename _Tp::value_type> >
struct indirectly_readable_traits<_Tp> : __fcxx_cond_value_type<typename _Tp::value_type> {};

/* The alias templates with constraints go through class templates, as
 * Clang before 16 does not check the constraints of alias templates in
 * requires-expressions. */
template <class _Tp>
struct __fcxx_iter_reference {};

template <__fcxx_dereferenceable _Tp>
struct __fcxx_iter_reference<_Tp> {
    using type = decltype(*std::declval<_Tp&>());
};

template <__fcxx_dereferenceable _Tp>
using iter_reference_t = typename __fcxx_iter_reference<_Tp>::type;

/* [iterator.traits], the exposition-only concepts of the members of the
 * specializations of iterator_traits generated from the primary template. */
template <class _It>
concept __fcxx_cpp17_iterator = requires(_It __i) {
    { *__i } -> __fcxx_can_reference;
    { ++__i } -> same_as<_It&>;
    { *__i++ } -> __fcxx_can_reference;
} && copyable<_It>;

template <class _It>
concept __fcxx_cpp17_input_iterator = __fcxx_cpp17_iterator<_It> && equality_comparable<_It> && requires(_It __i) {
    typename incrementable_traits<_It>::difference_type;
    typename indirectly_readable_traits<_It>::value_type;
    typename common_reference_t<iter_reference_t<_It>&&, typename indirectly_readable_traits<_It>::value_type&>;
    typename common_reference_t<decltype(*__i++)&&, typename indirectly_readable_traits<_It>::value_type&>;
    requires signed_integral<typename incrementable_traits<_It>::difference_type>;
};

template <class _It>
concept __fcxx_cpp17_forward_iterator =
    __fcxx_cpp17_input_iterator<_It> && constructible_from<_It> && is_reference_v<iter_reference_t<_It> >
    && same_as<remove_cvref_t<iter_reference_t<_It> >, typename indirectly_readable_traits<_It>::value_type>
    && requires(_It __i) {
           { __i++ } -> convertible_to<const _It&>;
           { *__i++ } -> same_as<iter_reference_t<_It> >;
       };

template <class _It>
concept __fcxx_cpp17_bidirectional_iterator = __fcxx_cpp17_forward_iterator<_It> && requires(_It __i) {
    { --__i } -> same_as<_It&>;
    { __i-- } -> convertible_to<const _It&>;
    { *__i-- } -> same_as<iter_reference_t<_It> >;
};

template <class _It>
concept __fcxx_cpp17_random_access_iterator =
    __fcxx_cpp17_bidirectional_iterator<_It> && totally_ordered<_It>
    && requires(_It __i, typename incrementable_traits<_It>::difference_type __n) {
           { __i += __n } -> same_as<_It&>;
           { __i -= __n } -> same_as<_It&>;
           { __i + __n } -> same_as<_It>;
           { __n + __i } -> same_as<_It>;
           { __i - __n } -> same_as<_It>;
           { __i - __i } -> same_as<decltype(__n)>;
           { __i[__n] } -> convertible_to<iter_reference_t<_It> >;
       };

template <class _It>
concept __fcxx_iter_has_members = requires {
    typename _It::difference_type;
    typename _It::value_type;
    typename _It::reference;
    typename _It::iterator_category;
};

template <class _It>
struct __fcxx_iter_member_pointer {
    using type = void;
};

template <class _It>
    requires requires { typename _It::pointer; }
struct __fcxx_iter_member_pointer<_It> {
    using type = typename _It::pointer;
};

template <class _It>
struct __fcxx_iter_input_pointer : __fcxx_iter_member_pointer<_It> {};

template <class _It>
    requires(!requires { typename _It::pointer; }) && requires { std::declval<_It&>().operator->(); }
struct __fcxx_iter_input_pointer<_It> {
    using type = decltype(std::declval<_It&>().operator->());
};

template <class _It>
struct __fcxx_iter_input_reference {
    using type = iter_reference_t<_It>;
};

template <class _It>
    requires requires { typename _It::reference; }
struct __fcxx_iter_input_reference<_It> {
    using type = typename _It::reference;
};

template <class _It>
struct __fcxx_iter_input_category {
    using type = __fcxx_conditional_t<
        __fcxx_cpp17_random_access_iterator<_It>, random_access_iterator_tag,
        __fcxx_conditional_t<__fcxx_cpp17_bidirectional_iterator<_It>, bidirectional_iterator_tag,
                             __fcxx_conditional_t<__fcxx_cpp17_forward_iterator<_It>, forward_iterator_tag,
                                                  input_iterator_tag> > >;
};

template <class _It>
    requires requires { typename _It::iterator_category; }
struct __fcxx_iter_input_category<_It> {
    using type = typename _It::iterator_category;
};

template <class _It>
struct __fcxx_iter_output_difference {
    using type = void;
};

template <class _It>
    requires requires { typename incrementable_traits<_It>::difference_type; }
struct __fcxx_iter_output_difference<_It> {
    using type = typename incrementable_traits<_It>::difference_type;
};

template <class _It>
struct __fcxx_iterator_traits_impl {};

template <class _It>
    requires __fcxx_iter_has_members<_It>
struct __fcxx_iterator_traits_impl<_It> {
    using iterator_category = typename _It::iterator_category;
    using value_type = typename _It::value_type;
    using difference_type = typename _It::difference_type;
    using pointer = typename __fcxx_iter_member_pointer<_It>::type;
    using reference = typename _It::reference;
};

template <class _It>
    requires(!__fcxx_iter_has_members<_It>) && __fcxx_cpp17_input_iterator<_It>
struct __fcxx_iterator_traits_impl<_It> {
    using iterator_category = typename __fcxx_iter_input_category<_It>::type;
    using value_type = typename indirectly_readable_traits<_It>::value_type;
    using difference_type = typename incrementable_traits<_It>::difference_type;
    using pointer = typename __fcxx_iter_input_pointer<_It>::type;
    using reference = typename __fcxx_iter_input_reference<_It>::type;
};

template <class _It>
    requires(!__fcxx_iter_has_members<_It>) && (!__fcxx_cpp17_input_iterator<_It>) && __fcxx_cpp17_iterator<_It>
struct __fcxx_iterator_traits_impl<_It> {
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = typename __fcxx_iter_output_difference<_It>::type;
    using pointer = void;
    using reference = void;
};

/* [iterator.traits], iterator traits. The primary template names itself as
 * a member, which tells its specializations from those of the program. */
template <class _It>
struct iterator_traits : __fcxx_iterator_traits_impl<_It> {
    using __fcxx_primary_template = iterator_traits;
};

/* The value type of pointers to cv-qualified types is not cv-qualified, as
 * LWG 2952 has it. */
template <class _Tp>
    requires is_object_v<_Tp>
struct iterator_traits<_Tp*> {
    typedef contiguous_iterator_tag iterator_concept;
    typedef random_access_iterator_tag iterator_category;
    typedef typename __fcxx_iter_remove_cv<_Tp>::type value_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
};

/* [range.utility.helpers], different-from */
template <class _Tp, class _Up>
concept __fcxx_different_from = !same_as<remove_cvref_t<_Tp>, remove_cvref_t<_Up> >;

/* What the library means by an expression convertible to bool. */
template <class _Tp>
concept __fcxx_convertible_to_bool = is_convertible_v<_Tp, bool>;

template <class _It>
concept __fcxx_primary_iterator_traits =
    same_as<typename iterator_traits<_It>::__fcxx_primary_template, iterator_traits<_It> >;

/* [incrementable.traits] */
template <class _Tp>
using iter_difference_t =
    typename __fcxx_conditional_t<__fcxx_primary_iterator_traits<remove_cvref_t<_Tp> >,
                                  incrementable_traits<remove_cvref_t<_Tp> >,
                                  iterator_traits<remove_cvref_t<_Tp> > >::difference_type;

/* [readable.traits] */
template <class _Tp>
using iter_value_t = typename __fcxx_conditional_t<__fcxx_primary_iterator_traits<remove_cvref_t<_Tp> >,
                                                   indirectly_readable_traits<remove_cvref_t<_Tp> >,
                                                   iterator_traits<remove_cvref_t<_Tp> > >::value_type;

/* [iterator.cust.move], ranges::iter_move */
namespace ranges {
namespace __iter_move {

/* Found by ordinary lookup, never viable, so that only argument-dependent
 * lookup finds functions named iter_move. */
void iter_move();

template <class _Tp>
concept __class_or_enum = is_class_v<_Tp> || is_union_v<_Tp> || is_enum_v<_Tp>;

template <class _Tp>
concept __adl_iter_move = __class_or_enum<remove_cvref_t<_Tp> > && requires(_Tp&& __t) {
    iter_move(std::forward<_Tp>(__t));
};

template <class _Tp>
concept __deref_iter_move = !__adl_iter_move<_Tp> && __fcxx_not_void_pointer<_Tp>
                         && requires(_Tp&& __t) { *std::forward<_Tp>(__t); };

struct __fn {
    template <class _Tp>
        requires __adl_iter_move<_Tp>
    constexpr decltype(auto) operator()(_Tp&& __t) const noexcept(noexcept(iter_move(std::forward<_Tp>(__t)))) {
        return iter_move(std::forward<_Tp>(__t));
    }

    template <class _Tp>
        requires __deref_iter_move<_Tp> && is_lvalue_reference_v<decltype(*std::declval<_Tp>())>
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(*std::forward<_Tp>(__t)))
        -> remove_reference_t<decltype(*std::declval<_Tp>())>&& {
        return static_cast<remove_reference_t<decltype(*std::declval<_Tp>())>&&>(*std::forward<_Tp>(__t));
    }

    template <class _Tp>
        requires __deref_iter_move<_Tp> && (!is_lvalue_reference_v<decltype(*std::declval<_Tp>())>)
    constexpr auto operator()(_Tp&& __t) const noexcept(noexcept(*std::forward<_Tp>(__t)))
        -> decltype(*std::declval<_Tp>()) {
        return *std::forward<_Tp>(__t);
    }
};

} /* namespace __iter_move */

inline namespace __cpo {
inline constexpr __iter_move::__fn iter_move{};
}

} /* namespace ranges */

template <class _Tp>
struct __fcxx_iter_rvalue_reference {};

template <__fcxx_dereferenceable _Tp>
    requires requires(_Tp& __t) {
        { ranges::iter_move(__t) } -> __fcxx_can_reference;
    }
struct __fcxx_iter_rvalue_reference<_Tp> {
    using type = decltype(ranges::iter_move(std::declval<_Tp&>()));
};

template <__fcxx_dereferenceable _Tp>
    requires requires(_Tp& __t) {
        { ranges::iter_move(__t) } -> __fcxx_can_reference;
    }
using iter_rvalue_reference_t = typename __fcxx_iter_rvalue_reference<_Tp>::type;

/* [iterator.concept.readable], concept indirectly_readable */
template <class _In>
concept __fcxx_indirectly_readable_impl =
    requires(const _In __in) {
        typename iter_value_t<_In>;
        typename iter_reference_t<_In>;
        typename iter_rvalue_reference_t<_In>;
        { *__in } -> same_as<iter_reference_t<_In> >;
        { ranges::iter_move(__in) } -> same_as<iter_rvalue_reference_t<_In> >;
    } && common_reference_with<iter_reference_t<_In>&&, iter_value_t<_In>&>
    && common_reference_with<iter_reference_t<_In>&&, iter_rvalue_reference_t<_In>&&>
    && common_reference_with<iter_rvalue_reference_t<_In>&&, const iter_value_t<_In>&>;

template <class _In>
concept indirectly_readable = __fcxx_indirectly_readable_impl<remove_cvref_t<_In> >;

/* The iterator and the projection of the specializations of projected,
 * which name themselves and their arguments as members, for
 * indirect-value-t. */
template <class _Tp>
struct __fcxx_projected_args {};

template <class _Tp>
    requires requires { typename _Tp::__fcxx_projected_self; } && same_as<_Tp, typename _Tp::__fcxx_projected_self>
struct __fcxx_projected_args<_Tp> {
    using __iterator = typename _Tp::__fcxx_projected_iterator;
    using __proj = typename _Tp::__fcxx_projected_proj;
};

/* [indirectcallable.traits], indirect-value-t */
template <class _Tp>
struct __fcxx_indirect_value {
    using type = iter_value_t<_Tp>&;
};

template <class _Tp>
    requires requires { typename __fcxx_projected_args<_Tp>::__iterator; }
struct __fcxx_indirect_value<_Tp> {
    using type = invoke_result_t<typename __fcxx_projected_args<_Tp>::__proj&,
                                 typename __fcxx_indirect_value<typename __fcxx_projected_args<_Tp>::__iterator>::type>;
};

template <class _Tp>
using __fcxx_indirect_value_t = typename __fcxx_indirect_value<_Tp>::type;

template <indirectly_readable _Tp>
using iter_common_reference_t = common_reference_t<iter_reference_t<_Tp>, __fcxx_indirect_value_t<_Tp> >;

/* [iterator.concept.writable], concept indirectly_writable */
template <class _Out, class _Tp>
concept indirectly_writable = requires(_Out&& __o, _Tp&& __t) {
    *__o = std::forward<_Tp>(__t);
    *std::forward<_Out>(__o) = std::forward<_Tp>(__t);
    const_cast<const iter_reference_t<_Out>&&>(*__o) = std::forward<_Tp>(__t);
    const_cast<const iter_reference_t<_Out>&&>(*std::forward<_Out>(__o)) = std::forward<_Tp>(__t);
};

/* [iterator.concept.winc], concept weakly_incrementable. There are no
 * integer-class types: __int128 is an integral type. It does not require
 * default_initializable, as P2325 has it. */
template <class _Tp>
inline constexpr bool __fcxx_is_integer_like = integral<_Tp> && !same_as<remove_cv_t<_Tp>, bool>;

template <class _Tp>
inline constexpr bool __fcxx_is_signed_integer_like = signed_integral<_Tp>;

template <class _It>
concept weakly_incrementable = movable<_It> && requires(_It __i) {
    typename iter_difference_t<_It>;
    requires __fcxx_is_signed_integer_like<iter_difference_t<_It> >;
    { ++__i } -> same_as<_It&>;
    __i++;
};

/* [iterator.concept.inc], concept incrementable */
template <class _It>
concept incrementable = regular<_It> && weakly_incrementable<_It> && requires(_It __i) {
    { __i++ } -> same_as<_It>;
};

/* [iterator.concept.iterator], concept input_or_output_iterator */
template <class _It>
concept input_or_output_iterator = requires(_It __i) {
    { *__i } -> __fcxx_can_reference;
} && weakly_incrementable<_It>;

/* [iterator.concept.sentinel], concept sentinel_for. Integer-like types are
 * not sentinels, as LWG 4510 has it. */
template <class _Sp, class _It>
concept sentinel_for = semiregular<_Sp> && !__fcxx_is_integer_like<_Sp> && input_or_output_iterator<_It>
                    && __weakly_equality_comparable_with<_Sp, _It>;

/* [iterator.concept.sizedsentinel], concept sized_sentinel_for */
template <class _Sp, class _It>
inline constexpr bool disable_sized_sentinel_for = false;

template <class _Sp, class _It>
concept sized_sentinel_for = sentinel_for<_Sp, _It> && !disable_sized_sentinel_for<remove_cv_t<_Sp>, remove_cv_t<_It> >
                          && requires(const _It& __i, const _Sp& __s) {
                                 { __s - __i } -> same_as<iter_difference_t<_It> >;
                                 { __i - __s } -> same_as<iter_difference_t<_It> >;
                             };

/* [iterator.concepts.general], ITER_CONCEPT */
template <class _It>
using __fcxx_iter_traits_t = __fcxx_conditional_t<__fcxx_primary_iterator_traits<_It>, _It, iterator_traits<_It> >;

template <class _It>
struct __fcxx_iter_concept_impl {};

template <class _It>
    requires requires { typename __fcxx_iter_traits_t<_It>::iterator_concept; }
struct __fcxx_iter_concept_impl<_It> {
    using type = typename __fcxx_iter_traits_t<_It>::iterator_concept;
};

template <class _It>
    requires(!requires { typename __fcxx_iter_traits_t<_It>::iterator_concept; })
         && requires { typename __fcxx_iter_traits_t<_It>::iterator_category; }
struct __fcxx_iter_concept_impl<_It> {
    using type = typename __fcxx_iter_traits_t<_It>::iterator_category;
};

template <class _It>
    requires(!requires { typename __fcxx_iter_traits_t<_It>::iterator_concept; })
         && (!requires { typename __fcxx_iter_traits_t<_It>::iterator_category; })
         && __fcxx_primary_iterator_traits<_It>
struct __fcxx_iter_concept_impl<_It> {
    using type = random_access_iterator_tag;
};

template <class _It>
using __fcxx_iter_concept_t = typename __fcxx_iter_concept_impl<_It>::type;

/* [iterator.concept.input], concept input_iterator */
template <class _It>
concept input_iterator = input_or_output_iterator<_It> && indirectly_readable<_It>
                      && requires { typename __fcxx_iter_concept_t<_It>; }
                      && derived_from<__fcxx_iter_concept_t<_It>, input_iterator_tag>;

/* [iterator.concept.output], concept output_iterator */
template <class _It, class _Tp>
concept output_iterator = input_or_output_iterator<_It> && indirectly_writable<_It, _Tp>
                       && requires(_It __i, _Tp&& __t) { *__i++ = std::forward<_Tp>(__t); };

/* [iterator.concept.forward], concept forward_iterator */
template <class _It>
concept forward_iterator = input_iterator<_It> && derived_from<__fcxx_iter_concept_t<_It>, forward_iterator_tag>
                        && incrementable<_It> && sentinel_for<_It, _It>;

/* [iterator.concept.bidir], concept bidirectional_iterator */
template <class _It>
concept bidirectional_iterator = forward_iterator<_It>
                              && derived_from<__fcxx_iter_concept_t<_It>, bidirectional_iterator_tag>
                              && requires(_It __i) {
                                     { --__i } -> same_as<_It&>;
                                     { __i-- } -> same_as<_It>;
                                 };

/* [iterator.concept.random.access], concept random_access_iterator */
template <class _It>
concept random_access_iterator =
    bidirectional_iterator<_It> && derived_from<__fcxx_iter_concept_t<_It>, random_access_iterator_tag>
    && totally_ordered<_It> && sized_sentinel_for<_It, _It>
    && requires(_It __i, const _It __j, const iter_difference_t<_It> __n) {
           { __i += __n } -> same_as<_It&>;
           { __j + __n } -> same_as<_It>;
           { __n + __j } -> same_as<_It>;
           { __i -= __n } -> same_as<_It&>;
           { __j - __n } -> same_as<_It>;
           { __j[__n] } -> same_as<iter_reference_t<_It> >;
       };

/* [iterator.concept.contiguous], concept contiguous_iterator */
template <class _It>
concept contiguous_iterator = random_access_iterator<_It>
                           && derived_from<__fcxx_iter_concept_t<_It>, contiguous_iterator_tag>
                           && is_lvalue_reference_v<iter_reference_t<_It> >
                           && same_as<iter_value_t<_It>, remove_cvref_t<iter_reference_t<_It> > >
                           && requires(const _It& __i) {
                                  { std::to_address(__i) } -> same_as<add_pointer_t<iter_reference_t<_It> > >;
                              };

/* [indirectcallable.indirectinvocable], indirect callables. They require
 * the common reference type to be invocable only until C++26, as P2997 has
 * it. */
#if _FCXX_STD >= 2026
#  define _FCXX_INDIRECT_COMMON_REFERENCE(...) true
#else
#  define _FCXX_INDIRECT_COMMON_REFERENCE(...) __VA_ARGS__
#endif

template <class _Fp, class _It>
concept indirectly_unary_invocable =
    indirectly_readable<_It> && copy_constructible<_Fp> && invocable<_Fp&, __fcxx_indirect_value_t<_It> >
    && invocable<_Fp&, iter_reference_t<_It> >
    && _FCXX_INDIRECT_COMMON_REFERENCE(invocable<_Fp&, iter_common_reference_t<_It> >)
    && common_reference_with<invoke_result_t<_Fp&, __fcxx_indirect_value_t<_It> >,
                             invoke_result_t<_Fp&, iter_reference_t<_It> > >;

template <class _Fp, class _It>
concept indirectly_regular_unary_invocable =
    indirectly_readable<_It> && copy_constructible<_Fp> && regular_invocable<_Fp&, __fcxx_indirect_value_t<_It> >
    && regular_invocable<_Fp&, iter_reference_t<_It> >
    && _FCXX_INDIRECT_COMMON_REFERENCE(regular_invocable<_Fp&, iter_common_reference_t<_It> >)
    && common_reference_with<invoke_result_t<_Fp&, __fcxx_indirect_value_t<_It> >,
                             invoke_result_t<_Fp&, iter_reference_t<_It> > >;

template <class _Fp, class _It>
concept indirect_unary_predicate = indirectly_readable<_It> && copy_constructible<_Fp>
                                && predicate<_Fp&, __fcxx_indirect_value_t<_It> >
                                && predicate<_Fp&, iter_reference_t<_It> >
                                && _FCXX_INDIRECT_COMMON_REFERENCE(predicate<_Fp&, iter_common_reference_t<_It> >);

template <class _Fp, class _I1, class _I2>
concept indirect_binary_predicate =
    indirectly_readable<_I1> && indirectly_readable<_I2> && copy_constructible<_Fp>
    && predicate<_Fp&, __fcxx_indirect_value_t<_I1>, __fcxx_indirect_value_t<_I2> >
    && predicate<_Fp&, __fcxx_indirect_value_t<_I1>, iter_reference_t<_I2> >
    && predicate<_Fp&, iter_reference_t<_I1>, __fcxx_indirect_value_t<_I2> >
    && predicate<_Fp&, iter_reference_t<_I1>, iter_reference_t<_I2> >
    && _FCXX_INDIRECT_COMMON_REFERENCE(predicate<_Fp&, iter_common_reference_t<_I1>, iter_common_reference_t<_I2> >);

template <class _Fp, class _I1, class _I2 = _I1>
concept indirect_equivalence_relation =
    indirectly_readable<_I1> && indirectly_readable<_I2> && copy_constructible<_Fp>
    && equivalence_relation<_Fp&, __fcxx_indirect_value_t<_I1>, __fcxx_indirect_value_t<_I2> >
    && equivalence_relation<_Fp&, __fcxx_indirect_value_t<_I1>, iter_reference_t<_I2> >
    && equivalence_relation<_Fp&, iter_reference_t<_I1>, __fcxx_indirect_value_t<_I2> >
    && equivalence_relation<_Fp&, iter_reference_t<_I1>, iter_reference_t<_I2> >
    && _FCXX_INDIRECT_COMMON_REFERENCE(
        equivalence_relation<_Fp&, iter_common_reference_t<_I1>, iter_common_reference_t<_I2> >);

template <class _Fp, class _I1, class _I2 = _I1>
concept indirect_strict_weak_order =
    indirectly_readable<_I1> && indirectly_readable<_I2> && copy_constructible<_Fp>
    && strict_weak_order<_Fp&, __fcxx_indirect_value_t<_I1>, __fcxx_indirect_value_t<_I2> >
    && strict_weak_order<_Fp&, __fcxx_indirect_value_t<_I1>, iter_reference_t<_I2> >
    && strict_weak_order<_Fp&, iter_reference_t<_I1>, __fcxx_indirect_value_t<_I2> >
    && strict_weak_order<_Fp&, iter_reference_t<_I1>, iter_reference_t<_I2> >
    && _FCXX_INDIRECT_COMMON_REFERENCE(
        strict_weak_order<_Fp&, iter_common_reference_t<_I1>, iter_common_reference_t<_I2> >);

#undef _FCXX_INDIRECT_COMMON_REFERENCE

template <class _Fp, class... _Is>
    requires(indirectly_readable<_Is> && ...) && invocable<_Fp, iter_reference_t<_Is>...>
using indirect_result_t = invoke_result_t<_Fp, iter_reference_t<_Is>...>;

/* [projected], projected. It is a class template until C++26, and an alias
 * of a member class of a class template since, which does not make the
 * namespaces of its arguments associated with it, as P2538 has it. */
#if _FCXX_STD >= 2026

template <class _It, class _Proj>
struct __fcxx_projected_impl {
    struct __type {
        using value_type = remove_cvref_t<indirect_result_t<_Proj&, _It> >;
        using __fcxx_projected_self = __type;
        using __fcxx_projected_iterator = _It;
        using __fcxx_projected_proj = _Proj;
        indirect_result_t<_Proj&, _It> operator*() const;
    };
};

template <weakly_incrementable _It, class _Proj>
struct __fcxx_projected_impl<_It, _Proj> {
    struct __type {
        using value_type = remove_cvref_t<indirect_result_t<_Proj&, _It> >;
        using difference_type = iter_difference_t<_It>;
        using __fcxx_projected_self = __type;
        using __fcxx_projected_iterator = _It;
        using __fcxx_projected_proj = _Proj;
        indirect_result_t<_Proj&, _It> operator*() const;
    };
};

template <indirectly_readable _It, indirectly_regular_unary_invocable<_It> _Proj>
using projected = typename __fcxx_projected_impl<_It, _Proj>::__type;

template <indirectly_readable _It, indirectly_regular_unary_invocable<_It> _Proj>
using projected_value_t = remove_cvref_t<invoke_result_t<_Proj&, iter_value_t<_It>&> >;

#else

template <indirectly_readable _It, indirectly_regular_unary_invocable<_It> _Proj>
struct projected {
    using value_type = remove_cvref_t<indirect_result_t<_Proj&, _It> >;
    using __fcxx_projected_self = projected;
    using __fcxx_projected_iterator = _It;
    using __fcxx_projected_proj = _Proj;
    indirect_result_t<_Proj&, _It> operator*() const;
};

template <weakly_incrementable _It, class _Proj>
struct incrementable_traits<projected<_It, _Proj> > {
    using difference_type = iter_difference_t<_It>;
};

#endif

/* [alg.req.ind.move], concept indirectly_movable */
template <class _In, class _Out>
concept indirectly_movable = indirectly_readable<_In> && indirectly_writable<_Out, iter_rvalue_reference_t<_In> >;

template <class _In, class _Out>
concept indirectly_movable_storable =
    indirectly_movable<_In, _Out> && indirectly_writable<_Out, iter_value_t<_In> > && movable<iter_value_t<_In> >
    && constructible_from<iter_value_t<_In>, iter_rvalue_reference_t<_In> >
    && assignable_from<iter_value_t<_In>&, iter_rvalue_reference_t<_In> >;

/* [alg.req.ind.copy], concept indirectly_copyable */
template <class _In, class _Out>
concept indirectly_copyable = indirectly_readable<_In> && indirectly_writable<_Out, iter_reference_t<_In> >;

template <class _In, class _Out>
concept indirectly_copyable_storable =
    indirectly_copyable<_In, _Out> && indirectly_writable<_Out, iter_value_t<_In>&>
    && indirectly_writable<_Out, const iter_value_t<_In>&> && indirectly_writable<_Out, iter_value_t<_In>&&>
    && indirectly_writable<_Out, const iter_value_t<_In>&&> && copyable<iter_value_t<_In> >
    && constructible_from<iter_value_t<_In>, iter_reference_t<_In> >
    && assignable_from<iter_value_t<_In>&, iter_reference_t<_In> >;

/* [iterator.cust.swap], ranges::iter_swap */
namespace ranges {
namespace __iter_swap {

template <class _I1, class _I2>
void iter_swap(_I1, _I2) = delete;

template <class _T1, class _T2>
concept __adl_iter_swap = (__iter_move::__class_or_enum<remove_cvref_t<_T1> >
                           || __iter_move::__class_or_enum<remove_cvref_t<_T2> >)
                       && requires(_T1&& __a, _T2&& __b) {
                              (void)iter_swap(std::forward<_T1>(__a), std::forward<_T2>(__b));
                          };

template <class _T1, class _T2>
concept __swap_references = !__adl_iter_swap<_T1, _T2> && indirectly_readable<_T1> && indirectly_readable<_T2>
                         && swappable_with<iter_reference_t<_T1>, iter_reference_t<_T2> >;

template <class _T1, class _T2>
concept __exchange_values = !__adl_iter_swap<_T1, _T2> && !__swap_references<_T1, _T2>
                         && indirectly_movable_storable<remove_reference_t<_T1>, remove_reference_t<_T2> >
                         && indirectly_movable_storable<remove_reference_t<_T2>, remove_reference_t<_T1> >;

template <class _Xp, class _Yp>
constexpr iter_value_t<_Xp> __iter_exchange_move(_Xp&& __x, _Yp&& __y) noexcept(
    noexcept(iter_value_t<_Xp>(ranges::iter_move(__x))) && noexcept(*__x = ranges::iter_move(__y))) {
    iter_value_t<_Xp> __old_value(ranges::iter_move(__x));
    *__x = ranges::iter_move(__y);
    return __old_value;
}

struct __fn {
    template <class _T1, class _T2>
        requires __adl_iter_swap<_T1, _T2>
    constexpr void operator()(_T1&& __a, _T2&& __b) const
        noexcept(noexcept(iter_swap(std::forward<_T1>(__a), std::forward<_T2>(__b)))) {
        (void)iter_swap(std::forward<_T1>(__a), std::forward<_T2>(__b));
    }

    template <class _T1, class _T2>
        requires __swap_references<_T1, _T2>
    constexpr void operator()(_T1&& __a, _T2&& __b) const
        noexcept(noexcept(ranges::swap(*std::forward<_T1>(__a), *std::forward<_T2>(__b)))) {
        ranges::swap(*std::forward<_T1>(__a), *std::forward<_T2>(__b));
    }

    template <class _T1, class _T2>
        requires __exchange_values<_T1, _T2>
    constexpr void operator()(_T1&& __a, _T2&& __b) const
        noexcept(noexcept(*__a = __iter_swap::__iter_exchange_move(std::forward<_T2>(__b), __a))) {
        (void)(*__a = __iter_swap::__iter_exchange_move(std::forward<_T2>(__b), __a));
    }
};

} /* namespace __iter_swap */

inline namespace __cpo {
inline constexpr __iter_swap::__fn iter_swap{};
}

} /* namespace ranges */

/* [alg.req.ind.swap], concept indirectly_swappable */
template <class _I1, class _I2 = _I1>
concept indirectly_swappable = indirectly_readable<_I1> && indirectly_readable<_I2>
                            && requires(const _I1 __i1, const _I2 __i2) {
                                   ranges::iter_swap(__i1, __i1);
                                   ranges::iter_swap(__i2, __i2);
                                   ranges::iter_swap(__i1, __i2);
                                   ranges::iter_swap(__i2, __i1);
                               };

/* [alg.req.ind.cmp], concept indirectly_comparable */
template <class _I1, class _I2, class _Rp, class _P1 = identity, class _P2 = identity>
concept indirectly_comparable = indirect_binary_predicate<_Rp, projected<_I1, _P1>, projected<_I2, _P2> >;

/* [alg.req.permutable], concept permutable */
template <class _It>
concept permutable = forward_iterator<_It> && indirectly_movable_storable<_It, _It> && indirectly_swappable<_It, _It>;

/* [alg.req.mergeable], concept mergeable */
template <class _I1, class _I2, class _Out, class _Rp = ranges::less, class _P1 = identity, class _P2 = identity>
concept mergeable = input_iterator<_I1> && input_iterator<_I2> && weakly_incrementable<_Out>
                 && indirectly_copyable<_I1, _Out> && indirectly_copyable<_I2, _Out>
                 && indirect_strict_weak_order<_Rp, projected<_I1, _P1>, projected<_I2, _P2> >;

/* [alg.req.sortable], concept sortable */
template <class _It, class _Rp = ranges::less, class _Pp = identity>
concept sortable = permutable<_It> && indirect_strict_weak_order<_Rp, projected<_It, _Pp> >;

} /* namespace std */

#endif
