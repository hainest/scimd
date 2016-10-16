#include "cksimd_test.hpp"
#include <cmath>
#include <cstdlib>
#include <string>
#include <numeric>

template<typename T, typename U>
struct answer {
	T sum, diff, prod;
	U quot, srty, rsrt;
	T prsqrt;
};

template<typename T, typename U, typename V>
void test_mixed_mode(T x, U y, std::string const& name, answer<T, U> const& ans, V tol) {
	SECTION("Testing mixed-mode for " + name);

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

	REQUIRE_ALL(1, s && d && p && q);
	REQUIRE_ALL(2, m);
	REQUIRE_ALL(3, n);
	REQUIRE_ALL(4, o);
}

template <typename T>
void test_memory() {
	SECTION("Testing memory for T = " + std::string{fp_name<T>::value});

	constexpr auto tol = fp_tol<T>::value;
	constexpr auto const N = cksimd<T>::size;
	T in_arr[N];
	std::iota(std::begin(in_arr), std::end(in_arr), 0.0f);

	cksimd<T> x;
	asm volatile("simd_memory_begin%=:" :);
	x.pack(std::begin(in_arr), std::end(in_arr), [](T x) {return x;});
	asm volatile("simd_memory_end%=:" :);

	x += 2.0;

	T out_arr[N];
	x.unpack(std::begin(out_arr), std::end(out_arr), [](T &x, T y) {x = y;});

	REQUIRE_ALL_LTE(1, (out_arr[0] - (2.0 + in_arr[0])), tol);

	// Make sure the default pack value is working
	if (N > 4) {
		REQUIRE_ALL(2, out_arr[5] != 2.0);
	}

	x.load(in_arr);
	x += 2.0;
	x.store(out_arr);
	REQUIRE_ALL_LTE(3, (out_arr[0] - (2.0 + in_arr[0])), tol);

	x.load(in_arr);
	auto mask = x <= 2.0;
	if(any(mask)) {
		auto a = x + 2.0;
		x.blend(a, mask);
	}
	x.store(out_arr);

	switch (N) {
	case 8:
		REQUIRE_ALL_LTE(7.8, (out_arr[7] - in_arr[7]), tol)
		REQUIRE_ALL_LTE(7.7, (out_arr[6] - in_arr[6]), tol)
		REQUIRE_ALL_LTE(7.6, (out_arr[5] - in_arr[5]), tol)
		REQUIRE_ALL_LTE(7.5, (out_arr[4] - in_arr[4]), tol)
		/* no break */
	case 4:
		REQUIRE_ALL_LTE(7.4, (out_arr[3] - in_arr[3]), tol)
		REQUIRE_ALL_LTE(7.3, (out_arr[2] - (2.0 + in_arr[2])), tol)
		/* no break */
	case 2:
		REQUIRE_ALL_LTE(7.2, (out_arr[1] - (2.0 + in_arr[1])), tol)
		/* no break */
	case 1:
		REQUIRE_ALL_LTE(7.1, (out_arr[0] - (2.0 + in_arr[0])), tol)
	}
}

template<typename T>
void test_mixed_mode() {
	T x = 3.0;
	T y = 17.0;
	T sum = x + y;
	T diff = x - y;
	T prod = x * y;
	T quot = x / y;
	T srty = std::sqrt(y);
	T rsrt = x / std::sqrt(y);
	T prsqrt = 1.0 / std::sqrt(y);
	const T tol = fp_tol<T>::value;

	std::string const fpname{fp_name<T>::value};
	auto name = "cksimd<" + fpname + ">+cksimd<" + fpname + ">";
	test_mixed_mode(cksimd<T> { x }, cksimd<T> { y }, name, answer<cksimd<T>, cksimd<T>> { sum, diff, prod, quot, srty, rsrt, prsqrt }, tol);
	name = "cksimd<" + fpname + ">+" + fpname;
	test_mixed_mode(cksimd<T> { x }, y, name, answer<cksimd<T>, T> { sum, diff, prod, quot, srty, rsrt, prsqrt }, tol);
	name = fpname + "+cksimd<" + fpname + ">";
	test_mixed_mode(x, cksimd<T> { y }, name, answer<T, cksimd<T>> { sum, diff, prod, quot, srty, rsrt, prsqrt }, tol);
	name = fpname + "+" + fpname;
	test_mixed_mode(x, y, name, answer<T, T> { sum, diff, prod, quot, srty, rsrt, prsqrt }, tol);
}

