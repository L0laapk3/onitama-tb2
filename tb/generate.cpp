#include "generate.h"

#include "table.h"
#include "types.h"



template<U32 HALFMEN>
void initializeWinIn1(Table<HALFMEN>& table) {
	table.iterateFixedMenTables([&]<U32 P0, U32 P1>(auto& fixedMenTable) {
		for (auto& row : fixedMenTable) {
			for (auto& cell : row) {
				for (auto& elem : cell) {
					// TODO: initialize tablebase
				}
			}
		}
	});
}




void generate() {

}