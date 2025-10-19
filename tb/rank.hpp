#pragma once

#include "types.h"
#include "math.h"

#include <array>
#include <bit>
#include <cassert>
#include <utility>


template<typename T, U32 N, U32 K>
struct CombArray : public std::array<T, binomial(N, K)> {
	using Base = std::array<T, binomial(N, K)>;
	using Base::Base;

	// rank in combinatorial number system
	template<bool REVERSE>
	static constexpr U32 rank(U32 bits) {
		static_assert(K > 0);
		assert(std::popcount(bits) == K);
		U32 index = 0;
		if constexpr (REVERSE)
			bits <<= 32 - N;
		#pragma unroll
		for (U32 k = K; k--> 0;) {
			U32 bitPos;
			if constexpr (!REVERSE) {
				bitPos = std::countr_zero(bits);
				bits &= bits - 1;
			} else {
				bitPos = std::countl_zero(bits);
				bits &= ~(1U << 31 >> bitPos);
			}
			if (k == 0)
				return index / fact(K) + bitPos;
			U32 i = 0;
			for (U32 j = 0; j < k; j++)
				i += (bitPos - j) * fact(K, k + 1);
			index += i;
		}
		std::unreachable();
	}

	// To reduce the number of LUT's, the assumption is made that UNRANK_LUT<i> matches exactly with the start of UNRANK_LUT<i+1>
	static constexpr U32 LUT_N = 25;

	template<bool REVERSE>
	static constexpr auto UNRANK_LUT = []{
		std::array<U32, binomial(LUT_N, K)> lut{};
		for (U32 bits = 0; bits < lut.size(); bits++) {
			if (std::popcount(bits) == K) {
				lut[rank<REVERSE>(bits)] = bits;
			}
		}
		return lut;
	}();

	template<bool REVERSE>
	static U32 unrank(U32 index) {
		static_assert(N <= LUT_N);
		assert(index < binomial(N, K));
		return UNRANK_LUT<REVERSE>[index];
	}
};