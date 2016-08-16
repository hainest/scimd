#pragma once

#if defined(__AVX__) && defined(CMK_USE_AVX)
#include "cksimd_avx.h"
#endif

#if defined(__SSE2__) && defined(CMK_USE_SSE2)
#include "cksimd_sse.h"
#endif

namespace ck_simd {
	template <typename T>
	struct sqrt_proxy {
		T value;
		explicit sqrt_proxy(T x) : value{x} {}
		operator T() {
			return sqrt(value, typename T::category());
		}
	};
}
