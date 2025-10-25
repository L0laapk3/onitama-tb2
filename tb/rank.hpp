#pragma once

#include "types.h"
#include "math.h"

#include <array>
#include <bit>
#include <cassert>
#include <utility>


template<typename T, U32 N, U32 K>
struct CombArray : public std::array<T, binomial(N, K)> {
	using std::array<T, binomial(N, K)>::array;

	// rank in combinatorial number system
	template<bool REVERSE>
	static inline constexpr U32 rank(U32 bits) {
		static_assert(K > 0);
		assert(std::popcount(bits) == K);
		if constexpr (REVERSE)
			bits <<= 32 - N;
		U32 ip0, index = 0;
		#pragma unroll
		for (U32 k = 0; k < K; k++) {
			U32 bitPos;
			if constexpr (!REVERSE) {
				bitPos = std::countr_zero(bits);
				bits &= bits - 1;
			} else {
				bitPos = std::countl_zero(bits);
				bits &= ~(1U << (31 - bitPos));
			}
			if (k == 0) {
				ip0 = bitPos;
				continue;
			}
			U32 i = fact(K, k + 1);
			for (U32 j = 0; j <= k; j++)
				i *= bitPos - j;
			index += i;
		}
		return index / fact(K) + ip0;
	}

	// To reduce the number of LUT's, the assumption is made that UNRANK_LUT<i> matches exactly with the start of UNRANK_LUT<i+1>
	static constexpr U32 LUT_N = 25;

	template<bool REVERSE>
	static constexpr auto UNRANK_LUT = []{
		std::array<U32, binomial(LUT_N, K)> lut{};
		// Use Gosper's hack to iterate through all K-bit combinations efficiently
		U32 bits = (1U << K) - 1;  // Start with lowest K bits set
		while (bits < 1U << LUT_N) {
			lut[rank<REVERSE>(bits)] = bits;
			U32 c = bits & -bits;
			U32 r = bits + c;
			bits = (((r ^ bits) >> 2) / c) | r;
		}
		for (auto& entry : lut)
			assert(entry);
		return lut;
	}();

	template<bool REVERSE>
	static inline U32 unrank(U32 index) {
		static_assert(N <= LUT_N);
		assert(index < binomial(N, K));
		return UNRANK_LUT<REVERSE>[index];
	}
};