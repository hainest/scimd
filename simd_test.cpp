#include "SSE-Float.h"
#include "SSE-Double.h"
#include "cksimd.h"
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
template <> struct mask<float>  { typedef maskf_t type; };
template <> struct mask<double> { typedef mask_t type; };
template <typename T>
bool to_bool(T *x, size_t n) {
	uint32_t r{0};
	typedef typename mask<T>::type mask_type;
	for(size_t i=0; i<n; ++i) {
		r |= mask_type(x[i]).i;
	}
	return r;
}
template <typename T>
bool to_bool(T f) {
	typedef typename T::value_type value_type;
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

template <typename T>
std::ostream& operator<<(std::ostream &o, cksimd<T> f) {
	typedef typename cksimd<T>::value_type value_type;
	value_type *x = reinterpret_cast<value_type*>(&(f.val));
	o << '{' << x[0];
	if(cksimd<T>::nelem >= 2) {
		o << ' ' << x[1];
		if(cksimd<T>::nelem >= 4) {
			o << ' ' << x[2] << ' ' << x[3];
			if(cksimd<T>::nelem == 8) {
				o << ' ' << x[4] << ' ' << x[5]
				  << ' ' << x[6] << ' ' << x[7];
			}
		}
	}
	return o << '}';
}
template <typename T, typename U>
void test(T x, U y, char const* name, answer<T,U> const& ans) {
	std::cout << std::setw(20) << std::left << name << ": ";
	asm volatile("sqrt_begin%=:" :);
	auto srty = U(sqrt(y));
	auto rsrt = x / sqrt(y);
	asm volatile("sqrt_end%=:" :);
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