template <typename T>
void test_operators() {
	SECTION("Testing operators for T = " + std::string{fp_name<T>::value});

	constexpr auto tol = fp_tol<T>::value;
	constexpr T x = 0.1, y = 0.2;
	REQUIRE_ALL_LTE(0, (-cksimd<T>{x} - cksimd<T>{-x}), tol);
	REQUIRE_ALL_LTE(1, (cksimd<T>{x + y} - (cksimd<T>{x} + cksimd<T>{y})), tol);
	REQUIRE_ALL_LTE(2, (cksimd<T>{x - y} - (cksimd<T>{x} - cksimd<T>{y})), tol);
	REQUIRE_ALL_LTE(3, (cksimd<T>{x * y} - (cksimd<T>{x} * cksimd<T>{y})), tol);
	REQUIRE_ALL_LTE(4, (cksimd<T>{x / y} - (cksimd<T>{x} / cksimd<T>{y})), tol);

	REQUIRE_ALL_LTE(5, ((cksimd<T>{y} += x) - cksimd<T>{y + x}), tol);
	REQUIRE_ALL_LTE(6, ((cksimd<T>{y} -= x) - cksimd<T>{y - x}), tol);
	REQUIRE_ALL_LTE(7, ((cksimd<T>{y} *= x) - cksimd<T>{y * x}), tol);
	REQUIRE_ALL_LTE(8, ((cksimd<T>{y} /= x) - cksimd<T>{y / x}), tol);

	REQUIRE_ALL_LTE( 9, cksimd<T>{x}, cksimd<T>{y});
	REQUIRE_ALL_LT (10, cksimd<T>{x}, cksimd<T>{y});
	REQUIRE_ALL_GT (11, cksimd<T>{y}, cksimd<T>{x});
	REQUIRE_ALL_GTE(12, cksimd<T>{y}, cksimd<T>{x});

	REQUIRE_ALL_LT (13, (cksimd<T>{x} - cksimd<T>{y}), tol);
	REQUIRE_ALL_LTE(14, (cksimd<T>{y} - cksimd<T>{y}), tol);
	REQUIRE_ALL_GT (15, (cksimd<T>{y} + cksimd<T>{x}), tol);
	REQUIRE_ALL_GTE(16, (cksimd<T>{y} + cksimd<T>{x}), tol);

	REQUIRE_NONE_GT (17, tol, (cksimd<T>{y} + cksimd<T>{x}));
	REQUIRE_NONE_GTE(18, tol, (cksimd<T>{y} + cksimd<T>{x}));
	REQUIRE_NONE_LT (19, tol,  (cksimd<T>{y} - cksimd<T>{y}));
	REQUIRE_NONE_LTE(20, tol, (cksimd<T>{y} - cksimd<T>{y}));

	REQUIRE_ALL_LTE(21, (max(cksimd<T>{x}, cksimd<T>{y}) - cksimd<T>{y}), tol);
	REQUIRE_ALL_LTE(22, (min(cksimd<T>{x}, cksimd<T>{y}) - cksimd<T>{x}), tol);

	REQUIRE_ALL(23, ((x < y) && (y > x)));
	REQUIRE_ALL(24, ((x < y) || (y < x)));
	REQUIRE_ALL(25, ((x < y)  ^ (y < x)));
	REQUIRE_ALL(26, (!(y < x)));

	REQUIRE_NONE(27, ((x > y) && (y < x)));
	REQUIRE_NONE(28, ((x > y) || (y < x)));
	REQUIRE_NONE(29, ((x > y)  ^ (y < x)));
	REQUIRE_NONE(30, (!(y > x)));
}
int main() {
	// Don't let the size get bigger than the underlying SIMD type
	static_assert(sizeof(cksimd<float>::simd_t) == sizeof(cksimd<float>), "cksimd<float> must be the size of cksimd<float>::simd_t");
	static_assert(sizeof(cksimd<double>::simd_t) == sizeof(cksimd<double>), "cksimd<double> must be the size of cksimd<double>::simd_t");

	show_simd_arch();

	test_operators<float>();
	test_operators<double>();
	test_mixed_mode<float>();
	test_mixed_mode<double>();
	test_memory<float>();
	test_memory<double>();

	std::cout << "PASSED\n";
}
