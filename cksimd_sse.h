#pragma once

#include <xmmintrin.h>

namespace ck_simd {

	struct sse_float_tag {};
	struct sse_double_tag {};

	template <typename T>
	struct simd_category {};

	template <> struct simd_category<float>  { typedef sse_float_tag type; };
	template <> struct simd_category<double> { typedef sse_double_tag type; };

	template <typename T>
	struct simd_type {};

	template <> struct simd_type<float>  { typedef __m128  type; };
	template <> struct simd_type<double> { typedef __m128d type; };

/**
 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
 * 	does not properly mangle the SIMD types.
 */
__m128 sqrt(__m128 x, sse_float_tag) {
	return _mm_sqrt_ps(x);
}
__m128d sqrt(__m128d x, sse_double_tag) {
	return _mm_sqrt_pd(x);
}
__m128 rsqrt(__m128 x, sse_float_tag) {
	return _mm_rsqrt_ps(x);
}
__m128d rsqrt(__m128d x, sse_double_tag) {
	return _mm_div_pd(_mm_set1_pd(1.0), x);
}
__m128 zero(sse_float_tag) {
	return _mm_setzero_ps();
}
__m128 set1(float x, sse_float_tag) {
	return _mm_set1_ps(x);
}
__m128d set1(double x, sse_double_tag) {
	return _mm_set1_pd(x);
}
__m128 setr(float x, float y, float z, float w, sse_float_tag) {
	return _mm_setr_ps(x, y, z, w);
}
__m128d setr(double x, double y, sse_double_tag) {
	return _mm_setr_pd(x, y);
}
__m128d zero(sse_double_tag) {
	return _mm_setzero_pd();
}
__m128 neg(__m128 x, sse_float_tag) {
	return _mm_sub_ps(_mm_setzero_ps(), x);
}
__m128d neg(__m128d x, sse_double_tag) {
	return _mm_sub_pd(_mm_setzero_pd(), x);
}
__m128 add(__m128 x, __m128 y, sse_float_tag) {
	return _mm_add_ps(x, y);
}
__m128d add(__m128d x, __m128d y, sse_double_tag) {
	return _mm_add_pd(x, y);
}
__m128 sub(__m128 x, __m128 y, sse_float_tag) {
	return _mm_sub_ps(x, y);
}
__m128d sub(__m128d x, __m128d y, sse_double_tag) {
	return _mm_sub_pd(x, y);
}
__m128 mul(__m128 x, __m128 y, sse_float_tag) {
	return _mm_mul_ps(x, y);
}
__m128d mul(__m128d x, __m128d y, sse_double_tag) {
	return _mm_mul_pd(x, y);
}
__m128 div(__m128 x, __m128 y, sse_float_tag) {
	return _mm_div_ps(x, y);
}
__m128d div(__m128d x, __m128d y, sse_double_tag) {
	return _mm_div_pd(x, y);
}
__m128 bitwise_and(__m128 x, __m128 y, sse_float_tag) {
	return _mm_and_ps(x, y);
}
__m128d bitwise_and(__m128d x, __m128d y, sse_double_tag) {
	return _mm_and_pd(x, y);
}
__m128 bitwise_or(__m128 x, __m128 y, sse_float_tag) {
	return _mm_or_ps(x, y);
}
__m128d bitwise_or(__m128d x, __m128d y, sse_double_tag) {
	return _mm_or_pd(x, y);
}
__m128 bitwise_xor(__m128 x, __m128 y, sse_float_tag) {
	return _mm_xor_ps(x, y);
}
__m128d bitwise_xor(__m128d x, __m128d y, sse_double_tag) {
	return _mm_xor_pd(x, y);
}
__m128 andnot(__m128 x, __m128 y, sse_float_tag) {
	return _mm_andnot_ps(x, y);
}
__m128d andnot(__m128d x, __m128d y, sse_double_tag) {
	return _mm_andnot_pd(x, y);
}
__m128 less(__m128 x, __m128 y, sse_float_tag) {
	return _mm_cmplt_ps(x, y);
}
__m128d less(__m128d x, __m128d y, sse_double_tag) {
	return _mm_cmplt_pd(x, y);
}
__m128 greater(__m128 x, __m128 y, sse_float_tag) {
	return _mm_cmpgt_ps(x, y);
}
__m128d greater(__m128d x, __m128d y, sse_double_tag) {
	return _mm_cmpgt_pd(x, y);
}
__m128 equals(__m128 x, __m128 y, sse_float_tag) {
	return _mm_cmpeq_ps(x, y);
}
__m128d equals(__m128d x, __m128d y, sse_double_tag) {
	return _mm_cmpeq_pd(x, y);
}

};
