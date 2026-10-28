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


/* The feature-test macros of the headers of this area, in the form of those
 * of <__fcxx/version.h>, which includes this. */

/* <utility> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_as_const
#  define __cpp_lib_as_const 201510L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_HAS_CONSTANT_WRAPPER
#  undef __cpp_lib_constant_wrapper
#  define __cpp_lib_constant_wrapper 202603L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_CONSTEXPR20
#  undef __cpp_lib_constexpr_utility
#  define __cpp_lib_constexpr_utility 201811L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_exchange_function
#  define __cpp_lib_exchange_function 201304L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_forward_like
#  define __cpp_lib_forward_like 202207L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_utility
#  define __cpp_lib_freestanding_utility 202306L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) && _FCXX_STD >= 2023 && _FCXX_HAS_IF_CONSTEXPR
#  undef __cpp_lib_integer_comparison_functions
#  define __cpp_lib_integer_comparison_functions 202002L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_integer_sequence
#  if _FCXX_STD >= 2026
#    define __cpp_lib_integer_sequence 202511L
#  else
#    define __cpp_lib_integer_sequence 201304L
#  endif
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_observable_checkpoint
#  define __cpp_lib_observable_checkpoint 202506L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_to_underlying
#  define __cpp_lib_to_underlying 202102L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_unreachable
#  define __cpp_lib_unreachable 202202L
#endif

/* <utility> and <tuple> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY) || defined(_FCXX_VERSION_TUPLE)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_tuples_by_type
#  define __cpp_lib_tuples_by_type 201304L
#endif

/* <utility>, <tuple>, <optional>, <variant> and <expected> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_UTILITY) || defined(_FCXX_VERSION_TUPLE) \
        || defined(_FCXX_VERSION_OPTIONAL) || defined(_FCXX_VERSION_VARIANT) || defined(_FCXX_VERSION_EXPECTED)) \
    && _FCXX_STD >= 2026 && _FCXX_CONSTRAINED_TUPLES
#  undef __cpp_lib_constrained_equality
#  define __cpp_lib_constrained_equality 202411L
#endif

/* <tuple> */

#if _FCXX_STD >= 2023 \
    && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_TUPLE) \
        || (defined(_FCXX_VERSION_TYPE_TRAITS) && _FCXX_STD >= 2026))
#  undef __cpp_lib_apply
#  if _FCXX_STD >= 2026
#    define __cpp_lib_apply 202506L
#  else
#    define __cpp_lib_apply 201603L
#  endif
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_TUPLE)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_CONSTEXPR20
#  undef __cpp_lib_constexpr_tuple
#  define __cpp_lib_constexpr_tuple 201811L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_TUPLE)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_tuple
#  define __cpp_lib_freestanding_tuple 202306L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_TUPLE)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_make_from_tuple
#  define __cpp_lib_make_from_tuple 201606L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_TUPLE)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_tuple_element_t
#  define __cpp_lib_tuple_element_t 201402L
#endif
