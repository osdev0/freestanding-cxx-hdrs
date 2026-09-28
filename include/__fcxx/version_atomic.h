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

/* <atomic>. __cpp_lib_char8_t is with the other headers that have it, in
 * <__fcxx/version.h>, and __cpp_lib_constexpr_atomic is not among the
 * freestanding macros. */

/* The lock-free type aliases, which a freestanding implementation may have
 * in C++20, as this one does where an integer type is always lock-free, and
 * which are not freestanding from C++23 on. */
#if !defined(__cpp_lib_atomic_lock_free_type_aliases) \
    && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ATOMIC)) && _FCXX_STD == 2020 \
    && (defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1) || defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2) \
        || defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4) || defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) \
        || __GCC_ATOMIC_CHAR_LOCK_FREE == 2 || __GCC_ATOMIC_SHORT_LOCK_FREE == 2 || __GCC_ATOMIC_INT_LOCK_FREE == 2 \
        || __GCC_ATOMIC_LONG_LOCK_FREE == 2 || __GCC_ATOMIC_LLONG_LOCK_FREE == 2)
#  define __cpp_lib_atomic_lock_free_type_aliases 201907L
#endif

#if !defined(__cpp_lib_atomic_is_always_lock_free) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ATOMIC)) \
    && _FCXX_STD >= 2017
#  define __cpp_lib_atomic_is_always_lock_free 201603L
#endif

#if !defined(__cpp_lib_atomic_flag_test) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ATOMIC)) \
    && _FCXX_STD >= 2020
#  define __cpp_lib_atomic_flag_test 201907L
#endif

#if !defined(__cpp_lib_atomic_float) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ATOMIC)) \
    && _FCXX_STD >= 2020
#  define __cpp_lib_atomic_float 201711L
#endif

#if !defined(__cpp_lib_atomic_ref) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ATOMIC)) \
    && _FCXX_STD >= 2020
#  if _FCXX_STD >= 2026
#    define __cpp_lib_atomic_ref 202603L
#  else
#    define __cpp_lib_atomic_ref 201806L
#  endif
#endif

#if !defined(__cpp_lib_atomic_value_initialization) \
    && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ATOMIC) || defined(_FCXX_VERSION_MEMORY)) \
    && _FCXX_STD >= 2020
#  define __cpp_lib_atomic_value_initialization 201911L
#endif

#if !defined(__cpp_lib_atomic_wait) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ATOMIC)) \
    && _FCXX_STD >= 2020
#  define __cpp_lib_atomic_wait 201907L
#endif

#if !defined(__cpp_lib_atomic_min_max) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ATOMIC)) \
    && _FCXX_STD >= 2026
#  define __cpp_lib_atomic_min_max 202506L
#endif

#if !defined(__cpp_lib_atomic_reductions) && (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_ATOMIC)) \
    && _FCXX_STD >= 2026
#  define __cpp_lib_atomic_reductions 202506L
#endif
