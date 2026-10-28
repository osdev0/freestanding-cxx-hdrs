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


/* The feature-test macros of the library, which are only those that the
 * standard deems freestanding. Each is defined when the feature is there in
 * the revision of the language and with the compiler in use, to the value
 * of that revision.
 *
 * <version> defines all of them, and every other header those it is listed
 * with, by defining _FCXX_VERSION_<HEADER> before including this, which is
 * why there is no include guard. */

#include <__fcxx/config.h>

/* <cstddef> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_CSTDDEF)) \
    && _FCXX_STD >= 2017
#  undef __cpp_lib_byte
#  define __cpp_lib_byte 201603L
#endif

/* <cstdlib>, <cmath>, <cstring>, <cwchar>, <system_error> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_CSTDLIB) || defined(_FCXX_VERSION_CMATH)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_cstdlib
#  define __cpp_lib_freestanding_cstdlib 202306L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_CSTRING)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_cstring
#  define __cpp_lib_freestanding_cstring 202311L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_CWCHAR)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_cwchar
#  define __cpp_lib_freestanding_cwchar 202306L
#endif

/* Not by <cerrno>, which is not there, as the error numbers it would have
 * have no values in a freestanding program (see <system_error>). */
#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_SYSTEM_ERROR)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_errc
#  define __cpp_lib_freestanding_errc 202306L
#endif

/* <new> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NEW)) \
    && _FCXX_STD >= 2017
#  undef __cpp_lib_launder
#  define __cpp_lib_launder 201606L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NEW)) \
    && _FCXX_STD >= 2017
#  undef __cpp_lib_hardware_interference_size
#  define __cpp_lib_hardware_interference_size 201703L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NEW)) \
    && _FCXX_STD >= 2020 && defined(__cpp_impl_destroying_delete)
#  undef __cpp_lib_destroying_delete
#  define __cpp_lib_destroying_delete 201806L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NEW)) \
    && _FCXX_STD >= 2026 && defined(__cpp_constexpr) && __cpp_constexpr >= 202406L
#  undef __cpp_lib_constexpr_new
#  define __cpp_lib_constexpr_new 202406L
#endif

/* 0, as the headers have no default versions of the replaceable allocation
 * functions, which the program provides, that could meet the requirements of
 * a hosted implementation. */
#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_NEW)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_operator_new
#  define __cpp_lib_freestanding_operator_new 0
#endif

/* <exception> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_EXCEPTION)) \
    && _FCXX_STD >= 2017
#  undef __cpp_lib_uncaught_exceptions
#  define __cpp_lib_uncaught_exceptions 201411L
#endif

/* <typeinfo> */

/* GCC before 12 cannot compare the addresses of two type_info objects in
 * constant expressions. */
#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_TYPEINFO)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_IS_CONSTANT_EVALUATED && !(_FCXX_GCC && _FCXX_GCC < 1200)
#  undef __cpp_lib_constexpr_typeinfo
#  define __cpp_lib_constexpr_typeinfo 202106L
#endif

/* <initializer_list> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_INITIALIZER_LIST)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_initializer_list
#  define __cpp_lib_initializer_list 202511L
#endif

/* <limits>, and the other headers with specializations for char8_t */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_LIMITS) || defined(_FCXX_VERSION_ATOMIC) \
        || defined(_FCXX_VERSION_STRING) || defined(_FCXX_VERSION_STRING_VIEW)) \
    && _FCXX_STD >= 2020 && _FCXX_HAS_CHAR8_T
#  undef __cpp_lib_char8_t
#  define __cpp_lib_char8_t 201907L
#endif

