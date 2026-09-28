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


/* pointer_traits and to_address, of <memory>, which <iterator> needs too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_POINTER_TRAITS_H
#define __FREESTANDING_CXX_HDRS_FCXX_POINTER_TRAITS_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#include <type_traits>
#include <__fcxx/addressof.h>

namespace std {

/* The element type of a pointer-like type: its element_type, or else the
 * first argument of the template it is a specialization of. */
template <class _Tp>
struct __fcxx_ptr_elem_from_args {};

template <template <class...> class _Sp, class _Tp, class... _Args>
struct __fcxx_ptr_elem_from_args<_Sp<_Tp, _Args...> > {
    typedef _Tp type;
};

template <class _Tp, class = void>
struct __fcxx_ptr_traits_elem : __fcxx_ptr_elem_from_args<_Tp> {};

template <class _Tp>
struct __fcxx_ptr_traits_elem<_Tp, __fcxx_void_t<typename _Tp::element_type> > {
    typedef typename _Tp::element_type type;
};

template <class _Tp, class = void>
struct __fcxx_ptr_traits_diff {
    typedef ptrdiff_t type;
};

template <class _Tp>
struct __fcxx_ptr_traits_diff<_Tp, __fcxx_void_t<typename _Tp::difference_type> > {
    typedef typename _Tp::difference_type type;
};

template <class _Tp, class _Up>
struct __fcxx_ptr_rebind_args {};

template <template <class...> class _Sp, class _Tp, class... _Args, class _Up>
struct __fcxx_ptr_rebind_args<_Sp<_Tp, _Args...>, _Up> {
    typedef _Sp<_Up, _Args...> type;
};

template <class _Tp, class _Up, class = void>
struct __fcxx_ptr_traits_rebind : __fcxx_ptr_rebind_args<_Tp, _Up> {};

template <class _Tp, class _Up>
struct __fcxx_ptr_traits_rebind<_Tp, _Up, __fcxx_void_t<typename _Tp::template rebind<_Up> > > {
    typedef typename _Tp::template rebind<_Up> type;
};

/* The parameter of pointer_to where the element type is void, which nothing
 * converts to. */
struct __fcxx_ptr_no_ref {
private:
    __fcxx_ptr_no_ref(const __fcxx_ptr_no_ref&);
};

template <class _Tp>
struct __fcxx_ptr_ref {
    typedef _Tp& type;
};

template <>
struct __fcxx_ptr_ref<void> {
    typedef __fcxx_ptr_no_ref& type;
};

template <>
struct __fcxx_ptr_ref<const void> {
    typedef __fcxx_ptr_no_ref& type;
};

template <>
struct __fcxx_ptr_ref<volatile void> {
    typedef __fcxx_ptr_no_ref& type;
};

template <>
struct __fcxx_ptr_ref<const volatile void> {
    typedef __fcxx_ptr_no_ref& type;
};

/* The members of pointer_traits of the pointer-like types that it can tell
 * the element type of, and none otherwise. */
template <class _Ptr, class = void>
struct __fcxx_pointer_traits_impl {};

template <class _Ptr>
struct __fcxx_pointer_traits_impl<_Ptr, __fcxx_void_t<typename __fcxx_ptr_traits_elem<_Ptr>::type> > {
    typedef _Ptr pointer;
    typedef typename __fcxx_ptr_traits_elem<_Ptr>::type element_type;
    typedef typename __fcxx_ptr_traits_diff<_Ptr>::type difference_type;

    template <class _Up>
    using rebind = typename __fcxx_ptr_traits_rebind<_Ptr, _Up>::type;

    static _FCXX_CONSTEXPR26 pointer pointer_to(typename __fcxx_ptr_ref<element_type>::type __r) {
        return _Ptr::pointer_to(__r);
    }
};

/* [pointer.traits], pointer traits */
template <class _Ptr>
struct pointer_traits : __fcxx_pointer_traits_impl<_Ptr> {};

template <class _Tp>
struct pointer_traits<_Tp*> {
    typedef _Tp* pointer;
    typedef _Tp element_type;
    typedef ptrdiff_t difference_type;

    template <class _Up>
    using rebind = _Up*;

    static constexpr pointer pointer_to(typename __fcxx_ptr_ref<_Tp>::type __r) noexcept {
        return std::addressof(__r);
    }
};

/* [pointer.conversion], pointer conversion */
template <class _Tp>
constexpr _Tp* to_address(_Tp* __p) noexcept {
    static_assert(!is_function_v<_Tp>, "std::to_address of a pointer to function");
    return __p;
}

template <class _Ptr>
concept __fcxx_has_to_address = requires(const _Ptr& __p) { pointer_traits<_Ptr>::to_address(__p); };

template <class _Ptr>
    requires __fcxx_has_to_address<_Ptr>
constexpr auto to_address(const _Ptr& __p) noexcept {
    return pointer_traits<_Ptr>::to_address(__p);
}

template <class _Ptr>
    requires (!__fcxx_has_to_address<_Ptr> && requires(const _Ptr& __p) { __p.operator->(); })
constexpr auto to_address(const _Ptr& __p) noexcept {
    return std::to_address(__p.operator->());
}

} /* namespace std */

#endif
