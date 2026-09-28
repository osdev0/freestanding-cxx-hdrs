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


/* The specialized memory algorithms of <memory>, [specialized.algorithms]:
 * the uninitialized algorithms, destroy and destroy_n, and their forms of
 * namespace ranges, which the headers of the containers can use too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_UNINITIALIZED_H
#define __FREESTANDING_CXX_HDRS_FCXX_UNINITIALIZED_H 1

#pragma GCC system_header

#include <__fcxx/config.h>
#include <new>
#include <__fcxx/iterator_base.h>

#include <type_traits>
#include <utility>
#include <__fcxx/addressof.h>
#include <__fcxx/construct_at.h>
#if _FCXX_STD >= 2026
#  include <__fcxx/execution_policy.h>
#endif
#include <__fcxx/algorithm_results.h>
#include <__fcxx/ranges_access.h>
#include <__fcxx/ranges_core.h>

/* The placement new-expressions of the algorithms are in functions that are
 * constexpr from C++26 on, where the compiler can evaluate them. */
#if _FCXX_STD >= 2026 && defined(__cpp_constexpr) && __cpp_constexpr >= 202406L
#  define _FCXX_CONSTEXPR_PLACEMENT constexpr
#else
#  define _FCXX_CONSTEXPR_PLACEMENT inline
#endif

namespace std {

/* The address of an object, as a pointer to void, for placement new. */
template <class _Tp>
_FCXX_CONSTEXPR_PLACEMENT void* __fcxx_uninit_voidify(_Tp& __obj) noexcept {
    return std::__fcxx_voidify(__obj);
}

/* The destruction of the objects that an algorithm constructed before an
 * exception. */
template <class _It>
_FCXX_CONSTEXPR_PLACEMENT void __fcxx_uninit_rollback(_It __first, _It __last) {
    for (; __first != __last; ++__first) {
        std::destroy_at(std::addressof(*__first));
    }
}

#if _FCXX_HAS_EXCEPTIONS
#  define _FCXX_UNINIT_TRY try {
#  define _FCXX_UNINIT_CATCH(first, last) \
      }                                     \
      catch (...) {                         \
          std::__fcxx_uninit_rollback(first, last); \
          throw;                            \
      }
#else
#  define _FCXX_UNINIT_TRY {
#  define _FCXX_UNINIT_CATCH(first, last) }
#endif

/* [uninitialized.copy], uninitialized_copy */
template <class _In, class _Fwd>
_FCXX_CONSTEXPR_PLACEMENT _Fwd uninitialized_copy(_In __first, _In __last, _Fwd __result) {
    typedef typename iterator_traits<_Fwd>::value_type _Tp;
    _Fwd __current = __result;
    _FCXX_UNINIT_TRY
    for (; __first != __last; ++__current, (void)++__first) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Tp(*__first);
    }
    _FCXX_UNINIT_CATCH(__result, __current)
    return __current;
}

template <class _In, class _Size, class _Fwd>
_FCXX_CONSTEXPR_PLACEMENT _Fwd uninitialized_copy_n(_In __first, _Size __n, _Fwd __result) {
    typedef typename iterator_traits<_Fwd>::value_type _Tp;
    _Fwd __current = __result;
    _FCXX_UNINIT_TRY
    for (; __n > 0; ++__current, (void)++__first, --__n) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Tp(*__first);
    }
    _FCXX_UNINIT_CATCH(__result, __current)
    return __current;
}

/* [uninitialized.fill], uninitialized_fill. uninitialized_fill_n returns
 * the end of the range, as LWG 1339 has it. The value type is the default
 * of the type of the value from C++26 on, as P2248 has it. */
#if _FCXX_STD >= 2026
#  define _FCXX_UNINIT_FILL_DEFAULT(it) = typename iterator_traits<it>::value_type
#else
#  define _FCXX_UNINIT_FILL_DEFAULT(it)
#endif

template <class _Fwd, class _Tp _FCXX_UNINIT_FILL_DEFAULT(_Fwd)>
_FCXX_CONSTEXPR_PLACEMENT void uninitialized_fill(_Fwd __first, _Fwd __last, const _Tp& __x) {
    typedef typename iterator_traits<_Fwd>::value_type _Vt;
    _Fwd __current = __first;
    _FCXX_UNINIT_TRY
    for (; __current != __last; ++__current) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Vt(__x);
    }
    _FCXX_UNINIT_CATCH(__first, __current)
}

