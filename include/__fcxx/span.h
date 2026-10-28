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


/* The class template span, of <span>, which views::counted of <ranges> makes
 * views of contiguous ranges of in C++23 too, whose freestanding items it is
 * one of in C++23 only as their type: <span> is freestanding from C++26 on,
 * where the rest of the header is declared too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_SPAN_H
#define __FREESTANDING_CXX_HDRS_FCXX_SPAN_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
_FCXX_BEGIN_EXTERN_CXX

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <__fcxx/const_iterator.h>
#include <__fcxx/integral_constant_like.h>
#include <__fcxx/iterator_base.h>
#include <__fcxx/pointer_traits.h>
#include <__fcxx/range_access.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_base.h>
#include <__fcxx/reverse_iterator.h>

namespace std {

/* dynamic_extent, of <span>, under a name of the library's own, as span is
 * there in C++23 too, as the type of the views that views::counted makes of
 * contiguous ranges, but <span> is only freestanding from C++26 on, where
 * the rest of it is declared. */
inline constexpr size_t __fcxx_dynamic_extent = static_cast<size_t>(-1);

#if _FCXX_STD >= 2026
/* [span.syn], constants */
inline constexpr size_t dynamic_extent = __fcxx_dynamic_extent;

/* Defined by <array>. */
template <class _Tp, size_t _Np>
struct array;
#endif

template <class _Tp, size_t _Extent>
class span;

/* maybe-static-ext, the extent that a size of a type gives: that of the
 * constant carried by the type, or a dynamic one. */
#if _FCXX_STD >= 2026
template <class _Tp>
inline constexpr size_t __fcxx_maybe_static_ext = __fcxx_dynamic_extent;

template <__fcxx_integral_constant_like _Tp>
inline constexpr size_t __fcxx_maybe_static_ext<_Tp> = {_Tp::value};
#endif

template <class _Tp>
inline constexpr bool __fcxx_span_is_span = false;

template <class _Tp, size_t _Extent>
inline constexpr bool __fcxx_span_is_span<span<_Tp, _Extent> > = true;

template <class _Tp>
inline constexpr bool __fcxx_span_is_array = false;

#if _FCXX_STD >= 2026
template <class _Tp, size_t _Np>
inline constexpr bool __fcxx_span_is_array<array<_Tp, _Np> > = true;
#endif

/* Whether a sequence of objects of one type can be viewed as one of another,
 * which only allows adding qualifiers. */
template <class _From, class _To>
concept __fcxx_span_compatible = is_convertible_v<_From (*)[], _To (*)[]>;

/* The size of a span, which only those of dynamic extent store. */
template <size_t _Extent>
struct __fcxx_span_size {
    constexpr explicit __fcxx_span_size(size_t) noexcept {}
    constexpr size_t __get() const noexcept { return _Extent; }
};

template <>
struct __fcxx_span_size<__fcxx_dynamic_extent> {
    constexpr explicit __fcxx_span_size(size_t __n) noexcept : __value(__n) {}
    constexpr size_t __get() const noexcept { return __value; }

    size_t __value;
};

/* [views.span], class template span, whose iterators are pointers. */
template <class _Tp, size_t _Extent = __fcxx_dynamic_extent>
class span {
public:
    using element_type = _Tp;
    using value_type = remove_cv_t<_Tp>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = element_type*;
    using const_pointer = const element_type*;
    using reference = element_type&;
    using const_reference = const element_type&;
    using iterator = pointer;
    using const_iterator = std::const_iterator<iterator>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::const_iterator<reverse_iterator>;
    static constexpr size_type extent = _Extent;

    /* [span.cons], constructors, copy, and assignment */
    constexpr span() noexcept
        requires (_Extent == __fcxx_dynamic_extent || _Extent == 0)
        : __data_ptr(nullptr), __data_size(0) {}

    template <class _It>
        requires (contiguous_iterator<_It>
                  && __fcxx_span_compatible<remove_reference_t<iter_reference_t<_It> >, element_type>)
    constexpr explicit(extent != __fcxx_dynamic_extent) span(_It __first, size_type __count)
        : __data_ptr(std::to_address(__first)), __data_size(__count) {}

