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


/* What <stdatomic.h>, that of freestanding-c-hdrs, has in C++, from C++23
 * on, which it includes this for: _Atomic(T) is std::atomic<T>, and the
 * names of <atomic> that C has are in the global namespace too. */

#ifndef __FREESTANDING_CXX_HDRS_FCXX_C_STDATOMIC_H
#define __FREESTANDING_CXX_HDRS_FCXX_C_STDATOMIC_H 1

#pragma GCC system_header

#include <__fcxx/config.h>

#if _FCXX_STD >= 2023

#include <atomic>

namespace std {

template <class _Tp>
using __fcxx_c_atomic = atomic<_Tp>;

} /* namespace std */

#undef _Atomic
#define _Atomic(T) ::std::__fcxx_c_atomic<T>

/* C++ has <stdatomic.h> itself define this, not <atomic>, and not <version>
 * either, as it is no freestanding header. */
#undef __cpp_lib_stdatomic_h
#define __cpp_lib_stdatomic_h 202011L

using std::memory_order;
using std::memory_order_relaxed;
using std::memory_order_consume;
using std::memory_order_acquire;
using std::memory_order_release;
using std::memory_order_acq_rel;
using std::memory_order_seq_cst;

using std::atomic_flag;

using std::atomic_bool;
using std::atomic_char;
using std::atomic_schar;
using std::atomic_uchar;
using std::atomic_short;
using std::atomic_ushort;
using std::atomic_int;
using std::atomic_uint;
using std::atomic_long;
using std::atomic_ulong;
using std::atomic_llong;
using std::atomic_ullong;
#if _FCXX_HAS_CHAR8_T
using std::atomic_char8_t;
#endif
using std::atomic_char16_t;
using std::atomic_char32_t;
using std::atomic_wchar_t;

#ifdef INT8_MAX
using std::atomic_int8_t;
#endif
#ifdef UINT8_MAX
using std::atomic_uint8_t;
#endif
#ifdef INT16_MAX
using std::atomic_int16_t;
#endif
#ifdef UINT16_MAX
using std::atomic_uint16_t;
#endif
#ifdef INT32_MAX
using std::atomic_int32_t;
#endif
#ifdef UINT32_MAX
using std::atomic_uint32_t;
#endif
#ifdef INT64_MAX
using std::atomic_int64_t;
#endif
#ifdef UINT64_MAX
using std::atomic_uint64_t;
#endif

using std::atomic_int_least8_t;
using std::atomic_uint_least8_t;
using std::atomic_int_least16_t;
using std::atomic_uint_least16_t;
using std::atomic_int_least32_t;
using std::atomic_uint_least32_t;
using std::atomic_int_least64_t;
using std::atomic_uint_least64_t;

using std::atomic_int_fast8_t;
using std::atomic_uint_fast8_t;
using std::atomic_int_fast16_t;
using std::atomic_uint_fast16_t;
using std::atomic_int_fast32_t;
using std::atomic_uint_fast32_t;
using std::atomic_int_fast64_t;
using std::atomic_uint_fast64_t;

#ifdef INTPTR_MAX
using std::atomic_intptr_t;
#endif
#ifdef UINTPTR_MAX
using std::atomic_uintptr_t;
#endif
using std::atomic_size_t;
using std::atomic_ptrdiff_t;
using std::atomic_intmax_t;
using std::atomic_uintmax_t;

using std::atomic_is_lock_free;
using std::atomic_load;
using std::atomic_load_explicit;
using std::atomic_store;
using std::atomic_store_explicit;
using std::atomic_exchange;
using std::atomic_exchange_explicit;
using std::atomic_compare_exchange_strong;
using std::atomic_compare_exchange_strong_explicit;
using std::atomic_compare_exchange_weak;
using std::atomic_compare_exchange_weak_explicit;
using std::atomic_fetch_add;
using std::atomic_fetch_add_explicit;
using std::atomic_fetch_sub;
using std::atomic_fetch_sub_explicit;
using std::atomic_fetch_and;
using std::atomic_fetch_and_explicit;
using std::atomic_fetch_or;
using std::atomic_fetch_or_explicit;
using std::atomic_fetch_xor;
using std::atomic_fetch_xor_explicit;
using std::atomic_flag_test_and_set;
using std::atomic_flag_test_and_set_explicit;
using std::atomic_flag_clear;
using std::atomic_flag_clear_explicit;
using std::atomic_thread_fence;
using std::atomic_signal_fence;

#endif

#endif