template <class _Fwd, class _Size, class _Tp _FCXX_UNINIT_FILL_DEFAULT(_Fwd)>
_FCXX_CONSTEXPR_PLACEMENT _Fwd uninitialized_fill_n(_Fwd __first, _Size __n, const _Tp& __x) {
    typedef typename iterator_traits<_Fwd>::value_type _Vt;
    _Fwd __current = __first;
    _FCXX_UNINIT_TRY
    for (; __n > 0; ++__current, (void)--__n) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Vt(__x);
    }
    _FCXX_UNINIT_CATCH(__first, __current)
    return __current;
}

#undef _FCXX_UNINIT_FILL_DEFAULT

/* [specialized.algorithms.general], deref-move */
template <class _It>
_FCXX_CONSTEXPR_PLACEMENT decltype(auto) __fcxx_deref_move(_It& __it) {
    if constexpr (is_lvalue_reference_v<decltype(*__it)>) {
        return std::move(*__it);
    } else {
        return *__it;
    }
}

/* [uninitialized.construct.default], uninitialized_default_construct */
template <class _Fwd>
_FCXX_CONSTEXPR_PLACEMENT void uninitialized_default_construct(_Fwd __first, _Fwd __last) {
    typedef typename iterator_traits<_Fwd>::value_type _Tp;
    _Fwd __current = __first;
    _FCXX_UNINIT_TRY
    for (; __current != __last; ++__current) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Tp;
    }
    _FCXX_UNINIT_CATCH(__first, __current)
}

template <class _Fwd, class _Size>
_FCXX_CONSTEXPR_PLACEMENT _Fwd uninitialized_default_construct_n(_Fwd __first, _Size __n) {
    typedef typename iterator_traits<_Fwd>::value_type _Tp;
    _Fwd __current = __first;
    _FCXX_UNINIT_TRY
    for (; __n > 0; (void)++__current, --__n) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Tp;
    }
    _FCXX_UNINIT_CATCH(__first, __current)
    return __current;
}

/* [uninitialized.construct.value], uninitialized_value_construct */
template <class _Fwd>
_FCXX_CONSTEXPR_PLACEMENT void uninitialized_value_construct(_Fwd __first, _Fwd __last) {
    typedef typename iterator_traits<_Fwd>::value_type _Tp;
    _Fwd __current = __first;
    _FCXX_UNINIT_TRY
    for (; __current != __last; ++__current) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Tp();
    }
    _FCXX_UNINIT_CATCH(__first, __current)
}

template <class _Fwd, class _Size>
_FCXX_CONSTEXPR_PLACEMENT _Fwd uninitialized_value_construct_n(_Fwd __first, _Size __n) {
    typedef typename iterator_traits<_Fwd>::value_type _Tp;
    _Fwd __current = __first;
    _FCXX_UNINIT_TRY
    for (; __n > 0; (void)++__current, --__n) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Tp();
    }
    _FCXX_UNINIT_CATCH(__first, __current)
    return __current;
}

/* [uninitialized.move], uninitialized_move. The elements are moved from
 * with deref-move, which leaves the prvalues of iterators alone. */
template <class _In, class _Fwd>
_FCXX_CONSTEXPR_PLACEMENT _Fwd uninitialized_move(_In __first, _In __last, _Fwd __result) {
    typedef typename iterator_traits<_Fwd>::value_type _Tp;
    _Fwd __current = __result;
    _FCXX_UNINIT_TRY
    for (; __first != __last; (void)++__current, ++__first) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Tp(std::__fcxx_deref_move(__first));
    }
    _FCXX_UNINIT_CATCH(__result, __current)
    return __current;
}

template <class _In, class _Size, class _Fwd>
_FCXX_CONSTEXPR_PLACEMENT pair<_In, _Fwd> uninitialized_move_n(_In __first, _Size __n, _Fwd __result) {
    typedef typename iterator_traits<_Fwd>::value_type _Tp;
    _Fwd __current = __result;
    _FCXX_UNINIT_TRY
    for (; __n > 0; ++__current, (void)++__first, --__n) {
        ::new (std::__fcxx_uninit_voidify(*__current)) _Tp(std::__fcxx_deref_move(__first));
    }
    _FCXX_UNINIT_CATCH(__result, __current)
    return pair<_In, _Fwd>(__first, __current);
}

