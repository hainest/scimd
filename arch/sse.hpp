#pragma once

#include <nmmintrin.h>
#include "../traits.hpp"
#include <cstdint>

namespace ck_simd {

	template <> struct simd_category<float>  { using type = sse_float_tag; };
	template <> struct simd_category<double> { using type = sse_double_tag; };

	template <> struct simd_type<float>  { using type = __m128; };
	template <> struct simd_type<double> { using type = __m128d; };

	template <> struct bool_type<float> { using type = __m128; };
	template <> struct bool_type<double> { using type = __m128d; };

	namespace {
		union punf {
			float f; uint32_t i;
			punf(uint32_t i) : i{i} {}
		};
		union pund {
			double d; uint64_t i;
			pund(uint64_t i) : i{i} {}
		};

		template <typename T>
		struct true_type {};
		template<> struct true_type<sse_float_tag> {
			operator __m128() { return _mm_set1_ps(punf(0xffffffff).f); }
		};
		template<> struct true_type<sse_double_tag> {
			operator __m128d() { return _mm_set1_pd(pund(0xffffffffffffffff).d); }
		};

		template <typename T>
		struct mask_t {};
		template <> struct mask_t<sse_float_tag> { static const int value = 0xf; };
		template <> struct mask_t<sse_double_tag> { static const int value = 0x3; };
	}

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
	static inline __m128 zero(sse_float_tag) {
		return _mm_setzero_ps();
	}
	static inline __m128d zero(sse_double_tag) {
		return _mm_setzero_pd();
	}
	static inline __m128 set1(float x, sse_float_tag) {
		return _mm_set1_ps(x);
	}
	static inline __m128d set1(double x, sse_double_tag) {
		return _mm_set1_pd(x);
	}
	/*************************************************************************/
	static inline __m128 neg(__m128 x, sse_float_tag) {
		return _mm_sub_ps(_mm_setzero_ps(), x);
	}
	static inline __m128d neg(__m128d x, sse_double_tag) {
		return _mm_sub_pd(_mm_setzero_pd(), x);
	}
	static inline __m128 add(__m128 x, __m128 y, sse_float_tag) {
		return _mm_add_ps(x, y);
	}
	static inline __m128d add(__m128d x, __m128d y, sse_double_tag) {
		return _mm_add_pd(x, y);
	}
	static inline __m128 sub(__m128 x, __m128 y, sse_float_tag) {
		return _mm_sub_ps(x, y);
	}
	static inline __m128d sub(__m128d x, __m128d y, sse_double_tag) {
		return _mm_sub_pd(x, y);
	}
	static inline __m128 mul(__m128 x, __m128 y, sse_float_tag) {
		return _mm_mul_ps(x, y);
	}
	static inline __m128d mul(__m128d x, __m128d y, sse_double_tag) {
		return _mm_mul_pd(x, y);
	}
	static inline __m128 div(__m128 x, __m128 y, sse_float_tag) {
		return _mm_div_ps(x, y);
	}
	static inline __m128d div(__m128d x, __m128d y, sse_double_tag) {
		return _mm_div_pd(x, y);
	}
	/*************************************************************************/
	static inline __m128 max(__m128 x, __m128 y, sse_float_tag) {
		return _mm_max_ps(x, y);
	}
	static inline __m128d max(__m128d x, __m128d y, sse_double_tag) {
		return _mm_max_pd(x, y);
	}
	static inline __m128 min(__m128 x, __m128 y, sse_float_tag) {
		return _mm_min_ps(x, y);
	}
	static inline __m128d min(__m128d x, __m128d y, sse_double_tag) {
		return _mm_min_pd(x, y);
	}
	/*************************************************************************/
	static inline __m128 less(__m128 x, __m128 y, sse_float_tag) {
		return _mm_cmplt_ps(x, y);
	}
	static inline __m128d less(__m128d x, __m128d y, sse_double_tag) {
		return _mm_cmplt_pd(x, y);
	}
	static inline __m128 greater(__m128 x, __m128 y, sse_float_tag) {
		return _mm_cmpgt_ps(x, y);
	}
	static inline __m128d greater(__m128d x, __m128d y, sse_double_tag) {
		return _mm_cmpgt_pd(x, y);
	}
	static inline __m128 less_eq(__m128 x, __m128 y, sse_float_tag) {
		return _mm_cmple_ps(x, y);
	}
	static inline __m128d less_eq(__m128d x, __m128d y, sse_double_tag) {
		return _mm_cmple_pd(x, y);
	}
	static inline __m128 greater_eq(__m128 x, __m128 y, sse_float_tag) {
		return _mm_cmpge_ps(x, y);
	}
	static inline __m128d greater_eq(__m128d x, __m128d y, sse_double_tag) {
		return _mm_cmpge_pd(x, y);
	}
	/*************************************************************************/
	static inline __m128 logical_and(__m128 x, __m128 y, sse_float_tag) {
		return _mm_and_ps(x, y);
	}
	static inline __m128d logical_and(__m128d x, __m128d y, sse_double_tag) {
		return _mm_and_pd(x, y);
	}
	static inline __m128 logical_or(__m128 x, __m128 y, sse_float_tag) {
		return _mm_or_ps(x, y);
	}
	static inline __m128d logical_or(__m128d x, __m128d y, sse_double_tag) {
		return _mm_or_pd(x, y);
	}
	static inline __m128 logical_xor(__m128 x, __m128 y, sse_float_tag) {
		return _mm_xor_ps(x, y);
	}
	static inline __m128d logical_xor(__m128d x, __m128d y, sse_double_tag) {
		return _mm_xor_pd(x, y);
	}
	static inline __m128 logical_not(__m128 x, sse_float_tag) {
		return _mm_andnot_ps(x, true_type<sse_float_tag>());
	}
	static inline __m128d logical_not(__m128d x, sse_double_tag) {
		return _mm_andnot_pd(x, true_type<sse_double_tag>());
	}
	static inline bool logical_all(__m128 x, sse_float_tag) {
		return _mm_movemask_ps(x) == mask_t<sse_float_tag>::value;
	}
	static inline bool logical_all(__m128d x, sse_double_tag) {
		return _mm_movemask_pd(x) == mask_t<sse_double_tag>::value;
	}
	static inline bool logical_none(__m128 x, sse_float_tag) {
		return _mm_movemask_ps(x) == 0;
	}
	static inline bool logical_none(__m128d x, sse_double_tag) {
		return _mm_movemask_pd(x) == 0;
	}
	/*************************************************************************/
	static inline void store(float *p, __m128 x, sse_float_tag) {
		_mm_storeu_ps(p, x);
	}
	static inline void store(double *p, __m128d x, sse_double_tag) {
		_mm_storeu_pd(p, x);
	}
	static inline __m128 load(float const* p, sse_float_tag) {
		return _mm_loadu_ps(p);
	}
	static inline __m128d load(double const* p, sse_double_tag) {
		return _mm_loadu_pd(p);
	}
	static inline __m128 blend(__m128 x, __m128 y, __m128 mask, sse_float_tag) {
		return _mm_blendv_ps(x, y, mask);
	}
	static inline __m128d blend(__m128d x, __m128d y, __m128d mask, sse_double_tag) {
		return _mm_blendv_pd(x, y, mask);
	}
	/*************************************************************************/
	static inline __m128 sqrt(__m128 x, sse_float_tag) {
		return _mm_sqrt_ps(x);
	}
	static inline __m128d sqrt(__m128d x, sse_double_tag) {
		return _mm_sqrt_pd(x);
	}
	static inline __m128 rsqrt(__m128 x, sse_float_tag) {
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
	static inline __m128d rsqrt(__m128d a, sse_double_tag) {
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
