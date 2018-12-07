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
#include "memory.hpp"

namespace scimd {

	template <typename T>
	struct conditional_t {
		typename T::bool_t val;
		conditional_t(typename T::bool_t val) : val(val) {}
	};

	template <typename T>
	struct pack {
		using value_type = T;
		using category = simd_category::type;
		using simd_t = typename simd_type<value_type>::type;
		static constexpr auto size = sizeof(simd_t) / sizeof(value_type);
		using bool_t = typename bool_type<value_type>::type;

		simd_t val;

		explicit pack() : val(zero(T{}, category{})) {}
		pack(simd_t x) : val(x) {}

		template <typename U, typename =
				typename std::enable_if<
					  std::is_floating_point<U>::value &&
					 !is_scalar<category>::value, U>::type>
		pack(U x) : val(set1(static_cast<T>(x), T{}, category{})) {}

		~pack() noexcept {}

		pack operator -()		const { return neg(val, T{},        category{}); }
		pack operator +(pack x) const { return add(val, x.val, T{}, category{}); }
		pack operator -(pack x) const { return sub(val, x.val, T{}, category{}); }
		pack operator *(pack x) const { return mul(val, x.val, T{}, category{}); }
		pack operator /(pack x) const { return div(val, x.val, T{}, category{}); }

		pack operator +=(pack x) { val = add(val, x.val, T{}, category{}); return *this; }
		pack operator -=(pack x) { val = sub(val, x.val, T{}, category{}); return *this; }
		pack operator *=(pack x) { val = mul(val, x.val, T{}, category{}); return *this; }
		pack operator /=(pack x) { val = div(val, x.val, T{}, category{}); return *this; }

		conditional_t<pack> operator < (pack x) const { return less       (val, x.val, T{}, category{}); }
		conditional_t<pack> operator > (pack x) const { return greater    (val, x.val, T{}, category{}); }
		conditional_t<pack> operator <=(pack x) const { return less_eq    (val, x.val, T{}, category{}); }
		conditional_t<pack> operator >=(pack x) const { return greater_eq (val, x.val, T{}, category{}); }

		pack blend(pack x, conditional_t<pack> mask) {
			val = ::scimd::blend(val, x.val, mask.val, T{}, category{});
			return *this;
		}

		/* ----------------------------------------------------------
		 * 			Load methods
		 *---------------------------------------------------------*/
	private:
		template <typename align_t>
		value_type const* load(value_type const* p, align_t) {
			val = ::scimd::load(p, T{}, category{}, align_t{});
			return p + size;
		}
	public:
		/**
		 * \brief Directly load a pack from memory
		 *
		 * \warn These assume the memory is compact!
		 */
		value_type const* load(                   value_type const* p) { return this->load(p, memory::unaligned{}); }
		value_type const* load(memory::unaligned, value_type const* p) { return this->load(p, memory::unaligned{}); }
		value_type const* load(memory::aligned,   value_type const* p) { return this->load(p, memory::aligned{}); }

		/**
		 * \brief Load a pack from memory using the supplied function
		 *
		 * This is primarily intended for AoS->SoA conversions as it has very high overhead compared to `load(T*)`.
		 */
		template <typename FwdIter, typename UnaryFunc>
		FwdIter load(memory::ragged, FwdIter beg, FwdIter end, UnaryFunc f, value_type default_val) {
			alignas(pack) value_type arr[size];
			size_t i = 0;
			for(; i < size && beg != end; i++) {
				arr[i] = f(*beg);
				++beg;
			}
			for(; i < size; i++) {
				arr[i] = default_val;
			}
			val = ::scimd::load(arr, T{}, category{}, memory::aligned{});
			std::advance(beg, size);
			return beg;
		}
		template <typename FwdIter, typename UnaryFunc>
		FwdIter load(memory::compact, FwdIter beg, FwdIter end, UnaryFunc f, value_type) {
			alignas(pack) value_type arr[size];
			for(size_t i = 0; i < size; i++) {
				arr[i] = f(*beg);
				++beg;
			}
			(void)end;
			val = ::scimd::load(arr, T{}, category{}, memory::aligned{});
			std::advance(beg, size);
			return beg;
		}
		template <typename FwdIter, typename UnaryFunc>
		FwdIter load(FwdIter beg, FwdIter end, UnaryFunc f, value_type default_val = value_type{}) {
			return this->load(memory::ragged{}, beg, end, f, default_val);
		}

