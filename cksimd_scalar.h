#pragma once

#include <cstdint>
#include <cmath>
#include <algorithm>
#include "cksimd_traits.h"

namespace ck_simd {

	template <> struct simd_category<float>  { typedef scalar_float_tag type; };
	template <> struct simd_category<double> { typedef scalar_double_tag type; };

	template <> struct simd_type<float>  { typedef float  type; };
	template <> struct simd_type<double> { typedef double type; };

	namespace {
		union mask_t  {
			double d;
			uint64_t i;
			mask_t(double d) : d(d) {}
			mask_t(uint64_t i) : i(i) {}
		};
		union maskf_t {
			float f;
			uint32_t i;
			maskf_t(float f) : f(f) {}
			maskf_t(uint32_t i) : i(i) {}
		};

		const uint32_t true_mask_float = 0xffffffff;
		const uint64_t true_mask_double = 0xffffffffffffffff;
	}

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
	static inline float sqrt(float x, scalar_float_tag) {
		return std::sqrt(x);
	}
	static inline double sqrt(double x, scalar_double_tag) {
		return std::sqrt(x);
	}
	static inline float rsqrt(float x, scalar_float_tag) {
		return 1.0f / std::sqrt(x);
	}
	static inline double rsqrt(double x, scalar_double_tag) {
		return 1.0 / std::sqrt(x);
	}
	static inline float zero(scalar_float_tag) {
		return 0.0f;
	}
	static inline double zero(scalar_double_tag) {
		return 0.0;
	}
	static inline float set1(float x, scalar_float_tag) {
		return x;
	}
	static inline double set1(double x, scalar_double_tag) {
		return x;
	}
	static inline float neg(float x, scalar_float_tag) {
		return -x;
	}
	static inline double neg(double x, scalar_double_tag) {
		return -x;
	}
	static inline float add(float x, float y, scalar_float_tag) {
		return x + y;
	}
	static inline double add(double x, double y, scalar_double_tag) {
		return x + y;
	}
	static inline float sub(float x, float y, scalar_float_tag) {
		return x - y;
	}
	static inline double sub(double x, double y, scalar_double_tag) {
		return x - y;
	}
	static inline float mul(float x, float y, scalar_float_tag) {
		return x * y;
	}
	static inline double mul(double x, double y, scalar_double_tag) {
		return x * y;
	}
	static inline float div(float x, float y, scalar_float_tag) {
		return x / y;
	}
	static inline double div(double x, double y, scalar_double_tag) {
		return x / y;
	}
	static inline float mask_and(float x, float y, scalar_float_tag) {
		return maskf_t(maskf_t(x).i & maskf_t(y).i).f;
	}
	static inline double mask_and(double x, double y, scalar_double_tag) {
		return mask_t(mask_t(x).i & mask_t(y).i).d;
	}
	static inline float mask_or(float x, float y, scalar_float_tag) {
		return maskf_t(maskf_t(x).i | maskf_t(y).i).f;
	}
	static inline double mask_or(double x, double y, scalar_double_tag) {
		return mask_t(mask_t(x).i | mask_t(y).i).d;
	}
	static inline float mask_xor(float x, float y, scalar_float_tag) {
		return maskf_t(maskf_t(x).i ^ maskf_t(y).i).f;
	}
	static inline double mask_xor(double x, double y, scalar_double_tag) {
		return mask_t(mask_t(x).i ^ mask_t(y).i).d;
	}
	static inline float mask_andnot(float x, float y, scalar_float_tag) {
		return maskf_t(~(maskf_t(x).i) & maskf_t(y).i).f;
	}
	static inline double mask_andnot(double x, double y, scalar_double_tag) {
		return mask_t(~(mask_t(x).i) & mask_t(y).i).d;
	}
	static inline float less(float x, float y, scalar_float_tag) {
		return (x < y) ? maskf_t(true_mask_float).f : 0;
	}
	static inline double less(double x, double y, scalar_double_tag) {
		return (x < y) ? mask_t(true_mask_double).d : 0;
	}
	static inline float greater(float x, float y, scalar_float_tag) {
		return (x > y) ? maskf_t(true_mask_float).f : 0;
	}
	static inline double greater(double x, double y, scalar_double_tag) {
		return (x > y) ? mask_t(true_mask_double).d : 0;
	}
	static inline float less_eq(float x, float y, scalar_float_tag) {
		return (x <= y) ? maskf_t(true_mask_float).f : 0;
	}
	static inline double less_eq(double x, double y, scalar_double_tag) {
		return (x <= y) ? mask_t(true_mask_double).d : 0;
	}
	static inline float greater_eq(float x, float y, scalar_float_tag) {
		return (x >= y) ? maskf_t(true_mask_float).f : 0;
	}
	static inline double greater_eq(double x, double y, scalar_double_tag) {
		return (x >= y) ? mask_t(true_mask_double).d : 0;
	}
	static inline float equals(float x, float y, scalar_float_tag) {
		return (x == y) ? maskf_t(true_mask_float).f : 0;
	}
	static inline double equals(double x, double y, scalar_double_tag) {
		return (x == y) ? mask_t(true_mask_double).d : 0;
	}
	static inline int movemask(float x, scalar_float_tag) {
		/**
		 *  Emulate the movmskps instruction.
		 * 	Extract the sign bit from x and format it into a 4-bit mask
		 */
		return (x < 0.0f) ? 0xf : 0;
	}
	static inline int movemask(double x, scalar_double_tag) {
		/**
		 *  Emulate the movmskpd instruction.
		 * 	Extract the sign bit from x and format it into a 2-bit mask
		 */
		return (x < 0.0) ? 0x3 : 0;
	}
	static inline void store(float *p, float x, scalar_float_tag, aligned_store_tag) {
		*p = x;
	}
	static inline void store(double *p, double x, scalar_double_tag, aligned_store_tag) {
		*p = x;
	}
	static inline void store(float *p, float x, scalar_float_tag, unaligned_store_tag) {
		*p = x;
	}
	static inline void store(double *p, double x, scalar_double_tag, unaligned_store_tag) {
		*p = x;
	}
	static inline float max(float x, float y, scalar_float_tag) {
		return std::max(x,y);
	}
	static inline double max(double x, double y, scalar_double_tag) {
		return std::max(x,y);
	}
	static inline float min(float x, float y, scalar_float_tag) {
		return std::min(x,y);
	}
	static inline double min(double x, double y, scalar_double_tag) {
		return std::min(x,y);
	}
	static inline float load(float const* p, scalar_float_tag, aligned_load_tag) {
		return *p;
	}
	static inline double load(double const* p, scalar_double_tag, aligned_load_tag) {
		return *p;
	}
	static inline float load(float const* p, scalar_float_tag, unaligned_load_tag) {
		return *p;
	}
	static inline double load(double const* p, scalar_double_tag, unaligned_load_tag) {
		return *p;
	}
};
