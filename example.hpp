#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <string>

template <typename T>
struct float_name {};

template <> struct float_name<float> { static constexpr char const* value = "single"; };
template <> struct float_name<double> { static constexpr char const* value = "double"; };

template <typename T>
struct particle {
	T x, y, z;
	particle operator+(T val) { return {x + val, y + val, z + val}; }
	particle operator*(T val) { return {x * val, y * val, z * val}; }
	particle operator+(particle val) { return {val.x + x, val.y + y, val.z + z}; };
	particle operator*(particle val) { return {val.x * x, val.y * y, val.z * z}; };
};

template <typename T>
class particle_list {
	std::vector<particle<T>> particles;

public:
	using value_type = T;
	using iterator = typename decltype(particles)::iterator;
	using const_iterator = typename decltype(particles)::const_iterator;

	particle_list(size_t N) { particles.reserve(N); }
	particle_list(particle_list const&) = default;
	particle_list& operator=(particle_list const&) = default;
	particle_list(particle_list &&) = default;
	particle_list& operator=(particle_list &&) = default;

	iterator begin() { return particles.begin(); }
	const_iterator begin() const { return particles.begin(); }
	iterator end() { return particles.end(); }
	iterator end() const { return particles.end(); }
	size_t size() const { return particles.size(); }
	value_type const* data() const { return particles.data(); }
	value_type* data() { return particles.data(); }

	particle<T>& operator[](size_t i) { return particles[i]; }
	particle<T> operator[](size_t i) const { return particles[i]; }

	particle_list operator+(particle_list const& rhs) {
		particle_list output(rhs.size());
		std::transform(particles.begin(), particles.end(), rhs.begin(), output.begin(),
			[](particle<T> a, particle<T> b){ return a + b; });
		return output;
	}
	particle_list operator*(particle_list const& rhs) {
		particle_list output(rhs.size());
		std::transform(particles.begin(), particles.end(), rhs.begin(), output.begin(),
			[](particle<T> a, particle<T> b){ return a * b; });
		return output;
	}
};

template <typename T>
particle_list<T> make_particles(size_t N) {
	particle_list<T> x(N);
	const auto num_bytes = static_cast<std::streamsize>(N * sizeof(typename decltype(x)::value_type));
	const std::string fname = std::string{"particles_"} + float_name<T>::value + ".dat";

	std::ifstream fin { fname, std::ios::binary };
	if (!fin) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<T> dist(0.1, 0.9);
		std::generate(std::begin(x), std::end(x),
				[&dist, &gen]() -> particle<T>{return {dist(gen),dist(gen),dist(gen)};});
		std::ofstream fout { fname, std::ios::binary };
		fout.write(reinterpret_cast<char*>(&x[0]), num_bytes);
	} else {
		fin.read(reinterpret_cast<char*>(&x[0]), num_bytes);
	}
	return x;
}