		/* ----------------------------------------------------------
		 * 			Store methods
		 *---------------------------------------------------------*/
	private:
		template <typename align_t>
		value_type* store(value_type* p, align_t) {
			::scimd::store(p, val, T{}, category{}, align_t{});
			return p + size;
		}
	public:
		value_type* store(                   value_type* p) { return this->store(p, memory::unaligned{}); }
		value_type* store(memory::unaligned, value_type* p) { return this->store(p, memory::unaligned{}); }
		value_type* store(memory::aligned,   value_type* p) { return this->store(p, memory::aligned{}); }

		/* --- Scalar versions --- */
		template <typename FwdIter, typename BinaryFunc>
		typename std::enable_if<is_scalar<category>::value, FwdIter>::type
		store(FwdIter beg, FwdIter end, BinaryFunc f) {
			f(*beg, val);
			++beg;
			(void)end;
			std::advance(beg, size);
			return beg;
		}
		template <typename FwdIter, typename BinaryFunc>
		typename std::enable_if<is_scalar<category>::value, FwdIter>::type
		store(memory::ragged, FwdIter beg, FwdIter end, BinaryFunc f) {
			return this->store(beg, end, f);
		}
		template <typename FwdIter, typename BinaryFunc>
		typename std::enable_if<is_scalar<category>::value, FwdIter>::type
		store(memory::compact, FwdIter beg, FwdIter end, BinaryFunc f) {
			return this->store(beg, end, f);
		}

		/* --- Vector versions --- */
		template <typename FwdIter, typename BinaryFunc>
		typename std::enable_if<!is_scalar<category>::value, FwdIter>::type
		store(memory::ragged, FwdIter beg, FwdIter end, BinaryFunc f) {
			for(size_t i = 0; i < size && beg != end; i++) {
				f(*beg, val[i]);
				++beg;
			}
			return beg;
		}
		template <typename FwdIter, typename BinaryFunc>
		typename std::enable_if<!is_scalar<category>::value, FwdIter>::type
		store(memory::compact, FwdIter beg, FwdIter end, BinaryFunc f) {
			for(size_t i = 0; i < size; i++) {
				f(*beg, val[i]);
				++beg;
			}
			(void)end;
			return beg;
		}
		template <typename FwdIter, typename BinaryFunc>
		typename std::enable_if<!is_scalar<category>::value, FwdIter>::type
		store(FwdIter beg, FwdIter end, BinaryFunc f) {
			return this->store(memory::ragged{}, beg, end, f);
		}
	};
}

/* ----------------------------------------------------------
 * 			Range Functions
 *---------------------------------------------------------*/
template <typename T>
inline scimd::pack<T> max(scimd::pack<T> x, scimd::pack<T> b) {
	return scimd::max(x.val, b.val, typename scimd::pack<T>::value_type{}, typename scimd::pack<T>::category{});
}
template <typename T>
inline scimd::pack<T> min(scimd::pack<T> x, scimd::pack<T> b) {
	return scimd::min(x.val, b.val, typename scimd::pack<T>::value_type{}, typename scimd::pack<T>::category{});
}
template <typename T>
typename std::enable_if<!scimd::is_avx512<T>::value, scimd::pack<T>>::type
inline abs(scimd::pack<T> x) {
	auto const mask = (x < static_cast<T>(0.0));
	x.blend(-x, mask);
	return x;
}
template <typename T>
typename std::enable_if<scimd::is_avx512<T>::value, scimd::pack<T>>::type
inline abs(scimd::pack<T> x) {
	return abs(x, typename scimd::pack<T>::value_type{}, typename scimd::pack<T>::category{});
}

