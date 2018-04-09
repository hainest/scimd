#include "example.hpp"
#include "cksimd.h"
#include <iostream>

template<typename T>
std::ostream& operator<<(std::ostream &o, cksimd<T> f) {
	using value_type = typename cksimd<T>::value_type;
	value_type *x = reinterpret_cast<value_type*>(&(f.val));
	constexpr auto N = cksimd<T>::size;
	o << '{' << x[0];
	if (N >= 2) {
		o << ' ' << x[1];
	if (N >= 4) {
		o << ' ' << x[2] << ' ' << x[3];
	if (N >= 8) {
		o << ' ' << x[4] << ' ' << x[5] << ' ' << x[6] << ' ' << x[7];
	if (N == 16) {
		o << ' ' << x[8] << ' ' << x[9] << ' ' << x[10] << ' ' << x[11];
		o << ' ' << x[12] << ' ' << x[13] << ' ' << x[14] << ' ' << x[15];
	}
	}}}
	return o << '}';
}

template <typename T>
void SPLINE(cksimd<T> r2, cksimd<T> twoh, cksimd<T> &a, cksimd<T> &b);

template <typename T>
void test(size_t N) {
	auto particles = make_particles<T>(N);

	for(auto p : particles) {
		std::cout << p << '\n';
	}

//	{
//		/*
//		 * 	Testcase where all(r2 >= (twoh * twoh)) == true
//		 */
//		auto twoh = particles;
//		auto r2 = particles * particles + static_cast<T>(2.0);
//		auto cur_particle = std::begin(particles);
//
//		do {
//			cksimd<T> x, y, z;
//			auto end = x.pack(cur_particle, std::end(particles), [](particle<T> p){ return p.x; });
//			y.pack(cur_particle, end, [](particle<T> p){ return p.y; });
//			z.pack(cur_particle, end, [](particle<T> p){ return p.z; });
//
////			SPLINE(r2, twoh, a, b);
//			SPLINE(x, y, x, y);
//			std::cout << x << '\n';
//		} while(cur_particle != std::end(particles));
//	}
}

int main() {
	constexpr size_t N = 64;
	test<float>(N);
//	test<double>(N);
}

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

//inline int partBucketForce(ExternalGravityParticle *part, Tree::GenericTreeNode *req,
//						   GravityParticle **activeParticles, Vector3D<cosmoType> offset,
//						   int nActiveParts) {
//	Vector3D <SSEcosmoType> r;
//	SSEcosmoType rsq;
//	SSEcosmoType twoh, a, b;
//
//	for (int i = 0; i < nActiveParts; i += SSE_VECTOR_WIDTH) {
//		Vector3D<SSEcosmoType> packedPos(
//			SSELoad(SSEcosmoType, activeParticles, i, ->position.x),
//			SSELoad(SSEcosmoType, activeParticles, i, ->position.y),
//			SSELoad(SSEcosmoType, activeParticles, i, ->position.z)
//		);
//		SSELoad(SSEcosmoType packedSoft, activeParticles, i, ->soft);
//
//		r = -packedPos + offset + part->position;
//		rsq = r.lengthSquared();
//		twoh = part->soft + packedSoft;
//		SSEcosmoType select = rsq > COSMO_CONST(0.0);
//		int compare = movemask(select);
//		if (compare) {
//			SPLINE(rsq, twoh, a, b);
//			if ((~compare) & cosmoMask) {
//				a = select & a;
//				b = select & b;
//			}
//			SSEcosmoType SSELoad(packedMass, activeParticles, i, ->mass);
//			SSEcosmoType SSELoad(packedDtGrav, activeParticles, i, ->dtGrav);
//			Vector3D<SSEcosmoType> packedAcc(
//				SSELoad(SSEcosmoType, activeParticles, i, ->treeAcceleration.x),
//				SSELoad(SSEcosmoType, activeParticles, i, ->treeAcceleration.y),
//				SSELoad(SSEcosmoType, activeParticles, i, ->treeAcceleration.z)
//			);
//			SSEcosmoType SSELoad(packedPotential, activeParticles, i, ->potential);
//
//			SSEcosmoType idt2 = (packedMass + part->mass) * b;
//			idt2 = max(idt2, packedDtGrav);
//			packedAcc += r * (b * part->mass);
//			packedPotential -= part->mass * a;
//
//			SSEStore(packedAcc.x, activeParticles, i, ->treeAcceleration.x);
//			SSEStore(packedAcc.y, activeParticles, i, ->treeAcceleration.y);
//			SSEStore(packedAcc.z, activeParticles, i, ->treeAcceleration.z);
//			SSEStore(packedPotential, activeParticles, i, ->potential);
//			SSEStore(idt2, activeParticles, i, ->dtGrav);
//		}
//	}
//	return nActiveParts;
//}
