#include "SSE-Float.h"
#include "SSE-Double.h"
#include <iostream>
#include <iomanip>
#include <cmath>

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
template <> struct mask<float>  { using type = maskf_t; };
template <> struct mask<double> { using type = mask_t; };
template <typename T>
bool to_bool(T *x, size_t n) {
	uint32_t r{0};
	using mask_type = typename mask<T>::type;
	for(size_t i=0; i<n; ++i) {
		r |= mask_type(x[i]).i;
	}
	return r;
}
template <typename T>
bool to_bool(T f) {
	using value_type = typename T::value_type;
	value_type *x = reinterpret_cast<value_type*>(&(f.val));
	return to_bool(x, T::size);
}
template<>
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
	mask &= v;
}
template<>
void combine(SSEFloat &f, bool v) {
	f &= SSEFloat(float(v));
}
template<>
void combine(SSEDouble &f, bool v) {
	f &= SSEDouble(double(v));
}

template <typename T, typename U>
void test(T x, U y, char const* name, answer<T,U> const& ans) {
	std::cout << std::setw(20) << std::left << name << ": ";
	auto srty = U(sqrt(y));
	auto rsrt = x / sqrt(y);
	auto a = ((x + y) == ans.sum ) &
			 ((x - y) == ans.diff) &
			 ((x * y) == ans.prod) &
			 ((x / y) == ans.quot);
	combine(a, srty == ans.srty);
	combine(a, rsrt == ans.rsrt);
	std::cout << (to_bool(a) ? "PASSED" : "FAILED") << '\n';
}

int main() {
	float p1 = 4.0f;
	float p2 = 16.0f;

	float sum = p1 + p2;
	float diff = p1 - p2;
	float prod = p1 * p2;
	float quot = p1 / p2;
	float srty = sqrt(p2);
	float rsrt = p1 / sqrt(p2);

	test(SSEFloat{p1}, SSEFloat{p2}, "SSEFloat+SSEFloat", answer<SSEFloat, SSEFloat>{sum,diff,prod,quot,srty,rsrt});
	test(SSEFloat{p1}, p2, "SSEFloat+float", answer<SSEFloat, float>{sum,diff,prod,quot,srty,rsrt});
	test(p1, SSEFloat{p2}, "float+SSEFloat", answer<float, SSEFloat>{sum,diff,prod,quot,srty,rsrt});
	test(p1, p2, "float+float", answer<float, float>{sum,diff,prod,quot,srty,rsrt});
	test(SSEDouble{p1}, SSEDouble{p2}, "SSEDouble+SSEDouble", answer<SSEDouble, SSEDouble>{sum,diff,prod,quot,srty,rsrt});
	test(SSEDouble{p1}, double{p2}, "SSEDouble+double", answer<SSEDouble, double>{sum,diff,prod,quot,srty,rsrt});
	test(double{p1}, SSEDouble{p2}, "double+SSEDouble", answer<double, SSEDouble>{sum,diff,prod,quot,srty,rsrt});
	test(double{p1}, double{p2}, "double+double", answer<double, double>{sum,diff,prod,quot,srty,rsrt});
}
