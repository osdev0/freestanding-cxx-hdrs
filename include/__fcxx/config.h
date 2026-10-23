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

/* Internal configuration shared by all of the headers. Nothing in here is
 * part of the interface of the library. The macros are prefixed with _FCXX_
 * (freestanding C++) as they are used all over the headers. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_CONFIG_H
#define __FREESTANDING_CXX_HDRS_FCXX_CONFIG_H 1

#pragma GCC system_header

/* The revision of the language, as the year of the standard. The values in
 * between the final ones are those of the draft modes. */
#if __cplusplus > 202302L
#  define _FCXX_STD 2026
#elif __cplusplus > 202002L
#  define _FCXX_STD 2023
#elif __cplusplus > 201703L
#  define _FCXX_STD 2020
#elif __cplusplus > 201402L
#  define _FCXX_STD 2017
#elif __cplusplus > 201103L
#  define _FCXX_STD 2014
#elif __cplusplus >= 201103L
#  define _FCXX_STD 2011
#else
#  define _FCXX_STD 1998
#endif

/* Compiler versions, as major * 100 + minor, or 0 for the other compiler.
 * Clang also defines __GNUC__, so it is checked for first. */
#if defined(__clang__)
#  define _FCXX_CLANG (__clang_major__ * 100 + __clang_minor__)
#  define _FCXX_GCC 0
#elif defined(__GNUC__)
#  define _FCXX_CLANG 0
#  define _FCXX_GCC (__GNUC__ * 100 + __GNUC_MINOR__)
#else
#  error "The freestanding C++ headers require GCC or Clang."
#endif

#if (_FCXX_GCC && _FCXX_GCC < 408) || (_FCXX_CLANG && _FCXX_CLANG < 600)
#  error "The freestanding C++ headers require GCC 4.8 or later, or Clang 6 or later."
#endif

/* Whether functions can have ref-qualifiers.
 * Workaround: GCC 4.8.0, all targets: it has none, which came in 4.8.1. */
#if _FCXX_GCC == 408 && __GNUC_PATCHLEVEL__ == 0
#  define _FCXX_HAS_REF_QUALIFIERS 0
#else
#  define _FCXX_HAS_REF_QUALIFIERS 1
#endif

#ifdef __has_builtin
#  define _FCXX_HAS_BUILTIN(x) __has_builtin(x)
#else
#  define _FCXX_HAS_BUILTIN(x) 0
#endif

#ifdef __has_feature
#  define _FCXX_HAS_FEATURE(x) __has_feature(x)
#else
#  define _FCXX_HAS_FEATURE(x) 0
#endif

#ifdef __has_attribute
#  define _FCXX_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#  define _FCXX_HAS_ATTRIBUTE(x) 0
#endif

#if defined(__has_cpp_attribute) && _FCXX_STD >= 2011
#  define _FCXX_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#  define _FCXX_HAS_CPP_ATTRIBUTE(x) 0
#endif

/* Clang reports the type trait keywords through __has_builtin only from
 * version 10 on, and through __has_feature before that. GCC reports them
 * through __has_builtin, from version 10 on; the users of this macro check
 * the version of older GCCs themselves. */
#define _FCXX_HAS_TRAIT(builtin, feature) \
    (_FCXX_HAS_BUILTIN(builtin) || _FCXX_HAS_FEATURE(feature))

/* constexpr, for the functions the standard makes constexpr from a given
 * revision on. The C++11 kind can only be used with functions written to
 * the C++11 rules, and never with non-const member functions, which C++11
 * makes const. The others need the relaxed rules of C++14, and the C++20
 * and later kinds need those of C++20. Where the compiler lacks the rules,
 * the functions are left inline, and not constexpr. */
#if _FCXX_STD >= 2011
#  define _FCXX_CONSTEXPR11 constexpr
#else
#  define _FCXX_CONSTEXPR11 inline
#endif