/* [specialized.destroy], destroy and destroy_n, which destroy_at of
 * <__fcxx/construct_at.h> goes with. */
template <class _Fwd>
constexpr void destroy(_Fwd __first, _Fwd __last) {
    for (; __first != __last; ++__first) {
        std::destroy_at(std::addressof(*__first));
    }
}

template <class _Fwd, class _Size>
constexpr _Fwd destroy_n(_Fwd __first, _Size __n) {
    for (; __n > 0; (void)++__first, --__n) {
        std::destroy_at(std::addressof(*__first));
    }
    return __first;
}

/* The overloads with execution policies, which are freestanding deleted
 * functions from C++26 on, and not freestanding before, are deleted. */
#if _FCXX_STD >= 2026
template <class _Ep, class _Fwd, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
void uninitialized_default_construct(_Ep&&, _Fwd, _Fwd) = delete;

template <class _Ep, class _Fwd, class _Size, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
_Fwd uninitialized_default_construct_n(_Ep&&, _Fwd, _Size) = delete;

template <class _Ep, class _Fwd, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
void uninitialized_value_construct(_Ep&&, _Fwd, _Fwd) = delete;

template <class _Ep, class _Fwd, class _Size, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
_Fwd uninitialized_value_construct_n(_Ep&&, _Fwd, _Size) = delete;

template <class _Ep, class _In, class _Fwd, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
_Fwd uninitialized_copy(_Ep&&, _In, _In, _Fwd) = delete;

template <class _Ep, class _In, class _Size, class _Fwd, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
_Fwd uninitialized_copy_n(_Ep&&, _In, _Size, _Fwd) = delete;

template <class _Ep, class _In, class _Fwd, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
_Fwd uninitialized_move(_Ep&&, _In, _In, _Fwd) = delete;

template <class _Ep, class _In, class _Size, class _Fwd, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
pair<_In, _Fwd> uninitialized_move_n(_Ep&&, _In, _Size, _Fwd) = delete;

template <class _Ep, class _Fwd, class _Tp = typename iterator_traits<_Fwd>::value_type,
          __fcxx_enable_if_execution_policy_t<_Ep> = 0>
void uninitialized_fill(_Ep&&, _Fwd, _Fwd, const _Tp&) = delete;

template <class _Ep, class _Fwd, class _Size, class _Tp = typename iterator_traits<_Fwd>::value_type,
          __fcxx_enable_if_execution_policy_t<_Ep> = 0>
_Fwd uninitialized_fill_n(_Ep&&, _Fwd, _Size, const _Tp&) = delete;

template <class _Ep, class _Fwd, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
void destroy(_Ep&&, _Fwd, _Fwd) = delete;

template <class _Ep, class _Fwd, class _Size, __fcxx_enable_if_execution_policy_t<_Ep> = 0>
_Fwd destroy_n(_Ep&&, _Fwd, _Size) = delete;
#endif

