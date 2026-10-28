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

/* <functional> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_bind_back
#  if _FCXX_STD >= 2026
#    define __cpp_lib_bind_back 202306L
#  else
#    define __cpp_lib_bind_back 202202L
#  endif
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_bind_front
#  if _FCXX_STD >= 2026
#    define __cpp_lib_bind_front 202306L
#  else
#    define __cpp_lib_bind_front 201907L
#  endif
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) && _FCXX_STD >= 2023 && _FCXX_HAS_CONCEPTS
#  undef __cpp_lib_common_reference_wrapper
#  define __cpp_lib_common_reference_wrapper 202302L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_CONSTEXPR20
#  undef __cpp_lib_constexpr_functional
#  define __cpp_lib_constexpr_functional 201907L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_functional
#  define __cpp_lib_freestanding_functional 202306L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) \
    && _FCXX_HAS_CONSTANT_WRAPPER
#  undef __cpp_lib_function_ref
#  define __cpp_lib_function_ref 202604L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_invoke
#  define __cpp_lib_invoke 201411L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_CONCEPTS
#  undef __cpp_lib_invoke_r
#  define __cpp_lib_invoke_r 202106L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_not_fn
#  if _FCXX_STD >= 2026
#    define __cpp_lib_not_fn 202306L
#  else
#    define __cpp_lib_not_fn 201603L
#  endif
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL)) \
    && _FCXX_STD >= 2026 && _FCXX_CONSTRAINED_TUPLES
#  undef __cpp_lib_reference_wrapper
#  define __cpp_lib_reference_wrapper 202403L
#endif

/* <functional> and <memory> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_FUNCTIONAL) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_transparent_operators
#  define __cpp_lib_transparent_operators 201510L
#endif
