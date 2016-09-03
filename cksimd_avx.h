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
		/**
		 * 	Do one Newton-Raphson iteration to bring the precision to ~23
		 * 	bits (~2e-7). This works equally well on Bulldozer, Piledriver,
		 *	Sandybridge, and Skylake.
		 */
		const __m256 three = _mm256_set1_ps(3.0f), half = _mm256_set1_ps(0.5f);
		const __m256 rsrt = _mm256_rsqrt_ps(x);
		const __m256 muls = _mm256_mul_ps(_mm256_mul_ps(x, rsrt), rsrt);
		return _mm256_mul_ps(_mm256_mul_ps(half, rsrt), _mm256_sub_ps(three, muls));
	}
	inline __m256d rsqrt(__m256d a, avx_double_tag) {
		/**
		* 	This routine is adapted from
		* 	https://github.com/stgatilov/recip_rsqrt_benchmark
		*
		* 	It uses a 5th-order polynomial to bring the error to 51.5 bits (~3e-16).
		*
		* 	The speedup compared to using sqrt+div varies by CPU architecture, but
		* 	is between 10% (Skylake) and 3x (Sandybridge) faster.
		*/
		__m256d one = _mm256_set1_pd(1.0), c1 = _mm256_set1_pd(1.0/2.0),
				c2 = _mm256_set1_pd(3.0/8.0), c3 = _mm256_set1_pd(15.0/48.0),
				c4 = _mm256_set1_pd(105.0/384.0);
		__m256d x = _mm256_cvtps_pd(_mm_rsqrt_ps(_mm256_cvtpd_ps(a)));
		__m256d r = _mm256_sub_pd(one, _mm256_mul_pd(_mm256_mul_pd(a, x), x));
		__m256d r2 = _mm256_mul_pd(r, r);
		__m256d t1 = _mm256_add_pd(_mm256_mul_pd(c2, r), c1);
		__m256d t3 = _mm256_add_pd(_mm256_mul_pd(c4, r), c3);
		__m256d poly = _mm256_add_pd(_mm256_mul_pd(r2, t3), t1);
		return _mm256_add_pd(_mm256_mul_pd(_mm256_mul_pd(x, r), poly), x);
	}
	inline __m256 zero(avx_float_tag) {
		return _mm256_setzero_ps();
	}
	inline __m256d zero(avx_double_tag) {
		return _mm256_setzero_pd();
	}
	inline __m256 set1(float x, avx_float_tag) {
		return _mm256_set1_ps(x);
	}
	inline __m256d set1(double x, avx_double_tag) {
		return _mm256_set1_pd(x);
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
