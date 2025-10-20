#pragma once

#include "types.h"

#include <bit>
#include <cassert>
#include <immintrin.h>


// pdep and pext emulation functions optimized for small number of unset bits in mask
// beats ucode implementation by a mile on zen/zen2

constexpr bool USE_PDEP = true;


template<U32 MAX_MASK_UNSET_BITS>
inline U32 pdep(U32 src, U32 mask) {
	U32 res = _pdep_u32(src, mask);
	mask = ~mask;
	assert(std::popcount(mask) <= MAX_MASK_UNSET_BITS);
	#pragma unroll
	for (int i = 0; i < MAX_MASK_UNSET_BITS - 1; i++) {
		U64 bb = mask & -mask;
		src += src & ~(bb - 1);
		mask &= mask - 1;
	}
	src += src & ~(mask - 1);
	assert(res == src);
	return USE_PDEP ? res : src;
}

template<U32 MAX_MASK_UNSET_BITS>
inline U32 pext(U32 src, U32 mask) {
	U32 res = _pext_u32(src, mask);
	mask = ~mask;
	assert(std::popcount(mask) <= MAX_MASK_UNSET_BITS);
	#pragma unroll
	for (int i = 0; i < MAX_MASK_UNSET_BITS - 1; i++) {
		src += src & (mask ^ (mask - 1));
		mask &= mask - 1;
	}
	src += src & (mask - 1);
	src >>= MAX_MASK_UNSET_BITS;
	assert(res == src);
	return USE_PDEP ? res : src;
}