namespace ranges {

/* [special.mem.concepts], special memory concepts */
template <class _It>
concept __fcxx_nothrow_input_iterator = input_iterator<_It> && is_lvalue_reference_v<iter_reference_t<_It> >
                                     && same_as<remove_cvref_t<iter_reference_t<_It> >, iter_value_t<_It> >;

template <class _Sp, class _It>
concept __fcxx_nothrow_sentinel_for = sentinel_for<_Sp, _It>;

template <class _Sp, class _It>
concept __fcxx_nothrow_sized_sentinel_for = __fcxx_nothrow_sentinel_for<_Sp, _It> && sized_sentinel_for<_Sp, _It>;

template <class _Rp>
concept __fcxx_nothrow_input_range = range<_Rp> && __fcxx_nothrow_input_iterator<iterator_t<_Rp> >
                                  && __fcxx_nothrow_sentinel_for<sentinel_t<_Rp>, iterator_t<_Rp> >;

template <class _It>
concept __fcxx_nothrow_forward_iterator = __fcxx_nothrow_input_iterator<_It> && forward_iterator<_It>
                                       && __fcxx_nothrow_sentinel_for<_It, _It>;

template <class _Rp>
concept __fcxx_nothrow_forward_range = __fcxx_nothrow_input_range<_Rp>
                                    && __fcxx_nothrow_forward_iterator<iterator_t<_Rp> >;

template <class _It>
concept __fcxx_nothrow_bidirectional_iterator = __fcxx_nothrow_forward_iterator<_It> && bidirectional_iterator<_It>;

template <class _Rp>
concept __fcxx_nothrow_bidirectional_range = __fcxx_nothrow_forward_range<_Rp>
                                          && __fcxx_nothrow_bidirectional_iterator<iterator_t<_Rp> >;

template <class _It>
concept __fcxx_nothrow_random_access_iterator = __fcxx_nothrow_bidirectional_iterator<_It>
                                             && random_access_iterator<_It>
                                             && __fcxx_nothrow_sized_sentinel_for<_It, _It>;

template <class _Rp>
concept __fcxx_nothrow_random_access_range = __fcxx_nothrow_bidirectional_range<_Rp>
                                          && __fcxx_nothrow_random_access_iterator<iterator_t<_Rp> >;

template <class _Rp>
concept __fcxx_nothrow_sized_random_access_range = __fcxx_nothrow_random_access_range<_Rp> && sized_range<_Rp>;

/* The destruction of the objects that an algorithm of namespace ranges
 * constructed before an exception. */
template <class _It>
_FCXX_CONSTEXPR_PLACEMENT void __fcxx_uninit_rollback(_It __first, _It __last) noexcept {
    for (; __first != __last; ++__first) {
        std::destroy_at(std::addressof(*__first));
    }
}

#if _FCXX_HAS_EXCEPTIONS
#  define _FCXX_RANGES_UNINIT_CATCH(first, last) \
        }                                          \
        catch (...) {                              \
            ranges::__fcxx_uninit_rollback(first, last); \
            throw;                                 \
        }
#else
#  define _FCXX_RANGES_UNINIT_CATCH(first, last) }
#endif

/* [uninitialized.construct.default] */
struct __uninitialized_default_construct_fn {
    template <__fcxx_nothrow_forward_iterator _It, __fcxx_nothrow_sentinel_for<_It> _Sp>
        requires default_initializable<iter_value_t<_It> >
    _FCXX_CONSTEXPR_PLACEMENT _It operator()(_It __first, _Sp __last) const {
        _It __current = __first;
        _FCXX_UNINIT_TRY
        for (; __current != __last; ++__current) {
            ::new (std::__fcxx_voidify(*__current)) remove_reference_t<iter_reference_t<_It> >;
        }
        _FCXX_RANGES_UNINIT_CATCH(__first, __current)
        return __current;
    }

    template <__fcxx_nothrow_forward_range _Rp>
        requires default_initializable<range_value_t<_Rp> >
    _FCXX_CONSTEXPR_PLACEMENT borrowed_iterator_t<_Rp> operator()(_Rp&& __r) const {
        return (*this)(ranges::begin(__r), ranges::end(__r));
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_random_access_iterator _It,
              __fcxx_nothrow_sized_sentinel_for<_It> _Sp>
        requires default_initializable<iter_value_t<_It> >
    _It operator()(_Ep&&, _It, _Sp) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_sized_random_access_range _Rp>
        requires default_initializable<range_value_t<_Rp> >
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&) const = delete;
#endif
};

inline constexpr __uninitialized_default_construct_fn uninitialized_default_construct{};

