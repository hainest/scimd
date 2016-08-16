#pragma once

namespace ck_simd {

	template <typename T>
	struct simd_category {};

	template <typename T>
	struct simd_type {};

#if defined(__SSE2__) && defined(CMK_USE_SSE2)
	struct sse_float_tag {};
	struct sse_double_tag {};

	template <> struct simd_category<float>  { typedef sse_float_tag type; };
	template <> struct simd_category<double> { typedef sse_double_tag type; };

	template <> struct simd_type<float>  { typedef __m128  type; };
	template <> struct simd_type<double> { typedef __m128d type; };
#endif

#if defined(__AVX__) && defined(CMK_USE_AVX)
	struct avx_float_tag {};
	struct avx_double_tag {};

	template <> struct simd_category<float>  { typedef avx_float_tag type; };
	template <> struct simd_category<double> { typedef avx_double_tag type; };

	template <> struct simd_type<float>  { typedef __m256  type; };
	template <> struct simd_type<double> { typedef __m256d type; };
#endif
}
