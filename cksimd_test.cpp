#include "cksimd.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

union mask_t  {
	double d;
	uint64_t i;
	mask_t(double d) : d(d) {}
	mask_t(uint64_t i) : i(i) {}
};
union maskf_t {
	float f;
	uint32_t i;
	maskf_t(float f) : f(f) {}
	maskf_t(uint32_t i) : i(i) {}
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
	return r;
}
template <typename T>
bool to_bool(T f) {
	typedef typename T::value_type value_type;
	value_type *x = reinterpret_cast<value_type*>(&(f.val));
	return to_bool(x, T::nelem);
}
template<>
bool to_bool(bool b) {
	return b;
}
template<>
bool to_bool(int i) {
	return i != 0;
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
void combine(cksimd<float> &f, bool v) {
	f &= cksimd<float>(float(v));
}
template<>
void combine(cksimd<double> &f, bool v) {
	f &= cksimd<double>(double(v));
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
template <typename T, typename U, typename V>
void test(T x, U y, char const* name, answer<T,U> const& ans, V tol) {
	asm volatile("simd_test_begin%=:" :);
	auto srty = U(sqrt(y));
	auto rsrt = x / sqrt(y);
	auto a = (((x + y) - ans.sum)  <= tol) &
			 (((x - y) - ans.diff) <= tol) &
			 (((x * y) - ans.prod) <= tol) &
			 (((x / y) - ans.quot) <= tol);
	combine(a, (srty - ans.srty) <= tol);
	combine(a, (rsrt - ans.rsrt) <= tol);
	asm volatile("simd_test_end%=:" :);

	if(!to_bool(a)) {
		std::cerr << name << " FAILED\n";
		std::exit(-1);
	}
}

int main() {
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

		test(cksimd<float>{x}, cksimd<float>{y}, "cksimd<float>+cksimd<float>", answer<cksimd<float>, cksimd<float>>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(cksimd<float>{x}, y, "cksimd<float>+float", answer<cksimd<float>, float>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(x, cksimd<float>{y}, "float+cksimd<float>", answer<float, cksimd<float>>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(x, y, "float+float", answer<float, float>{sum,diff,prod,quot,srty,rsrt}, tol);
	}

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

		test(cksimd<double>{x}, cksimd<double>{y}, "cksimd<double>+cksimd<double>", answer<cksimd<double>, cksimd<double>>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(cksimd<double>{x}, y, "cksimd<double>+double", answer<cksimd<double>, double>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(x, cksimd<double>{y}, "double+cksimd<double>", answer<double, cksimd<double>>{sum,diff,prod,quot,srty,rsrt}, tol);
		test(x, y, "double+double", answer<double, double>{sum,diff,prod,quot,srty,rsrt}, tol);
	}

	std::cout << "DONE\n";
}