struct __uninitialized_default_construct_n_fn {
    template <__fcxx_nothrow_forward_iterator _It>
        requires default_initializable<iter_value_t<_It> >
    _FCXX_CONSTEXPR_PLACEMENT _It operator()(_It __first, iter_difference_t<_It> __n) const {
        _It __current = __first;
        _FCXX_UNINIT_TRY
        for (; __n > 0; (void)++__current, --__n) {
            ::new (std::__fcxx_voidify(*__current)) remove_reference_t<iter_reference_t<_It> >;
        }
        _FCXX_RANGES_UNINIT_CATCH(__first, __current)
        return __current;
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_random_access_iterator _It>
        requires default_initializable<iter_value_t<_It> >
    _It operator()(_Ep&&, _It, iter_difference_t<_It>) const = delete;
#endif
};

inline constexpr __uninitialized_default_construct_n_fn uninitialized_default_construct_n{};

/* [uninitialized.construct.value] */
struct __uninitialized_value_construct_fn {
    template <__fcxx_nothrow_forward_iterator _It, __fcxx_nothrow_sentinel_for<_It> _Sp>
        requires default_initializable<iter_value_t<_It> >
    _FCXX_CONSTEXPR_PLACEMENT _It operator()(_It __first, _Sp __last) const {
        _It __current = __first;
        _FCXX_UNINIT_TRY
        for (; __current != __last; ++__current) {
            ::new (std::__fcxx_voidify(*__current)) remove_reference_t<iter_reference_t<_It> >();
        }
        _FCXX_RANGES_UNINIT_CATCH(__first, __current)
        return __current;
    }

    template <__fcxx_nothrow_forward_range _Rp>
        requires default_initializable<range_value_t<_Rp> >
    _FCXX_CONSTEXPR_PLACEMENT borrowed_iterator_t<_Rp> operator()(_Rp&& __r) const {
        return (*this)(ranges::begin(__r), ranges::end(__r));
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_random_access_iterator _It,
              __fcxx_nothrow_sized_sentinel_for<_It> _Sp>
        requires default_initializable<iter_value_t<_It> >
    _It operator()(_Ep&&, _It, _Sp) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_sized_random_access_range _Rp>
        requires default_initializable<range_value_t<_Rp> >
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&) const = delete;
#endif
};

inline constexpr __uninitialized_value_construct_fn uninitialized_value_construct{};

struct __uninitialized_value_construct_n_fn {
    template <__fcxx_nothrow_forward_iterator _It>
        requires default_initializable<iter_value_t<_It> >
    _FCXX_CONSTEXPR_PLACEMENT _It operator()(_It __first, iter_difference_t<_It> __n) const {
        _It __current = __first;
        _FCXX_UNINIT_TRY
        for (; __n > 0; (void)++__current, --__n) {
            ::new (std::__fcxx_voidify(*__current)) remove_reference_t<iter_reference_t<_It> >();
        }
        _FCXX_RANGES_UNINIT_CATCH(__first, __current)
        return __current;
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_random_access_iterator _It>
        requires default_initializable<iter_value_t<_It> >
    _It operator()(_Ep&&, _It, iter_difference_t<_It>) const = delete;
#endif
};

inline constexpr __uninitialized_value_construct_n_fn uninitialized_value_construct_n{};

/* [uninitialized.copy] */
template <class _It, class _Out>
using uninitialized_copy_result = in_out_result<_It, _Out>;

template <class _It, class _Out>
using uninitialized_copy_n_result = in_out_result<_It, _Out>;

struct __uninitialized_copy_fn {
    template <input_iterator _It, sentinel_for<_It> _S1, __fcxx_nothrow_forward_iterator _Out,
              __fcxx_nothrow_sentinel_for<_Out> _S2>
        requires constructible_from<iter_value_t<_Out>, iter_reference_t<_It> >
    _FCXX_CONSTEXPR_PLACEMENT uninitialized_copy_result<_It, _Out> operator()(_It __ifirst, _S1 __ilast, _Out __ofirst,
                                                                            _S2 __olast) const {
        _Out __current = __ofirst;
        _FCXX_UNINIT_TRY
        for (; __ifirst != __ilast && __current != __olast; ++__current, (void)++__ifirst) {
            ::new (std::__fcxx_voidify(*__current)) remove_reference_t<iter_reference_t<_Out> >(*__ifirst);
        }
        _FCXX_RANGES_UNINIT_CATCH(__ofirst, __current)
        return {std::move(__ifirst), __current};
    }

