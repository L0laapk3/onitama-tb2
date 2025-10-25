#include "generate.h"

#include "board.hpp"
#include "movegen.hpp"
#include "index.hpp"
#include "table.h"
#include "types.h"
#include "pdep.hpp"

#include <bit>
#include <memory>
#include <iostream>
#include <cassert>
#include <cstdlib>



template<U32 HALFMEN>
void initializeWinIn1(Table<HALFMEN>& table, const CardPreCalc& cards) {
	U64 winIn1Count = 0;
	U64 totalEntries = 0;
	table.iterateFixedMenTables([&]<U32 P0, U32 P1>(auto& fixedMenTable) {
		for (U64 i0 = 0; i0 < fixedMenTable.size(); i0++) {
			auto& row = fixedMenTable[i0];
			U32 bb0 = fixedMenTable.template unrank<false>(i0);
			for (U64 i1 = 0; i1 < row.size(); i1++) {
				U32 bb1 = pdep<P0>(row.template unrank<false>(i1), ~bb0);
				auto& cell = row[i1];
				iterateKingPerms<P0, P1>(bb0, bb1, [&](int pk0, int pk1, int ik0, int ik1, U32 bbk0, U32 bbk1) {
					Move winMove = Board{bb0, bb1, ik0, ik1}.winInOne<0>();
					Perm winPerms = cards.get<1>().moveToPerms<0>(winMove);
					winIn1Count += std::popcount<U32>(winPerms);
					cell[pk0 * P0 + pk1] = winPerms;
					return true;
				});
			}
		}
		totalEntries += fixedMenTable.size() * fixedMenTable[0].size() * P0 * P1 * 30;
	});

	std::cout << "Win in 1 entries: " << winIn1Count << std::endl;
	std::cout << "Total entries: " << totalEntries << std::endl;
	if (HALFMEN == 1) assert(winIn1Count == 5181);
	if (HALFMEN == 2) assert(winIn1Count == 3787692);
	if (HALFMEN == 3) assert(winIn1Count == 537541377);
	if (HALFMEN == 4) assert(winIn1Count == 26991848172);
}


