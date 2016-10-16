#include "cksimd.h"

using cosmoType = double;

void SPLINE3(cksimd<cosmoType> r2, cksimd<cosmoType> twoh, cksimd<cosmoType> &a,
			 cksimd<cosmoType> &b)
{

	/*	Make the frequent case cheap
	 *
	 *	The extra mul in the test is cheaper than the cost difference between
	 *	the div+sqrt and the optimized cksimd::rsqrt.
	 */
	if (all(r2 >= (twoh * twoh))) {
		a = rsqrt(r2);
		b = a * a * a;
		return;
	}
	asm volatile("r2_end%=:" :);

	const cksimd<cosmoType> r = sqrt(r2);

	{
		auto mask = r >= twoh;
		const auto a1 = rsqrt(r2);
		const auto b1 = a * a * a;
		a.blend(a1, mask);
		b.blend(b1, mask);
	}

	/*
	 * 	Do we want to add an optimization for rcp to remove **all** divs?
	 */
	const cksimd<cosmoType> dih = cosmoType(2.0) / twoh;

	const cksimd<cosmoType> u = r * dih;

	// Do some strength reduction to help the compiler, and tidy up the code.
	const cksimd<cosmoType> u2 = u * u;
	const cksimd<cosmoType> u3 = u2 * u;
	const cksimd<cosmoType> u4 = u2 * u2;
	const cksimd<cosmoType> u5 = u4 * u;
	const cksimd<cosmoType> dih3 = (dih * dih) * dih;

	asm volatile("mask_begin%=:" :);
	const auto mask = u < cosmoType(1.0);
	if (any(mask)) {
		const cosmoType c1 = 7.0 /  5.0, c2 = 2.0 /  3.0,
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
		const cksimd<cosmoType> dir  = rsqrt(r2),
								dir3 = (dir * dir) * dir;

		const cosmoType c1 = -1.0 / 15.0, c2 = 8.0 /  5.0,
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
