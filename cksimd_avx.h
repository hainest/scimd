#pragma once

#include <immintrin.h>
#include "cksimd_traits.h"

namespace ck_simd {

	template <> struct simd_category<float>  { typedef avx_float_tag type; };
	template <> struct simd_category<double> { typedef avx_double_tag type; };

	template <> struct simd_type<float>  { typedef __m256  type; };
	template <> struct simd_type<double> { typedef __m256d type; };

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
	inline __m256 sqrt(__m256 x, avx_float_tag) {
		return _mm256_sqrt_ps(x);
	}
	inline __m256d sqrt(__m256d x, avx_double_tag) {
		return _mm256_sqrt_pd(x);
	}
	inline __m256 rsqrt(__m256 x, avx_float_tag) {
		return _mm256_rsqrt_ps(x);
	}
	inline __m256d rsqrt(__m256d x, avx_double_tag) {
		return _mm256_div_pd(_mm256_set1_pd(1.0), _mm256_sqrt_pd(x));
	}
	inline __m256 zero(avx_float_tag) {
		return _mm256_setzero_ps();
	}
	inline __m256 set1(float x, avx_float_tag) {
		return _mm256_set1_ps(x);
	}
	inline __m256d set1(double x, avx_double_tag) {
		return _mm256_set1_pd(x);
	}
	inline __m256 setr(float x, float y, float z, float w, avx_float_tag) {
		return _mm256_setr_ps(x, y, z, w, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	inline __m256d setr(double x, double y, double z, double w, avx_double_tag) {
		return _mm256_setr_pd(x, y, z, w);
	}
	inline __m256 setr(float x, float y, float z, float w,
					float a, float b, float c, float d, avx_float_tag) {
		return _mm256_setr_ps(x, y, z, w, a, b, c, d);
	}
	inline __m256d zero(avx_double_tag) {
		return _mm256_setzero_pd();
	}
	inline __m256 neg(__m256 x, avx_float_tag) {
		return _mm256_sub_ps(_mm256_setzero_ps(), x);
	}
	inline __m256d neg(__m256d x, avx_double_tag) {
		return _mm256_sub_pd(_mm256_setzero_pd(), x);
	}
	inline __m256 add(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_add_ps(x, y);
	}
	inline __m256d add(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_add_pd(x, y);
	}
	inline __m256 sub(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_sub_ps(x, y);
	}
	inline __m256d sub(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_sub_pd(x, y);
	}
	inline __m256 mul(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_mul_ps(x, y);
	}
	inline __m256d mul(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_mul_pd(x, y);
	}
	inline __m256 div(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_div_ps(x, y);
	}
	inline __m256d div(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_div_pd(x, y);
	}
	inline __m256 mask_and(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_and_ps(x, y);
	}
	inline __m256d mask_and(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_and_pd(x, y);
	}
	inline __m256 mask_or(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_or_ps(x, y);
	}
	inline __m256d mask_or(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_or_pd(x, y);
	}
	inline __m256 mask_xor(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_xor_ps(x, y);
	}
	inline __m256d mask_xor(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_xor_pd(x, y);
	}
	inline __m256 mask_andnot(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_andnot_ps(x, y);
	}
	inline __m256d mask_andnot(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_andnot_pd(x, y);
	}
	/**
	 * 	I don't like using signaling NaNs, but the legacy
	 * 	code does, and I don't want to break code relying
	 * 	on this behavior.
	 *
	 * 	Note: Some of the comparisons use quiet NaNs.
	 * 	This inconsistent usage was present in the
	 * 	legacy code.
	 */
	inline __m256 less(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_cmp_ps(x, y, _CMP_LT_OS);
	}
	inline __m256d less(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_cmp_pd(x, y, _CMP_LT_OS);
	}
	inline __m256 greater(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_cmp_ps(x, y, _CMP_GT_OS);
	}
	inline __m256d greater(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_cmp_pd(x, y, _CMP_GT_OS);
	}
	inline __m256 less_eq(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_cmp_ps(x, y, _CMP_LE_OQ);
	}
	inline __m256d less_eq(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_cmp_pd(x, y, _CMP_LE_OQ);
	}
	inline __m256 greater_eq(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_cmp_ps(x, y, _CMP_GE_OS);
	}
	inline __m256d greater_eq(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_cmp_pd(x, y, _CMP_GE_OS);
	}
	inline __m256 equals(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_cmp_ps(x, y, _CMP_EQ_OQ);
	}
	inline __m256d equals(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_cmp_pd(x, y, _CMP_EQ_OQ);
	}
	inline int movemask(__m256 x, avx_float_tag) {
		return _mm256_movemask_ps(x);
	}
	inline int movemask(__m256d x, avx_double_tag) {
		return _mm256_movemask_pd(x);
	}
	inline void storeu(float *p, __m256 x, avx_float_tag) {
		_mm256_storeu_ps(p, x);
	}
	inline void storeu(double *p, __m256d x, avx_double_tag) {
		_mm256_storeu_pd(p, x);
	}
	inline __m256 max(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_max_ps(x, y);
	}
	inline __m256d max(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_max_pd(x, y);
	}
	inline __m256 min(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_min_ps(x, y);
	}
	inline __m256d min(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_min_pd(x, y);
	}
	inline __m256 loadu(float const* p, avx_float_tag) {
		return _mm256_loadu_ps(p);
	}
	inline __m256d loadu(double const* p, avx_double_tag) {
		return _mm256_loadu_pd(p);
	}
};
