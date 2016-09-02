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
	inline float sqrt(float x, scalar_float_tag) {
		return std::sqrt(x);
	}
	inline double sqrt(double x, scalar_double_tag) {
		return std::sqrt(x);
	}
	inline float rsqrt(float x, scalar_float_tag) {
		return 1.0f / std::sqrt(x);
	}
	inline double rsqrt(double x, scalar_double_tag) {
		return 1.0 / std::sqrt(x);
	}
	inline float zero(scalar_float_tag) {
		return 0.0f;
	}
	inline double zero(scalar_double_tag) {
		return 0.0;
	}
	inline float set1(float x, scalar_float_tag) {
		return x;
	}
	inline double set1(double x, scalar_double_tag) {
		return x;
	}
	inline float setr(float x, scalar_float_tag) {
		return x;
	}
	inline double setr(double x, scalar_double_tag) {
		return x;
	}
	inline float neg(float x, scalar_float_tag) {
		return -x;
	}
	inline double neg(double x, scalar_double_tag) {
		return -x;
	}
	inline float add(float x, float y, scalar_float_tag) {
		return x + y;
	}
	inline double add(double x, double y, scalar_double_tag) {
		return x + y;
	}
	inline float sub(float x, float y, scalar_float_tag) {
		return x - y;
	}
	inline double sub(double x, double y, scalar_double_tag) {
		return x - y;
	}
	inline float mul(float x, float y, scalar_float_tag) {
		return x * y;
	}
	inline double mul(double x, double y, scalar_double_tag) {
		return x * y;
	}
	inline float div(float x, float y, scalar_float_tag) {
		return x / y;
	}
	inline double div(double x, double y, scalar_double_tag) {
		return x / y;
	}
	inline float mask_and(float x, float y, scalar_float_tag) {
		return maskf_t(maskf_t(x).i & maskf_t(y).i).f;
	}
	inline double mask_and(double x, double y, scalar_double_tag) {
		return mask_t(mask_t(x).i & mask_t(y).i).d;
	}
	inline float mask_or(float x, float y, scalar_float_tag) {
		return maskf_t(maskf_t(x).i | maskf_t(y).i).f;
	}
	inline double mask_or(double x, double y, scalar_double_tag) {
		return mask_t(mask_t(x).i | mask_t(y).i).d;
	}
	inline float mask_xor(float x, float y, scalar_float_tag) {
		return maskf_t(maskf_t(x).i ^ maskf_t(y).i).f;
	}
	inline double mask_xor(double x, double y, scalar_double_tag) {
		return mask_t(mask_t(x).i ^ mask_t(y).i).d;
	}
	inline float mask_andnot(float x, float y, scalar_float_tag) {
		return maskf_t(~(maskf_t(x).i) & maskf_t(y).i).f;
	}
	inline double mask_andnot(double x, double y, scalar_double_tag) {
		return mask_t(~(mask_t(x).i) & mask_t(y).i).d;
	}
	inline float less(float x, float y, scalar_float_tag) {
		return (x < y) ? maskf_t(true_mask_float).f : 0;
	}
	inline double less(double x, double y, scalar_double_tag) {
		return (x < y) ? mask_t(true_mask_double).d : 0;
	}
	inline float greater(float x, float y, scalar_float_tag) {
		return (x > y) ? maskf_t(true_mask_float).f : 0;
	}
	inline double greater(double x, double y, scalar_double_tag) {
		return (x > y) ? mask_t(true_mask_double).d : 0;
	}
	inline float less_eq(float x, float y, scalar_float_tag) {
		return (x <= y) ? maskf_t(true_mask_float).f : 0;
	}
	inline double less_eq(double x, double y, scalar_double_tag) {
		return (x <= y) ? mask_t(true_mask_double).d : 0;
	}
	inline float greater_eq(float x, float y, scalar_float_tag) {
		return (x >= y) ? maskf_t(true_mask_float).f : 0;
	}
	inline double greater_eq(double x, double y, scalar_double_tag) {
		return (x >= y) ? mask_t(true_mask_double).d : 0;
	}
	inline float equals(float x, float y, scalar_float_tag) {
		return (x == y) ? maskf_t(true_mask_float).f : 0;
	}
	inline double equals(double x, double y, scalar_double_tag) {
		return (x == y) ? mask_t(true_mask_double).d : 0;
	}
	inline int movemask(float x, scalar_float_tag) {
		/**
		 *  Emulate the movmskps instruction.
		 * 	Extract the sign bit from x and format it into a 4-bit mask
		 */
		return (x < 0.0f) ? 0xf : 0;
	}
	inline int movemask(double x, scalar_double_tag) {
		/**
		 *  Emulate the movmskpd instruction.
		 * 	Extract the sign bit from x and format it into a 2-bit mask
		 */
		return (x < 0.0) ? 0x3 : 0;
	}
	inline void storeu(float *p, float x, scalar_float_tag) {
		*p = x;
	}
	inline void storeu(double *p, double x, scalar_double_tag) {
		*p = x;
	}
	inline float max(float x, float y, scalar_float_tag) {
		return std::max(x,y);
	}
	inline double max(double x, double y, scalar_double_tag) {
		return std::max(x,y);
	}
	inline float min(float x, float y, scalar_float_tag) {
		return std::min(x,y);
	}
	inline double min(double x, double y, scalar_double_tag) {
		return std::min(x,y);
	}
	inline float loadu(float const* p, scalar_float_tag) {
		return *p;
	}
	inline double loadu(double const* p, scalar_double_tag) {
		return *p;
	}
};
