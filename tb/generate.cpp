#include "generate.h"

#include "index.hpp"
#include "table.h"
#include "types.h"

#include <iostream>
#include <memory>



template<U32 HALFMEN>
void initializeWinIn1(Table<HALFMEN>& table) {
	table.iterateFixedMenTables([&]<U32 P0, U32 P1>(FixedMenTable<P0, P1>& fixedMenTable) {
		for (U64 i0 = 0; i0 < fixedMenTable.size(); i0++) {
			auto& row = fixedMenTable[i0];
			U32 bb0 = unrank<25, P0, false>(i0);
			for (U64 i1 = 0; i1 < row.size(); i1++) {
				auto& cell = row[i1];
				U32 bb1 = unrank<25 - P0, P1, false>(i1);
				for (auto& elem : cell) {
					// TODO: initialize tablebase
					elem++;
				}
			}
		}
	});
}




void generate(const CardsInfo& cardsInfo) {

	std::cout << sizeof(UNRANK_LUT<25, 5, false>) << std::endl;

	auto table = std::make_unique<Table<4>>();
	initializeWinIn1(*table);
}