#include "catch2.hpp"
#include "scimd.hpp"
#include <cmath>
#include <cstdlib>
#include <string>
#include <numeric>
#include <array>
#include <algorithm>

using std::abs;

// These will eventually be replaced by versions from the standard library
bool all(bool x) { return x; }
bool none(bool x) { return !x; }
bool any(bool x) { return !none(x); }

template <typename T>
struct fp_tol{};
template <> struct fp_tol<float> { static constexpr float value = 2e-07; };
template <> struct fp_tol<double> { static constexpr double value = 2e-16; };

template <typename T>
struct fp_name{};
template <> struct fp_name<float> { static constexpr char const* value = "float"; };
template <> struct fp_name<double> { static constexpr char const* value = "double"; };

template <typename T>
void test_memory() {
	constexpr auto tol = fp_tol<T>::value;
	constexpr auto N = scimd::pack<T>::size;
	alignas(scimd::pack<T>) std::array<T, N> input;
	std::iota(std::begin(input), std::end(input), 0.0f);

	auto is_same = [](std::array<T, N> const& in, std::array<T, N> const& out) -> bool {
		return std::equal(
			std::begin(out),
			std::end(out),
			std::begin(in),
			[](T x, T y) { return (x-y) <= tol; }
		);
	};

	SECTION("alignment load/store for T = " + std::string{fp_name<T>::value}) {
		alignas(scimd::pack<T>) std::array<T,N> out;

		// Default load/store
		scimd::pack<T> x;
		REQUIRE(x.load(input.data()) == (std::begin(input) + N));
		REQUIRE(x.store(out.data()) == std::end(out));
		REQUIRE(is_same(input, out));

		// unaligned load, default store
		REQUIRE(x.load(scimd::memory::unaligned{}, input.data()) == (std::begin(input) + N));
		REQUIRE(x.store(out.data()) == std::end(out));
		REQUIRE(is_same(input, out));

		// aligned load, default store
		REQUIRE(x.load(scimd::memory::aligned{}, input.data()) == (std::begin(input) + N));
		REQUIRE(x.store(out.data()) == std::end(out));
		REQUIRE(is_same(input, out));

		// default load, unaligned store
		REQUIRE(x.load(input.data()) == (std::begin(input) + N));
		REQUIRE(x.store(scimd::memory::unaligned{}, out.data()) == std::end(out));
		REQUIRE(is_same(input, out));

		// default load, aligned store
		REQUIRE(x.load(input.data()) == (std::begin(input) + N));
		REQUIRE(x.store(scimd::memory::unaligned{}, out.data()) == std::end(out));
		REQUIRE(is_same(input, out));

		// aligned load, aligned store
		REQUIRE(x.load(scimd::memory::aligned{}, input.data()) == (std::begin(input) + N));
		REQUIRE(x.store(scimd::memory::aligned{}, out.data()) == std::end(out));
		REQUIRE(is_same(input, out));

		// unaligned load, unaligned store
		REQUIRE(x.load(scimd::memory::unaligned{}, input.data()) == (std::begin(input) + N));
		REQUIRE(x.store(scimd::memory::unaligned{}, out.data()) == std::end(out));
		REQUIRE(is_same(input, out));
	}

	SECTION("load/store for T = " + std::string{fp_name<T>::value}) {
		// use simple load of N values
		scimd::pack<T> x;
		asm volatile("scimd_load_begin%=:" :);
		x.load(input.data());
		asm volatile("scimd_load_end%=:" :);

		// use lambda load of N values (no default used here)
		scimd::pack<T> y;
		asm volatile("scimd_load_lambda_begin%=:" :);
		y.load(std::begin(input), std::end(input), [](T x) {return x;});
		asm volatile("scimd_load_lambda_end%=:" :);

		// x and y should be equal to within FP tolerance
		REQUIRE(all(abs(x-y) <= tol));

		alignas(scimd::pack<T>) std::array<T,N> out_lambda;
		x.store(std::begin(out_lambda), std::end(out_lambda), [](T &x, T y) {x = y;});

		alignas(scimd::pack<T>) std::array<T, N> out_plain;
		x.store(out_plain.data());
		REQUIRE(is_same(out_lambda, out_plain));
	}

	SECTION("load/store with default for T = " + std::string{fp_name<T>::value}) {
		// use lambda load of 1 value and fill the rest of the SIMD vector
		// with the default value
		scimd::pack<T> x;
		asm volatile("scimd_load_lambda_def_begin%=:" :);
		x.load(std::end(input)-1, std::end(input), [](T x) {return x;});
		asm volatile("scimd_load_lambda_def_end%=:" :);

		alignas(scimd::pack<T>) std::array<T,N> output;
		x.store(output.data());

		// The first element should be the last value in 'input'
		// The rest should be T{} (the default)
		alignas(scimd::pack<T>) std::array<T,N> expected = {{T{}}};
		expected[0] = *(std::end(input)-1);
		REQUIRE(is_same(output, expected));
	}
}

template<typename T, typename U>
struct answer {
	T sum, diff, prod;
	U quot, srty, rsrt;
	T prsqrt;
};