    template <input_range _IR, __fcxx_nothrow_forward_range _OR>
        requires constructible_from<range_value_t<_OR>, range_reference_t<_IR> >
    _FCXX_CONSTEXPR_PLACEMENT uninitialized_copy_result<borrowed_iterator_t<_IR>, borrowed_iterator_t<_OR> >
    operator()(_IR&& __in, _OR&& __out) const {
        return (*this)(ranges::begin(__in), ranges::end(__in), ranges::begin(__out), ranges::end(__out));
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _S1,
              __fcxx_nothrow_random_access_iterator _Out, __fcxx_nothrow_sized_sentinel_for<_Out> _S2>
        requires constructible_from<iter_value_t<_Out>, iter_reference_t<_It> >
    uninitialized_copy_result<_It, _Out> operator()(_Ep&&, _It, _S1, _Out, _S2) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _IR,
              __fcxx_nothrow_sized_random_access_range _OR>
        requires constructible_from<range_value_t<_OR>, range_reference_t<_IR>>
    uninitialized_copy_result<borrowed_iterator_t<_IR>, borrowed_iterator_t<_OR>> operator()(_Ep&&, _IR&&,
                                                                                             _OR&&) const = delete;
#  endif
};

inline constexpr __uninitialized_copy_fn uninitialized_copy{};

struct __uninitialized_copy_n_fn {
    template <input_iterator _It, __fcxx_nothrow_forward_iterator _Out, __fcxx_nothrow_sentinel_for<_Out> _Sp>
        requires constructible_from<iter_value_t<_Out>, iter_reference_t<_It>>
    _FCXX_CONSTEXPR_PLACEMENT uninitialized_copy_n_result<_It, _Out>
    operator()(_It __ifirst, iter_difference_t<_It> __n, _Out __ofirst, _Sp __olast) const {
        _Out __current = __ofirst;
        _FCXX_UNINIT_TRY
        for (; __n > 0 && __current != __olast; ++__current, (void)++__ifirst, --__n) {
            ::new (std::__fcxx_voidify(*__current)) remove_reference_t<iter_reference_t<_Out> >(*__ifirst);
        }
        _FCXX_RANGES_UNINIT_CATCH(__ofirst, __current)
        return {std::move(__ifirst), __current};
    }

#  if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, random_access_iterator _It, __fcxx_nothrow_random_access_iterator _Out,
              __fcxx_nothrow_sized_sentinel_for<_Out> _Sp>
        requires constructible_from<iter_value_t<_Out>, iter_reference_t<_It> >
    uninitialized_copy_n_result<_It, _Out> operator()(_Ep&&, _It, iter_difference_t<_It>, _Out, _Sp) const = delete;
#endif
};

inline constexpr __uninitialized_copy_n_fn uninitialized_copy_n{};

/* [uninitialized.move] */
template <class _It, class _Out>
using uninitialized_move_result = in_out_result<_It, _Out>;

template <class _It, class _Out>
using uninitialized_move_n_result = in_out_result<_It, _Out>;

struct __uninitialized_move_fn {
    template <input_iterator _It, sentinel_for<_It> _S1, __fcxx_nothrow_forward_iterator _Out,
              __fcxx_nothrow_sentinel_for<_Out> _S2>
        requires constructible_from<iter_value_t<_Out>, iter_rvalue_reference_t<_It> >
    _FCXX_CONSTEXPR_PLACEMENT uninitialized_move_result<_It, _Out> operator()(_It __ifirst, _S1 __ilast, _Out __ofirst,
                                                                            _S2 __olast) const {
        _Out __current = __ofirst;
        _FCXX_UNINIT_TRY
        for (; __ifirst != __ilast && __current != __olast; ++__current, (void)++__ifirst) {
            ::new (std::__fcxx_voidify(*__current))
                remove_reference_t<iter_reference_t<_Out> >(ranges::iter_move(__ifirst));
        }
        _FCXX_RANGES_UNINIT_CATCH(__ofirst, __current)
        return {std::move(__ifirst), __current};
    }

