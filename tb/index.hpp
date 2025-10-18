#pragma once

#include "types.h"
#include "math.h"

#include <array>
#include <bit>
#include <cassert>
#include <utility>



// rank in combinatorial number system
template<U32 N, U32 K, bool REVERSE>
constexpr U32 rank(U32 bits) {
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


template<U32 N, U32 K, bool REVERSE>
static auto UNRANK_LUT = []{
	std::array<U32, binomial(25, K)> lut{};
	for (U32 bits = 0; bits < lut.size(); bits++) {
		if (std::popcount(bits) == K) {
			lut[rank<N, K, REVERSE>(bits)] = bits;
		}
	}
	return lut;
}();

static_assert(sizeof(UNRANK_LUT<25, 5, false>) == 212520);

// To reduce the number of LUT's, the assumption is made that UNRANK_LUT<i> matches exactly with the start of UNRANK_LUT<i+1>
constexpr U32 LUT_N = 25;

// unrank in combinatorial number system
template<U32 N, U32 K, bool REVERSE>
constexpr U32 unrank(U32 index) {
	static_assert(N <= LUT_N);
	assert(index < binomial(N, K));
	return UNRANK_LUT<LUT_N, K, REVERSE>[index];
}