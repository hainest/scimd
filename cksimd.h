#pragma once

/**
 *  These are mutually exclusive by design as switching between
 *  SIMD modes is expensive on even modern CPUs. If you need to use
 *  more than one instruction set, put them in separate translation
 *  units.
 */
#if defined(__AVX512F__) && defined(__AVX512ER__) && defined(__AVX512PF__) && defined(__AVX512CD__) && !defined(CMK_SIMD_DISABLE_KNL)
#include "cksimd_knl.h"
#elif defined(__AVX__) && !defined(CMK_SIMD_DISABLE_AVX)
#include "cksimd_avx.h"
#elif defined(__SSE4_2__) && !defined(CMK_SIMD_DISABLE_SSE)
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

template <typename T>
struct conditional_t {
	typename T::bool_t val;
	conditional_t(typename T::bool_t val) : val(val) {}

	conditional_t operator &&(conditional_t x) const { return ck_simd::logical_and (val, x.val, typename T::category()); }
	conditional_t operator ||(conditional_t x) const { return ck_simd::logical_or  (val, x.val, typename T::category()); }
	conditional_t operator  ^(conditional_t x) const { return ck_simd::logical_xor (val, x.val, typename T::category()); }
	conditional_t operator  !() 			   const { return ck_simd::logical_not (val, typename T::category()); }

	friend bool all(conditional_t x) { return ck_simd::logical_all(x.val, typename T::category()); }
	friend bool none(conditional_t x) { return ck_simd::logical_none(x.val, typename T::category()); }
	friend bool any(conditional_t x) { return !none(x); }
};

bool all(bool x) { return x; }
bool none(bool x) { return !x; }
bool any(bool x) { return !none(x); }

template <typename T>
struct cksimd {
	using value_type = T;
	using category = typename ck_simd::simd_category<value_type>::type;
	using simd_t = typename ck_simd::simd_type<value_type>::type;
	static constexpr auto size = sizeof(simd_t) / sizeof(value_type);
	using bool_t = typename ck_simd::bool_type<value_type>::type;

	simd_t val;

	cksimd() 		 : val(ck_simd::zero(category())) {}
	cksimd(simd_t x) : val(x) {}

	template <typename U, typename =
			typename std::enable_if<
				  std::is_floating_point<U>::value &&
				 !ck_simd::is_scalar<category()>::value, U>::type>
	cksimd(U x) : val(ck_simd::set1(x, category())) {}

	cksimd operator -()			const { return ck_simd::neg(val, 		category()); }
	cksimd operator +(cksimd x) const { return ck_simd::add(val, x.val, category()); }
	cksimd operator -(cksimd x) const { return ck_simd::sub(val, x.val, category()); }
	cksimd operator *(cksimd x) const { return ck_simd::mul(val, x.val, category()); }
	cksimd operator /(cksimd x) const { return ck_simd::div(val, x.val, category()); }

	cksimd operator +=(cksimd x) { val = ck_simd::add(val, x.val, category()); return *this; }
	cksimd operator -=(cksimd x) { val = ck_simd::sub(val, x.val, category()); return *this; }
	cksimd operator *=(cksimd x) { val = ck_simd::mul(val, x.val, category()); return *this; }
	cksimd operator /=(cksimd x) { val = ck_simd::div(val, x.val, category()); return *this; }

	friend cksimd operator +(value_type x, cksimd b) { return cksimd(x) + b; }
	friend cksimd operator -(value_type x, cksimd b) { return cksimd(x) - b; }
	friend cksimd operator *(value_type x, cksimd b) { return cksimd(x) * b; }

	// See the respective definitions of ck_simd::rsqrt for the relative errors.
	friend cksimd rsqrt(cksimd x) { return ck_simd::rsqrt(x.val, category()); }

	/*
	 * 	This allows code like `T x(4.0), y(2.0/sqrt(x));` to work correctly for
	 * 	all types and uses the rsqrt optimization for T=cksimd.
	 *
	 * 	If you just need 1.0 / sqrt(x), use rsqrt directly to avoid the extra mul.
	 */
	friend cksimd operator/(cksimd lhs, ck_simd::sqrt_proxy<cksimd> rhs) {
		return lhs * rsqrt(rhs.value);
	}

	// The TMP here is is just to disambiguate this from operator/ with a sqrt_proxy<cksimd<U>>
	template <typename U>
	friend typename std::enable_if<std::is_floating_point<U>::value, cksimd<U>>::type
	operator /(value_type a, cksimd<U> b) { return cksimd<U>(a) / b; }

	conditional_t<cksimd> operator < (cksimd x) const { return ck_simd::less	   (val, x.val, category()); }
	conditional_t<cksimd> operator > (cksimd x) const { return ck_simd::greater    (val, x.val, category()); }
	conditional_t<cksimd> operator <=(cksimd x) const { return ck_simd::less_eq    (val, x.val, category()); }
	conditional_t<cksimd> operator >=(cksimd x) const { return ck_simd::greater_eq (val, x.val, category()); }

	friend conditional_t<cksimd> operator < (value_type x, cksimd y) { return cksimd{x}  < y; }
	friend conditional_t<cksimd> operator > (value_type x, cksimd y) { return cksimd{x}  > y; }
	friend conditional_t<cksimd> operator <=(value_type x, cksimd y) { return cksimd{x} <= y; }
	friend conditional_t<cksimd> operator >=(value_type x, cksimd y) { return cksimd{x} >= y; }

	friend cksimd max (cksimd x, cksimd b) { return ck_simd::max(x.val, b.val, category()); }
	friend cksimd min (cksimd x, cksimd b) { return ck_simd::min(x.val, b.val, category()); }

	void store(value_type *p) { ck_simd::store(p, val, category()); }
	void load (value_type *p) { val = ck_simd::load(p, category()); }

	cksimd blend(cksimd x, conditional_t<cksimd> mask) { val = ck_simd::blend(val, x.val, mask.val, category()); return *this; }

	template <typename FwdIter, typename UnaryFunc>
	FwdIter pack(FwdIter beg, FwdIter end, UnaryFunc f, value_type default_val = value_type{}) {
		value_type arr[size];
		size_t i = 0;
		for(; i < size && beg != end; i++) {
			arr[i] = f(*beg);
			++beg;
		}
		for(; i < size; i++) {
			arr[i] = default_val;
		}
		val = ck_simd::load(arr, category());
		return beg;
	}

	template <typename FwdIter, typename BinaryFunc>
	typename std::enable_if<ck_simd::is_scalar<category()>::value, FwdIter>::type
	unpack(FwdIter beg, FwdIter end, BinaryFunc f) {
		f(*beg, val);
		++beg;
		(void)end;
		return beg;
	}

	template <typename FwdIter, typename BinaryFunc>
	typename std::enable_if<!ck_simd::is_scalar<category()>::value, FwdIter>::type
	unpack(FwdIter beg, FwdIter end, BinaryFunc f) {
		for(size_t i = 0; i < size && beg != end; i++) {
			f(*beg, val[i]);
			++beg;
		}
		return beg;
	}

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

	template <typename T>
	inline typename std::enable_if<
						std::is_floating_point<T>::value,
						cksimd<T>
					>::type
	rsqrt(T x) { return rsqrt(cksimd<T>{x}); }
}

