#pragma once

#include "board.hpp"
#include "table.h"
#include "types.h"


template<typename Callable>
bool inline iteratePieceMoves(U32 bb, U32 dest, Callable&& cb) {

	U32 bb_loop = bb;
	while (bb_loop) {
		U32 fromBit = bb_loop & -bb_loop;
		U32 bb0_empty = bb ^ fromBit;
		int from = std::countr_zero(bb_loop);

		U32 dest_loop = dest & MOVE_MASK[from];
		while (dest_loop) { // Loop over move bits and apply them
			int offset = std::countr_zero(dest_loop) - from;
			int to = (from + offset) & 31;
			U32 toBit = 1U << to;
			U32 bb_new = bb0_empty | toBit;

			if (!cb(bb_new, from, fromBit, to, toBit))
				return false;

			dest_loop &= dest_loop - 1;
		}
		bb_loop &= bb_loop - 1;
	}

	return true;
}