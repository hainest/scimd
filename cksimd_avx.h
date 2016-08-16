#pragma once

#include <immintrin.h>
#include "cksimd_traits.h"

namespace ck_simd {

	/**
	 * 	Tag dispatch is used here because the gcc ABI before gcc-4.9
	 * 	does not properly mangle the SIMD types.
	 */
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
};