    template <input_range _IR, __fcxx_nothrow_forward_range _OR>
        requires constructible_from<range_value_t<_OR>, range_rvalue_reference_t<_IR> >
    _FCXX_CONSTEXPR_PLACEMENT uninitialized_move_result<borrowed_iterator_t<_IR>, borrowed_iterator_t<_OR> >
    operator()(_IR&& __in, _OR&& __out) const {
        return (*this)(ranges::begin(__in), ranges::end(__in), ranges::begin(__out), ranges::end(__out));
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, random_access_iterator _It, sized_sentinel_for<_It> _S1,
              __fcxx_nothrow_random_access_iterator _Out, __fcxx_nothrow_sized_sentinel_for<_Out> _S2>
        requires constructible_from<iter_value_t<_Out>, iter_rvalue_reference_t<_It> >
    uninitialized_move_result<_It, _Out> operator()(_Ep&&, _It, _S1, _Out, _S2) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_sized_random_access_range _IR,
              __fcxx_nothrow_sized_random_access_range _OR>
        requires constructible_from<range_value_t<_OR>, range_rvalue_reference_t<_IR>>
    uninitialized_move_result<borrowed_iterator_t<_IR>, borrowed_iterator_t<_OR>> operator()(_Ep&&, _IR&&,
                                                                                             _OR&&) const = delete;
#  endif
};

inline constexpr __uninitialized_move_fn uninitialized_move{};

struct __uninitialized_move_n_fn {
    template <input_iterator _It, __fcxx_nothrow_forward_iterator _Out, __fcxx_nothrow_sentinel_for<_Out> _Sp>
        requires constructible_from<iter_value_t<_Out>, iter_rvalue_reference_t<_It>>
    _FCXX_CONSTEXPR_PLACEMENT uninitialized_move_n_result<_It, _Out>
    operator()(_It __ifirst, iter_difference_t<_It> __n, _Out __ofirst, _Sp __olast) const {
        _Out __current = __ofirst;
        _FCXX_UNINIT_TRY
        for (; __n > 0 && __current != __olast; ++__current, (void)++__ifirst, --__n) {
            ::new (std::__fcxx_voidify(*__current))
                remove_reference_t<iter_reference_t<_Out> >(ranges::iter_move(__ifirst));
        }
        _FCXX_RANGES_UNINIT_CATCH(__ofirst, __current)
        return {std::move(__ifirst), __current};
    }

#  if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, random_access_iterator _It, __fcxx_nothrow_random_access_iterator _Out,
              __fcxx_nothrow_sized_sentinel_for<_Out> _Sp>
        requires constructible_from<iter_value_t<_Out>, iter_rvalue_reference_t<_It> >
    uninitialized_move_n_result<_It, _Out> operator()(_Ep&&, _It, iter_difference_t<_It>, _Out, _Sp) const = delete;
#endif
};

inline constexpr __uninitialized_move_n_fn uninitialized_move_n{};

/* [uninitialized.fill]. The value type is the default of the type of the
 * value from C++26 on, as P2248 has it. */
#if _FCXX_STD >= 2026
#  define _FCXX_RANGES_FILL_DEFAULT(...) = __VA_ARGS__
#else
#  define _FCXX_RANGES_FILL_DEFAULT(...)
#endif

struct __uninitialized_fill_fn {
    template <__fcxx_nothrow_forward_iterator _It, __fcxx_nothrow_sentinel_for<_It> _Sp,
              class _Tp _FCXX_RANGES_FILL_DEFAULT(iter_value_t<_It>)>
        requires constructible_from<iter_value_t<_It>, const _Tp&>
    _FCXX_CONSTEXPR_PLACEMENT _It operator()(_It __first, _Sp __last, const _Tp& __x) const {
        _It __current = __first;
        _FCXX_UNINIT_TRY
        for (; __current != __last; ++__current) {
            ::new (std::__fcxx_voidify(*__current)) remove_reference_t<iter_reference_t<_It> >(__x);
        }
        _FCXX_RANGES_UNINIT_CATCH(__first, __current)
        return __current;
    }