    template <class _It, class _End>
        requires (contiguous_iterator<_It>
                  && __fcxx_span_compatible<remove_reference_t<iter_reference_t<_It> >, element_type>
                  && sized_sentinel_for<_End, _It> && !is_convertible_v<_End, size_t>)
    constexpr explicit(extent != __fcxx_dynamic_extent) span(_It __first, _End __last)
        : __data_ptr(std::to_address(__first)), __data_size(static_cast<size_type>(__last - __first)) {}

    template <size_t _Np>
        requires (_Extent == __fcxx_dynamic_extent || _Np == _Extent)
    constexpr span(type_identity_t<element_type> (&__arr)[_Np]) noexcept : __data_ptr(__arr), __data_size(_Np) {}

#if _FCXX_STD >= 2026
    template <class _Up, size_t _Np>
        requires ((_Extent == __fcxx_dynamic_extent || _Np == _Extent) && __fcxx_span_compatible<_Up, element_type>)
    constexpr span(array<_Up, _Np>& __arr) noexcept : __data_ptr(__arr.data()), __data_size(_Np) {}

    template <class _Up, size_t _Np>
        requires((_Extent == __fcxx_dynamic_extent || _Np == _Extent)
                 && __fcxx_span_compatible<const _Up, element_type>)
    constexpr span(const array<_Up, _Np>& __arr) noexcept : __data_ptr(__arr.data()), __data_size(_Np) {}
#endif

    template <class _Rp>
        requires (!__fcxx_span_is_span<remove_cvref_t<_Rp> > && !__fcxx_span_is_array<remove_cvref_t<_Rp> >
                  && !is_array_v<remove_cvref_t<_Rp> > && ranges::contiguous_range<_Rp> && ranges::sized_range<_Rp>
                  && (ranges::borrowed_range<_Rp> || is_const_v<element_type>)
                  && __fcxx_span_compatible<remove_reference_t<ranges::range_reference_t<_Rp> >, element_type>)
    constexpr explicit(extent != __fcxx_dynamic_extent) span(_Rp&& __r)
        : __data_ptr(ranges::data(__r)), __data_size(static_cast<size_type>(ranges::size(__r))) {}

    constexpr span(const span&) noexcept = default;

    template <class _OtherElementType, size_t _OtherExtent>
        requires ((_Extent == __fcxx_dynamic_extent || _OtherExtent == __fcxx_dynamic_extent || _Extent == _OtherExtent)
                  && __fcxx_span_compatible<_OtherElementType, element_type>)
    constexpr explicit(_Extent != __fcxx_dynamic_extent && _OtherExtent == __fcxx_dynamic_extent)
        span(const span<_OtherElementType, _OtherExtent>& __s) noexcept
        : __data_ptr(__s.data()), __data_size(__s.size()) {}

    constexpr span& operator=(const span&) noexcept = default;

    /* [span.sub], subviews */
    template <size_t _Count>
    constexpr span<element_type, _Count> first() const {
        static_assert(_Count <= _Extent, "std::span::first of more elements than the extent");
        return span<element_type, _Count>(data(), _Count);
    }

    template <size_t _Count>
    constexpr span<element_type, _Count> last() const {
        static_assert(_Count <= _Extent, "std::span::last of more elements than the extent");
        return span<element_type, _Count>(data() + (size() - _Count), _Count);
    }

    template <size_t _Offset, size_t _Count = __fcxx_dynamic_extent>
    constexpr span<element_type, _Count != __fcxx_dynamic_extent    ? _Count
                                 : _Extent != __fcxx_dynamic_extent ? _Extent - _Offset
                                                             : __fcxx_dynamic_extent>
    subspan() const {
        static_assert(_Offset <= _Extent && (_Count == __fcxx_dynamic_extent || _Count <= _Extent - _Offset),
                      "std::span::subspan out of the extent");
        using _Rp = span<element_type, _Count != __fcxx_dynamic_extent    ? _Count
                                       : _Extent != __fcxx_dynamic_extent ? _Extent - _Offset
                                                                   : __fcxx_dynamic_extent>;
        return _Rp(data() + _Offset, _Count != __fcxx_dynamic_extent ? _Count : size() - _Offset);
    }

    constexpr span<element_type, __fcxx_dynamic_extent> first(size_type __count) const {
        return span<element_type, __fcxx_dynamic_extent>(data(), __count);
    }

    constexpr span<element_type, __fcxx_dynamic_extent> last(size_type __count) const {
        return span<element_type, __fcxx_dynamic_extent>(data() + (size() - __count), __count);
    }

