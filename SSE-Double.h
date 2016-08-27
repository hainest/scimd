#pragma once

#include "cksimd.h"
#include <type_traits>

struct SSEDouble {
	typedef double value_type;
	typedef ck_simd::simd_category<value_type>::type category;
	typedef ck_simd::simd_type<value_type>::type simd_t;
	static const size_t size = sizeof(simd_t);

	simd_t val;
	SSEDouble() 		: val(ck_simd::zero(category())) {}
	SSEDouble(simd_t x)	: val(x) {}

	explicit operator simd_t() { return val; }

	template <typename U, typename =
			typename std::enable_if<
				  std::is_floating_point<U>::value &&
				 !ck_simd::is_scalar<category()>::value, U>::type>
	SSEDouble(U x) : val(ck_simd::set1(x, category())) {}

	/*
	 * 	Please don't use this constructor as it breaks genericity. It is only
	 *	here for backwards compatibility with the legacy interface.
	 */
//	template <typename U, typename =
//			typename std::enable_if<!ck_simd::is_scalar<category()>::value, U>::type>
//	SSEDouble(U f0, U f1, U f2, U f3)
//		: val(ck_simd::setr(f0,f1,f2,f3, category())) {}

	simd_t operator()() { return val; }
	SSEDouble operator -()			{ return ck_simd::neg(val,		  category()); }
	SSEDouble operator +(SSEDouble x) { return ck_simd::add(val, x.val, category()); }
	SSEDouble operator -(SSEDouble x) { return ck_simd::sub(val, x.val, category()); }
	SSEDouble operator *(SSEDouble x) { return ck_simd::mul(val, x.val, category()); }
	SSEDouble operator /(SSEDouble x) { return ck_simd::div(val, x.val, category()); }

	SSEDouble operator +=(SSEDouble x) { val = ck_simd::add(val, x.val, category()); return *this; }
	SSEDouble operator -=(SSEDouble x) { val = ck_simd::sub(val, x.val, category()); return *this; }
	SSEDouble operator *=(SSEDouble x) { val = ck_simd::mul(val, x.val, category()); return *this; }
	SSEDouble operator /=(SSEDouble x) { val = ck_simd::div(val, x.val, category()); return *this; }

	/*Masking Operators*/
	SSEDouble operator &(SSEDouble x) { return ck_simd::mask_and	  (x.val, val, category()); }
	SSEDouble operator |(SSEDouble x) { return ck_simd::mask_or	  (x.val, val, category()); }
	SSEDouble operator ^(SSEDouble x) { return ck_simd::mask_xor	  (x.val, val, category()); }
	SSEDouble andnot    (SSEDouble x) { return ck_simd::mask_andnot (x.val, val, category()); }

	SSEDouble operator &=(SSEDouble x) { val = ck_simd::mask_and (x.val, val, category()); return *this; }
	SSEDouble operator |=(SSEDouble x) { val = ck_simd::mask_or  (x.val, val, category()); return *this; }
	SSEDouble operator ^=(SSEDouble x) { val = ck_simd::mask_xor (x.val, val, category()); return *this; }

	SSEDouble operator < (SSEDouble x) { return ck_simd::less			(x.val, val, category()); }
	SSEDouble operator > (SSEDouble x) { return ck_simd::greater		(x.val, val, category()); }
	SSEDouble operator ==(SSEDouble x) { return ck_simd::equals		(x.val, val, category()); }
	SSEDouble operator <=(SSEDouble x) { return ck_simd::less_eq		(x.val, val, category()); }
	SSEDouble operator >=(SSEDouble x) { return ck_simd::greater_eq	(x.val, val, category()); }

	friend int		movemask(SSEDouble x)				{ return ck_simd::movemask(x.val, SSEDouble::category()); }
	friend void 	storeu	(value_type *p, SSEDouble x)	{ ck_simd::storeu(p, x.val, SSEDouble::category()); }
	friend SSEDouble loadu	(value_type *p)				{ return ck_simd::loadu(p, SSEDouble::category()); }

	friend SSEDouble max (SSEDouble x, SSEDouble b) { return ck_simd::max(x.val, b.val, SSEDouble::category()); }
	friend SSEDouble min (SSEDouble x, SSEDouble b) { return ck_simd::min(x.val, b.val, SSEDouble::category()); }

	friend SSEDouble operator +(value_type x, SSEDouble b) { return SSEDouble(x) + b; }
	friend SSEDouble operator -(value_type x, SSEDouble b) { return SSEDouble(x) - b; }
	friend SSEDouble operator *(value_type x, SSEDouble b) { return SSEDouble(x) * b; }
};

/**
 *  The TMP here is is just to disambiguate this from operator/ with a sqrt_proxy<SSEDouble>
 */
template <typename T>
inline typename std::enable_if<std::is_same<T, SSEDouble>::value, SSEDouble>::type
operator /(float a, T b) { return T(a) / b; }


SSEDouble operator/(SSEDouble lhs, ck_simd::sqrt_proxy<SSEDouble> rhs) {
	return lhs * ck_simd::rsqrt(rhs.value(), SSEDouble::category());
}

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
