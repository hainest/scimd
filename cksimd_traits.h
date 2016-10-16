#pragma once

#include <type_traits>

namespace ck_simd {

	template <typename T>
	struct simd_category {};

	// Category tags
	struct sse_float_tag {};
	struct sse_double_tag {};
	struct avx_float_tag {};
	struct avx_double_tag {};
	struct scalar_float_tag {};
	struct scalar_double_tag {};
	struct knl_float_tag {};
	struct knl_double_tag {};

	template <typename T>
	struct simd_type {};

	template <typename T>
	struct bool_type {};

	template <typename T>
	struct is_scalar {
		static const bool value =
				std::is_same<T, scalar_float_tag()>::value ||
				std::is_same<T, scalar_double_tag()>::value;
	};

	template <typename T>
	struct is_avx {
		static const bool value =
				std::is_same<T, avx_float_tag()>::value ||
				std::is_same<T, avx_double_tag()>::value;
	};

	template <typename T>
	struct is_sse {
		static const bool value =
				std::is_same<T, sse_float_tag()>::value ||
				std::is_same<T, sse_double_tag()>::value;
	};

	template <typename T>
	struct is_knl {
		static const bool value =
				std::is_same<T, knl_float_tag()>::value ||
				std::is_same<T, knl_double_tag()>::value;
	};
}
