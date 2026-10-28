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

/* <memory> */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_BUILTIN_ADDRESSOF
#  undef __cpp_lib_addressof_constexpr
#  define __cpp_lib_addressof_constexpr 201603L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MEMORY) || defined(_FCXX_VERSION_STRING)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_allocator_traits_is_always_equal
#  define __cpp_lib_allocator_traits_is_always_equal 201411L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_IS_CONSTANT_EVALUATED
#  undef __cpp_lib_assume_aligned
#  define __cpp_lib_assume_aligned 201811L
#endif

/* constexpr pointer_traits::pointer_to in C++20, unique_ptr in C++23, and
 * the specialized memory algorithms in C++26, which need the placement new
 * of constant evaluation. */
#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_CONSTEXPR20
#  undef __cpp_lib_constexpr_memory
#  if _FCXX_STD >= 2026 && defined(__cpp_constexpr) && __cpp_constexpr >= 202406L
#    define __cpp_lib_constexpr_memory 202506L
#  else
#    define __cpp_lib_constexpr_memory 202202L
#  endif
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_memory
#  define __cpp_lib_freestanding_memory 202502L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_is_sufficiently_aligned
#  define __cpp_lib_is_sufficiently_aligned 202411L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2026 && _FCXX_HAS_CONCEPTS
#  undef __cpp_lib_out_ptr
#  define __cpp_lib_out_ptr 202311L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2023
#  undef __cpp_lib_start_lifetime_as
#  define __cpp_lib_start_lifetime_as 202207L
#endif

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2023 && _FCXX_HAS_CONCEPTS
#  undef __cpp_lib_to_address
#  define __cpp_lib_to_address 201711L
#endif
