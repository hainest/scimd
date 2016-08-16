#include "SSE-Float.h"
#include "SSE-Double.h"
#include <iostream>
#include <string>
#include <cmath>

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

std::ostream& operator<<(std::ostream &o, SSEFloat x) {
	float *y = reinterpret_cast<float*>(&(x.val));
	o << '{' << y[0] << ", "
			 << y[1] << ", "
			 << y[2] << ", "
			 << y[3] << '}';
	return o;
}

std::ostream& operator<<(std::ostream &o, SSEDouble x) {
	double *y = reinterpret_cast<double*>(&(x.val));
	o << '{' << y[0] << ", "
			 << y[1] << ", "
			 << y[2] << ", "
			 << y[3] << '}';
	return o;
}


template <typename T>
void foo() {
	T i = 1;
}

int main(int argc, const char* argv[]) {
	if(argc < 3) {
		std::cerr << "Moo\n";
		return -1;
	}

	auto p1 = std::stof(argv[1]);
	auto p2 = std::stof(argv[2]);

#define VERSION 1

#if VERSION == 1
//	SSEFloat x{p1}, y{p2};
	SSEDouble x{p1}, y{p2};
#elif VERSION == 2
	SSEFloat x{p1};
	float y{p2};
#elif VERSION == 3
	float x{p1};
	SSEFloat y{p2};
#elif VERSION == 4
	float x{p1}, y{p2};
#else
#error specify VERSION
#endif

	asm volatile("simd_begin%=:" :);
	auto sum  = x + y;
	auto diff = x - y;
	auto prod = x * y;
	auto quot = x / y;
	auto srty = sqrt(y);
	auto rsrt = 1.0 / sqrt(y);
	asm volatile("simd_begin%=:" :);

	std::cout << "sum = " << sum << '\n'
			  << "diff = " << diff << '\n'
			  << "prod = " << prod << '\n'
			  << "quot = " << quot << '\n'
			  << "srty = " << srty << '\n'
			  << "rsrt = " << rsrt << '\n';

//#if VERSION == 1 || VERSION == 3
//	bool ok = 	std::is_same<std::decay<decltype(sum)>::type, SSEFloat>::value &&
//				std::is_same<std::decay<decltype(diff)>::type, SSEFloat>::value &&
//				std::is_same<std::decay<decltype(prod)>::type, SSEFloat>::value &&
//				std::is_same<std::decay<decltype(quot)>::type, SSEFloat>::value &&
//				std::is_same<std::decay<decltype(srty)>::type, ck_simd::sqrt_proxy<SSEFloat>>::value &&
//				std::is_same<std::decay<decltype(rsrt)>::type, SSEFloat>::value;
//#elif VERSION == 2
//	bool ok = 	std::is_same<std::decay<decltype(sum)>::type, SSEFloat>::value &&
//				std::is_same<std::decay<decltype(diff)>::type, SSEFloat>::value &&
//				std::is_same<std::decay<decltype(prod)>::type, SSEFloat>::value &&
//				std::is_same<std::decay<decltype(quot)>::type, SSEFloat>::value &&
//				std::is_same<std::decay<decltype(srty)>::type, double>::value &&
//				std::is_same<std::decay<decltype(rsrt)>::type, double>::value;
//#else
//	bool ok = 	std::is_same<std::decay<decltype(sum)>::type, float>::value &&
//				std::is_same<std::decay<decltype(diff)>::type, float>::value &&
//				std::is_same<std::decay<decltype(prod)>::type, float>::value &&
//				std::is_same<std::decay<decltype(quot)>::type, float>::value &&
//				std::is_same<std::decay<decltype(srty)>::type, double>::value &&
//				std::is_same<std::decay<decltype(rsrt)>::type, double>::value;
//#endif
//	std::cout << std::boolalpha << ((ok) ? "OK" : "FAIL") << std::endl;
}