    constexpr span<element_type, __fcxx_dynamic_extent> subspan(size_type __offset,
                                                                size_type __count = __fcxx_dynamic_extent) const {
        return span<element_type, __fcxx_dynamic_extent>(data() + __offset,
                                                  __count == __fcxx_dynamic_extent ? size() - __offset : __count);
    }

    /* [span.obs], observers */
    constexpr size_type size() const noexcept { return __data_size.__get(); }
    constexpr size_type size_bytes() const noexcept { return size() * sizeof(element_type); }
    [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }

    /* [span.elem], element access */
    constexpr reference operator[](size_type __idx) const { return *(data() + __idx); }
#if _FCXX_STD >= 2026
    /* freestanding-deleted */
    constexpr reference at(size_type) const = delete;
#endif
    constexpr reference front() const { return *data(); }
    constexpr reference back() const { return *(data() + (size() - 1)); }
    constexpr pointer data() const noexcept { return __data_ptr; }

    /* [span.iterators], iterator support */
    constexpr iterator begin() const noexcept { return data(); }
    constexpr iterator end() const noexcept { return data() + size(); }
    constexpr const_iterator cbegin() const noexcept { return begin(); }
    constexpr const_iterator cend() const noexcept { return end(); }
    constexpr reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
    constexpr reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }
    constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    constexpr const_reverse_iterator crend() const noexcept { return rend(); }

private:
    pointer __data_ptr;
    _FCXX_NO_UNIQUE_ADDRESS __fcxx_span_size<_Extent> __data_size;
};

/* [span.deduct], deduction guides, which deduce a static extent from a size
 * carried in a type from C++26 on. */
template <class _It, class _EndOrSize>
    requires contiguous_iterator<_It>
span(_It, _EndOrSize) -> span<remove_reference_t<iter_reference_t<_It> >
#if _FCXX_STD >= 2026
                              ,
                              __fcxx_maybe_static_ext<_EndOrSize>
#endif
                              >;

template <class _Tp, size_t _Np>
span(_Tp (&)[_Np]) -> span<_Tp, _Np>;

#if _FCXX_STD >= 2026
template <class _Tp, size_t _Np>
span(array<_Tp, _Np>&) -> span<_Tp, _Np>;

template <class _Tp, size_t _Np>
span(const array<_Tp, _Np>&) -> span<const _Tp, _Np>;
#endif

template <class _Rp>
    requires ranges::contiguous_range<_Rp>
span(_Rp&&) -> span<remove_reference_t<ranges::range_reference_t<_Rp> > >;

/* span is a view, of any extent, as P2325R3 has it for C++20 too, and a
 * borrowed range. */
template <class _Tp, size_t _Extent>
inline constexpr bool ranges::enable_view<span<_Tp, _Extent> > = true;

template <class _Tp, size_t _Extent>
inline constexpr bool ranges::enable_borrowed_range<span<_Tp, _Extent> > = true;

#if _FCXX_STD >= 2026
/* [span.objectrep], views of object representation, which exclude volatile
 * objects in all revisions, as LWG 4243 has it. */
template <class _Tp, size_t _Extent>
    requires (!is_volatile_v<_Tp>)
span<const byte, _Extent == __fcxx_dynamic_extent ? __fcxx_dynamic_extent : sizeof(_Tp) * _Extent>
as_bytes(span<_Tp, _Extent> __s) noexcept {
    using _Rp = span<const byte, _Extent == __fcxx_dynamic_extent ? __fcxx_dynamic_extent : sizeof(_Tp) * _Extent>;
    return _Rp{reinterpret_cast<const byte*>(__s.data()), __s.size_bytes()};
}

template <class _Tp, size_t _Extent>
    requires (!is_const_v<_Tp> && !is_volatile_v<_Tp>)
span<byte, _Extent == __fcxx_dynamic_extent ? __fcxx_dynamic_extent : sizeof(_Tp) * _Extent>
as_writable_bytes(span<_Tp, _Extent> __s) noexcept {
    using _Rp = span<byte, _Extent == __fcxx_dynamic_extent ? __fcxx_dynamic_extent : sizeof(_Tp) * _Extent>;
    return _Rp{reinterpret_cast<byte*>(__s.data()), __s.size_bytes()};
}
#endif

} /* namespace std */

_FCXX_END_EXTERN_CXX
#endif
