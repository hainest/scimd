#pragma once

#include <nmmintrin.h>
#include <cstdint>
#include "traits.hpp"
#include "memory.hpp"

namespace scimd {

	struct sse_tag {};

	template<> struct is_sse<sse_tag> : std::true_type {};

	struct simd_category { using type = sse_tag; };

	template <> struct simd_type<float>  { using type = __m128; };
	template <> struct simd_type<double> { using type = __m128d; };

	template <> struct bool_type<float> { using type = __m128; };
	template <> struct bool_type<double> { using type = __m128d; };

	namespace {
		template <typename T>
		struct mask_t {};
		template <> struct mask_t<float> { static const int value = 0xf; };
		template <> struct mask_t<double> { static const int value = 0x3; };
	}

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
	static inline __m128 zero(float, sse_tag) {
		return _mm_setzero_ps();
	}
	static inline __m128d zero(double, sse_tag) {
		return _mm_setzero_pd();
	}
	static inline __m128 set1(float x, float, sse_tag) {
		return _mm_set1_ps(x);
	}
	static inline __m128d set1(double x, double, sse_tag) {
		return _mm_set1_pd(x);
	}
	/*************************************************************************/
	static inline __m128 neg(__m128 x, float, sse_tag) {
		return _mm_sub_ps(_mm_setzero_ps(), x);
	}
	static inline __m128d neg(__m128d x, double, sse_tag) {
		return _mm_sub_pd(_mm_setzero_pd(), x);
	}
	static inline __m128 add(__m128 x, __m128 y, float, sse_tag) {
		return _mm_add_ps(x, y);
	}
	static inline __m128d add(__m128d x, __m128d y, double, sse_tag) {
		return _mm_add_pd(x, y);
	}
	static inline __m128 sub(__m128 x, __m128 y, float, sse_tag) {
		return _mm_sub_ps(x, y);
	}
	static inline __m128d sub(__m128d x, __m128d y, double, sse_tag) {
		return _mm_sub_pd(x, y);
	}
	static inline __m128 mul(__m128 x, __m128 y, float, sse_tag) {
		return _mm_mul_ps(x, y);
	}
	static inline __m128d mul(__m128d x, __m128d y, double, sse_tag) {
		return _mm_mul_pd(x, y);
	}
	static inline __m128 div(__m128 x, __m128 y, float, sse_tag) {
		return _mm_div_ps(x, y);
	}
	static inline __m128d div(__m128d x, __m128d y, double, sse_tag) {
		return _mm_div_pd(x, y);
	}
	/*************************************************************************/
	static inline __m128 max(__m128 x, __m128 y, float, sse_tag) {
		return _mm_max_ps(x, y);
	}
	static inline __m128d max(__m128d x, __m128d y, double, sse_tag) {
		return _mm_max_pd(x, y);
	}
	static inline __m128 min(__m128 x, __m128 y, float, sse_tag) {
		return _mm_min_ps(x, y);
	}
	static inline __m128d min(__m128d x, __m128d y, double, sse_tag) {
		return _mm_min_pd(x, y);
	}
	/*************************************************************************/
	static inline __m128 less(__m128 x, __m128 y, float, sse_tag) {
		return _mm_cmplt_ps(x, y);
	}
	static inline __m128d less(__m128d x, __m128d y, double, sse_tag) {
		return _mm_cmplt_pd(x, y);
	}
	static inline __m128 greater(__m128 x, __m128 y, float, sse_tag) {
		return _mm_cmpgt_ps(x, y);
	}
	static inline __m128d greater(__m128d x, __m128d y, double, sse_tag) {
		return _mm_cmpgt_pd(x, y);
	}
	static inline __m128 less_eq(__m128 x, __m128 y, float, sse_tag) {
		return _mm_cmple_ps(x, y);
	}
	static inline __m128d less_eq(__m128d x, __m128d y, double, sse_tag) {
		return _mm_cmple_pd(x, y);
	}
	static inline __m128 greater_eq(__m128 x, __m128 y, float, sse_tag) {
		return _mm_cmpge_ps(x, y);
	}
	static inline __m128d greater_eq(__m128d x, __m128d y, double, sse_tag) {
		return _mm_cmpge_pd(x, y);
	}
	/*************************************************************************/
	static inline bool logical_all(__m128 x, float, sse_tag) {
		return _mm_movemask_ps(x) == mask_t<float>::value;
	}
	static inline bool logical_all(__m128d x, double, sse_tag) {
		return _mm_movemask_pd(x) == mask_t<double>::value;
	}
	static inline bool logical_none(__m128 x, float, sse_tag) {
		return _mm_movemask_ps(x) == 0;
	}
	static inline bool logical_none(__m128d x, double, sse_tag) {
		return _mm_movemask_pd(x) == 0;
	}
	/*************************************************************************/
	static inline void store(float *p, __m128 x, float, sse_tag, memory::unaligned) {
		_mm_storeu_ps(p, x);
	}
	static inline void store(float *p, __m128 x, float, sse_tag, memory::aligned) {
		_mm_store_ps(p, x);
	}
	static inline void store(double *p, __m128d x, double, sse_tag, memory::unaligned) {
		_mm_storeu_pd(p, x);
	}
	static inline void store(double *p, __m128d x, double, sse_tag, memory::aligned) {
		_mm_store_pd(p, x);
	}
	static inline __m128 load(float const* p, float, sse_tag, memory::unaligned) {
		return _mm_loadu_ps(p);
	}
	static inline __m128 load(float const* p, float, sse_tag, memory::aligned) {
		return _mm_load_ps(p);
	}
	static inline __m128d load(double const* p, double, sse_tag, memory::unaligned) {
		return _mm_loadu_pd(p);
	}
	static inline __m128d load(double const* p, double, sse_tag, memory::aligned) {
		return _mm_load_pd(p);
	}
	static inline __m128 blend(__m128 x, __m128 y, __m128 mask, float, sse_tag) {
		return _mm_blendv_ps(x, y, mask);
	}
	static inline __m128d blend(__m128d x, __m128d y, __m128d mask, double, sse_tag) {
		return _mm_blendv_pd(x, y, mask);
	}
	/*************************************************************************/
	static inline __m128 sqrt(__m128 x, float, sse_tag) {
		return _mm_sqrt_ps(x);
	}
	static inline __m128d sqrt(__m128d x, double, sse_tag) {
		return _mm_sqrt_pd(x);
	}
	static inline __m128 rsqrt(__m128 x, float, sse_tag) {
		/**
		 * 	Do one Newton-Raphson iteration to bring the precision to ~23
		 * 	bits (~2e-7). This works equally well on Bulldozer, Piledriver,
		 *	Sandybridge, and Skylake.
		 */
		const __m128 three = _mm_set1_ps(3.0f), half = _mm_set1_ps(0.5f);
		const __m128 rsrt = _mm_rsqrt_ps(x);
		const __m128 muls = _mm_mul_ps(_mm_mul_ps(x, rsrt), rsrt);
		return _mm_mul_ps(_mm_mul_ps(half, rsrt), _mm_sub_ps(three, muls));
	}
	static inline __m128d rsqrt(__m128d a, double, sse_tag) {
		/**
		* 	This routine is adapted from
		* 	https://github.com/stgatilov/recip_rsqrt_benchmark
		*
		* 	It uses a 5th-order polynomial to bring the error to 51.5 bits (~3e-16).
		*
		* 	The speedup compared to using sqrt+div varies by CPU architecture, but
		* 	is between 10% (Skylake) and 3x (Sandybridge) faster.
		*/
		__m128d one = _mm_set1_pd(1.0), 	c1 = _mm_set1_pd(1.0/2.0),
				c2 = _mm_set1_pd(3.0/8.0), 	c3 = _mm_set1_pd(15.0/48.0),
				c4 = _mm_set1_pd(105.0/384.0);
		__m128d x = _mm_cvtps_pd(_mm_rsqrt_ps(_mm_cvtpd_ps(a)));
		__m128d r = _mm_sub_pd(one, _mm_mul_pd(_mm_mul_pd(a, x), x));
		__m128d r2 = _mm_mul_pd(r, r);
		__m128d t1 = _mm_add_pd(_mm_mul_pd(c2, r), c1);
		__m128d t3 = _mm_add_pd(_mm_mul_pd(c4, r), c3);
		__m128d poly = _mm_add_pd(_mm_mul_pd(r2, t3), t1);
		return _mm_add_pd(_mm_mul_pd(_mm_mul_pd(x, r), poly), x);
	}
};
