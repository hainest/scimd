#pragma once

#include <immintrin.h>
#include <cstdint>
#include "traits.hpp"
#include "memory.hpp"

namespace scimd {

	struct avx_tag {};

	template <> struct is_avx<avx_tag> : std::true_type {};

	struct simd_category { using type = avx_tag; };

	template <> struct simd_type<float>  { using type = __m256; };
	template <> struct simd_type<double> { using type = __m256d; };

	template <> struct bool_type<float> { using type = __m256; };
	template <> struct bool_type<double> { using type = __m256d; };

	namespace {
		template <typename T>
		struct mask_t {};
		template<> struct mask_t<float> { static const int value = 0xff; };
		template<> struct mask_t<double> { static const int value = 0xf; };
	}

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
	static inline __m256 zero(float, avx_tag) {
		return _mm256_setzero_ps();
	}
	static inline __m256d zero(double, avx_tag) {
		return _mm256_setzero_pd();
	}
	static inline __m256 set1(float x, float, avx_tag) {
		return _mm256_set1_ps(x);
	}
	static inline __m256d set1(double x, double, avx_tag) {
		return _mm256_set1_pd(x);
	}
	/*************************************************************************/
	static inline __m256 neg(__m256 x, float, avx_tag) {
		return _mm256_sub_ps(_mm256_setzero_ps(), x);
	}
	static inline __m256d neg(__m256d x, double, avx_tag) {
		return _mm256_sub_pd(_mm256_setzero_pd(), x);
	}
	static inline __m256 add(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_add_ps(x, y);
	}
	static inline __m256d add(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_add_pd(x, y);
	}
	static inline __m256 sub(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_sub_ps(x, y);
	}
	static inline __m256d sub(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_sub_pd(x, y);
	}
	static inline __m256 mul(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_mul_ps(x, y);
	}
	static inline __m256d mul(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_mul_pd(x, y);
	}
	static inline __m256 div(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_div_ps(x, y);
	}
	static inline __m256d div(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_div_pd(x, y);
	}
	/*************************************************************************/
	static inline __m256 max(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_max_ps(x, y);
	}
	static inline __m256d max(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_max_pd(x, y);
	}
	static inline __m256 min(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_min_ps(x, y);
	}
	static inline __m256d min(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_min_pd(x, y);
	}
	/*************************************************************************/
	static inline __m256 less(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_cmp_ps(x, y, _CMP_LT_OQ);
	}
	static inline __m256d less(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_cmp_pd(x, y, _CMP_LT_OQ);
	}
	static inline __m256 greater(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_cmp_ps(x, y, _CMP_GT_OQ);
	}
	static inline __m256d greater(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_cmp_pd(x, y, _CMP_GT_OQ);
	}
	static inline __m256 less_eq(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_cmp_ps(x, y, _CMP_LE_OQ);
	}
	static inline __m256d less_eq(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_cmp_pd(x, y, _CMP_LE_OQ);
	}
	static inline __m256 greater_eq(__m256 x, __m256 y, float, avx_tag) {
		return _mm256_cmp_ps(x, y, _CMP_GE_OQ);
	}
	static inline __m256d greater_eq(__m256d x, __m256d y, double, avx_tag) {
		return _mm256_cmp_pd(x, y, _CMP_GE_OQ);
	}
	/*************************************************************************/
	static inline bool logical_all(__m256 x, float, avx_tag) {
		return _mm256_movemask_ps(x) == mask_t<float>::value;
	}
	static inline bool logical_all(__m256d x, double, avx_tag) {
		return _mm256_movemask_pd(x) == mask_t<double>::value;
	}
	static inline bool logical_none(__m256 x, float, avx_tag) {
		return _mm256_movemask_ps(x) == 0;
	}
	static inline bool logical_none(__m256d x, double, avx_tag) {
		return _mm256_movemask_pd(x) == 0;
	}
	/*************************************************************************/
	static inline void store(float *p, __m256 x, float, avx_tag, memory::unaligned) {
		_mm256_storeu_ps(p, x);
	}
	static inline void store(float *p, __m256 x, float, avx_tag, memory::aligned) {
		_mm256_store_ps(p, x);
	}
	static inline void store(double *p, __m256d x, double, avx_tag, memory::unaligned) {
		_mm256_storeu_pd(p, x);
	}
	static inline void store(double *p, __m256d x, double, avx_tag, memory::aligned) {
		_mm256_store_pd(p, x);
	}
	static inline __m256 load(float const* p, float, avx_tag, memory::unaligned) {
		return _mm256_loadu_ps(p);
	}
	static inline __m256 load(float const* p, float, avx_tag, memory::aligned) {
		return _mm256_load_ps(p);
	}
	static inline __m256d load(double const* p, double, avx_tag, memory::unaligned) {
		return _mm256_loadu_pd(p);
	}
	static inline __m256d load(double const* p, double, avx_tag, memory::aligned) {
		return _mm256_load_pd(p);
	}
	static inline __m256 blend(__m256 x, __m256 y, __m256 mask, float, avx_tag) {
		return _mm256_blendv_ps(x, y, mask);
	}
	static inline __m256d blend(__m256d x, __m256d y, __m256d mask, double, avx_tag) {
		return _mm256_blendv_pd(x, y, mask);
	}
	/*************************************************************************/
	static inline __m256 sqrt(__m256 x, float, avx_tag) {
		return _mm256_sqrt_ps(x);
	}
	static inline __m256d sqrt(__m256d x, double, avx_tag) {
		return _mm256_sqrt_pd(x);
	}
	static inline __m256 rsqrt(__m256 x, float, avx_tag) {
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
	static inline __m256d rsqrt(__m256d a, double, avx_tag) {
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

};
