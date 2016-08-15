#define CMK_USE_SSE2
#include "SSE-Float.h"
#include <iostream>

#include <string>
#include <cmath>

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

std::ostream& operator<<(std::ostream &o, SSEFloat x) {
#if GCC_VERSION > 40707
	o << '{' << x.val[0] << ", "
			 << x.val[1] << ", "
			 << x.val[2] << ", "
			 << x.val[3] << '}';
#else
	float *y = reinterpret_cast<float*>(&(x.val));
	o << '{' << y[0] << ", "
			 << y[1] << ", "
			 << y[2] << ", "
			 << y[3] << '}';
#endif
	return o;
}

int main() {
#define VERSION 1

#if VERSION == 1
	SSEFloat x, y;
#elif VERSION == 2
	SSEFloat x;
	float y;
#elif VERSION == 3
	float x;
	SSEFloat y;
#elif VERSION == 4
	float x, y;
#else
#error specify VERSION
#endif

	auto sum  = x + y;
	auto diff = x - y;
	auto prod = x * y;
	auto quot = x / y;
	auto srty = sqrt(y);
	auto rsrt = 1.0 / sqrt(y);

#if VERSION == 1 || VERSION == 3
	bool ok = 	std::is_same<std::decay<decltype(sum)>::type, SSEFloat>::value &&
				std::is_same<std::decay<decltype(diff)>::type, SSEFloat>::value &&
				std::is_same<std::decay<decltype(prod)>::type, SSEFloat>::value &&
				std::is_same<std::decay<decltype(quot)>::type, SSEFloat>::value &&
				std::is_same<std::decay<decltype(srty)>::type, ck_simd::sqrt_proxy<SSEFloat>>::value &&
				std::is_same<std::decay<decltype(rsrt)>::type, SSEFloat>::value;
#elif VERSION == 2
	bool ok = 	std::is_same<std::decay<decltype(sum)>::type, SSEFloat>::value &&
				std::is_same<std::decay<decltype(diff)>::type, SSEFloat>::value &&
				std::is_same<std::decay<decltype(prod)>::type, SSEFloat>::value &&
				std::is_same<std::decay<decltype(quot)>::type, SSEFloat>::value &&
				std::is_same<std::decay<decltype(srty)>::type, double>::value &&
				std::is_same<std::decay<decltype(rsrt)>::type, double>::value;
#else
	bool ok = 	std::is_same<std::decay<decltype(sum)>::type, float>::value &&
				std::is_same<std::decay<decltype(diff)>::type, float>::value &&
				std::is_same<std::decay<decltype(prod)>::type, float>::value &&
				std::is_same<std::decay<decltype(quot)>::type, float>::value &&
				std::is_same<std::decay<decltype(srty)>::type, double>::value &&
				std::is_same<std::decay<decltype(rsrt)>::type, double>::value;
#endif
	std::cout << std::boolalpha << ((ok) ? "OK" : "FAIL") << std::endl;
}

