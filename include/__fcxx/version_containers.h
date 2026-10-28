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

/* <array> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ARRAY)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_to_array
#  define __cpp_lib_to_array 201907L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ARRAY)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_array
#  define __cpp_lib_freestanding_array 202311L
#endif

/* <span> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_SPAN)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_span
#  if _FCXX_STD >= 2026
#    define __cpp_lib_span 202311L
#  else
#    define __cpp_lib_span 202002L
#  endif
#endif

/* <mdspan>, whose element access needs the multidimensional subscript
 * operator. submdspan needs constant_wrapper, which the slices are
 * canonicalized to, and aligned_accessor needs to tell constant evaluation
 * apart. */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MDSPAN)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_MULTIDIMENSIONAL_SUBSCRIPT
#  undef __cpp_lib_mdspan
#  if _FCXX_STD >= 2026
#    define __cpp_lib_mdspan 202406L
#  else
#    define __cpp_lib_mdspan 202207L
#  endif
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MDSPAN)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_MULTIDIMENSIONAL_SUBSCRIPT
#  undef __cpp_lib_freestanding_mdspan
#  define __cpp_lib_freestanding_mdspan 202311L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MDSPAN)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_MULTIDIMENSIONAL_SUBSCRIPT && _FCXX_HAS_CONSTANT_WRAPPER
#  undef __cpp_lib_submdspan
#  define __cpp_lib_submdspan 202603L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MDSPAN)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_MULTIDIMENSIONAL_SUBSCRIPT \
    && _FCXX_HAS_IS_CONSTANT_EVALUATED
#  undef __cpp_lib_aligned_accessor
#  define __cpp_lib_aligned_accessor 202411L
#endif