template<U32 P0, U32 P1>
U64 singlePass(FixedMenTable<P0, P1>& table, const CardPreCalc& cards) {
	for (U64 i0 = 0; i0 < table.size(); i0++) {
		auto& row = table[i0];
		U32 bb0 = table.template unrank<false>(i0);

		for (U64 i1 = 0; i1 < row.size(); i1++) {
			auto& cell = row[i1];
			U32 bb1 = pdep<P0>(row.template unrank<false>(i1), ~bb0);

			std::array<Perm, P0 * P1> wonBoards{~0U};

			{ // Forwards movegen - check if all moves lead to wins
				Move<0> cardMoves = 0;
				#pragma unroll
				for (U32 cardI = 0; cardI < 5; cardI++) {
					iterateKingPerms<P0, P1>(bb0, bb1, [&](int pk0, int pk1, int ik0, int ik1, U32 bbk0, U32 bbk1) {
						Perm entry = cell[pk0 * P0 + pk1];
						if (CARDS_USED_IN<0>[cardI] & ~entry) { // we filter out the cards that don't affect any of the unresolved states
							cardMoves |= cards.cards[cardI];
							return false;
						}
						return true;
					});
				}
				if (cardMoves == 0)
					continue;

				iteratePieceMoves(bb0, cardMoves, [&](U32 bb0_new, int from, U32 fromBit, int to, U32 toBit) {
					// Board board{bb0_new, bb1, ik0_new, ik1};
					// TODO: avoid lookup with winin1 check? is it even worth bothering since we'll probably be looking up neighbors?

					U32 i0_new = table.template rank<1>(bb1); // TODO up
					auto& row_new = table[i0_new];
					U32 i1_new = row_new.template rank<1>(pext<P1>(bb0_new, ~bb1) << P1);
					auto& cell_new = row_new[i1_new];

					iterateKingPerms<P0, P1>(bb0, bb1, [&](int pk0, int pk1, int ik0, int ik1, U32 bbk0, U32 bbk1) {
						U32 bbk0_new = fromBit == bbk0 ? toBit : bbk0;
						int pk0_new = std::popcount(bb0_new & (bbk0_new - 1));
						Perm entry_new = cell_new[pk0_new * P0 + pk1];

						wonBoards[pk0 * P0 + pk1] &= entry_new.useP0Card0() | ~cards.permsWhereCardMovesTo[0][to];
						wonBoards[pk0 * P0 + pk1] &= entry_new.useP0Card1() | ~cards.permsWhereCardMovesTo[1][to];

						return true;
					});

					return true; // TODO: break on wonBoards == 0?
				});
			}

			// check if any new wins were found
			bool anyNewWins = false;
			iterateKingPerms<P0, P1>(bb0, bb1, [&](int pk0, int pk1, int ik0, int ik1, U32 bbk0, U32 bbk1) {
				wonBoards[pk0 * P0 + pk1] &= ~cell[pk0 * P0 + pk1]; // do another lookup to avoid race conditions
				anyNewWins |= wonBoards[pk0 * P0 + pk1];
				return true;
			});
			if (!anyNewWins)
				continue;

			{ // backwards movegen - mark that all previous boards lead to a win
				Move<1> cardMoves = 0;
				#pragma unroll
				for (U32 cardI = 0; cardI < 5; cardI++) {
					iterateKingPerms<P0, P1>(bb0, bb1, [&](int pk0, int pk1, int ik0, int ik1, U32 bbk0, U32 bbk1) {
						// TODO: only cards from permutations where there were new wins
						cardMoves |= cards.rCards[cardI];
						return true;
					});
				}

				iteratePieceMoves(bb0, cardMoves, [&](U32 bb0_new, int from, U32 fromBit, int to, U32 toBit) {
					// TODO: avoid lookup with winin1 check? is it even worth bothering since we'll probably be looking up neighbors?

					U32 i0_new = table.template rank<1>(bb1); // TODO up
					auto& row_new = table[i0_new];
					U32 i1_new = row_new.template rank<1>(pext<P1>(bb0_new, ~bb1) << P1);
					auto& cell_new = row_new[i1_new];

					iterateKingPerms<P0, P1>(bb0, bb1, [&](int pk0, int pk1, int ik0, int ik1, U32 bbk0, U32 bbk1) {
						U32 bbk0_new = fromBit == bbk0 ? toBit : bbk0;
						int pk0_new = std::popcount(bb0_new & (bbk0_new - 1));

						cell_new[pk0_new * P0 + pk1] |= wonBoards[pk0 * P0 + pk1].unuseP0Card0() | wonBoards[pk0 * P0 + pk1].useP0Card1();

						return true;
					});

					return true; // TODO: break on wonBoards == 0?
				});
			}


			iterateKingPerms<P0, P1>(bb0, bb1, [&](int pk0, int pk1, int ik0, int ik1, U32 bbk0, U32 bbk1) {
				cell[pk0 * P0 + pk1] |= wonBoards[pk0 * P0 + pk1];
				return true;
			});
		}
	}

	return 0;
}



void generate(const CardPreCalc& cards) {
	auto table = std::make_unique<Table<3>>();
	initializeWinIn1(*table, cards);

	table->iterateFixedMenTables([&]<U32 P0, U32 P1>(auto& fixedMenTable) {
		U64 totalChanged = 0, lastTotal = -1;
		while (totalChanged != lastTotal) {
			lastTotal = totalChanged;
			totalChanged += singlePass<P0, P1>(fixedMenTable, cards);
		}
		std::cout << "TB<" << P0 << ", " << P1 << "> total changed: " << totalChanged << std::endl;
	});
}