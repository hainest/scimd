#include "cksimd.h"

using cosmoType = double;

template <typename T>
void SPLINE(cksimd<T> r2, cksimd<T> twoh, cksimd<T> &a, cksimd<T> &b) {

	/*	Make the frequent case cheap
	 *
	 *	The extra mul in the test is cheaper than the cost difference between
	 *	the div+sqrt and the optimized cksimd::rsqrt.
	 */
	if (all(r2 >= (twoh * twoh))) {
		asm volatile("r2_begin%=:" :);
		a = rsqrt(r2);
		b = a * a * a;
		return;
	}
	asm volatile("r2_end%=:" :);

	const cksimd<T> r = sqrt(r2);

	{
		auto mask = r >= twoh;
		a.blend(rsqrt(r2), mask);
		b.blend(a * a * a, mask);
	}

	/*
	 * 	Do we want to add an optimization for rcp to remove **all** divs?
	 */
	auto const dih = static_cast<T>(2.0) / twoh;
	auto const u = r * dih;

	// Do some strength reduction to help the compiler and tidy up the code.
	auto const u2 = u * u;
	auto const u3 = u2 * u;
	auto const u4 = u2 * u2;
	auto const u5 = u4 * u;
	auto const dih3 = (dih * dih) * dih;

	asm volatile("mask_begin%=:" :);
	const auto mask = u < T(1.0);
	if (any(mask)) {
		const T c1 = 7.0 /  5.0, c2 = 2.0 /  3.0,
				c3 = 3.0 / 10.0, c4 = 1.0 / 10.0,
				c5 = 4.0 /  3.0, c6 = 6.0 /  5.0,
				c7 = 1.0 /  2.0;

		const auto a1 = dih  * (c1 - c2 * u2 + c3 * u4 - c4 * u5);
		const auto b1 = dih3 * (c5 - c6 * u2 + c7 * u3);

		a.blend(a1, mask);
		b.blend(b1, mask);
	}
	asm volatile("mask_end%=:" :);

	if (any(!mask)) {
		auto const dir = rsqrt(r2), dir3 = (dir * dir) * dir;

		const T c1 = -1.0 / 15.0, c2 = 8.0 /  5.0,
				c3 =  4.0 /  3.0, c4 = 3.0 / 10.0,
				c5 =  1.0 / 30.0, c6 = 8.0 /  3.0,
				c7 =  3.0		, c8 = 6.0 /  5.0,
				c9 =  1.0 /  6.0;

		const auto a1 = c1 * dir  + dih  * (c2 - c3 * u2 + u3 - c4 * u4 + c5 * u5);
		const auto b1 = c1 * dir3 + dih3 * (c6 - c7 * u  + c8 * u2 - c9 * u3);

		a.blend(a1, !mask);
		b.blend(b1, !mask);
	}
}