    template <__fcxx_nothrow_forward_range _Rp, class _Tp _FCXX_RANGES_FILL_DEFAULT(range_value_t<_Rp>)>
        requires constructible_from<range_value_t<_Rp>, const _Tp&>
    _FCXX_CONSTEXPR_PLACEMENT borrowed_iterator_t<_Rp> operator()(_Rp&& __r, const _Tp& __x) const {
        return (*this)(ranges::begin(__r), ranges::end(__r), __x);
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_random_access_iterator _It,
              __fcxx_nothrow_sized_sentinel_for<_It> _Sp, class _Tp = iter_value_t<_It> >
        requires constructible_from<iter_value_t<_It>, const _Tp&>
    _It operator()(_Ep&&, _It, _Sp, const _Tp&) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_sized_random_access_range _Rp,
              class _Tp = range_value_t<_Rp> >
        requires constructible_from<range_value_t<_Rp>, const _Tp&>
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&, const _Tp&) const = delete;
#endif
};

inline constexpr __uninitialized_fill_fn uninitialized_fill{};

struct __uninitialized_fill_n_fn {
    template <__fcxx_nothrow_forward_iterator _It, class _Tp _FCXX_RANGES_FILL_DEFAULT(iter_value_t<_It>)>
        requires constructible_from<iter_value_t<_It>, const _Tp&>
    _FCXX_CONSTEXPR_PLACEMENT _It operator()(_It __first, iter_difference_t<_It> __n, const _Tp& __x) const {
        _It __current = __first;
        _FCXX_UNINIT_TRY
        for (; __n > 0; (void)++__current, --__n) {
            ::new (std::__fcxx_voidify(*__current)) remove_reference_t<iter_reference_t<_It> >(__x);
        }
        _FCXX_RANGES_UNINIT_CATCH(__first, __current)
        return __current;
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_random_access_iterator _It, class _Tp = iter_value_t<_It> >
        requires constructible_from<iter_value_t<_It>, const _Tp&>
    _It operator()(_Ep&&, _It, iter_difference_t<_It>, const _Tp&) const = delete;
#endif
};

inline constexpr __uninitialized_fill_n_fn uninitialized_fill_n{};

#undef _FCXX_RANGES_FILL_DEFAULT

/* [specialized.construct], ranges::construct_at */
struct __construct_at_fn {
    template <class _Tp, class... _Args>
        requires requires(void* __p, _Args&&... __args) { ::new (__p) _Tp(static_cast<_Args&&>(__args)...); }
              && (!is_unbounded_array_v<_Tp>)
    constexpr _Tp* operator()(_Tp* __location, _Args&&... __args) const {
        return std::construct_at(__location, std::forward<_Args>(__args)...);
    }
};

inline constexpr __construct_at_fn construct_at{};

/* [specialized.destroy], ranges::destroy_at, destroy and destroy_n */
struct __destroy_at_fn {
    template <destructible _Tp>
    constexpr void operator()(_Tp* __location) const noexcept {
        std::destroy_at(__location);
    }
};

inline constexpr __destroy_at_fn destroy_at{};

struct __destroy_fn {
    template <__fcxx_nothrow_input_iterator _It, __fcxx_nothrow_sentinel_for<_It> _Sp>
        requires destructible<iter_value_t<_It> >
    constexpr _It operator()(_It __first, _Sp __last) const noexcept {
        for (; __first != __last; ++__first) {
            std::destroy_at(std::addressof(*__first));
        }
        return __first;
    }

    template <__fcxx_nothrow_input_range _Rp>
        requires destructible<range_value_t<_Rp> >
    constexpr borrowed_iterator_t<_Rp> operator()(_Rp&& __r) const noexcept {
        return (*this)(ranges::begin(__r), ranges::end(__r));
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_random_access_iterator _It,
              __fcxx_nothrow_sized_sentinel_for<_It> _Sp>
        requires destructible<iter_value_t<_It> >
    _It operator()(_Ep&&, _It, _Sp) const = delete;

    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_sized_random_access_range _Rp>
        requires destructible<range_value_t<_Rp> >
    borrowed_iterator_t<_Rp> operator()(_Ep&&, _Rp&&) const = delete;
#endif
};

inline constexpr __destroy_fn destroy{};

struct __destroy_n_fn {
    template <__fcxx_nothrow_input_iterator _It>
        requires destructible<iter_value_t<_It> >
    constexpr _It operator()(_It __first, iter_difference_t<_It> __n) const noexcept {
        for (; __n > 0; (void)++__first, --__n) {
            std::destroy_at(std::addressof(*__first));
        }
        return __first;
    }

#if _FCXX_STD >= 2026
    template <__fcxx_execution_policy _Ep, __fcxx_nothrow_random_access_iterator _It>
        requires destructible<iter_value_t<_It> >
    _It operator()(_Ep&&, _It, iter_difference_t<_It>) const = delete;
#endif
};

inline constexpr __destroy_n_fn destroy_n{};

#undef _FCXX_RANGES_UNINIT_CATCH

} /* namespace ranges */

} /* namespace std */

#undef _FCXX_UNINIT_TRY
#undef _FCXX_UNINIT_CATCH
#undef _FCXX_CONSTEXPR_PLACEMENT

#endif
