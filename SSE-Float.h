#pragma once

#include "cksimd.h"

struct SSEFloat : cksimd<SSEFloat, float> {
	SSEFloat() : cksimd<SSEFloat, float>() {}
	SSEFloat(simd_t x) : cksimd<SSEFloat, float>(x) {}
	SSEFloat(float f) : cksimd<SSEFloat, float>(f) {}
	SSEFloat(float f0, float f1,float f2, float f3) : cksimd<SSEFloat, float>(ck_simd::setr(f0,f1,f2,f3, category())) {}
	operator simd_t() { return val; }
};

/**
 * 	The converting constructor for in cksimd_t enables this overload in dangerous
 * 	ways. The constructor should be made `explicit`, but that could	break
 *	existing code. Use TMP to disable this overload.
 *
 *	In cmath, libstdc++ uses a `using` directive to pull in the global `sqrt` symbol
 *	which picks up this overload and conflicts with the template declaration
 *	there. Placing this version in an anaonymous namespace removes it from
 *	the global namespace.
 */
namespace {
	template <typename T>
	inline typename std::enable_if<
						std::is_same<T, SSEFloat>::value,
						ck_simd::sqrt_proxy<SSEFloat>
					>::type
	sqrt(SSEFloat a) {
		return ck_simd::sqrt_proxy<SSEFloat>(a);
	}
}
