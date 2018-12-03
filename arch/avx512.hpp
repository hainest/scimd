#pragma once

#include <immintrin.h>
#include "traits.hpp"
#include <cstdint>

namespace scimd {

	struct avx512_tag {};

	template <> struct is_avx512<avx512_tag> : std::true_type {};

	struct simd_category { using type = avx512_tag; };

	template <> struct simd_type<float>  { using type = __m512; };
	template <> struct simd_type<double> { using type = __m512d; };

	template <> struct bool_type<float> { using type = __mmask16; };
	template <> struct bool_type<double> { using type = __mmask8; };

	namespace {
		template <typename T>
		struct mask_t {};
		template<> struct mask_t<float> { static const int value = 0xffff; };
		template<> struct mask_t<double> { static const int value = 0xff; };
	}

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
	static inline __m512 zero(float, avx512_tag) {
		return _mm512_setzero_ps();
	}
	static inline __m512d zero(double, avx512_tag) {
		return _mm512_setzero_pd();
	}
	static inline __m512 set1(float x, float, avx512_tag) {
		return _mm512_set1_ps(x);
	}
	static inline __m512d set1(double x, double, avx512_tag) {
		return _mm512_set1_pd(x);
	}
	/*************************************************************************/
	static inline __m512 neg(__m512 x, float, avx512_tag) {
		return _mm512_sub_ps(_mm512_setzero_ps(), x);
	}
	static inline __m512d neg(__m512d x, double, avx512_tag) {
		return _mm512_sub_pd(_mm512_setzero_pd(), x);
	}
	static inline __m512 add(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_add_ps(x, y);
	}
	static inline __m512d add(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_add_pd(x, y);
	}
	static inline __m512 sub(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_sub_ps(x, y);
	}
	static inline __m512d sub(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_sub_pd(x, y);
	}
	static inline __m512 mul(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_mul_ps(x, y);
	}
	static inline __m512d mul(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_mul_pd(x, y);
	}
	static inline __m512 div(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_div_ps(x, y);
	}
	static inline __m512d div(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_div_pd(x, y);
	}
	/*************************************************************************/
	static inline __m512 max(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_max_ps(x, y);
	}
	static inline __m512d max(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_max_pd(x, y);
	}
	static inline __m512 min(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_min_ps(x, y);
	}
	static inline __m512d min(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_min_pd(x, y);
	}
	static inline __m512 abs(__m512 x, float, avx512_tag) {
#if (defined(__GNU__) && __GNU__ > 7) || defined(__ICC__)
		return _mm512_abs_ps(x);
#else
		auto const mask = _mm512_set1_epi32(0x7fffffff);
		return (__m512) _mm512_and_epi32 ((__m512i)x,mask);
#endif
	}
	static inline __m512d abs(__m512d x, double, avx512_tag) {
		// _mm512_abs_pd is broken before gcc-9
#if (defined(__GNU__) && __GNUC__ > 9) || defined(__ICC__)
		return _mm512_abs_pd(x);
#else
		auto const mask = _mm512_set1_epi64 (0x7fffffffffffffffLL);
		return (__m512d)_mm512_and_epi64 ((__m512i)x, mask);
#endif
	}
	/*************************************************************************/
	/*
	 * 	__mmask{16|8} _mm512_mask_cmp_p{s|d}_mask (__mmask{16|8} k1, __m512{|d} a,
	 * 								 			   __m512{|d} b, const int imm8)
	 *
	 * 	Compare packed double-precision (64-bit) floating-point elements in 'a'
	 * 	and 'b' based on the comparison operand specified by 'imm8', and store
	 * 	the results in mask vector 'k' using zeromask 'k1' (elements are zeroed
	 * 	out when the corresponding mask bit is not set).
	 */
	static inline __mmask16 less(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_mask_cmp_ps_mask(mask_t<float>::value, x, y, _CMP_LT_OQ);
	}
	static inline __mmask8 less(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_mask_cmp_pd_mask(mask_t<double>::value, x, y, _CMP_LT_OQ);
	}
	static inline __mmask16 greater(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_mask_cmp_ps_mask(mask_t<float>::value, x, y, _CMP_GT_OQ);
	}
	static inline __mmask8 greater(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_mask_cmp_pd_mask(mask_t<double>::value, x, y, _CMP_GT_OQ);
	}
	static inline __mmask16 less_eq(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_mask_cmp_ps_mask(mask_t<float>::value, x, y, _CMP_LE_OQ);
	}
	static inline __mmask8 less_eq(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_mask_cmp_pd_mask(mask_t<double>::value, x, y, _CMP_LE_OQ);
	}
	static inline __mmask16 greater_eq(__m512 x, __m512 y, float, avx512_tag) {
		return _mm512_mask_cmp_ps_mask(mask_t<float>::value, x, y, _CMP_GE_OQ);
	}
	static inline __mmask8 greater_eq(__m512d x, __m512d y, double, avx512_tag) {
		return _mm512_mask_cmp_pd_mask(mask_t<double>::value, x, y, _CMP_GE_OQ);
	}
	/*************************************************************************/
	static inline bool logical_all(__mmask16 x, float, avx512_tag) {
		return _mm512_kand(mask_t<float>::value, x) == mask_t<float>::value;
	}
	static inline bool logical_all(__mmask8 x, double, avx512_tag) {
		return _mm512_kand(x, mask_t<double>::value) == mask_t<double>::value;
	}
	static inline bool logical_none(__mmask16 x, float, avx512_tag) {
		return _mm512_kand(mask_t<float>::value, x) == 0;
	}
	static inline bool logical_none(__mmask8 x, double, avx512_tag) {
		return _mm512_kand(mask_t<double>::value, x) == 0;
	}
	/*************************************************************************/
	static inline void store(float *p, __m512 x, float, avx512_tag) {
		_mm512_storeu_ps(p, x);
	}
	static inline void store(double *p, __m512d x, double, avx512_tag) {
		_mm512_storeu_pd(p, x);
	}
	static inline __m512 load(float const* p, float, avx512_tag) {
		return _mm512_loadu_ps(p);
	}
	static inline __m512d load(double const* p, double, avx512_tag) {
		return _mm512_loadu_pd(p);
	}
	static inline __m512 blend(__m512 x, __m512 y, __mmask16 mask, float, avx512_tag) {
		return _mm512_mask_blend_ps(mask, x, y);
	}
	static inline __m512d blend(__m512d x, __m512d y, __mmask8 mask, double, avx512_tag) {
		return _mm512_mask_blend_pd(mask, x, y);
	}
	/*************************************************************************/
	static inline __m512 sqrt(__m512 x, float, avx512_tag) {
		return _mm512_sqrt_ps(x);
	}
	static inline __m512d sqrt(__m512d x, double, avx512_tag) {
		return _mm512_sqrt_pd(x);
	}
	static inline __m512 rsqrt(__m512 x, float, avx512_tag) {
		/**
		 * 	Do one Newton-Raphson iteration to bring the precision to ~23 bits (~2e-7).
		 */
		const __m512 three = _mm512_set1_ps(3.0f), half = _mm512_set1_ps(0.5f);
		const __m512 rsrt = _mm512_rsqrt14_ps(x);
		const __m512 muls = _mm512_mul_ps(_mm512_mul_ps(x, rsrt), rsrt);
		return _mm512_mul_ps(_mm512_mul_ps(half, rsrt), _mm512_sub_ps(three, muls));
	}
	static inline __m512d rsqrt(__m512d a, double, avx512_tag) {
		/**
		* 	This routine is adapted from
		* 	https://github.com/stgatilov/recip_rsqrt_benchmark
		*
		* 	It uses a 5th-order polynomial to bring the error to 51.5 bits (~3e-16).
		*/
		const __m512d one = _mm512_set1_pd(1.0),		c1 = _mm512_set1_pd(1.0/2.0),
					  c2  = _mm512_set1_pd(3.0/8.0),	c3 = _mm512_set1_pd(15.0/48.0),
					  c4  = _mm512_set1_pd(105.0/384.0);
//		__m512d x = _mm512_cvtps_pd(_mm_rsqrt_ps(_mm512_cvtpd_ps(a)));
		const __m512d x = _mm512_rsqrt14_pd(a);
		const __m512d r = _mm512_sub_pd(one, _mm512_mul_pd(_mm512_mul_pd(a, x), x));
		const __m512d r2 = _mm512_mul_pd(r, r);
		const __m512d t1 = _mm512_add_pd(_mm512_mul_pd(c2, r), c1);
		const __m512d t3 = _mm512_add_pd(_mm512_mul_pd(c4, r), c3);
		const __m512d poly = _mm512_add_pd(_mm512_mul_pd(r2, t3), t1);
		return _mm512_add_pd(_mm512_mul_pd(_mm512_mul_pd(x, r), poly), x);
	}

};