#if defined(__cpp_constexpr) && __cpp_constexpr >= 201304L
#  define _FCXX_HAS_CONSTEXPR14 1
#  define _FCXX_CONSTEXPR14 constexpr
#else
#  define _FCXX_HAS_CONSTEXPR14 0
#  define _FCXX_CONSTEXPR14 inline
#endif

#if _FCXX_STD >= 2017 && _FCXX_HAS_CONSTEXPR14
#  define _FCXX_CONSTEXPR17 constexpr
#else
#  define _FCXX_CONSTEXPR17 inline
#endif

#if _FCXX_STD >= 2020 && defined(__cpp_constexpr) && __cpp_constexpr >= 201907L
#  define _FCXX_HAS_CONSTEXPR20 1
#  define _FCXX_CONSTEXPR20 constexpr
#else
#  define _FCXX_HAS_CONSTEXPR20 0
#  define _FCXX_CONSTEXPR20 inline
#endif

#if _FCXX_STD >= 2023 && _FCXX_HAS_CONSTEXPR20
#  define _FCXX_CONSTEXPR23 constexpr
#else
#  define _FCXX_CONSTEXPR23 inline
#endif

#if _FCXX_STD >= 2026 && _FCXX_HAS_CONSTEXPR20
#  define _FCXX_CONSTEXPR26 constexpr
#else
#  define _FCXX_CONSTEXPR26 inline
#endif

/* Exception specifications. The first is for the functions that C++98
 * already gave an empty one, and the second for those that only got one
 * from C++11 on. Variadic macros are an extension in C++98, where the last
 * only takes a single argument. */
#if _FCXX_STD >= 2011
#  define _FCXX_NOEXCEPT noexcept
#  define _FCXX_NOEXCEPT11 noexcept
#  define _FCXX_NOEXCEPT_IF(...) noexcept(__VA_ARGS__)
#else
#  define _FCXX_NOEXCEPT throw()
#  define _FCXX_NOEXCEPT11
#  define _FCXX_NOEXCEPT_IF(x)
#endif

/* Attributes. [[noreturn]] is used where the language has it, as Clang
 * makes the attribute of GNU part of the type of the function. Before
 * C++11, Clang has the _Noreturn of C11 as an extension, which is not part
 * of the type, and which __extension__ keeps from being warned about at the
 * start of the declarations it is used in. */
#if _FCXX_STD >= 2011
#  define _FCXX_NORETURN [[noreturn]]
#elif _FCXX_CLANG
#  define _FCXX_NORETURN __extension__ _Noreturn
#else
#  define _FCXX_NORETURN __attribute__((__noreturn__))
#endif

#if _FCXX_STD >= 2017 && _FCXX_HAS_CPP_ATTRIBUTE(nodiscard) >= 201603L
#  define _FCXX_NODISCARD [[nodiscard]]
#else
#  define _FCXX_NODISCARD
#endif

#if _FCXX_STD >= 2017 && _FCXX_HAS_CPP_ATTRIBUTE(maybe_unused) >= 201603L
#  define _FCXX_MAYBE_UNUSED [[maybe_unused]]
#else
#  define _FCXX_MAYBE_UNUSED __attribute__((__unused__))
#endif

#if _FCXX_STD >= 2020 && _FCXX_HAS_CPP_ATTRIBUTE(no_unique_address) >= 201803L
#  define _FCXX_NO_UNIQUE_ADDRESS [[no_unique_address]]
#else
#  define _FCXX_NO_UNIQUE_ADDRESS
#endif

#define _FCXX_ALWAYS_INLINE __attribute__((__always_inline__))

/* Deprecation, for the entities the standard deprecates, from the revision
 * it does so in. */
#define _FCXX_DEPRECATED(msg) __attribute__((__deprecated__(msg)))

