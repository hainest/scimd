#pragma once

#include "cksimd.h"
#include <type_traits>

struct SSEFloat {
	typedef float value_type;
	typedef ck_simd::simd_category<value_type>::type category;
	typedef ck_simd::simd_type<value_type>::type simd_t;
	static const size_t size = sizeof(simd_t);

	simd_t val;
	SSEFloat() 		 	: val(ck_simd::zero(category())) {}
	SSEFloat(simd_t x)	: val(x) {}

	explicit operator simd_t() { return val; }

	template <typename U, typename =
			typename std::enable_if<
				  std::is_floating_point<U>::value &&
				 !ck_simd::is_scalar<category()>::value, U>::type>
	SSEFloat(U x) : val(ck_simd::set1(x, category())) {}

	/*
	 * 	Please don't use this constructor as it breaks genericity. It is only
	 *	here for backwards compatibility with the legacy interface.
	 */
//	template <typename U, typename =
//			typename std::enable_if<!ck_simd::is_scalar<category()>::value, U>::type>
//	SSEFloat(U f0, U f1, U f2, U f3)
//		: val(ck_simd::setr(f0,f1,f2,f3, category())) {}

	simd_t operator()() { return val; }
	SSEFloat operator -()			{ return ck_simd::neg(val,		  category()); }
	SSEFloat operator +(SSEFloat x) { return ck_simd::add(val, x.val, category()); }
	SSEFloat operator -(SSEFloat x) { return ck_simd::sub(val, x.val, category()); }
	SSEFloat operator *(SSEFloat x) { return ck_simd::mul(val, x.val, category()); }
	SSEFloat operator /(SSEFloat x) { return ck_simd::div(val, x.val, category()); }

	SSEFloat operator +=(SSEFloat x) { val = ck_simd::add(val, x.val, category()); return *this; }
	SSEFloat operator -=(SSEFloat x) { val = ck_simd::sub(val, x.val, category()); return *this; }
	SSEFloat operator *=(SSEFloat x) { val = ck_simd::mul(val, x.val, category()); return *this; }
	SSEFloat operator /=(SSEFloat x) { val = ck_simd::div(val, x.val, category()); return *this; }

	/*Masking Operators*/
	SSEFloat operator &(SSEFloat x) { return ck_simd::mask_and	  (x.val, val, category()); }
	SSEFloat operator |(SSEFloat x) { return ck_simd::mask_or	  (x.val, val, category()); }
	SSEFloat operator ^(SSEFloat x) { return ck_simd::mask_xor	  (x.val, val, category()); }
	SSEFloat andnot    (SSEFloat x) { return ck_simd::mask_andnot (x.val, val, category()); }

	SSEFloat operator &=(SSEFloat x) { val = ck_simd::mask_and (x.val, val, category()); return *this; }
	SSEFloat operator |=(SSEFloat x) { val = ck_simd::mask_or  (x.val, val, category()); return *this; }
	SSEFloat operator ^=(SSEFloat x) { val = ck_simd::mask_xor (x.val, val, category()); return *this; }

	SSEFloat operator < (SSEFloat x) { return ck_simd::less			(x.val, val, category()); }
	SSEFloat operator > (SSEFloat x) { return ck_simd::greater		(x.val, val, category()); }
	SSEFloat operator ==(SSEFloat x) { return ck_simd::equals		(x.val, val, category()); }
	SSEFloat operator <=(SSEFloat x) { return ck_simd::less_eq		(x.val, val, category()); }
	SSEFloat operator >=(SSEFloat x) { return ck_simd::greater_eq	(x.val, val, category()); }

	friend int		movemask(SSEFloat x)				{ return ck_simd::movemask(x.val, SSEFloat::category()); }
	friend void 	storeu	(value_type *p, SSEFloat x)	{ ck_simd::storeu(p, x.val, SSEFloat::category()); }
	friend SSEFloat loadu	(value_type *p)				{ return ck_simd::loadu(p, SSEFloat::category()); }

	friend SSEFloat max (SSEFloat x, SSEFloat b) { return ck_simd::max(x.val, b.val, SSEFloat::category()); }
	friend SSEFloat min (SSEFloat x, SSEFloat b) { return ck_simd::min(x.val, b.val, SSEFloat::category()); }

	friend SSEFloat operator +(value_type x, SSEFloat b) { return SSEFloat(x) + b; }
	friend SSEFloat operator -(value_type x, SSEFloat b) { return SSEFloat(x) - b; }
	friend SSEFloat operator *(value_type x, SSEFloat b) { return SSEFloat(x) * b; }
};

/**
 *  The TMP here is is just to disambiguate this from operator/ with a sqrt_proxy<SSEFloat>
 */
template <typename T>
inline typename std::enable_if<std::is_same<T, SSEFloat>::value, SSEFloat>::type
operator /(float a, T b) { return T(a) / b; }

/*
 * 	This is really the reciprocal square root function. Using a proxy object
 * 	allows code like `T x(4.0), y(1.0/sqrt(x));` to work correctly for
 * 	all types and use the rsqrt optimization for T=SSEFloat.
 *
 * 	Note: There is no rsqrt intrinsic for double-precision. There isn't even
 * 		  a reciprocal intrinsic, so there is no optimization for DP.
 */
SSEFloat operator/(SSEFloat lhs, ck_simd::sqrt_proxy<SSEFloat> rhs) {
	SSEFloat y0{ck_simd::rsqrt(rhs.value(), SSEFloat::category())};

	// Do a Newton-Raphson iteration to bring precision to ~23 bits
	// Explicitly construct this to override the built-in operator* in libc++
	const SSEFloat t{y0 * (SSEFloat(3.0) - rhs.value * y0 * y0) * SSEFloat(0.5)};
	return lhs * t;
}

/**
 * 	The converting constructor for SSEFloat enables this overload in dangerous
 * 	ways (e.g., when `std::sqrt` isn't visible and T=float). The constructor should be made
 * `explicit`, but that could break existing code. Use TMP to disable this overload.
 *
 *	The libstdc++ cmath uses a `using` declaration to pull in the global `sqrt` names
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
