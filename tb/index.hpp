#pragma once

#include "types.h"
#include "board.hpp"



template<U32 P0, U32 P1, typename Callable>
bool inline iterateKingPerms(U32 bb0, U32 bb1, Callable&& cb) {
	U32 bb0_k = bb0 & ~(1U << PTEMPLE[0]);
	#pragma unroll
	for (int pk0 = 0; pk0 < P0; pk0++) {
		if (pk0 == P0 - 1 && !bb0_k) [[unlikely]] // 2% chance to reduce permutations from 25 to 20...
			break;
		assert(bb0_k);
		int ik0 = std::countr_zero(bb0_k);
		U32 bbk0 = bb0_k & -bb0_k;
		U32 bb1_k = bb1 & ~(1U << PTEMPLE[1]);
		#pragma unroll
		for (int pk1 = 0; pk1 < P1; pk1++) {
			if (pk1 == P1 - 1 && !bb1_k) [[unlikely]] // 2%² chance to further reduce to 16...
				break;
			assert(bb1_k);
			int ik1 = std::countr_zero(bb1_k);
				U32 bbk1 = bb1_k & -bb1_k;
			if (!cb(pk0, pk1, ik0, ik1, bbk0, bbk1))
				return false;
			bb1_k &= bb1_k - 1;
		}
		bb0_k &= bb0_k - 1;
	}
	return true;
}