#if _FCXX_STD >= 2011
#  define _FCXX_DEPRECATED_CXX11(msg) _FCXX_DEPRECATED(msg)
#else
#  define _FCXX_DEPRECATED_CXX11(msg)
#endif
#if _FCXX_STD >= 2017
#  define _FCXX_DEPRECATED_CXX17(msg) _FCXX_DEPRECATED(msg)
#else
#  define _FCXX_DEPRECATED_CXX17(msg)
#endif
#if _FCXX_STD >= 2020
#  define _FCXX_DEPRECATED_CXX20(msg) _FCXX_DEPRECATED(msg)
#else
#  define _FCXX_DEPRECATED_CXX20(msg)
#endif
#if _FCXX_STD >= 2023
#  define _FCXX_DEPRECATED_CXX23(msg) _FCXX_DEPRECATED(msg)
#else
#  define _FCXX_DEPRECATED_CXX23(msg)
#endif
#if _FCXX_STD >= 2026
#  define _FCXX_DEPRECATED_CXX26(msg) _FCXX_DEPRECATED(msg)
#else
#  define _FCXX_DEPRECATED_CXX26(msg)
#endif

/* Inline variables, where the language has them. Variables that are not
 * inline are made constexpr or const, so that each translation unit gets
 * its own copy instead of a duplicate definition. */
#if defined(__cpp_inline_variables) && __cpp_inline_variables >= 201606L
#  define _FCXX_HAS_INLINE_VARIABLES 1
#  define _FCXX_INLINE_VAR inline
#else
#  define _FCXX_HAS_INLINE_VARIABLES 0
#  define _FCXX_INLINE_VAR
#endif

/* Whether the language has the features that parts of the library depend
 * on. Old compilers in the modes of newer revisions lack some of them, and
 * the parts of the library depending on them are left out there. Clang has
 * some of the newer ones in the modes of older revisions too, as extensions
 * it warns about, which the library does not use there. */
#if defined(__cpp_variable_templates) && __cpp_variable_templates >= 201304L
#  define _FCXX_HAS_VARIABLE_TEMPLATES 1
#else
#  define _FCXX_HAS_VARIABLE_TEMPLATES 0
#endif

#if _FCXX_STD >= 2020 && defined(__cpp_concepts) && __cpp_concepts >= 201907L
#  define _FCXX_HAS_CONCEPTS 1
#else
#  define _FCXX_HAS_CONCEPTS 0
#endif

#if _FCXX_STD >= 2020 && defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
#  define _FCXX_HAS_THREE_WAY_COMPARISON 1
#else
#  define _FCXX_HAS_THREE_WAY_COMPARISON 0
#endif

#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
#  define _FCXX_HAS_CHAR8_T 1
#else
#  define _FCXX_HAS_CHAR8_T 0
#endif

#if _FCXX_STD >= 2020 && defined(__cpp_consteval) && __cpp_consteval >= 201811L
#  define _FCXX_HAS_CONSTEVAL 1
#else
#  define _FCXX_HAS_CONSTEVAL 0
#endif

#if defined(__cpp_conditional_explicit) && __cpp_conditional_explicit >= 201806L
#  define _FCXX_HAS_CONDITIONAL_EXPLICIT 1
#  define _FCXX_EXPLICIT(...) explicit(__VA_ARGS__)
#else
#  define _FCXX_HAS_CONDITIONAL_EXPLICIT 0
#endif

/* GCC 7 has the deduction guides, but reports the value of the first paper
 * about them. */
#if _FCXX_STD >= 2017 && defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201606L
#  define _FCXX_HAS_DEDUCTION_GUIDES 1
#else
#  define _FCXX_HAS_DEDUCTION_GUIDES 0
#endif

#if defined(__cpp_if_constexpr) && __cpp_if_constexpr >= 201606L
#  define _FCXX_HAS_IF_CONSTEXPR 1
#else
#  define _FCXX_HAS_IF_CONSTEXPR 0
#endif

#if defined(__cpp_fold_expressions) && __cpp_fold_expressions >= 201603L
#  define _FCXX_HAS_FOLD_EXPRESSIONS 1
#else
#  define _FCXX_HAS_FOLD_EXPRESSIONS 0
#endif

