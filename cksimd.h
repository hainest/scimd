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
#endif

#include <type_traits>

namespace ck_simd {
	template <typename T>
	struct sqrt_proxy {
		T value;
		explicit sqrt_proxy(T x) : value{x} {}
		operator T() {
			return sqrt(value, typename T::category());
		}
	};
}

template <typename sse_type, typename T>
struct cksimd {
	typedef T value_type;
	typedef typename ck_simd::simd_category<T>::type category;
	typedef typename ck_simd::simd_type<T>::type simd_t;

	simd_t val;
	cksimd() 		 : val(ck_simd::zero(category())) {}
	cksimd(simd_t x) : val(x) {}
	cksimd(T x)		 : val(ck_simd::set1(x, category())) {}

	/* Arithmetic Operators*/
	sse_type operator -()		  { return ck_simd::neg(val,			category()); }
	sse_type operator +(sse_type a) { return ck_simd::add(val, a.val, category()); }
	sse_type operator -(sse_type a) { return ck_simd::sub(val, a.val, category()); }
	sse_type operator *(sse_type a) { return ck_simd::mul(val, a.val, category()); }
	sse_type operator /(sse_type a) { return ck_simd::div(val, a.val, category()); }

	sse_type operator +=(sse_type a) { val = ck_simd::add(val, a.val, category()); return *this; }
	sse_type operator -=(sse_type a) { val = ck_simd::sub(val, a.val, category()); return *this; }
	sse_type operator *=(sse_type a) { val = ck_simd::mul(val, a.val, category()); return *this; }
	sse_type operator /=(sse_type a) { val = ck_simd::div(val, a.val, category()); return *this; }

	/*Logical Operators*/
	sse_type operator &(sse_type a) { return ck_simd::bitwise_and(a.val, val, category()); }
	sse_type operator |(sse_type a) { return ck_simd::bitwise_or (a.val, val, category()); }
	sse_type operator ^(sse_type a) { return ck_simd::bitwise_xor(a.val, val, category()); }
	sse_type andnot    (sse_type a) { return ck_simd::andnot	   (a.val, val, category()); }

	/*Comparison Operators*/
	sse_type operator < (sse_type a) { return ck_simd::less	(a.val, val, category()); }
	sse_type operator > (sse_type a) { return ck_simd::greater(a.val, val, category()); }
	sse_type operator ==(sse_type a) { return ck_simd::equals	(a.val, val, category()); }

	friend sse_type operator/(sse_type lhs, ck_simd::sqrt_proxy<sse_type> rhs) {
		sse_type y0{rsqrt(rhs.value, category())};

		// Do a Newton-Raphson iteration to bring precision to ~23 bits
		// Explicitly construct this to override the built-in operator* in libc++
		const sse_type t{y0 * (3.0 - rhs.value * y0 * y0) * 0.5};
		return lhs * t;
	}
};

/**
 * 	These are safe from ODR violations because only one SSE
 * 	type is in flight per translation unit.
 **/

//inline int movemask(cksimd a) { return _mm_movemask_ps(a.val); }
//inline void storeu(float *p, cksimd a) { _mm_storeu_ps(p,a.val); }
//
//inline cksimd max (cksimd a, cksimd b) { return _mm_max_ps(a.val, b.val); }
//inline cksimd min (cksimd a, cksimd b) { return _mm_min_ps(a.val, b.val); }
//
//inline cksimd operator +(float a, cksimd b) { return cksimd(a) + b; }
//inline cksimd operator -(float a, cksimd b) { return cksimd(a) - b; }
//inline cksimd operator *(float a, cksimd b) { return cksimd(a) * b; }

/**
 *  The TMP here is is just to disambiguate this from operator/ with a sqrt_proxy<cksimd>
 */
template <typename T, typename U, template<class> class sse_type>
inline typename std::enable_if<std::is_same<sse_type<U>, cksimd<T,U>>::value, cksimd<T,U>>::type
operator /(T a, sse_type<U> b) { return sse_type<U>(a) / b; }


