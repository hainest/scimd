#pragma once

/**
 *  These are mutually exclusive by design as switching between
 *  SIMD modes is expensive on even modern CPUs. If you need to use
 *  more than one instruction set, put them in separate translation
 *  units.
 */
#if defined(__AVX__) && defined(CMK_USE_AVX)
#include "cksimd_avx.h"
#elif defined(__SSE2__) && defined(CMK_USE_SSE2)
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