/* ----------------------------------------------------------
 * 			Logical Functions
 *---------------------------------------------------------*/
template <typename T>
inline bool all(scimd::conditional_t<T> x) { return scimd::logical_all(x.val, typename T::value_type{}, typename T::category{}); }
template <typename T>
inline bool none(scimd::conditional_t<T> x) { return scimd::logical_none(x.val, typename T::value_type{}, typename T::category{}); }
template <typename T>
inline bool any(scimd::conditional_t<T> x) { return !none(x); }

/* ----------------------------------------------------------
 * 			Binary Arithmetic Operators
 *---------------------------------------------------------*/
template <typename T>
scimd::pack<T> operator +(typename scimd::pack<T>::value_type x, scimd::pack<T> b) { return scimd::pack<T>(x) + b; }
template <typename T>
scimd::pack<T> operator -(typename scimd::pack<T>::value_type x, scimd::pack<T> b) { return scimd::pack<T>(x) - b; }
template <typename T>
scimd::pack<T> operator *(typename scimd::pack<T>::value_type x, scimd::pack<T> b) { return scimd::pack<T>(x) * b; }

/* ----------------------------------------------------------
 * 			scalar+pack Comparison Operators
 *---------------------------------------------------------*/
template <typename T>
scimd::conditional_t<scimd::pack<T>> operator < (typename scimd::pack<T>::value_type x, scimd::pack<T> y) { return scimd::pack<T>{x}  < y; }
template <typename T>
scimd::conditional_t<scimd::pack<T>> operator > (typename scimd::pack<T>::value_type x, scimd::pack<T> y) { return scimd::pack<T>{x}  > y; }
template <typename T>
scimd::conditional_t<scimd::pack<T>> operator <=(typename scimd::pack<T>::value_type x, scimd::pack<T> y) { return scimd::pack<T>{x} <= y; }
template <typename T>
scimd::conditional_t<scimd::pack<T>> operator >=(typename scimd::pack<T>::value_type x, scimd::pack<T> y) { return scimd::pack<T>{x} >= y; }

/* ----------------------------------------------------------
 * 			Square-root Helpers
 *---------------------------------------------------------*/
template <typename T>
struct sqrt_proxy {
	T value;
	explicit sqrt_proxy(T x) : value{x} {}
	operator T() {
		return scimd::sqrt(value.val, typename T::value_type{}, typename T::category{});
	}
};

// See the respective definitions of rsqrt for the relative errors.
template <typename T>
scimd::pack<T> rsqrt(scimd::pack<T> x) { return rsqrt(x.val, T{}, typename scimd::pack<T>::category{}); }

/*
 * 	This allows code like `T x(4.0), y(2.0/sqrt(x));` to work correctly for
 * 	all types and uses the rsqrt optimization for T=pack.
 *
 * 	If you just need 1.0 / sqrt(x), use rsqrt directly to avoid the extra mul.
 */
template <typename T>
scimd::pack<T> operator/(scimd::pack<T> lhs, sqrt_proxy<scimd::pack<T>> rhs) { return lhs * rsqrt(rhs.value); }
template <typename T>
scimd::pack<T> operator/(typename scimd::pack<T>::value_type a, scimd::pack<T> b) { return scimd::pack<T>(a) / b; }
template <typename T>
scimd::pack<T> operator/(typename scimd::pack<T>::value_type a, sqrt_proxy<scimd::pack<T>> rhs) { return a * rsqrt(rhs.value); }

/**
 * 	The converting constructor for pack<T> enables this overload in dangerous
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
						sqrt_proxy<scimd::pack<T>>
					>::type
	sqrt(scimd::pack<T> a) {
		return sqrt_proxy<scimd::pack<T>>(a);
	}

	template <typename T>
	inline typename std::enable_if<
						std::is_floating_point<T>::value,
						scimd::pack<T>
					>::type
	rsqrt(T x) { return ::rsqrt(scimd::pack<T>{x}); }
}