/* <source_location> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_SOURCE_LOCATION)) \
    && _FCXX_STD >= 2020 && _FCXX_HAS_CONSTEVAL \
    && (_FCXX_HAS_BUILTIN(__builtin_source_location) || _FCXX_GCC >= 1100)
#  undef __cpp_lib_source_location
#  define __cpp_lib_source_location 201907L
#endif

/* <coroutine> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_COROUTINE)) \
    && _FCXX_STD >= 2020 && defined(__cpp_impl_coroutine) && __cpp_impl_coroutine >= 201902L
#  undef __cpp_lib_coroutine
#  define __cpp_lib_coroutine 201902L
#endif

/* <contracts> and <debugging> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_CONTRACTS)) \
    && _FCXX_STD >= 2026 && defined(__cpp_contracts)
#  undef __cpp_lib_contracts
#  define __cpp_lib_contracts 202502L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_DEBUGGING)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_debugging
#  define __cpp_lib_debugging 202403L
#endif

/* <concepts> and <compare> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_CONCEPTS) || defined(_FCXX_VERSION_COMPARE)) \
    && _FCXX_HAS_CONCEPTS
#  undef __cpp_lib_concepts
#  if _FCXX_STD >= 2023
#    define __cpp_lib_concepts 202207L
#  else
#    define __cpp_lib_concepts 202002L
#  endif
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_COMPARE)) \
    && _FCXX_HAS_THREE_WAY_COMPARISON && _FCXX_HAS_CONCEPTS && _FCXX_HAS_IS_CONSTANT_EVALUATED
#  undef __cpp_lib_three_way_comparison
#  define __cpp_lib_three_way_comparison 201907L
#endif

/* [version.syn] does not mark __cpp_lib_type_order as freestanding, but
 * type_order is in <compare>, all of which is freestanding, so the macro is
 * defined where type_order is there. __cpp_lib_define_static, which it marks
 * as freestanding, is not defined: its functions are those of <meta>, which
 * is not freestanding. */
#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_COMPARE)) \
    && _FCXX_STD >= 2026 && _FCXX_HEADER_COMPARE && _FCXX_HAS_BUILTIN(__builtin_type_order)
#  undef __cpp_lib_type_order
#  define __cpp_lib_type_order 202506L
#endif

/* <type_traits> */

#if defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_TYPE_TRAITS)

#  if _FCXX_STD >= 2014
#    undef __cpp_lib_integral_constant_callable
#    define __cpp_lib_integral_constant_callable 201304L
#  endif
#  if _FCXX_STD >= 2014
#    undef __cpp_lib_is_null_pointer
#    define __cpp_lib_is_null_pointer 201309L
#  endif
#  if _FCXX_STD >= 2014
#    undef __cpp_lib_is_final
#    define __cpp_lib_is_final 201402L
#  endif
#  if _FCXX_STD >= 2014
#    undef __cpp_lib_transformation_trait_aliases
#    define __cpp_lib_transformation_trait_aliases 201304L
#  endif
#  if _FCXX_STD >= 2017
#    undef __cpp_lib_bool_constant
#    define __cpp_lib_bool_constant 201505L
#  endif
#  if _FCXX_STD >= 2017                                                                                                \
      && _FCXX_HAS_UNIQUE_OBJECT_REPRESENTATIONS
#    undef __cpp_lib_has_unique_object_representations
#    define __cpp_lib_has_unique_object_representations 201606L
#  endif
#  if _FCXX_STD >= 2017 && _FCXX_HAS_IS_AGGREGATE
#    undef __cpp_lib_is_aggregate
#    define __cpp_lib_is_aggregate 201703L
#  endif
#  if _FCXX_STD >= 2017
#    undef __cpp_lib_is_invocable
#    define __cpp_lib_is_invocable 201703L
#  endif
#  if _FCXX_STD >= 2017
#    undef __cpp_lib_is_swappable
#    define __cpp_lib_is_swappable 201603L
#  endif
#  if _FCXX_STD >= 2017
#    undef __cpp_lib_logical_traits
#    define __cpp_lib_logical_traits 201510L
#  endif
#  if _FCXX_STD >= 2017 && _FCXX_HAS_VARIABLE_TEMPLATES
#    undef __cpp_lib_type_trait_variable_templates
#    define __cpp_lib_type_trait_variable_templates 201510L
#  endif
#  if _FCXX_STD >= 2017
#    undef __cpp_lib_void_t
#    define __cpp_lib_void_t 201411L
#  endif
#  if _FCXX_STD >= 2020
#    undef __cpp_lib_bounded_array_traits
#    define __cpp_lib_bounded_array_traits 201902L
#  endif
#  if _FCXX_STD >= 2020 && _FCXX_HAS_IS_CONSTANT_EVALUATED
#    undef __cpp_lib_is_constant_evaluated
#    define __cpp_lib_is_constant_evaluated 201811L
#  endif
#  if _FCXX_STD >= 2020 && _FCXX_HAS_IS_LAYOUT_COMPATIBLE                                                              \
      && _FCXX_HAS_IS_CORRESPONDING_MEMBER
