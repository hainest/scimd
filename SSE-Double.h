#pragma once

#include "cksimd.h"
#include <type_traits>

struct SSEDouble {
	typedef ck_simd::simd_category<double>::type category;
	typedef ck_simd::simd_type<double>::type simd_t;

	simd_t val;
	SSEDouble()			: val(ck_simd::zero(category())) {}
	SSEDouble(simd_t x)	: val(x) {}
	SSEDouble(double x)	: val(ck_simd::set1(x, category())) {}
	SSEDouble(double f0, double f1,double f2, double f3)
		: val(ck_simd::setr(f0,f1,f2,f3, category())) {}
	operator simd_t() { return val; }

	/* Arithmetic Operators*/
	SSEDouble operator -()			{ return ck_simd::neg(val,			category()); }
	SSEDouble operator +(SSEDouble a) { return ck_simd::add(val, a.val, category()); }
	SSEDouble operator -(SSEDouble a) { return ck_simd::sub(val, a.val, category()); }
	SSEDouble operator *(SSEDouble a) { return ck_simd::mul(val, a.val, category()); }
	SSEDouble operator /(SSEDouble a) { return ck_simd::div(val, a.val, category()); }

	SSEDouble operator +=(SSEDouble a) { val = ck_simd::add(val, a.val, category()); return *this; }
	SSEDouble operator -=(SSEDouble a) { val = ck_simd::sub(val, a.val, category()); return *this; }
	SSEDouble operator *=(SSEDouble a) { val = ck_simd::mul(val, a.val, category()); return *this; }
	SSEDouble operator /=(SSEDouble a) { val = ck_simd::div(val, a.val, category()); return *this; }

	/*Logical Operators*/
	SSEDouble operator &(SSEDouble a) { return ck_simd::bitwise_and (a.val, val, category()); }
	SSEDouble operator |(SSEDouble a) { return ck_simd::bitwise_or  (a.val, val, category()); }
	SSEDouble operator ^(SSEDouble a) { return ck_simd::bitwise_xor (a.val, val, category()); }
	SSEDouble andnot    (SSEDouble a) { return ck_simd::andnot		(a.val, val, category()); }

	/*Comparison Operators*/
	SSEDouble operator < (SSEDouble a) { return ck_simd::less	 (a.val, val, category()); }
	SSEDouble operator > (SSEDouble a) { return ck_simd::greater (a.val, val, category()); }
	SSEDouble operator ==(SSEDouble a) { return ck_simd::equals  (a.val, val, category()); }
};
/**
 * 	These are safe from ODR violations because only one SSE
 * 	type is in flight per translation unit.
 **/

inline int movemask(SSEDouble a) { return ck_simd::movemask(a.val, SSEDouble::category()); }
inline void storeu(double *p, SSEDouble a) { ck_simd::storeu(p, a.val, SSEDouble::category()); }

inline SSEDouble max (SSEDouble a, SSEDouble b) { return ck_simd::max(a.val, b.val, SSEDouble::category()); }
inline SSEDouble min (SSEDouble a, SSEDouble b) { return ck_simd::min(a.val, b.val, SSEDouble::category()); }

inline SSEDouble operator +(double a, SSEDouble b) { return SSEDouble(a) + b; }
inline SSEDouble operator -(double a, SSEDouble b) { return SSEDouble(a) - b; }
inline SSEDouble operator *(double a, SSEDouble b) { return SSEDouble(a) * b; }

SSEDouble operator /(double a, SSEDouble b) { return SSEDouble(a) / b; }

/**
 * 	The converting constructor for in SSEDouble enables this overload in dangerous
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
						std::is_same<T, SSEDouble>::value,
						ck_simd::sqrt_proxy<T>
					>::type
	sqrt(T a) {
		return ck_simd::sqrt_proxy<T>(a);
	}
}
