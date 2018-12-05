#pragma once

#include <malloc.h>
#include <new>
#include <stdexcept>

namespace scimd {
	namespace memory {
		/**
		 * \brief Memory alignment
		 *
		 * Indicates if the memory from which the pack is constructed is
		 * aligned on proper SIMD vector boundaries
		 */
		struct aligned{};
		struct unaligned{};

		/**
		 * \brief Memory layout
		 *
		 * Indicates if the memory from which the pack is constructed is
		 * compact.
		 *
		 * compact: The number of elements contained in the memory is an
		 * 			integral number of SIMD vector lengths. This allows
		 * 			skipping bounds checks when doing a load or store.
		 *
		 * ragged: The memory is not compact.
		 *
		 * \note Compactness does not imply contiguity. A array-of-structures format
		 * can be compact so long as the number of elements of the array is a multiple
		 * of the SIMD vector length.
		 *
		 * \note This is orthogonal to alignment.
		 */
		struct compact{};
		struct ragged{};

		/**
		 * \brief Check if alignments are convertible
		 *
		 * When assigning two packs, the underlying memory is either aligned or not.
		 * The only combination of aligned/unaligned sources that does not work is when
		 * attempting to assign unaligned memory to aligned memory.
		 */
		template <typename target, typename source>
		struct is_alignment_convertible {
			static constexpr bool value =
					!(std::is_same<target, aligned>::value &&
					  std::is_same<source, unaligned>::value);
		};
	}

	template<typename T, size_t Align = 16>
	class allocator {
	public:
		using value_type = T;

		allocator() noexcept = default;
		allocator(const allocator&) noexcept = default;
		~allocator() noexcept = default;

		template<typename U>
		struct rebind {
			using other = allocator;
		};

		T* allocate(size_t n) {
			T *data = static_cast<T*>(aligned_alloc(Align, n * sizeof(T)));
			if (!data) {
				throw std::runtime_error { "Bad allocation" };
			}
			return data;
		}

		void deallocate(T *p, size_t) {
			if (p) {
				free(p);
			}
		}
	};
}

template<class T, class U>
bool operator==(const scimd::allocator<T>&, const scimd::allocator<U>&) noexcept {
	return false;
}
template<class T, class U>
bool operator !=(const scimd::allocator<T>&a, const scimd::allocator<U>&b) noexcept {
	return !(a == b);
}
