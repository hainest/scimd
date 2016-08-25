#pragma once

#include <cmath>

namespace ck_simd {

	template <typename T>
	struct simd_category {};

	template <typename T>
	struct simd_type {};

	struct scalar_float_tag {};
	struct scalar_double_tag {};

	template <> struct simd_category<float>  { typedef scalar_float_tag type; };
	template <> struct simd_category<double> { typedef scalar_double_tag type; };

	template <> struct simd_type<float>  { typedef float  type; };
	template <> struct simd_type<double> { typedef double type; };

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
	inline float set1(float x, scalar_float_tag) {
		return x;
	}
	inline double set1(double x, scalar_double_tag) {
		return x;
	}
	inline double zero(scalar_double_tag) {
		return 0.0;
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
	inline float bitwise_and(float x, float y, scalar_float_tag) {
		return x & y; // TODO: fix me
	}
	inline double bitwise_and(double x, double y, scalar_double_tag) {
		return x & y; // TODO: fix me
	}
	inline float bitwise_or(float x, float y, scalar_float_tag) {
		return x & y; // TODO: fix me
	}
	inline double bitwise_or(double x, double y, scalar_double_tag) {
		return x & y; // TODO: fix me
	}
	inline float bitwise_xor(float x, float y, scalar_float_tag) {
		return x & y; // TODO: fix me
	}
	inline double bitwise_xor(double x, double y, scalar_double_tag) {
		return x & y; // TODO: fix me
	}
	inline float andnot(float x, float y, scalar_float_tag) {
		return x & y; // TODO: fix me
	}
	inline double andnot(double x, double y, scalar_double_tag) {
		return x & y; // TODO: fix me
	}
	inline float less(float x, float y, scalar_float_tag) {
		return x < y;
	}
	inline double less(double x, double y, scalar_double_tag) {
		return x < y;
	}
	inline float greater(float x, float y, scalar_float_tag) {
		return x > y;
	}
	inline double greater(double x, double y, scalar_double_tag) {
		return x > y;
	}
	inline float less_eq(float x, float y, scalar_float_tag) {
		return x <= y;
	}
	inline double less_eq(double x, double y, scalar_double_tag) {
		return x <= y;
	}
	inline float greater_eq(float x, float y, scalar_float_tag) {
		return x >= y;
	}
	inline double greater_eq(double x, double y, scalar_double_tag) {
		return x >= y;
	}
	inline float equals(float x, float y, scalar_float_tag) {
		return x == y;
	}
	inline double equals(double x, double y, scalar_double_tag) {
		return x == y;
	}
	inline int movemask(float x, scalar_float_tag) {
		return 1; // TODO: Fix me
	}
	inline int movemask(double x, scalar_double_tag) {
		return 1; // TODO: Fix me
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
};
