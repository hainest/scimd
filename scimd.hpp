#pragma once

/**
 *  These are mutually exclusive by design as switching between
 *  SIMD modes is expensive on even modern CPUs. If you need to use
 *  more than one instruction set, put them in separate translation
 *  units.
 */
#if defined(__AVX512F__) && defined(__AVX512ER__) && defined(__AVX512PF__) && defined(__AVX512CD__) && !defined(SCIMD_DISABLE_AVX512)
	#include "arch/avx512.hpp"
#elif defined(__AVX__) && !defined(SCIMD_DISABLE_AVX)
	#include "arch/avx.hpp"
#elif defined(__SSE4_2__) && !defined(SCIMD_DISABLE_SSE)
	#include "arch/sse.hpp"
#else
	#include "arch/scalar.hpp"
#endif

#include "arch/traits.hpp"

namespace scimd {

	template <typename T>
	struct sqrt_proxy {
		T value;
		explicit sqrt_proxy(T x) : value{x} {}
		operator T() {
			return sqrt(value.val, typename T::category());
		}
	};

	template <typename T>
	struct conditional_t {
		typename T::bool_t val;
		conditional_t(typename T::bool_t val) : val(val) {}

		conditional_t operator &&(conditional_t x) const { return logical_and (val, x.val, typename T::category()); }
		conditional_t operator ||(conditional_t x) const { return logical_or  (val, x.val, typename T::category()); }
		conditional_t operator  ^(conditional_t x) const { return logical_xor (val, x.val, typename T::category()); }
		conditional_t operator  !() 			   const { return logical_not (val, typename T::category()); }
	};

	template <typename T>
	bool all(conditional_t<T> x) { return logical_all(x.val, typename T::category()); }
	template <typename T>
	bool none(conditional_t<T> x) { return logical_none(x.val, typename T::category()); }
	template <typename T>
	bool any(conditional_t<T> x) { return !none(x); }

	bool all(bool x) { return x; }
	bool none(bool x) { return !x; }
	bool any(bool x) { return !none(x); }

	template <typename T>
	struct cksimd {
		using value_type = T;
		using category = typename simd_category<value_type>::type;
		using simd_t = typename simd_type<value_type>::type;
		static constexpr auto size = sizeof(simd_t) / sizeof(value_type);
		using bool_t = typename bool_type<value_type>::type;

		simd_t val;

		cksimd() 		 : val(zero(category())) {}
		cksimd(simd_t x) : val(x) {}

		template <typename U, typename =
				typename std::enable_if<
					  std::is_floating_point<U>::value &&
					 !detail::is_scalar<category>::value, U>::type>
		cksimd(U x) : val(set1(x, category())) {}

		cksimd operator -()			const { return neg(val,        category()); }
		cksimd operator +(cksimd x) const { return add(val, x.val, category()); }
		cksimd operator -(cksimd x) const { return sub(val, x.val, category()); }
		cksimd operator *(cksimd x) const { return mul(val, x.val, category()); }
		cksimd operator /(cksimd x) const { return div(val, x.val, category()); }

		cksimd operator +=(cksimd x) { val = add(val, x.val, category()); return *this; }
		cksimd operator -=(cksimd x) { val = sub(val, x.val, category()); return *this; }
		cksimd operator *=(cksimd x) { val = mul(val, x.val, category()); return *this; }
		cksimd operator /=(cksimd x) { val = div(val, x.val, category()); return *this; }

		friend cksimd operator +(value_type x, cksimd b) { return cksimd(x) + b; }
		friend cksimd operator -(value_type x, cksimd b) { return cksimd(x) - b; }
		friend cksimd operator *(value_type x, cksimd b) { return cksimd(x) * b; }

		// See the respective definitions of rsqrt for the relative errors.
		friend cksimd rsqrt(cksimd x) { return rsqrt(x.val, category()); }