#    undef __cpp_lib_is_layout_compatible
#    define __cpp_lib_is_layout_compatible 201907L
#  endif
#  if _FCXX_STD >= 2020
#    undef __cpp_lib_is_nothrow_convertible
#    define __cpp_lib_is_nothrow_convertible 201806L
#  endif
#  if _FCXX_STD >= 2020 && _FCXX_HAS_IS_POINTER_INTERCONVERTIBLE                                                       \
      && _FCXX_HAS_IS_POINTER_INTERCONVERTIBLE_WITH_CLASS
#    undef __cpp_lib_is_pointer_interconvertible
#    define __cpp_lib_is_pointer_interconvertible 201907L
#  endif
#  if _FCXX_STD >= 2020
#    undef __cpp_lib_remove_cvref
#    define __cpp_lib_remove_cvref 201711L
#  endif
#  if _FCXX_STD >= 2020
#    undef __cpp_lib_type_identity
#    define __cpp_lib_type_identity 201806L
#  endif
#  if _FCXX_STD >= 2020
#    undef __cpp_lib_unwrap_ref
#    define __cpp_lib_unwrap_ref 201811L
#  endif
#  if _FCXX_STD >= 2023
#    undef __cpp_lib_common_reference
#    define __cpp_lib_common_reference 202302L
#  endif
#  if _FCXX_STD >= 2023 && _FCXX_HAS_BUILTIN(__builtin_is_implicit_lifetime)
#    undef __cpp_lib_is_implicit_lifetime
#    define __cpp_lib_is_implicit_lifetime 202302L
#  endif
#  if _FCXX_STD >= 2023
#    undef __cpp_lib_is_scoped_enum
#    define __cpp_lib_is_scoped_enum 202011L
#  endif
#  if _FCXX_STD >= 2023                                                                                                \
      && _FCXX_HAS_BUILTIN(__reference_constructs_from_temporary)                                                      \
      && _FCXX_HAS_BUILTIN(__reference_converts_from_temporary)
#    undef __cpp_lib_reference_from_temporary
#    define __cpp_lib_reference_from_temporary 202202L
#  endif
#  if _FCXX_STD >= 2026 && _FCXX_HAS_IS_STRUCTURAL
#    undef __cpp_lib_is_structural
#    define __cpp_lib_is_structural 202603L
#  endif
#  if _FCXX_STD >= 2026 && _FCXX_HAS_IS_VIRTUAL_BASE_OF
#    undef __cpp_lib_is_virtual_base_of
#    define __cpp_lib_is_virtual_base_of 202406L
#  endif
#  if _FCXX_STD >= 2026 && _FCXX_HAS_IS_WITHIN_LIFETIME
#    undef __cpp_lib_is_within_lifetime
#    define __cpp_lib_is_within_lifetime 202603L
#  endif

#endif

/* <type_traits> and <functional> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_TYPE_TRAITS) || defined(_FCXX_VERSION_FUNCTIONAL)) \
    && _FCXX_STD >= 2014
#  undef __cpp_lib_result_of_sfinae
#  define __cpp_lib_result_of_sfinae 201210L
#endif

/* The headers written separately keep their macros in files of their own,
 * in the same form. */
#include <__fcxx/version_bit.h>
#include <__fcxx/version_atomic.h>
#include <__fcxx/version_ratio.h>
#include <__fcxx/version_charconv.h>
#include <__fcxx/version_random.h>
#include <__fcxx/version_containers.h>
#include <__fcxx/version_strings.h>
#include <__fcxx/version_optional.h>
#include <__fcxx/version_ranges.h>
#include <__fcxx/version_algorithm.h>
#include <__fcxx/version_utility.h>
#include <__fcxx/version_memory.h>
#include <__fcxx/version_functional.h>
#include <__fcxx/version_iterator.h>
