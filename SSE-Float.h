#pragma once

#include "cksimd.h"
#include <type_traits>

struct SSEFloat {
	typedef ck_simd::simd_category<float>::type category;
	typedef ck_simd::simd_type<float>::type simd_t;

	simd_t val;
	SSEFloat() 		 	: val(ck_simd::zero(category())) {}
	SSEFloat(simd_t x)	: val(x) {}
	SSEFloat(float x)	: val(ck_simd::set1(x, category())) {}
	SSEFloat(float f0, float f1,float f2, float f3)
		: val(ck_simd::setr(f0,f1,f2,f3, category())) {}
	operator simd_t() { return val; }

	/* Arithmetic Operators*/
	SSEFloat operator -()			{ return ck_simd::neg(val,		  category()); }
	SSEFloat operator +(SSEFloat a) { return ck_simd::add(val, a.val, category()); }
	SSEFloat operator -(SSEFloat a) { return ck_simd::sub(val, a.val, category()); }
	SSEFloat operator *(SSEFloat a) { return ck_simd::mul(val, a.val, category()); }
	SSEFloat operator /(SSEFloat a) { return ck_simd::div(val, a.val, category()); }

	SSEFloat operator +=(SSEFloat a) { val = ck_simd::add(val, a.val, category()); return *this; }
	SSEFloat operator -=(SSEFloat a) { val = ck_simd::sub(val, a.val, category()); return *this; }
	SSEFloat operator *=(SSEFloat a) { val = ck_simd::mul(val, a.val, category()); return *this; }
	SSEFloat operator /=(SSEFloat a) { val = ck_simd::div(val, a.val, category()); return *this; }

	/*Logical Operators*/
	SSEFloat operator &(SSEFloat a) { return ck_simd::bitwise_and(a.val, val, category()); }
	SSEFloat operator |(SSEFloat a) { return ck_simd::bitwise_or (a.val, val, category()); }
	SSEFloat operator ^(SSEFloat a) { return ck_simd::bitwise_xor(a.val, val, category()); }
	SSEFloat andnot    (SSEFloat a) { return ck_simd::andnot	 (a.val, val, category()); }

	/*Comparison Operators*/
	SSEFloat operator < (SSEFloat a) { return ck_simd::less		(a.val, val, category()); }
	SSEFloat operator > (SSEFloat a) { return ck_simd::greater	(a.val, val, category()); }
	SSEFloat operator ==(SSEFloat a) { return ck_simd::equals	(a.val, val, category()); }
};
/**
 * 	These are safe from ODR violations because only one SSE
 * 	type is in flight per translation unit.
 **/

inline int movemask(SSEFloat a) { return _mm_movemask_ps(a.val); }
inline void storeu(float *p, SSEFloat a) { _mm_storeu_ps(p,a.val); }

inline SSEFloat max (SSEFloat a, SSEFloat b) { return _mm_max_ps(a.val, b.val); }
inline SSEFloat min (SSEFloat a, SSEFloat b) { return _mm_min_ps(a.val, b.val); }

inline SSEFloat operator +(float a, SSEFloat b) { return SSEFloat(a) + b; }
inline SSEFloat operator -(float a, SSEFloat b) { return SSEFloat(a) - b; }
inline SSEFloat operator *(float a, SSEFloat b) { return SSEFloat(a) * b; }

/**
 *  The TMP here is is just to disambiguate this from operator/ with a sqrt_proxy<SSEFloat>
 */
template <typename T>
inline typename std::enable_if<std::is_same<T, SSEFloat>::value, SSEFloat>::type
operator /(float a, T b) { return T(a) / b; }


SSEFloat operator/(SSEFloat lhs, ck_simd::sqrt_proxy<SSEFloat> rhs) {
	SSEFloat y0{ck_simd::rsqrt(rhs.value, SSEFloat::category())};

	// Do a Newton-Raphson iteration to bring precision to ~23 bits
	// Explicitly construct this to override the built-in operator* in libc++
	const SSEFloat t{y0 * (3.0 - rhs.value * y0 * y0) * 0.5};
	return lhs * t;
}

/**
 * 	The converting constructor for in cksimd_t enables this overload in dangerous
 * 	ways. The constructor should be made `explicit`, but that could	break
 *	existing code. Use TMP to disable this overload.
 *
 *	In cmath, libstdc++ uses a `using` declaration to pull in the global `sqrt` symbol
 *	which picks up this overload and conflicts with the template declaration
 *	there. Placing this version in an anaonymous namespace removes it from
 *	the global namespace.
 */
namespace {
	template <typename T>
	inline typename std::enable_if<
						std::is_same<T, SSEFloat>::value,
						ck_simd::sqrt_proxy<T>
					>::type
	sqrt(T a) {
		return ck_simd::sqrt_proxy<T>(a);
	}
}
