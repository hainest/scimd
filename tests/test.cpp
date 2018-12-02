#include "catch2.hpp"
#include "scimd.hpp"
#include <cmath>
#include <cstdlib>
#include <string>
#include <numeric>
#include <array>
#include <algorithm>

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
	std::array<T, N> input;
	std::iota(std::begin(input), std::end(input), 0.0f);

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
		REQUIRE(scimd::all((x-y) <= tol));

		std::array<T,N> out_lambda;
		x.store(std::begin(out_lambda), std::end(out_lambda), [](T &x, T y) {x = y;});

		std::array<T, N> out_plain;
		x.store(out_plain.data());

		// Ensure the stored values are the same to within FP tolerance
		auto const is_same =
			std::equal(
				std::begin(out_lambda),
				std::end(out_lambda),
				std::begin(out_plain),
				[](T x, T y) { return (x-y) <= tol; }
			);
		REQUIRE(is_same);
	}

	SECTION("load/store with default for T = " + std::string{fp_name<T>::value}) {
		// use lambda load of 1 value and fill the rest of the SIMD vector
		// with the default value
		scimd::pack<T> x;
		asm volatile("scimd_load_lambda_def_begin%=:" :);
		x.load(std::end(input)-1, std::end(input), [](T x) {return x;});
		asm volatile("scimd_load_lambda_def_end%=:" :);

		std::array<T,N> output;
		x.store(output.data());

		// The first element should be the last value in 'input'
		// The rest should be T{} (the default)
		std::array<T,N> expected = {{T{}}};
		expected[0] = *(std::end(input)-1);

		// Ensure the stored values are the same to within FP tolerance
		auto const is_same =
			std::equal(
				std::begin(output),
				std::end(output),
				std::begin(expected),
				[](T x, T y) { return (x-y) <= tol; }
			);
		REQUIRE(is_same);
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
		auto s = (((x + y) - ans.sum) <= tol);
		auto d = (((x - y) - ans.diff) <= tol);
		auto p = (((x * y) - ans.prod) <= tol);
		auto q = (((x / y) - ans.quot) <= tol);
		auto m = (srty - ans.srty) <= tol;
		auto n = (rsrt - ans.rsrt) <= tol;
		auto o = (rsqrt(y) - ans.prsqrt) <= tol;
		asm volatile("arith_end%=:" :);

		// These are split out so that analyzing the objdump is easier
		REQUIRE(scimd::all(s));
		REQUIRE(scimd::all(d));
		REQUIRE(scimd::all(p));
		REQUIRE(scimd::all(q));
		REQUIRE(scimd::all(m));
		REQUIRE(scimd::all(n));
		REQUIRE(scimd::all(o));
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
		REQUIRE(scimd::all((-x - (-x)) <= tol));
		REQUIRE(scimd::all(((x + T{}) - x) <= tol));
		REQUIRE(scimd::all(((x + y) - (x) - (y)) <= tol));
		REQUIRE(scimd::all(((x - y) - (x) + (y)) <= tol));
		REQUIRE(scimd::all(((x * y) - (x * y)) <= tol));
		REQUIRE(scimd::all(((x / y) - (x / y)) <= tol));
	}

	SECTION("in-place arithmetic operators for T = " + std::string{fp_name<T>::value}) {
		REQUIRE(scimd::all(((scimd::pack<T>{y} += x) - (y + x)) <= tol));
		REQUIRE(scimd::all(((scimd::pack<T>{y} -= x) - (y - x)) <= tol));
		REQUIRE(scimd::all(((scimd::pack<T>{y} *= x) - (y * x)) <= tol));
		REQUIRE(scimd::all(((scimd::pack<T>{y} /= x) - (y / x)) <= tol));
	}

	SECTION("logical operators for T = " + std::string{fp_name<T>::value}) {
		REQUIRE(scimd::all(y > x));
		REQUIRE(scimd::none(y < x));
		REQUIRE(scimd::any(y >= x));
		REQUIRE(!scimd::any(y <= x));
	}

	SECTION("vector+scalar logical operators for T = " + std::string{fp_name<T>::value}) {
		REQUIRE(scimd::all(y > T{0.0}));
		REQUIRE(scimd::none(y < T{0.0}));
		REQUIRE(scimd::any(y >= T{0.0}));
		REQUIRE(!scimd::any(y <= T{0.0}));
	}

	SECTION("range operators for T = " + std::string{fp_name<T>::value}) {
		// These assume x and y are positive and y>x
		REQUIRE(scimd::all((max(x, y) - y) <= tol));
		REQUIRE(scimd::all((min(x, y) - x) <= tol));
		REQUIRE(scimd::all(abs(x) >= x));
		REQUIRE(scimd::all(abs(-x) >= x));
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