#if defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510L
#  define _FCXX_HAS_NOEXCEPT_FUNCTION_TYPE 1
#else
#  define _FCXX_HAS_NOEXCEPT_FUNCTION_TYPE 0
#endif

#if _FCXX_STD >= 2023 && defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
#  define _FCXX_HAS_EXPLICIT_THIS 1
#else
#  define _FCXX_HAS_EXPLICIT_THIS 0
#endif

#if _FCXX_STD >= 2023 && defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
#  define _FCXX_HAS_STATIC_CALL_OPERATOR 1
#else
#  define _FCXX_HAS_STATIC_CALL_OPERATOR 0
#endif

#if _FCXX_STD >= 2023 && defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
#  define _FCXX_HAS_MULTIDIMENSIONAL_SUBSCRIPT 1
#else
#  define _FCXX_HAS_MULTIDIMENSIONAL_SUBSCRIPT 0
#endif

#if _FCXX_STD >= 2023 && defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202211L
#  define _FCXX_HAS_STATIC_SUBSCRIPT_OPERATOR 1
#else
#  define _FCXX_HAS_STATIC_SUBSCRIPT_OPERATOR 0
#endif

/* Non-type template parameters of class types. Clang has had them since
 * version 12 without ever raising its feature-test macro. */
#if _FCXX_STD >= 2020 \
    && ((defined(__cpp_nontype_template_args) && __cpp_nontype_template_args >= 201911L) || _FCXX_CLANG >= 1200)
#  define _FCXX_HAS_CLASS_NTTP 1
#else
#  define _FCXX_HAS_CLASS_NTTP 0
#endif

#if _FCXX_STD >= 2023 && defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
#  define _FCXX_HAS_IF_CONSTEVAL 1
#else
#  define _FCXX_HAS_IF_CONSTEVAL 0
#endif

#if _FCXX_STD >= 2026 && defined(__cpp_pack_indexing) && __cpp_pack_indexing >= 202311L
#  define _FCXX_HAS_PACK_INDEXING 1
#else
#  define _FCXX_HAS_PACK_INDEXING 0
#endif

#if defined(__cpp_constexpr_dynamic_alloc) && __cpp_constexpr_dynamic_alloc >= 201907L
#  define _FCXX_HAS_CONSTEXPR_DYNAMIC_ALLOC 1
#else
#  define _FCXX_HAS_CONSTEXPR_DYNAMIC_ALLOC 0
#endif

/* constant_wrapper, of <utility>, which some of <functional> needs too, and
 * which needs a lot of the language of C++23. */
#define _FCXX_HAS_CONSTANT_WRAPPER                                                                       \
    (_FCXX_STD >= 2026 && _FCXX_HAS_CONCEPTS && _FCXX_HAS_CLASS_NTTP && _FCXX_HAS_EXPLICIT_THIS          \
     && _FCXX_HAS_STATIC_CALL_OPERATOR && _FCXX_HAS_STATIC_SUBSCRIPT_OPERATOR)

/* pair and tuple are written with requires-clauses and conditional explicit
 * where the compiler has them, along with three-way comparison, which the
 * additions of C++20 and later to them need. They are written with SFINAE
 * otherwise, as for C++17. */
#if _FCXX_HAS_CONCEPTS && _FCXX_HAS_CONDITIONAL_EXPLICIT && _FCXX_HAS_THREE_WAY_COMPARISON
#  define _FCXX_CONSTRAINED_TUPLES 1
#else
#  define _FCXX_CONSTRAINED_TUPLES 0
#endif

/* The iterator adaptors of <iterator> are written with the concepts and the
 * comparisons of C++20 where the compiler has them, and as in C++17
 * otherwise. */
#if _FCXX_HAS_CONCEPTS && _FCXX_HAS_THREE_WAY_COMPARISON
#  define _FCXX_CONCEPT_ITERATORS 1
#else
#  define _FCXX_CONCEPT_ITERATORS 0
#endif

