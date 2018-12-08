#include <tuple>
#include "scimd.hpp"

namespace simd {
	template<typename T>
	T* load(T* beg, T* end) {
	}
}

template<typename T>
struct particle {
	T x, y, z;
};

template<typename T>
void test_gravity() {
	particle<float> particles[10];
	for(auto chunk : simd::load(particles, std::end(particles))) {
		scimd::pack<T> x, y, z;
		auto end = x.pack(chunk, [](particle<T> const& p) {return p.x;});
		y.pack(chunk, [](particle<T> const& p) {return p.y;});
		z.pack(chunk, [](particle<T> const& p) {return p.z;});
		auto const r2 = x*x + y*y + z*z;
		auto const f = 3.14159 / sqrt(r2);
		x += f;
		y *= f;
		z += 2.0 * f;
		x.unpack(chunk, [](particle<T>& p, T val) {p.x = val;});
		y.unpack(chunk, [](particle<T>& p, T val) {p.y = val;});
		z.unpack(chunk, [](particle<T>& p, T val) {p.z = val;});
	}
}

int main() {}


//template<typename T>
//std::tuple<cksimd<T>, cksimd<T>, cksimd<T>> pack_coords(
//		simd::range const& r) {
//	cksimd<T> x, y, z;
//	auto end = x.pack(chunk, [](particle<T> const& p) {return p.x;});
//	y.pack(chunk, [](particle<T> const& p) {return p.y;});
//	z.pack(chunk, [](particle<T> const& p) {return p.z;});
//	return std::make_tuple(x, y, z);
//}
//template<typename T>
//void unpack_coords(simd::range& chunk, cksimd<T> x, cksimd<T> y, cksimd<T> z) {
//	x.unpack(chunk, [](particle<T>& p, T val) {p.x = val;});
//	y.unpack(chunk, [](particle<T>& p, T val) {p.y = val;});
//	z.unpack(chunk, [](particle<T>& p, T val) {p.z = val;});
//}
//	for(auto chunk : simd::load(particles, std::end(particles))) {
//		cksimd<T> x, y, z;
//		std::tie(x, y, z) = pack_coords(chunk); // auto [x,y,z] = .. in C++17
//		auto const r2 = x*x + y*y + z*z;
//		auto const f = 3.14159 / sqrt(r2);
//		x += f;
//		y *= f;
//		z += 2.0 * f;
//		unpack_coords(chunk, x, y, z);
//	}
