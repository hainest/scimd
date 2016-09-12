/**
 * 	Tests for the legacy SSE-*.h interfaces
 */

#include "SSE-Float.h"
#include "SSE-Double.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

union maskf_t {
	float f;
	uint32_t i;
	maskf_t(float f) : f(f) {}
};
union mask_t {
	double d;
	uint64_t i;
	mask_t(double d) : d(d) {}
};
template <typename T>
struct mask {};
template <> struct mask<float>  { typedef maskf_t type; };
template <> struct mask<double> { typedef mask_t type; };
template <typename T>
bool to_bool(T *x, size_t n) {
	typedef typename mask<T>::type mask_type;
	decltype(mask_type::i) r{0};
	for(size_t i=0; i<n; ++i) {
		r |= mask_type(x[i]).i;
	}
	return r != 0;
}
bool to_bool(SSEFloat f) {
	float *x = reinterpret_cast<float*>(&(f.val));
	return to_bool(x, 4);
}
bool to_bool(SSEDouble f) {
	double *x = reinterpret_cast<double*>(&(f.val));
#ifdef CMK_USE_SSE2
	return to_bool(x, 4);
#else
	return to_bool(x, 8);
#endif
}
bool to_bool(int f) {
	return f;
}
template <typename T, typename U>
struct answer {
	T sum, diff, prod;
	U quot, srty, rsrt;
};
template <typename T, typename U>
void combine(T &mask, U v) {
	mask = mask & v;
}
template<>
void combine(SSEFloat &mask, bool v) {
	// This isn't correct, but it works.
	// The correct method throws an ICE in gcc-4.4.7
	mask = mask & SSEFloat(float(v));
}
template<>
void combine(SSEDouble &mask, bool v) {
	// This isn't correct, but it works.
	// The correct method throws an ICE in gcc-4.4.7
	mask = mask & SSEDouble(double(v));
}
template <typename T, typename U, typename V>
void test(T x, U y, char const* name, answer<T,U> const& ans, V tol) {
	asm volatile("simd_test_begin%=:" :);
	auto srty = U(sqrt(y));
	auto rsrt = x / sqrt(y);
	auto a = (((x + y) - ans.sum)  < tol) &
			 (((x - y) - ans.diff) < tol) &
			 (((x * y) - ans.prod) < tol) &
			 (((x / y) - ans.quot) < tol);
	combine(a, (srty - ans.srty) < tol);
	combine(a, (rsrt - ans.rsrt) < tol);
	asm volatile("simd_test_end%=:" :);

	if(!to_bool(a)) {
		std::cerr << name << " FAILED\n";
		std::exit(-1);
	}
}


int main() {
#ifdef CMK_USE_SSE2
	{
		float x = 3.0f;
		float y = 17.0f;
		float sum = x + y;
		float diff = x - y;
		float prod = x * y;
		float quot = x / y;
		float srty = sqrt(y);
		float rsrt = x / sqrt(y);
		const float tol = 2e-07;

		test(SSEFloat{x}, SSEFloat{y}, "SSEFloat+SSEFloat", answer<SSEFloat, SSEFloat>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(SSEFloat{x}, y, "SSEFloat+float", answer<SSEFloat, float>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(x, SSEFloat{y}, "float+SSEFloat", answer<float, SSEFloat>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(x, y, "float+float", answer<float, float>{sum,diff,prod,quot,srty,rsrt}, tol);
	}
#endif

	{
		double x = 3.0;
		double y = 17.0;
		double sum = x + y;
		double diff = x - y;
		double prod = x * y;
		double quot = x / y;
		double srty = sqrt(y);
		double rsrt = x / sqrt(y);
		const double tol = 2e-16;

		test(SSEDouble{x}, SSEDouble{y}, "SSEDouble+SSEDouble", answer<SSEDouble, SSEDouble>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(SSEDouble{x}, y, "SSEDouble+double", answer<SSEDouble, double>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(x, SSEDouble{y}, "double+SSEDouble", answer<double, SSEDouble>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(x, y, "double+double", answer<double, double>{sum,diff,prod,quot,srty,rsrt}, tol);
	}
	std::cout << "DONE\n";
}
