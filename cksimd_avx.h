#pragma once

#include <immintrin.h>
#include "cksimd_traits.h"

namespace ck_simd {

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
	__m256 sqrt(__m256 x, avx_float_tag) {
		return _mm256_sqrt_ps(x);
	}
	__m256d sqrt(__m256d x, avx_double_tag) {
		return _mm256_sqrt_pd(x);
	}
	__m256 rsqrt(__m256 x, avx_float_tag) {
		return _mm256_rsqrt_ps(x);
	}
	__m256d rsqrt(__m256d x, avx_double_tag) {
		return _mm256_div_pd(_mm256_set1_pd(1.0), x);
	}
	__m256 zero(avx_float_tag) {
		return _mm256_setzero_ps();
	}
	__m256 set1(float x, avx_float_tag) {
		return _mm256_set1_ps(x);
	}
	__m256d set1(double x, avx_double_tag) {
		return _mm256_set1_pd(x);
	}
	__m256 setr(float x, float y, float z, float w,
				float a, float b, float c, float d, avx_float_tag) {
		return _mm256_setr_ps(x, y, z, w, a, b, c, d);
	}
	__m256d setr(double x, double y, double z, double w, avx_double_tag) {
		return _mm256_setr_pd(x, y, z, w);
	}
	__m256d zero(avx_double_tag) {
		return _mm256_setzero_pd();
	}
	__m256 neg(__m256 x, avx_float_tag) {
		return _mm256_sub_ps(_mm256_setzero_ps(), x);
	}
	__m256d neg(__m256d x, avx_double_tag) {
		return _mm256_sub_pd(_mm256_setzero_pd(), x);
	}
	__m256 add(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_add_ps(x, y);
	}
	__m256d add(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_add_pd(x, y);
	}
	__m256 sub(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_sub_ps(x, y);
	}
	__m256d sub(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_sub_pd(x, y);
	}
	__m256 mul(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_mul_ps(x, y);
	}
	__m256d mul(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_mul_pd(x, y);
	}
	__m256 div(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_div_ps(x, y);
	}
	__m256d div(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_div_pd(x, y);
	}
	__m256 bitwise_and(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_and_ps(x, y);
	}
	__m256d bitwise_and(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_and_pd(x, y);
	}
	__m256 bitwise_or(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_or_ps(x, y);
	}
	__m256d bitwise_or(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_or_pd(x, y);
	}
	__m256 bitwise_xor(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_xor_ps(x, y);
	}
	__m256d bitwise_xor(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_xor_pd(x, y);
	}
	__m256 andnot(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_andnot_ps(x, y);
	}
	__m256d andnot(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_andnot_pd(x, y);
	}
	// I don't like using signaling NaNs, but the legacy
	// code does, and I don't want to break code relying
	// on this behavior.
	__m256 less(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_cmp_ps(x, y, _CMP_LT_OS);
	}
	__m256d less(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_cmp_pd(x, y, _CMP_LT_OS);
	}
	__m256 greater(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_cmp_ps(x, y, _CMP_GT_OS);
	}
	__m256d greater(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_cmp_pd(x, y, _CMP_GT_OS);
	}
	// Why are these using quiet NaNs? No idea. Again,
	// I don't want to break legacy code.
	__m256 equals(__m256 x, __m256 y, avx_float_tag) {
		return _mm256_cmp_ps(x, y, _CMP_EQ_OQ);
	}
	__m256d equals(__m256d x, __m256d y, avx_double_tag) {
		return _mm256_cmp_pd(x, y, _CMP_EQ_OQ);
	}
};