/* The exception classes of the language support library are constexpr from
 * C++26 on, which makes all of their members inline. Before that, their
 * virtual functions are defined by the C++ runtime, as the Itanium C++ ABI
 * has it. */
#if _FCXX_STD >= 2026 && _FCXX_HAS_CONSTEXPR20
#  define _FCXX_CONSTEXPR_EXCEPTION_CLASSES 1
#else
#  define _FCXX_CONSTEXPR_EXCEPTION_CLASSES 0
#endif

/* Throwing exceptions in constant evaluation, which GCC 16 supports. */
#if _FCXX_STD >= 2026 && defined(__cpp_constexpr_exceptions) && __cpp_constexpr_exceptions >= 202411L
#  define _FCXX_HAS_CONSTEXPR_EXCEPTIONS 1
#else
#  define _FCXX_HAS_CONSTEXPR_EXCEPTIONS 0
#endif

/* Exceptions and RTTI. Clang defines __EXCEPTIONS for -fexceptions alone,
 * which does not enable C++ exceptions, and GCC before 5 has neither of the
 * feature-test macros. */
#if defined(__cpp_exceptions) || (_FCXX_GCC && defined(__EXCEPTIONS))
#  define _FCXX_HAS_EXCEPTIONS 1
#else
#  define _FCXX_HAS_EXCEPTIONS 0
#endif

#if defined(__cpp_rtti) || defined(__GXX_RTTI)
#  define _FCXX_HAS_RTTI 1
#else
#  define _FCXX_HAS_RTTI 0
#endif

/* Builtins. GCC before 10 has no __has_builtin, so its version is checked
 * for them. */
#if _FCXX_HAS_BUILTIN(__builtin_is_constant_evaluated) || _FCXX_GCC >= 900
#  define _FCXX_HAS_IS_CONSTANT_EVALUATED 1
#else
#  define _FCXX_HAS_IS_CONSTANT_EVALUATED 0
#endif

#if _FCXX_HAS_BUILTIN(__builtin_addressof) || _FCXX_GCC >= 700
#  define _FCXX_HAS_BUILTIN_ADDRESSOF 1
#else
#  define _FCXX_HAS_BUILTIN_ADDRESSOF 0
#endif

#if _FCXX_HAS_BUILTIN(__builtin_launder) || _FCXX_GCC >= 700
#  define _FCXX_HAS_BUILTIN_LAUNDER 1
#else
#  define _FCXX_HAS_BUILTIN_LAUNDER 0
#endif

#if _FCXX_HAS_BUILTIN(__builtin_bit_cast) || _FCXX_GCC >= 1100
#  define _FCXX_HAS_BUILTIN_BIT_CAST 1
#else
#  define _FCXX_HAS_BUILTIN_BIT_CAST 0
#endif

#define _FCXX_UNREACHABLE() __builtin_unreachable()

/* Token pasting, after the expansion of the operands. */
#define _FCXX_CONCAT_(a, b) a##b
#define _FCXX_CONCAT(a, b) _FCXX_CONCAT_(a, b)

/* Whether the compiler has the builtin for a type trait, named without its
 * leading underscores. Clang only reports the type trait keywords through
 * __has_builtin from version 10 on, and fails on them before that, where
 * __has_feature knows the older traits. GCC has had no __has_builtin before
 * version 10, so its version is checked for the traits older than that. */
#if defined(__clang__) && _FCXX_CLANG < 1000
#  define _FCXX_HAS_TYPE_TRAIT(name, gcc) _FCXX_HAS_FEATURE(name)
#elif defined(__has_builtin)
#  define _FCXX_HAS_TYPE_TRAIT(name, gcc) __has_builtin(__##name)
#else
#  define _FCXX_HAS_TYPE_TRAIT(name, gcc) (_FCXX_GCC >= (gcc))
#endif

/* The type traits and other facilities that can only be provided with the
 * builtins of the compiler. Every version of Clang has the first two, which
 * those before 10 do not report. */
