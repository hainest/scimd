#pragma once

#include "scimd.hpp"
#include <iostream>
#include <iomanip>

#ifdef VERBOSE
#define REQUIRE(type, id, lhs, rhs, op)		\
	std::cout << std::setw(5) << id			\
			  << "\t" #type "(" << (lhs) 	\
			  << " " #op " " << (rhs) 		\
			  << ")\n";						\
	if (!type((lhs) op (rhs))) {			\
		std::cout << "\tFAILED\n";			\
		std::exit(1);						\
	}

#define SECTION(msg) std::cout << msg << '\n';
#else
#define REQUIRE(type, id, lhs, rhs, op)		\
	if (!type((lhs) op (rhs))) { 			\
		std::cout << id << " FAILED\n";		\
		std::exit(1);						\
	}
#define SECTION(msg) (void)(msg)
#endif

#define REQUIRE_ALL(name, expr) 		 REQUIRE(all, name, expr, expr, &&)
#define REQUIRE_ALL_LT( name, lhs, rhs)  REQUIRE(all, name, lhs, rhs, < )
#define REQUIRE_ALL_LTE(name, lhs, rhs)  REQUIRE(all, name, lhs, rhs, <=)
#define REQUIRE_ALL_GT( name, lhs, rhs)  REQUIRE(all, name, lhs, rhs, > )
#define REQUIRE_ALL_GTE(name, lhs, rhs)  REQUIRE(all, name, lhs, rhs, >=)

#define REQUIRE_NONE(name, expr) REQUIRE(none, name, expr, expr, &&)
#define REQUIRE_NONE_LT( name, lhs, rhs) REQUIRE(none, name, lhs, rhs, < )
#define REQUIRE_NONE_LTE(name, lhs, rhs) REQUIRE(none, name, lhs, rhs, <=)
#define REQUIRE_NONE_GT( name, lhs, rhs) REQUIRE(none, name, lhs, rhs, > )
#define REQUIRE_NONE_GTE(name, lhs, rhs) REQUIRE(none, name, lhs, rhs, >=)

#define REQUIRE_ANY(name, expr) REQUIRE(any, name, expr, expr, &&)
#define REQUIRE_ANY_LT( name, lhs, rhs)  REQUIRE(any, name, lhs, rhs, < )
#define REQUIRE_ANY_LTE(name, lhs, rhs)  REQUIRE(any, name, lhs, rhs, <=)
#define REQUIRE_ANY_GT( name, lhs, rhs)  REQUIRE(any, name, lhs, rhs, > )
#define REQUIRE_ANY_GTE(name, lhs, rhs)  REQUIRE(any, name, lhs, rhs, >=)

template <typename T>
struct fp_tol{};
template <> struct fp_tol<float> { static constexpr float value = 2e-07; };
template <> struct fp_tol<double> { static constexpr double value = 2e-16; };

template <typename T>
struct fp_name{};
template <> struct fp_name<float> { static constexpr char const* value = "float"; };
template <> struct fp_name<double> { static constexpr char const* value = "double"; };

template<typename T>
std::ostream& operator<<(std::ostream &o, cksimd<T> f) {
	using value_type = typename cksimd<T>::value_type;
	value_type *x = reinterpret_cast<value_type*>(&(f.val));
	constexpr auto N = cksimd<T>::size;
	o << '{' << x[0];
	if (N >= 2) {
		o << ' ' << x[1];
	if (N >= 4) {
		o << ' ' << x[2] << ' ' << x[3];
	if (N >= 8) {
		o << ' ' << x[4] << ' ' << x[5] << ' ' << x[6] << ' ' << x[7];
	if (N == 16) {
		o << ' ' << x[8] << ' ' << x[9] << ' ' << x[10] << ' ' << x[11];
		o << ' ' << x[12] << ' ' << x[13] << ' ' << x[14] << ' ' << x[15];
	}
	}}}
	return o << '}';
}

#if defined(__AVX512F__) && defined(__AVX512ER__) && defined(__AVX512PF__) && defined(__AVX512CD__)
	std::ostream& print(std::ostream &o, __mmask16 x, ck_simd::knl_float_tag) { return o << std::hex << x; }
	std::ostream& print(std::ostream &o, __mmask8 x, ck_simd::knl_double_tag) { return o << std::hex << x; }
#elif defined(__AVX__)
	std::ostream& print(std::ostream &o, __m256 x, ck_simd::avx_float_tag) { return o << cksimd<float>(x); }
	std::ostream& print(std::ostream &o, __m256d x, ck_simd::avx_double_tag) { return o << cksimd<double>(x); }
#elif defined(__SSE4_2__)
	std::ostream& print(std::ostream &o, __m128 x, ck_simd::sse_float_tag) { return o << cksimd<float>(x); }
	std::ostream& print(std::ostream &o, __m128d x, ck_simd::sse_double_tag) { return o << cksimd<double>(x); }
#endif

std::ostream& print(std::ostream &o, bool x, ck_simd::scalar_float_tag) { return o << std::boolalpha << x; }
std::ostream& print(std::ostream &o, bool x, ck_simd::scalar_double_tag) { return o << std::boolalpha << x; }

template <typename T>
std::ostream& operator<<(std::ostream &o, conditional_t<cksimd<T>> c) {
	return print(o, c.val, typename cksimd<T>::category());
}


void show_simd_arch() {
#define SHOW_ARCH(arch, status) std::cout << #arch " enabled: " #status "\n";

#if defined(__AVX512F__) && defined(__AVX512ER__) && defined(__AVX512PF__) && defined(__AVX512CD__)
	SHOW_ARCH(knl, yes)
#else
	SHOW_ARCH(knl, no)
#endif
#ifdef __AVX__
	SHOW_ARCH(avx, yes)
#else
	SHOW_ARCH(avx, no)
#endif
#ifdef __SSE4_2__
	SHOW_ARCH(sse4.2, yes)
#else
	SHOW_ARCH(sse4.2, no)
#endif
}


