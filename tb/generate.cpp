#include "generate.h"

#include "table.h"
#include "types.h"
#include "pdep.hpp"

#include <memory>



template<U32 HALFMEN>
void initializeWinIn1(Table<HALFMEN>& table) {
	table.iterateFixedMenTables([&]<U32 P0, U32 P1>(FixedMenTable<P0, P1>& fixedMenTable) {
		for (U64 i0 = 0; i0 < fixedMenTable.size(); i0++) {
			auto& row = fixedMenTable[i0];
			U32 bb0 = fixedMenTable.template unrank<false>(i0);
			for (U64 i1 = 0; i1 < row.size(); i1++) {
				auto& cell = row[i1];
				U32 bb1 = pdep<P0>(row.template unrank<false>(i1), ~bb0);
				for (auto& elem : cell) {
					// TODO: initialize tablebase
					elem++;
				}
			}
		}
	});
}




void generate(const CardsInfo& cardsInfo) {
	auto table = std::make_unique<Table<4>>();
	initializeWinIn1(*table);
}