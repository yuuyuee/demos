// -*- C++ -*- header.

// Copyright (C) 2008-2015 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file include/atomic
 *  This is a Standard C++ Library header.
 */

// Based on "C++ Atomic Types and Operations" by Hans Boehm and Lawrence Crowl.
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2427.html

#ifndef _GLIBCXX_ATOMIC
#define _GLIBCXX_ATOMIC 1

#pragma GCC system_header

#if __cplusplus < 201103L
# include <bits/c++0x_warning.h>
#else

#include <bits/atomic_base.h>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   * @addtogroup atomics
   * @{
   */

  template<typename _Tp>
    struct atomic;

  /// atomic<bool>
  // NB: No operators or fetch-operations for this type.
  template<>
  struct atomic<bool>
  {
  private:
    __atomic_base<bool>	_M_base;

  public:
    atomic() noexcept = default;
    ~atomic() noexcept = default;
    atomic(const atomic&) = delete;
    atomic& operator=(const atomic&) = delete;
    atomic& operator=(const atomic&) volatile = delete;

    constexpr atomic(bool __i) noexcept : _M_base(__i) { }

    bool
    operator=(bool __i) noexcept
    { return _M_base.operator=(__i); }

    bool
    operator=(bool __i) volatile noexcept
    { return _M_base.operator=(__i); }

    operator bool() const noexcept
    { return _M_base.load(); }

    operator bool() const volatile noexcept
    { return _M_base.load(); }

    bool
    is_lock_free() const noexcept { return _M_base.is_lock_free(); }

    bool
    is_lock_free() const volatile noexcept { return _M_base.is_lock_free(); }

    void
    store(bool __i, memory_order __m = memory_order_seq_cst) noexcept
    { _M_base.store(__i, __m); }

    void
    store(bool __i, memory_order __m = memory_order_seq_cst) volatile noexcept
    { _M_base.store(__i, __m); }

    bool
    load(memory_order __m = memory_order_seq_cst) const noexcept
    { return _M_base.load(__m); }

    bool
    load(memory_order __m = memory_order_seq_cst) const volatile noexcept
    { return _M_base.load(__m); }

    bool
    exchange(bool __i, memory_order __m = memory_order_seq_cst) noexcept
    { return _M_base.exchange(__i, __m); }

    bool
    exchange(bool __i,
	     memory_order __m = memory_order_seq_cst) volatile noexcept
    { return _M_base.exchange(__i, __m); }

    bool
    compare_exchange_weak(bool& __i1, bool __i2, memory_order __m1,
			  memory_order __m2) noexcept
    { return _M_base.compare_exchange_weak(__i1, __i2, __m1, __m2); }

    bool
    compare_exchange_weak(bool& __i1, bool __i2, memory_order __m1,
			  memory_order __m2) volatile noexcept
    { return _M_base.compare_exchange_weak(__i1, __i2, __m1, __m2); }

    bool
    compare_exchange_weak(bool& __i1, bool __i2,
			  memory_order __m = memory_order_seq_cst) noexcept
    { return _M_base.compare_exchange_weak(__i1, __i2, __m); }

    bool
    compare_exchange_weak(bool& __i1, bool __i2,
		     memory_order __m = memory_order_seq_cst) volatile noexcept
    { return _M_base.compare_exchange_weak(__i1, __i2, __m); }

    bool
    compare_exchange_strong(bool& __i1, bool __i2, memory_order __m1,
			    memory_order __m2) noexcept
    { return _M_base.compare_exchange_strong(__i1, __i2, __m1, __m2); }

    bool
    compare_exchange_strong(bool& __i1, bool __i2, memory_order __m1,
			    memory_order __m2) volatile noexcept
    { return _M_base.compare_exchange_strong(__i1, __i2, __m1, __m2); }

    bool
    compare_exchange_strong(bool& __i1, bool __i2,
			    memory_order __m = memory_order_seq_cst) noexcept
    { return _M_base.compare_exchange_strong(__i1, __i2, __m); }

    bool
    compare_exchange_strong(bool& __i1, bool __i2,
		    memory_order __m = memory_order_seq_cst) volatile noexcept
    { return _M_base.compare_exchange_strong(__i1, __i2, __m); }
  };


  /**
   *  @brief Generic atomic type, primary class template.
   *
   *  @tparam _Tp  Type to be made atomic, must be trivally copyable.
   */
  template<typename _Tp>
    struct atomic
    {
    private:
      // Align 1/2/4/8/16-byte types to at least their size.
      static constexpr int _S_min_alignment
	= (sizeof(_Tp) & (sizeof(_Tp) - 1)) || sizeof(_Tp) > 16
	? 0 : sizeof(_Tp);

      static constexpr int _S_alignment
        = _S_min_alignment > alignof(_Tp) ? _S_min_alignment : alignof(_Tp);

      alignas(_S_alignment) _Tp _M_i;

      static_assert(__is_trivially_copyable(_Tp),
		    "std::atomic requires a trivially copyable type");

      static_assert(sizeof(_Tp) > 0,
		    "Incomplete or zero-sized types are not supported");

    public:
      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(_Tp __i) noexcept : _M_i(__i) { }

      operator _Tp() const noexcept
      { return load(); }

      operator _Tp() const volatile noexcept
      { return load(); }

      _Tp
      operator=(_Tp __i) noexcept 
      { store(__i); return __i; }

      _Tp
      operator=(_Tp __i) volatile noexcept 
      { store(__i); return __i; }

      bool
      is_lock_free() const noexcept
      {
	// Produce a fake, minimally aligned pointer.
	return __atomic_is_lock_free(sizeof(_M_i),
	    reinterpret_cast<void *>(-__alignof(_M_i)));
      }

      bool
      is_lock_free() const volatile noexcept
      {
	// Produce a fake, minimally aligned pointer.
	return __atomic_is_lock_free(sizeof(_M_i),
	    reinterpret_cast<void *>(-__alignof(_M_i)));
      }

      void
      store(_Tp __i, memory_order __m = memory_order_seq_cst) noexcept
      { __atomic_store(&_M_i, &__i, __m); }

      void
      store(_Tp __i, memory_order __m = memory_order_seq_cst) volatile noexcept
      { __atomic_store(&_M_i, &__i, __m); }

      _Tp
      load(memory_order __m = memory_order_seq_cst) const noexcept
      { 
        _Tp tmp;
	__atomic_load(&_M_i, &tmp, __m);
	return tmp;
      }

      _Tp
      load(memory_order __m = memory_order_seq_cst) const volatile noexcept
      { 
        _Tp tmp;
	__atomic_load(&_M_i, &tmp, __m);
	return tmp;
      }

      _Tp
      exchange(_Tp __i, memory_order __m = memory_order_seq_cst) noexcept
      { 
        _Tp tmp;
	__atomic_exchange(&_M_i, &__i, &tmp, __m);
	return tmp;
      }

      _Tp
      exchange(_Tp __i, 
	       memory_order __m = memory_order_seq_cst) volatile noexcept
      { 
        _Tp tmp;
	__atomic_exchange(&_M_i, &__i, &tmp, __m);
	return tmp;
      }

      bool
      compare_exchange_weak(_Tp& __e, _Tp __i, memory_order __s, 
			    memory_order __f) noexcept
      {
	return __atomic_compare_exchange(&_M_i, &__e, &__i, true, __s, __f);
      }

      bool
      compare_exchange_weak(_Tp& __e, _Tp __i, memory_order __s, 
			    memory_order __f) volatile noexcept
      {
	return __atomic_compare_exchange(&_M_i, &__e, &__i, true, __s, __f);
      }

      bool
      compare_exchange_weak(_Tp& __e, _Tp __i,
			    memory_order __m = memory_order_seq_cst) noexcept
      { return compare_exchange_weak(__e, __i, __m,
                                     __cmpexch_failure_order(__m)); }

      bool
      compare_exchange_weak(_Tp& __e, _Tp __i,
		     memory_order __m = memory_order_seq_cst) volatile noexcept
      { return compare_exchange_weak(__e, __i, __m,
                                     __cmpexch_failure_order(__m)); }

      bool
      compare_exchange_strong(_Tp& __e, _Tp __i, memory_order __s, 
			      memory_order __f) noexcept
      {
	return __atomic_compare_exchange(&_M_i, &__e, &__i, false, __s, __f);
      }

      bool
      compare_exchange_strong(_Tp& __e, _Tp __i, memory_order __s, 
			      memory_order __f) volatile noexcept
      {
	return __atomic_compare_exchange(&_M_i, &__e, &__i, false, __s, __f);
      }

      bool
      compare_exchange_strong(_Tp& __e, _Tp __i,
			       memory_order __m = memory_order_seq_cst) noexcept
      { return compare_exchange_strong(__e, __i, __m,
                                       __cmpexch_failure_order(__m)); }

      bool
      compare_exchange_strong(_Tp& __e, _Tp __i,
		     memory_order __m = memory_order_seq_cst) volatile noexcept
      { return compare_exchange_strong(__e, __i, __m,
                                       __cmpexch_failure_order(__m)); }
    };


  /// Partial specialization for pointer types.
  template<typename _Tp>
    struct atomic<_Tp*>
    {
      typedef _Tp* 			__pointer_type;
      typedef __atomic_base<_Tp*>	__base_type;
      __base_type			_M_b;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__pointer_type __p) noexcept : _M_b(__p) { }

      operator __pointer_type() const noexcept
      { return __pointer_type(_M_b); }

      operator __pointer_type() const volatile noexcept
      { return __pointer_type(_M_b); }

      __pointer_type
      operator=(__pointer_type __p) noexcept
      { return _M_b.operator=(__p); }

      __pointer_type
      operator=(__pointer_type __p) volatile noexcept
      { return _M_b.operator=(__p); }

      __pointer_type
      operator++(int) noexcept
      { return _M_b++; }

      __pointer_type
      operator++(int) volatile noexcept
      { return _M_b++; }

      __pointer_type
      operator--(int) noexcept
      { return _M_b--; }

      __pointer_type
      operator--(int) volatile noexcept
      { return _M_b--; }

      __pointer_type
      operator++() noexcept
      { return ++_M_b; }

      __pointer_type
      operator++() volatile noexcept
      { return ++_M_b; }

      __pointer_type
      operator--() noexcept
      { return --_M_b; }

      __pointer_type
      operator--() volatile noexcept
      { return --_M_b; }

      __pointer_type
      operator+=(ptrdiff_t __d) noexcept
      { return _M_b.operator+=(__d); }

      __pointer_type
      operator+=(ptrdiff_t __d) volatile noexcept
      { return _M_b.operator+=(__d); }

      __pointer_type
      operator-=(ptrdiff_t __d) noexcept
      { return _M_b.operator-=(__d); }

      __pointer_type
      operator-=(ptrdiff_t __d) volatile noexcept
      { return _M_b.operator-=(__d); }

      bool
      is_lock_free() const noexcept
      { return _M_b.is_lock_free(); }

      bool
      is_lock_free() const volatile noexcept
      { return _M_b.is_lock_free(); }

      void
      store(__pointer_type __p,
	    memory_order __m = memory_order_seq_cst) noexcept
      { return _M_b.store(__p, __m); }

      void
      store(__pointer_type __p,
	    memory_order __m = memory_order_seq_cst) volatile noexcept
      { return _M_b.store(__p, __m); }

      __pointer_type
      load(memory_order __m = memory_order_seq_cst) const noexcept
      { return _M_b.load(__m); }

      __pointer_type
      load(memory_order __m = memory_order_seq_cst) const volatile noexcept
      { return _M_b.load(__m); }

      __pointer_type
      exchange(__pointer_type __p,
	       memory_order __m = memory_order_seq_cst) noexcept
      { return _M_b.exchange(__p, __m); }

      __pointer_type
      exchange(__pointer_type __p,
	       memory_order __m = memory_order_seq_cst) volatile noexcept
      { return _M_b.exchange(__p, __m); }

      bool
      compare_exchange_weak(__pointer_type& __p1, __pointer_type __p2,
			    memory_order __m1, memory_order __m2) noexcept
      { return _M_b.compare_exchange_strong(__p1, __p2, __m1, __m2); }

      bool
      compare_exchange_weak(__pointer_type& __p1, __pointer_type __p2,
			    memory_order __m1,
			    memory_order __m2) volatile noexcept
      { return _M_b.compare_exchange_strong(__p1, __p2, __m1, __m2); }

      bool
      compare_exchange_weak(__pointer_type& __p1, __pointer_type __p2,
			    memory_order __m = memory_order_seq_cst) noexcept
      {
	return compare_exchange_weak(__p1, __p2, __m,
				     __cmpexch_failure_order(__m));
      }

      bool
      compare_exchange_weak(__pointer_type& __p1, __pointer_type __p2,
		    memory_order __m = memory_order_seq_cst) volatile noexcept
      {
	return compare_exchange_weak(__p1, __p2, __m,
				     __cmpexch_failure_order(__m));
      }

      bool
      compare_exchange_strong(__pointer_type& __p1, __pointer_type __p2,
			      memory_order __m1, memory_order __m2) noexcept
      { return _M_b.compare_exchange_strong(__p1, __p2, __m1, __m2); }

      bool
      compare_exchange_strong(__pointer_type& __p1, __pointer_type __p2,
			      memory_order __m1,
			      memory_order __m2) volatile noexcept
      { return _M_b.compare_exchange_strong(__p1, __p2, __m1, __m2); }

      bool
      compare_exchange_strong(__pointer_type& __p1, __pointer_type __p2,
			      memory_order __m = memory_order_seq_cst) noexcept
      {
	return _M_b.compare_exchange_strong(__p1, __p2, __m,
					    __cmpexch_failure_order(__m));
      }

      bool
      compare_exchange_strong(__pointer_type& __p1, __pointer_type __p2,
		    memory_order __m = memory_order_seq_cst) volatile noexcept
      {
	return _M_b.compare_exchange_strong(__p1, __p2, __m,
					    __cmpexch_failure_order(__m));
      }

      __pointer_type
      fetch_add(ptrdiff_t __d,
		memory_order __m = memory_order_seq_cst) noexcept
      { return _M_b.fetch_add(__d, __m); }

      __pointer_type
      fetch_add(ptrdiff_t __d,
		memory_order __m = memory_order_seq_cst) volatile noexcept
      { return _M_b.fetch_add(__d, __m); }

      __pointer_type
      fetch_sub(ptrdiff_t __d,
		memory_order __m = memory_order_seq_cst) noexcept
      { return _M_b.fetch_sub(__d, __m); }

      __pointer_type
      fetch_sub(ptrdiff_t __d,
		memory_order __m = memory_order_seq_cst) volatile noexcept
      { return _M_b.fetch_sub(__d, __m); }
    };


  /// Explicit specialization for char.
  template<>
    struct atomic<char> : __atomic_base<char>
    {
      typedef char 			__integral_type;
      typedef __atomic_base<char> 	__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for signed char.
  template<>
    struct atomic<signed char> : __atomic_base<signed char>
    {
      typedef signed char 		__integral_type;
      typedef __atomic_base<signed char> 	__base_type;

      atomic() noexcept= default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for unsigned char.
  template<>
    struct atomic<unsigned char> : __atomic_base<unsigned char>
    {
      typedef unsigned char 		__integral_type;
      typedef __atomic_base<unsigned char> 	__base_type;

      atomic() noexcept= default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for short.
  template<>
    struct atomic<short> : __atomic_base<short>
    {
      typedef short 			__integral_type;
      typedef __atomic_base<short> 		__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for unsigned short.
  template<>
    struct atomic<unsigned short> : __atomic_base<unsigned short>
    {
      typedef unsigned short 	      	__integral_type;
      typedef __atomic_base<unsigned short> 		__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for int.
  template<>
    struct atomic<int> : __atomic_base<int>
    {
      typedef int 			__integral_type;
      typedef __atomic_base<int> 		__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for unsigned int.
  template<>
    struct atomic<unsigned int> : __atomic_base<unsigned int>
    {
      typedef unsigned int		__integral_type;
      typedef __atomic_base<unsigned int> 	__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for long.
  template<>
    struct atomic<long> : __atomic_base<long>
    {
      typedef long 			__integral_type;
      typedef __atomic_base<long> 	__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for unsigned long.
  template<>
    struct atomic<unsigned long> : __atomic_base<unsigned long>
    {
      typedef unsigned long 		__integral_type;
      typedef __atomic_base<unsigned long> 	__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for long long.
  template<>
    struct atomic<long long> : __atomic_base<long long>
    {
      typedef long long 		__integral_type;
      typedef __atomic_base<long long> 		__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for unsigned long long.
  template<>
    struct atomic<unsigned long long> : __atomic_base<unsigned long long>
    {
      typedef unsigned long long       	__integral_type;
      typedef __atomic_base<unsigned long long> 	__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for wchar_t.
  template<>
    struct atomic<wchar_t> : __atomic_base<wchar_t>
    {
      typedef wchar_t 			__integral_type;
      typedef __atomic_base<wchar_t> 	__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for char16_t.
  template<>
    struct atomic<char16_t> : __atomic_base<char16_t>
    {
      typedef char16_t 			__integral_type;
      typedef __atomic_base<char16_t> 	__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };

  /// Explicit specialization for char32_t.
  template<>
    struct atomic<char32_t> : __atomic_base<char32_t>
    {
      typedef char32_t 			__integral_type;
      typedef __atomic_base<char32_t> 	__base_type;

      atomic() noexcept = default;
      ~atomic() noexcept = default;
      atomic(const atomic&) = delete;
      atomic& operator=(const atomic&) = delete;
      atomic& operator=(const atomic&) volatile = delete;

      constexpr atomic(__integral_type __i) noexcept : __base_type(__i) { }

      using __base_type::operator __integral_type;
      using __base_type::operator=;
    };


  /// atomic_bool
  typedef atomic<bool>			atomic_bool;

  /// atomic_char
  typedef atomic<char>			atomic_char;

  /// atomic_schar
  typedef atomic<signed char>		atomic_schar;

  /// atomic_uchar
  typedef atomic<unsigned char>		atomic_uchar;

  /// atomic_short
  typedef atomic<short>			atomic_short;

  /// atomic_ushort
  typedef atomic<unsigned short>	atomic_ushort;

  /// atomic_int
  typedef atomic<int>			atomic_int;

  /// atomic_uint
  typedef atomic<unsigned int>		atomic_uint;

  /// atomic_long
  typedef atomic<long>			atomic_long;

  /// atomic_ulong
  typedef atomic<unsigned long>		atomic_ulong;

  /// atomic_llong
  typedef atomic<long long>		atomic_llong;

  /// atomic_ullong
  typedef atomic<unsigned long long>	atomic_ullong;

  /// atomic_wchar_t
  typedef atomic<wchar_t>		atomic_wchar_t;

  /// atomic_char16_t
  typedef atomic<char16_t>		atomic_char16_t;

  /// atomic_char32_t
  typedef atomic<char32_t>		atomic_char32_t;


  /// atomic_int_least8_t
  typedef atomic<int_least8_t>		atomic_int_least8_t;

  /// atomic_uint_least8_t
  typedef atomic<uint_least8_t>		atomic_uint_least8_t;

  /// atomic_int_least16_t
  typedef atomic<int_least16_t>		atomic_int_least16_t;

  /// atomic_uint_least16_t
  typedef atomic<uint_least16_t>	atomic_uint_least16_t;

  /// atomic_int_least32_t
  typedef atomic<int_least32_t>		atomic_int_least32_t;

  /// atomic_uint_least32_t
  typedef atomic<uint_least32_t>	atomic_uint_least32_t;

  /// atomic_int_least64_t
  typedef atomic<int_least64_t>		atomic_int_least64_t;

  /// atomic_uint_least64_t
  typedef atomic<uint_least64_t>	atomic_uint_least64_t;


  /// atomic_int_fast8_t
  typedef atomic<int_fast8_t>		atomic_int_fast8_t;

  /// atomic_uint_fast8_t
  typedef atomic<uint_fast8_t>		atomic_uint_fast8_t;

  /// atomic_int_fast16_t
  typedef atomic<int_fast16_t>		atomic_int_fast16_t;

  /// atomic_uint_fast16_t
  typedef atomic<uint_fast16_t>		atomic_uint_fast16_t;

  /// atomic_int_fast32_t
  typedef atomic<int_fast32_t>		atomic_int_fast32_t;

  /// atomic_uint_fast32_t
  typedef atomic<uint_fast32_t>		atomic_uint_fast32_t;

  /// atomic_int_fast64_t
  typedef atomic<int_fast64_t>		atomic_int_fast64_t;

  /// atomic_uint_fast64_t
  typedef atomic<uint_fast64_t>		atomic_uint_fast64_t;


  /// atomic_intptr_t
  typedef atomic<intptr_t>		atomic_intptr_t;

  /// atomic_uintptr_t
  typedef atomic<uintptr_t>		atomic_uintptr_t;

  /// atomic_size_t
  typedef atomic<size_t>		atomic_size_t;

  /// atomic_intmax_t
  typedef atomic<intmax_t>		atomic_intmax_t;

  /// atomic_uintmax_t
  typedef atomic<uintmax_t>		atomic_uintmax_t;

  /// atomic_ptrdiff_t
  typedef atomic<ptrdiff_t>		atomic_ptrdiff_t;


  // Function definitions, atomic_flag operations.
  inline bool
  atomic_flag_test_and_set_explicit(atomic_flag* __a,
				    memory_order __m) noexcept
  { return __a->test_and_set(__m); }

  inline bool
  atomic_flag_test_and_set_explicit(volatile atomic_flag* __a,
				    memory_order __m) noexcept
  { return __a->test_and_set(__m); }

  inline void
  atomic_flag_clear_explicit(atomic_flag* __a, memory_order __m) noexcept
  { __a->clear(__m); }

  inline void
  atomic_flag_clear_explicit(volatile atomic_flag* __a,
			     memory_order __m) noexcept
  { __a->clear(__m); }

  inline bool
  atomic_flag_test_and_set(atomic_flag* __a) noexcept
  { return atomic_flag_test_and_set_explicit(__a, memory_order_seq_cst); }

  inline bool
  atomic_flag_test_and_set(volatile atomic_flag* __a) noexcept
  { return atomic_flag_test_and_set_explicit(__a, memory_order_seq_cst); }

  inline void
  atomic_flag_clear(atomic_flag* __a) noexcept
  { atomic_flag_clear_explicit(__a, memory_order_seq_cst); }

  inline void
  atomic_flag_clear(volatile atomic_flag* __a) noexcept
  { atomic_flag_clear_explicit(__a, memory_order_seq_cst); }


  // Function templates generally applicable to atomic types.
  template<typename _ITp>
    inline bool
    atomic_is_lock_free(const atomic<_ITp>* __a) noexcept
    { return __a->is_lock_free(); }

  template<typename _ITp>
    inline bool
    atomic_is_lock_free(const volatile atomic<_ITp>* __a) noexcept
    { return __a->is_lock_free(); }

  template<typename _ITp>
    inline void
    atomic_init(atomic<_ITp>* __a, _ITp __i) noexcept
    { __a->store(__i, memory_order_relaxed); }

  template<typename _ITp>
    inline void
    atomic_init(volatile atomic<_ITp>* __a, _ITp __i) noexcept
    { __a->store(__i, memory_order_relaxed); }

  template<typename _ITp>
    inline void
    atomic_store_explicit(atomic<_ITp>* __a, _ITp __i,
			  memory_order __m) noexcept
    { __a->store(__i, __m); }

  template<typename _ITp>
    inline void
    atomic_store_explicit(volatile atomic<_ITp>* __a, _ITp __i,
			  memory_order __m) noexcept
    { __a->store(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_load_explicit(const atomic<_ITp>* __a, memory_order __m) noexcept
    { return __a->load(__m); }

  template<typename _ITp>
    inline _ITp
    atomic_load_explicit(const volatile atomic<_ITp>* __a,
			 memory_order __m) noexcept
    { return __a->load(__m); }

  template<typename _ITp>
    inline _ITp
    atomic_exchange_explicit(atomic<_ITp>* __a, _ITp __i,
			     memory_order __m) noexcept
    { return __a->exchange(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_exchange_explicit(volatile atomic<_ITp>* __a, _ITp __i,
			     memory_order __m) noexcept
    { return __a->exchange(__i, __m); }

  template<typename _ITp>
    inline bool
    atomic_compare_exchange_weak_explicit(atomic<_ITp>* __a,
					  _ITp* __i1, _ITp __i2,
					  memory_order __m1,
					  memory_order __m2) noexcept
    { return __a->compare_exchange_weak(*__i1, __i2, __m1, __m2); }

  template<typename _ITp>
    inline bool
    atomic_compare_exchange_weak_explicit(volatile atomic<_ITp>* __a,
					  _ITp* __i1, _ITp __i2,
					  memory_order __m1,
					  memory_order __m2) noexcept
    { return __a->compare_exchange_weak(*__i1, __i2, __m1, __m2); }

  template<typename _ITp>
    inline bool
    atomic_compare_exchange_strong_explicit(atomic<_ITp>* __a,
					    _ITp* __i1, _ITp __i2,
					    memory_order __m1,
					    memory_order __m2) noexcept
    { return __a->compare_exchange_strong(*__i1, __i2, __m1, __m2); }

  template<typename _ITp>
    inline bool
    atomic_compare_exchange_strong_explicit(volatile atomic<_ITp>* __a,
					    _ITp* __i1, _ITp __i2,
					    memory_order __m1,
					    memory_order __m2) noexcept
    { return __a->compare_exchange_strong(*__i1, __i2, __m1, __m2); }


  template<typename _ITp>
    inline void
    atomic_store(atomic<_ITp>* __a, _ITp __i) noexcept
    { atomic_store_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline void
    atomic_store(volatile atomic<_ITp>* __a, _ITp __i) noexcept
    { atomic_store_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_load(const atomic<_ITp>* __a) noexcept
    { return atomic_load_explicit(__a, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_load(const volatile atomic<_ITp>* __a) noexcept
    { return atomic_load_explicit(__a, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_exchange(atomic<_ITp>* __a, _ITp __i) noexcept
    { return atomic_exchange_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_exchange(volatile atomic<_ITp>* __a, _ITp __i) noexcept
    { return atomic_exchange_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline bool
    atomic_compare_exchange_weak(atomic<_ITp>* __a,
				 _ITp* __i1, _ITp __i2) noexcept
    {
      return atomic_compare_exchange_weak_explicit(__a, __i1, __i2,
						   memory_order_seq_cst,
						   memory_order_seq_cst);
    }

  template<typename _ITp>
    inline bool
    atomic_compare_exchange_weak(volatile atomic<_ITp>* __a,
				 _ITp* __i1, _ITp __i2) noexcept
    {
      return atomic_compare_exchange_weak_explicit(__a, __i1, __i2,
						   memory_order_seq_cst,
						   memory_order_seq_cst);
    }

  template<typename _ITp>
    inline bool
    atomic_compare_exchange_strong(atomic<_ITp>* __a,
				   _ITp* __i1, _ITp __i2) noexcept
    {
      return atomic_compare_exchange_strong_explicit(__a, __i1, __i2,
						     memory_order_seq_cst,
						     memory_order_seq_cst);
    }

  template<typename _ITp>
    inline bool
    atomic_compare_exchange_strong(volatile atomic<_ITp>* __a,
				   _ITp* __i1, _ITp __i2) noexcept
    {
      return atomic_compare_exchange_strong_explicit(__a, __i1, __i2,
						     memory_order_seq_cst,
						     memory_order_seq_cst);
    }

  // Function templates for atomic_integral operations only, using
  // __atomic_base. Template argument should be constricted to
  // intergral types as specified in the standard, excluding address
  // types.
  template<typename _ITp>
    inline _ITp
    atomic_fetch_add_explicit(__atomic_base<_ITp>* __a, _ITp __i,
			      memory_order __m) noexcept
    { return __a->fetch_add(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_add_explicit(volatile __atomic_base<_ITp>* __a, _ITp __i,
			      memory_order __m) noexcept
    { return __a->fetch_add(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_sub_explicit(__atomic_base<_ITp>* __a, _ITp __i,
			      memory_order __m) noexcept
    { return __a->fetch_sub(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_sub_explicit(volatile __atomic_base<_ITp>* __a, _ITp __i,
			      memory_order __m) noexcept
    { return __a->fetch_sub(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_and_explicit(__atomic_base<_ITp>* __a, _ITp __i,
			      memory_order __m) noexcept
    { return __a->fetch_and(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_and_explicit(volatile __atomic_base<_ITp>* __a, _ITp __i,
			      memory_order __m) noexcept
    { return __a->fetch_and(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_or_explicit(__atomic_base<_ITp>* __a, _ITp __i,
			     memory_order __m) noexcept
    { return __a->fetch_or(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_or_explicit(volatile __atomic_base<_ITp>* __a, _ITp __i,
			     memory_order __m) noexcept
    { return __a->fetch_or(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_xor_explicit(__atomic_base<_ITp>* __a, _ITp __i,
			      memory_order __m) noexcept
    { return __a->fetch_xor(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_xor_explicit(volatile __atomic_base<_ITp>* __a, _ITp __i,
			      memory_order __m) noexcept
    { return __a->fetch_xor(__i, __m); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_add(__atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_add_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_add(volatile __atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_add_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_sub(__atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_sub_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_sub(volatile __atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_sub_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_and(__atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_and_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_and(volatile __atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_and_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_or(__atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_or_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_or(volatile __atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_or_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_xor(__atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_xor_explicit(__a, __i, memory_order_seq_cst); }

  template<typename _ITp>
    inline _ITp
    atomic_fetch_xor(volatile __atomic_base<_ITp>* __a, _ITp __i) noexcept
    { return atomic_fetch_xor_explicit(__a, __i, memory_order_seq_cst); }


  // Partial specializations for pointers.
  template<typename _ITp>
    inline _ITp*
    atomic_fetch_add_explicit(atomic<_ITp*>* __a, ptrdiff_t __d,
			      memory_order __m) noexcept
    { return __a->fetch_add(__d, __m); }

  template<typename _ITp>
    inline _ITp*
    atomic_fetch_add_explicit(volatile atomic<_ITp*>* __a, ptrdiff_t __d,
			      memory_order __m) noexcept
    { return __a->fetch_add(__d, __m); }

  template<typename _ITp>
    inline _ITp*
    atomic_fetch_add(volatile atomic<_ITp*>* __a, ptrdiff_t __d) noexcept
    { return __a->fetch_add(__d); }

  template<typename _ITp>
    inline _ITp*
    atomic_fetch_add(atomic<_ITp*>* __a, ptrdiff_t __d) noexcept
    { return __a->fetch_add(__d); }

  template<typename _ITp>
    inline _ITp*
    atomic_fetch_sub_explicit(volatile atomic<_ITp*>* __a,
			      ptrdiff_t __d, memory_order __m) noexcept
    { return __a->fetch_sub(__d, __m); }

  template<typename _ITp>
    inline _ITp*
    atomic_fetch_sub_explicit(atomic<_ITp*>* __a, ptrdiff_t __d,
			      memory_order __m) noexcept
    { return __a->fetch_sub(__d, __m); }

  template<typename _ITp>
    inline _ITp*
    atomic_fetch_sub(volatile atomic<_ITp*>* __a, ptrdiff_t __d) noexcept
    { return __a->fetch_sub(__d); }

  template<typename _ITp>
    inline _ITp*
    atomic_fetch_sub(atomic<_ITp*>* __a, ptrdiff_t __d) noexcept
    { return __a->fetch_sub(__d); }
  // @} group atomics

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif // C++11

#endif // _GLIBCXX_ATOMIC
