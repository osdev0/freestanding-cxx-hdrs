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


/* The copy and move constructors and assignment operators of optional,
 * variant and expected, which are trivial where those of the types they hold
 * are, deleted or left out where those types cannot be copied or moved, and
 * otherwise do what the storage class they are given does. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_SPECIAL_MEMBERS_H
#define __FREESTANDING_CXX_HDRS_FCXX_SPECIAL_MEMBERS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <type_traits>

/* The special member functions are chosen with requires-clauses, which the
 * compilers take into account for the triviality of the class (P0848,
 * conditionally trivial special member functions). */

namespace std {

/* What a copy or move constructor or assignment operator is: trivial, done
 * by the storage class, deleted, or not there at all, so that the copy is
 * used instead of a move. */
enum __fcxx_smf_kind { __fcxx_smf_trivial, __fcxx_smf_provided, __fcxx_smf_deleted, __fcxx_smf_absent };

template <bool _Available, bool _Trivial, __fcxx_smf_kind _Otherwise>
struct __fcxx_smf_select
    : integral_constant<__fcxx_smf_kind,
                        !_Available ? _Otherwise : _Trivial ? __fcxx_smf_trivial : __fcxx_smf_provided> {};

/* The tag of the constructors of the storage classes that copy or move the
 * objects of another. */
struct __fcxx_smf_construct_tag {
    explicit __fcxx_smf_construct_tag() = default;
};

/* The member of the unions of the storage classes that is there while no
 * object is, and the tag of the constructors that construct none. */
struct __fcxx_union_empty {};

struct __fcxx_no_value_tag {
    explicit __fcxx_no_value_tag() = default;
};

/* The storage class, _Sp, has defaulted copy and move constructors and
 * assignment operators, which are trivial where needed, and these members:
 *
 *     _Sp(__fcxx_smf_construct_tag, const _Sp&), and from _Sp&&
 *     void __fcxx_assign_from(const _Sp&), and from _Sp&&
 *
 * whose exception specifications are those of the special member functions
 * doing what they do. The copies are made by constructors, so that what
 * could not be copied is not destroyed afterwards. */
template <class _Sp>
struct __fcxx_smf_nothrow_construct
    : __fcxx_bool_constant<is_nothrow_constructible<_Sp, __fcxx_smf_construct_tag, const _Sp&>::value> {};

template <class _Sp>
struct __fcxx_smf_nothrow_move_construct
    : __fcxx_bool_constant<is_nothrow_constructible<_Sp, __fcxx_smf_construct_tag, _Sp&&>::value> {};

template <class _Sp>
struct __fcxx_smf_nothrow_assign
    : __fcxx_bool_constant<noexcept(std::declval<_Sp&>().__fcxx_assign_from(std::declval<const _Sp&>()))> {};

template <class _Sp>
struct __fcxx_smf_nothrow_move_assign
    : __fcxx_bool_constant<noexcept(std::declval<_Sp&>().__fcxx_assign_from(std::declval<_Sp&&>()))> {};

template <class _Sp, __fcxx_smf_kind _CopyCtor, __fcxx_smf_kind _MoveCtor, __fcxx_smf_kind _CopyAssign,
          __fcxx_smf_kind _MoveAssign>
struct __fcxx_smf : _Sp {
    using _Sp::_Sp;

    __fcxx_smf(const __fcxx_smf&)
        requires (_CopyCtor == __fcxx_smf_trivial)
    = default;

    constexpr __fcxx_smf(const __fcxx_smf& __other) noexcept(__fcxx_smf_nothrow_construct<_Sp>::value)
        requires (_CopyCtor == __fcxx_smf_provided)
        : _Sp(__fcxx_smf_construct_tag(), static_cast<const _Sp&>(__other)) {}

    __fcxx_smf(const __fcxx_smf&)
        requires (_CopyCtor == __fcxx_smf_deleted)
    = delete;

    __fcxx_smf(__fcxx_smf&&)
        requires (_MoveCtor == __fcxx_smf_trivial)
    = default;

    constexpr __fcxx_smf(__fcxx_smf&& __other) noexcept(__fcxx_smf_nothrow_move_construct<_Sp>::value)
        requires (_MoveCtor == __fcxx_smf_provided)
        : _Sp(__fcxx_smf_construct_tag(), static_cast<_Sp&&>(__other)) {}

    __fcxx_smf(__fcxx_smf&&)
        requires (_MoveCtor == __fcxx_smf_deleted)
    = delete;

    __fcxx_smf& operator=(const __fcxx_smf&)
        requires (_CopyAssign == __fcxx_smf_trivial)
    = default;

    constexpr __fcxx_smf& operator=(const __fcxx_smf& __other) noexcept(__fcxx_smf_nothrow_assign<_Sp>::value)
        requires (_CopyAssign == __fcxx_smf_provided) {
        _Sp::__fcxx_assign_from(static_cast<const _Sp&>(__other));
        return *this;
    }

    __fcxx_smf& operator=(const __fcxx_smf&)
        requires (_CopyAssign == __fcxx_smf_deleted)
    = delete;

    __fcxx_smf& operator=(__fcxx_smf&&)
        requires (_MoveAssign == __fcxx_smf_trivial)
    = default;

    constexpr __fcxx_smf& operator=(__fcxx_smf&& __other) noexcept(__fcxx_smf_nothrow_move_assign<_Sp>::value)
        requires (_MoveAssign == __fcxx_smf_provided) {
        _Sp::__fcxx_assign_from(static_cast<_Sp&&>(__other));
        return *this;
    }

    __fcxx_smf& operator=(__fcxx_smf&&)
        requires (_MoveAssign == __fcxx_smf_deleted)
    = delete;
};

} /* namespace std */

#endif