template<typename T, typename U, typename V>
void test_mixed_mode(T x, U y, std::string const& name, answer<T, U> const& ans, V tol) {
	SECTION("mixed-mode for " + name) {
		asm volatile("arith_begin%=:" :);
		U srty = sqrt(y);
		auto rsrt = x / sqrt(y);
		auto s = (abs((x + y) - ans.sum) <= tol);
		auto d = (abs((x - y) - ans.diff) <= tol);
		auto p = (abs((x * y) - ans.prod) <= tol);
		auto q = (abs((x / y) - ans.quot) <= tol);
		auto m = abs(srty - ans.srty) <= tol;
		auto n = abs(rsrt - ans.rsrt) <= tol;
		auto o = abs(rsqrt(y) - ans.prsqrt) <= tol;
		asm volatile("arith_end%=:" :);

		// These are split out so that analyzing the objdump is easier
		REQUIRE(all(s));
		REQUIRE(all(d));
		REQUIRE(all(p));
		REQUIRE(all(q));
		REQUIRE(all(m));
		REQUIRE(all(n));
		REQUIRE(all(o));
	}
}

template<typename T>
void test_mixed_mode() {
	T const x = 3.0;
	T const y = 17.0;
	T const sum = x + y;
	T const diff = x - y;
	T const prod = x * y;
	T const quot = x / y;
	T const srty = std::sqrt(y);
	T const rsrt = x / std::sqrt(y);
	T const prsqrt = 1.0 / std::sqrt(y);
	T const tol = fp_tol<T>::value;

	std::string const fpname{fp_name<T>::value};
	auto name = "scimd::pack<" + fpname + ">+scimd::pack<" + fpname + ">";
	test_mixed_mode(scimd::pack<T> { x }, scimd::pack<T> { y }, name, answer<scimd::pack<T>, scimd::pack<T>> { sum, diff, prod, quot, srty, rsrt, prsqrt }, tol);
	name = "scimd::pack<" + fpname + ">+" + fpname;
	test_mixed_mode(scimd::pack<T> { x }, y, name, answer<scimd::pack<T>, T> { sum, diff, prod, quot, srty, rsrt, prsqrt }, tol);
	name = fpname + "+scimd::pack<" + fpname + ">";
	test_mixed_mode(x, scimd::pack<T> { y }, name, answer<T, scimd::pack<T>> { sum, diff, prod, quot, srty, rsrt, prsqrt }, tol);
	name = fpname + "+" + fpname;
	test_mixed_mode(x, y, name, answer<T, T> { sum, diff, prod, quot, srty, rsrt, prsqrt }, tol);
}

template <typename T>
void test_operators() {
	constexpr auto tol = fp_tol<T>::value;
	scimd::pack<T> const x{0.1}, y{0.2};

	SECTION("arithmetic operators for T = " + std::string{fp_name<T>::value}) {
		REQUIRE(all(abs(-x - (-x)) <= tol));
		REQUIRE(all(abs((x + T{}) - x) <= tol));
		REQUIRE(all(abs((x + y) - (x) - (y)) <= tol));
		REQUIRE(all(abs((x - y) - (x) + (y)) <= tol));
		REQUIRE(all(abs((x * y) - (x * y)) <= tol));
		REQUIRE(all(abs((x / y) - (x / y)) <= tol));
	}

	SECTION("in-place arithmetic operators for T = " + std::string{fp_name<T>::value}) {
		REQUIRE(all(abs((scimd::pack<T>{y} += x) - (y + x)) <= tol));
		REQUIRE(all(abs((scimd::pack<T>{y} -= x) - (y - x)) <= tol));
		REQUIRE(all(abs((scimd::pack<T>{y} *= x) - (y * x)) <= tol));
		REQUIRE(all(abs((scimd::pack<T>{y} /= x) - (y / x)) <= tol));
	}

	SECTION("logical operators for T = " + std::string{fp_name<T>::value}) {
		REQUIRE(all(y > x));
		REQUIRE(none(y < x));
		REQUIRE(any(y >= x));
		REQUIRE(!any(y <= x));
	}

	SECTION("vector+scalar logical operators for T = " + std::string{fp_name<T>::value}) {
		REQUIRE(all(y > T{0.0}));
		REQUIRE(none(y < T{0.0}));
		REQUIRE(any(y >= T{0.0}));
		REQUIRE(!any(y <= T{0.0}));
	}

	SECTION("range operators for T = " + std::string{fp_name<T>::value}) {
		// These assume x and y are positive and y>x
		REQUIRE(all(abs(max(x, y) - y) <= tol));
		REQUIRE(all(abs(min(x, y) - x) <= tol));
		REQUIRE(all(abs(x) >= x));
		REQUIRE(all(abs(-x) >= x));
	}
}

// Don't let the size get bigger than the underlying SIMD type
static_assert(sizeof(scimd::pack<float>::simd_t) == sizeof(scimd::pack<float>), "scimd::pack<float> must be the size of scimd::pack<float>::simd_t");
static_assert(sizeof(scimd::pack<double>::simd_t) == sizeof(scimd::pack<double>), "scimd::pack<double> must be the size of scimd::pack<double>::simd_t");

TEST_CASE("operators") {
	test_operators<float>();
	test_operators<double>();
}
TEST_CASE("mixed mode") {
	test_mixed_mode<float>();
	test_mixed_mode<double>();
}
TEST_CASE("memory") {
	test_memory<float>();
	test_memory<double>();
}
