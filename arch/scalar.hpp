#pragma once

#include <cstdint>
#include <cmath>
#include <algorithm>
#include "traits.hpp"
#include "memory.hpp"

namespace scimd {

	struct scalar_tag {};

	template <> struct is_scalar<scalar_tag> : std::true_type{};

	struct simd_category { using type = scalar_tag; };

	template <> struct simd_type<float>  { using type = float; };
	template <> struct simd_type<double> { using type = double; };

	template <> struct bool_type<float> { using type = bool; };
	template <> struct bool_type<double> { using type = bool; };

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
	static inline float zero(float, scalar_tag) {
		return 0.0f;
	}
	static inline double zero(double, scalar_tag) {
		return 0.0;
	}
	static inline float set1(float x, float, scalar_tag) {
		return x;
	}
	static inline double set1(double x, double, scalar_tag) {
		return x;
	}
	/*************************************************************************/
	static inline float neg(float x, float, scalar_tag) {
		return -x;
	}
	static inline double neg(double x, double, scalar_tag) {
		return -x;
	}
	static inline float add(float x, float y, float, scalar_tag) {
		return x + y;
	}
	static inline double add(double x, double y, double, scalar_tag) {
		return x + y;
	}
	static inline float sub(float x, float y, float, scalar_tag) {
		return x - y;
	}
	static inline double sub(double x, double y, double, scalar_tag) {
		return x - y;
	}
	static inline float mul(float x, float y, float, scalar_tag) {
		return x * y;
	}
	static inline double mul(double x, double y, double, scalar_tag) {
		return x * y;
	}
	static inline float div(float x, float y, float, scalar_tag) {
		return x / y;
	}
	static inline double div(double x, double y, double, scalar_tag) {
		return x / y;
	}
	/*************************************************************************/
	static inline float max(float x, float y, float, scalar_tag) {
		return std::max(x,y);
	}
	static inline double max(double x, double y, double, scalar_tag) {
		return std::max(x,y);
	}
	static inline float min(float x, float y, float, scalar_tag) {
		return std::min(x,y);
	}
	static inline double min(double x, double y, double, scalar_tag) {
		return std::min(x,y);
	}
	/*************************************************************************/
	static inline bool less(float x, float y, float, scalar_tag) {
		return x < y;
	}
	static inline bool less(double x, double y, double, scalar_tag) {
		return x < y;
	}
	static inline bool greater(float x, float y, float, scalar_tag) {
		return x > y;
	}
	static inline bool greater(double x, double y, double, scalar_tag) {
		return x > y;
	}
	static inline bool less_eq(float x, float y, float, scalar_tag) {
		return x <= y;
	}
	static inline bool less_eq(double x, double y, double, scalar_tag) {
		return x <= y;
	}
	static inline bool greater_eq(float x, float y, float, scalar_tag) {
		return x >= y;
	}
	static inline bool greater_eq(double x, double y, double, scalar_tag) {
		return x >= y;
	}
	/*************************************************************************/
	static inline bool logical_all(bool x, float, scalar_tag) {
		return x;
	}
	static inline bool logical_all(bool x, double, scalar_tag) {
		return x;
	}
	static inline bool logical_none(bool x, float, scalar_tag) {
		return !x;
	}
	static inline bool logical_none(bool x, double, scalar_tag) {
		return !x;
	}
	/*************************************************************************/
	static inline void store(float *p, float x, float, scalar_tag, memory::aligned) {
		*p = x;
	}
	static inline void store(float *p, float x, float, scalar_tag, memory::unaligned) {
		*p = x;
	}
	static inline void store(double *p, double x, double, scalar_tag, memory::aligned) {
		*p = x;
	}
	static inline void store(double *p, double x, double, scalar_tag, memory::unaligned) {
		*p = x;
	}
	static inline float load(float const* p, float, scalar_tag, memory::unaligned) {
		return *p;
	}
	static inline float load(float const* p, float, scalar_tag, memory::aligned) {
		return *p;
	}
	static inline double load(double const* p, double, scalar_tag, memory::unaligned) {
		return *p;
	}
	static inline double load(double const* p, double, scalar_tag, memory::aligned) {
		return *p;
	}
	static inline float blend(float x, float y, bool mask, float, scalar_tag) {
		return (mask) ? y : x;
	}
	static inline double blend(double x, double y, bool mask, double, scalar_tag) {
		return (mask) ? y : x;
	}
	/*************************************************************************/
	static inline float sqrt(float x, float, scalar_tag) {
		return std::sqrt(x);
	}
	static inline double sqrt(double x, double, scalar_tag) {
		return std::sqrt(x);
	}
	static inline float rsqrt(float x, float, scalar_tag) {
		return 1.0f / std::sqrt(x);
	}
	static inline double rsqrt(double x, double, scalar_tag) {
		return 1.0 / std::sqrt(x);
	}
};