#define _FCXX_HAS_IS_AGGREGATE (_FCXX_CLANG || _FCXX_HAS_TYPE_TRAIT(is_aggregate, 700))
#define _FCXX_HAS_UNIQUE_OBJECT_REPRESENTATIONS \
    (_FCXX_CLANG || _FCXX_HAS_TYPE_TRAIT(has_unique_object_representations, 700))
#define _FCXX_HAS_IS_LAYOUT_COMPATIBLE _FCXX_HAS_BUILTIN(__is_layout_compatible)
#define _FCXX_HAS_IS_POINTER_INTERCONVERTIBLE _FCXX_HAS_BUILTIN(__is_pointer_interconvertible_base_of)
#define _FCXX_HAS_IS_POINTER_INTERCONVERTIBLE_WITH_CLASS \
    _FCXX_HAS_BUILTIN(__builtin_is_pointer_interconvertible_with_class)
#define _FCXX_HAS_IS_CORRESPONDING_MEMBER _FCXX_HAS_BUILTIN(__builtin_is_corresponding_member)
#define _FCXX_HAS_IS_VIRTUAL_BASE_OF _FCXX_HAS_BUILTIN(__builtin_is_virtual_base_of)
#define _FCXX_HAS_IS_STRUCTURAL _FCXX_HAS_BUILTIN(__builtin_is_structural)
#define _FCXX_HAS_IS_WITHIN_LIFETIME (_FCXX_HAS_BUILTIN(__builtin_is_within_lifetime) && _FCXX_HAS_CONSTEVAL)

#if _FCXX_HAS_BUILTIN(__builtin_source_location)
#  define _FCXX_HAS_BUILTIN_SOURCE_LOCATION 1
#else
#  define _FCXX_HAS_BUILTIN_SOURCE_LOCATION 0
#endif

#if _FCXX_STD >= 2020 && defined(__cpp_impl_coroutine) && __cpp_impl_coroutine >= 201902L
#  define _FCXX_HAS_COROUTINES 1
#else
#  define _FCXX_HAS_COROUTINES 0
#endif

/* Whether the headers of C++20 that need more of the compiler than their
 * revision of the language has can be used. Each header stops with an error
 * where it cannot, and in the revisions it is not freestanding in, and the
 * headers only include one another where they can be used. Every compiler
 * with a mode for C++23 or C++26 has what the headers that are freestanding
 * from those revisions on need. */
#define _FCXX_HEADER_COMPARE _FCXX_HAS_THREE_WAY_COMPARISON
#define _FCXX_HEADER_CONCEPTS _FCXX_HAS_CONCEPTS
#define _FCXX_HEADER_COROUTINE _FCXX_HAS_COROUTINES
#define _FCXX_HEADER_SOURCE_LOCATION \
    (_FCXX_STD >= 2020 && (_FCXX_HAS_BUILTIN_SOURCE_LOCATION || _FCXX_HAS_BUILTIN(__builtin_COLUMN)))

/* The unsigned integer type of the size of pointers. */
#define _FCXX_UINTPTR __UINTPTR_TYPE__

/* The largest alignment of any type of the target. */
#define _FCXX_BIGGEST_ALIGNMENT __BIGGEST_ALIGNMENT__

/* Throwing an exception, which calls std::terminate() when exceptions are
 * disabled. Variadic macros are an extension in C++98, where they are not
 * used. */
#if _FCXX_HAS_EXCEPTIONS && _FCXX_STD >= 2011
#  define _FCXX_THROW(...) throw __VA_ARGS__
#elif _FCXX_HAS_EXCEPTIONS
#  define _FCXX_THROW(x) throw x
#elif _FCXX_STD >= 2011
#  define _FCXX_THROW(...) ::std::terminate()
#else
#  define _FCXX_THROW(x) ::std::terminate()
#endif

#endif

/* The declarations of namespace std that every header needs. */
#include <__fcxx/std_decls.h>
