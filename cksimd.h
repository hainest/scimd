#pragma once

/**
 *  These are mutually exclusive by design as switching between
 *  SIMD modes is expensive on even modern CPUs. If you need to use
 *  more than one instruction set, put them in separate translation
 *  units.
 */
#if defined(__AVX__) && defined(CMK_USE_AVX)
#include "cksimd_avx.h"
#elif defined(__SSE2__) && defined(CMK_USE_SSE2)
#include "cksimd_sse.h"
#else
#include "cksimd_scalar.h"
#endif

#include <type_traits>
#include "cksimd_traits.h"

namespace ck_simd {

	template <typename T>
	struct sqrt_proxy {
		T value;
		explicit sqrt_proxy(T x) : value{x} {}
		operator T() {
			return ck_simd::sqrt(value.val, typename T::category());
		}
	};

}

template <typename T,
		  typename load_alignment  = ck_simd::unaligned_load_tag,
		  typename store_alignment = ck_simd::unaligned_store_tag>
struct cksimd {
	typedef T value_type;
	typedef typename ck_simd::simd_category<value_type>::type category;
	typedef typename ck_simd::simd_type<value_type>::type simd_t;
	static const size_t size = sizeof(simd_t);
	static const size_t nelem = size / sizeof(value_type);

	simd_t val;
	cksimd() 		 : val(ck_simd::zero(category())) {}
	cksimd(simd_t x) : val(x) {}

	template <typename U, typename =
			typename std::enable_if<
				  std::is_floating_point<U>::value &&
				 !ck_simd::is_scalar<category()>::value, U>::type>
	cksimd(U x) : val(ck_simd::set1(x, category())) {}

	cksimd operator -()			{ return ck_simd::neg(val,		  category()); }
	cksimd operator +(cksimd x) { return ck_simd::add(val, x.val, category()); }
	cksimd operator -(cksimd x) { return ck_simd::sub(val, x.val, category()); }
	cksimd operator *(cksimd x) { return ck_simd::mul(val, x.val, category()); }
	cksimd operator /(cksimd x) { return ck_simd::div(val, x.val, category()); }

	cksimd operator +=(cksimd x) { val = ck_simd::add(val, x.val, category()); return *this; }
	cksimd operator -=(cksimd x) { val = ck_simd::sub(val, x.val, category()); return *this; }
	cksimd operator *=(cksimd x) { val = ck_simd::mul(val, x.val, category()); return *this; }
	cksimd operator /=(cksimd x) { val = ck_simd::div(val, x.val, category()); return *this; }

	/*Masking Operators*/
	cksimd operator &(cksimd x) { return ck_simd::mask_and	  (val, x.val, category()); }
	cksimd operator |(cksimd x) { return ck_simd::mask_or	  (val, x.val, category()); }
	cksimd operator ^(cksimd x) { return ck_simd::mask_xor	  (val, x.val, category()); }
	cksimd andnot    (cksimd x) { return ck_simd::mask_andnot (val, x.val, category()); }

	cksimd operator &=(cksimd x) { val = ck_simd::mask_and (val, x.val, category()); return *this; }
	cksimd operator |=(cksimd x) { val = ck_simd::mask_or  (val, x.val, category()); return *this; }
	cksimd operator ^=(cksimd x) { val = ck_simd::mask_xor (val, x.val, category()); return *this; }

	cksimd operator < (cksimd x) { return ck_simd::less		  (val, x.val, category()); }
	cksimd operator > (cksimd x) { return ck_simd::greater	  (val, x.val, category()); }
	cksimd operator ==(cksimd x) { return ck_simd::equals	  (val, x.val, category()); }
	cksimd operator <=(cksimd x) { return ck_simd::less_eq	  (val, x.val, category()); }
	cksimd operator >=(cksimd x) { return ck_simd::greater_eq (val, x.val, category()); }

	friend void   store(value_type *p, cksimd x){ ck_simd::store(p, x.val, cksimd::category(), store_alignment()); }
	friend cksimd load (value_type *p)			{ return ck_simd::load(p, cksimd::category(), load_alignment()); }

	friend void storel(value_type *p, cksimd x) { _mm_storel_pd(p, x.val);}
	friend void storeh(value_type *p, cksimd x) { _mm_storeh_pd(p, x.val);}


	friend int    movemask(cksimd x)	 { return ck_simd::movemask(x.val, cksimd::category()); }

	friend cksimd max (cksimd x, cksimd b) { return ck_simd::max(x.val, b.val, cksimd::category()); }
	friend cksimd min (cksimd x, cksimd b) { return ck_simd::min(x.val, b.val, cksimd::category()); }

	friend cksimd operator +(value_type x, cksimd b) { return cksimd(x) + b; }
	friend cksimd operator -(value_type x, cksimd b) { return cksimd(x) - b; }
	friend cksimd operator *(value_type x, cksimd b) { return cksimd(x) * b; }

	friend cksimd operator < (value_type x, cksimd y) { return cksimd{x}  < y; }
	friend cksimd operator > (value_type x, cksimd y) { return cksimd{x}  > y; }
	friend cksimd operator ==(value_type x, cksimd y) { return cksimd{x} == y; }
	friend cksimd operator <=(value_type x, cksimd y) { return cksimd{x} <= y; }
	friend cksimd operator >=(value_type x, cksimd y) { return cksimd{x} >= y; }

	/*
	 * 	This is really the reciprocal square root function. Using a proxy object
	 * 	allows code like `T x(4.0), y(1.0/sqrt(x));` to work correctly for
	 * 	all types and use the rsqrt optimization for T=cksimd.
	 *
	 * 	NOTE: This is **NOT** IEEE754 compliant. See the respective definitions of
	 * 		  ck_simd::rsqrt to see the relative errors.
	 *
	 */
	friend cksimd operator/(cksimd lhs, ck_simd::sqrt_proxy<cksimd> rhs) {
		return lhs * ck_simd::rsqrt(rhs.value.val, cksimd::category());
	}

	/**
	 *  The TMP here is is just to disambiguate this from operator/ with a sqrt_proxy<cksimd<U>>
	 */
	template <typename U>
	friend typename std::enable_if<std::is_floating_point<U>::value, cksimd<U>>::type
	operator /(value_type a, cksimd<U> b) { return cksimd<U>(a) / b; }

};

/**
 * 	The converting constructor for cksimd<T> enables this overload in dangerous
 * 	ways (e.g., when `std::sqrt` isn't visible and T=float). Use TMP to disable
 *	this overload.
 *
 *	The libstdc++ cmath uses a `using` declaration to pull in the global `sqrt` names
 *	which picks up this overload and conflicts with the template declaration
 *	there. Placing this version in an anonymous namespace removes it from
 *	the overload set.
 */
namespace {
	template <typename T>
	inline typename std::enable_if<
						std::is_floating_point<T>::value,
						ck_simd::sqrt_proxy<cksimd<T>>
					>::type
	sqrt(cksimd<T> a) {
		return ck_simd::sqrt_proxy<cksimd<T>>(a);
	}
}