		/*
		 * 	This allows code like `T x(4.0), y(2.0/sqrt(x));` to work correctly for
		 * 	all types and uses the rsqrt optimization for T=cksimd.
		 *
		 * 	If you just need 1.0 / sqrt(x), use rsqrt directly to avoid the extra mul.
		 */
		friend cksimd operator/(cksimd lhs, sqrt_proxy<cksimd> rhs) {
			return lhs * rsqrt(rhs.value);
		}

		// The TMP here is is just to disambiguate this from operator/ with a sqrt_proxy<cksimd<U>>
		template <typename U>
		friend typename std::enable_if<std::is_floating_point<U>::value, cksimd<U>>::type
		operator /(value_type a, cksimd<U> b) { return cksimd<U>(a) / b; }

		conditional_t<cksimd> operator < (cksimd x) const { return less	   (val, x.val, category()); }
		conditional_t<cksimd> operator > (cksimd x) const { return greater    (val, x.val, category()); }
		conditional_t<cksimd> operator <=(cksimd x) const { return less_eq    (val, x.val, category()); }
		conditional_t<cksimd> operator >=(cksimd x) const { return greater_eq (val, x.val, category()); }

		friend conditional_t<cksimd> operator < (value_type x, cksimd y) { return cksimd{x}  < y; }
		friend conditional_t<cksimd> operator > (value_type x, cksimd y) { return cksimd{x}  > y; }
		friend conditional_t<cksimd> operator <=(value_type x, cksimd y) { return cksimd{x} <= y; }
		friend conditional_t<cksimd> operator >=(value_type x, cksimd y) { return cksimd{x} >= y; }

		value_type      * store(value_type      * p) { ::scimd::store(p, val, category()); return p; }
		value_type const* load (value_type const* p) { val = ::scimd::load(p, category()); return p; }

		cksimd blend(cksimd x, conditional_t<cksimd> mask) { val = ::scimd::blend(val, x.val, mask.val, category()); return *this; }

		template <typename FwdIter, typename UnaryFunc>
		FwdIter load(FwdIter beg, FwdIter end, UnaryFunc f, value_type default_val = value_type{}) {
			value_type arr[size];
			size_t i = 0;
			for(; i < size && beg != end; i++) {
				arr[i] = f(*beg);
				++beg;
			}
			for(; i < size; i++) {
				arr[i] = default_val;
			}
			val = ::scimd::load(arr, category{});
			return beg;
		}

		template <typename FwdIter, typename BinaryFunc>
		typename std::enable_if<detail::is_scalar<category>::value, FwdIter>::type
		store(FwdIter beg, FwdIter end, BinaryFunc f) {
			f(*beg, val);
			++beg;
			(void)end;
			return beg;
		}

		template <typename FwdIter, typename BinaryFunc>
		typename std::enable_if<!detail::is_scalar<category>::value, FwdIter>::type
		store(FwdIter beg, FwdIter end, BinaryFunc f) {
			for(size_t i = 0; i < size && beg != end; i++) {
				f(*beg, val[i]);
				++beg;
			}
			return beg;
		}
	};

	template <typename T>
	cksimd<T> max(cksimd<T> x, cksimd<T> b) {
		return ::scimd::max(x.val, b.val, typename cksimd<T>::category());
	}
	template <typename T>
	cksimd<T> min(cksimd<T> x, cksimd<T> b) {
		return ::scimd::min(x.val, b.val, typename cksimd<T>::category());
	}
}

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
						scimd::sqrt_proxy<scimd::cksimd<T>>
					>::type
	sqrt(scimd::cksimd<T> a) {
		return scimd::sqrt_proxy<scimd::cksimd<T>>(a);
	}

	template <typename T>
	inline typename std::enable_if<
						std::is_floating_point<T>::value,
						scimd::cksimd<T>
					>::type
	rsqrt(T x) { return rsqrt(scimd::cksimd<T>{x}); }
}
