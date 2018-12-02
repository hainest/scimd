#pragma once

#include <cstdint>
#include <cmath>
#include <algorithm>
#include "traits.hpp"

namespace scimd {

	struct scalar_float_tag {};
	struct scalar_double_tag {};

	namespace detail {
		template <>
		struct is_scalar<scalar_float_tag> : std::true_type{};
		template <>
		struct is_scalar<scalar_double_tag> : std::true_type{};
	}

	template <> struct simd_category<float>  { using type = scalar_float_tag; };
	template <> struct simd_category<double> { using type = scalar_double_tag; };

	template <> struct simd_type<float>  { using type = float; };
	template <> struct simd_type<double> { using type = double; };

	template <> struct bool_type<float> { using type = bool; };
	template <> struct bool_type<double> { using type = bool; };

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
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
	/*************************************************************************/
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
	/*************************************************************************/
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
	/*************************************************************************/
	static inline bool less(float x, float y, scalar_float_tag) {
		return x < y;
	}
	static inline bool less(double x, double y, scalar_double_tag) {
		return x < y;
	}
	static inline bool greater(float x, float y, scalar_float_tag) {
		return x > y;
	}
	static inline bool greater(double x, double y, scalar_double_tag) {
		return x > y;
	}
	static inline bool less_eq(float x, float y, scalar_float_tag) {
		return x <= y;
	}
	static inline bool less_eq(double x, double y, scalar_double_tag) {
		return x <= y;
	}
	static inline bool greater_eq(float x, float y, scalar_float_tag) {
		return x >= y;
	}
	static inline bool greater_eq(double x, double y, scalar_double_tag) {
		return x >= y;
	}
	/*************************************************************************/
	static inline bool logical_all(bool x, scalar_float_tag) {
		return x;
	}
	static inline bool logical_all(bool x, scalar_double_tag) {
		return x;
	}
	static inline bool logical_none(bool x, scalar_float_tag) {
		return !x;
	}
	static inline bool logical_none(bool x, scalar_double_tag) {
		return !x;
	}
	/*************************************************************************/
	static inline void store(float *p, float x, scalar_float_tag) {
		*p = x;
	}
	static inline void store(double *p, double x, scalar_double_tag) {
		*p = x;
	}
	static inline float load(float const* p, scalar_float_tag) {
		return *p;
	}
	static inline double load(double const* p, scalar_double_tag) {
		return *p;
	}
	static inline float blend(float x, float y, bool mask, scalar_float_tag) {
		return (mask) ? x : y;
	}
	static inline double blend(double x, double y, bool mask, scalar_double_tag) {
		return (mask) ? x : y;
	}
	/*************************************************************************/
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
};
