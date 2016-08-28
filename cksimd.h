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
#else
#include "cksimd_scalar.h"
#endif

namespace ck_simd {

// NOTE: gcc-4.4 at -O2 cannot see through these abstractions
	template <typename T>
	struct sqrt_proxy {
		T value;
		explicit sqrt_proxy(T x) : value{x} {}
		operator T() {
			return sqrt(value.val, typename T::category());
		}
	};

}
