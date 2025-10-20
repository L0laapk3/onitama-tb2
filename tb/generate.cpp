#include "generate.h"

#include "board.hpp"
#include "table.h"
#include "types.h"
#include "pdep.hpp"

#include <bit>
#include <memory>
#include <iostream>
#include <cassert>



template<U32 HALFMEN>
void initializeWinIn1(Table<HALFMEN>& table, const Cards<1>& rCards) {
	U64 winIn1Count = 0;
	U64 totalEntries = 0;
	table.iterateFixedMenTables([&]<U32 P0, U32 P1>(FixedMenTable<P0, P1>& fixedMenTable) {
		for (U64 i0 = 0; i0 < fixedMenTable.size(); i0++) {
			auto& row = fixedMenTable[i0];
			U32 bb0 = fixedMenTable.template unrank<false>(i0);
			for (U64 i1 = 0; i1 < row.size(); i1++) {
				U32 bb1 = pdep<P0>(row.template unrank<false>(i1), ~bb0);
				auto entry = row[i1].begin();
				U32 bb0_k = bb0 & ~(1U << PTEMPLE[0]);
				#pragma unroll
				for (U32 _ik0 = 0; _ik0 < P0; _ik0++) {
					if (_ik0 == P0 - 1 && !bb0_k) [[unlikely]] break;
					assert(bb0_k);
					U32 ik0 = std::countr_zero(bb0_k);
					bb0_k &= bb0_k - 1;
					U32 bb1_k = bb1 & ~(1U << PTEMPLE[1]);
					#pragma unroll
					for (U32 _ik1 = 0; _ik1 < P1; _ik1++) {
						if (_ik1 == P1 - 1 && !bb1_k) [[unlikely]] break;
						assert(bb1_k);
						U32 ik1 = std::countr_zero(bb1_k);
						bb1_k &= bb1_k - 1;
						Board board{ bb0, bb1, ik0, ik1 };
						Move winMove = board.winInOne<0>();
						U32 winPerms = rCards.validPerms<0>(winMove);
						*entry++ = winPerms;
						winIn1Count += std::popcount(winPerms);
					}
				}
				totalEntries += 30 * P0 * P1;
			}
		}
	});
	std::cout << "Win in 1 entries: " << winIn1Count << std::endl;
	std::cout << "Total entries: " << totalEntries << std::endl;
}




void generate(const Cards<>& cards) {
	auto table = std::make_unique<Table<4>>();
	initializeWinIn1(*table, cards.reverse());
}