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

/* <string>, whose only freestanding part, char_traits, is there in every
 * revision. __cpp_lib_char8_t and __cpp_lib_nonmember_container_access, which
 * it defines too, are with those of the other headers defining them. */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_STRING)) \
    && _FCXX_STD >= 2026
#  undef __cpp_lib_freestanding_char_traits
#  define __cpp_lib_freestanding_char_traits 202306L
#endif

/* <string_view>, whose parts from C++20 on need concepts. None of the other
 * macros of basic_string_view is freestanding. */

#if (defined(_FCXX_VERSION_ALL) || defined(_FCXX_VERSION_STRING_VIEW)) && _FCXX_STD >= 2026 \
    && _FCXX_CONCEPT_ITERATORS && _FCXX_HAS_DEDUCTION_GUIDES
#  undef __cpp_lib_freestanding_string_view
#  define __cpp_lib_freestanding_string_view 202311L
#endif

/* <inplace_vector> has no freestanding macro. */
