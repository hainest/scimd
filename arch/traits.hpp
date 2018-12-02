#pragma once

#include <type_traits>

namespace scimd {

	template <typename T>
	struct simd_category {};

	template <typename T>
	struct simd_type {};

	template <typename T>
	struct bool_type {};

	namespace detail {
		template <typename T>
		struct is_scalar : std::false_type {};

		template <typename T>
		struct is_avx512 : std::false_type {};
	}
}
