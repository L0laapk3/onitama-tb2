#pragma once

#include "card.hpp"
#include "index.hpp"

#include <iostream>
#include <memory>
#include <array>



template<U32 TB_MEN>
struct TableBase {
	using Table = std::array<U32, TOTAL_SIZE<TB_MEN>>; // 1 bit per card combination - 30 combinations
	std::unique_ptr<Table> table;

	TableBase(const CardsInfo& cardsInfo) : table() {
		std::cout << "TB Size: " << sizeof(Table) / 1024 / 1024 << " MiB" << std::endl;

		// table = std::make_unique<Table>();
		// initialize_win_in_1();
	}

	void initialize_win_in_1() {
		// // Fill the table with winning positions for player 1
		// for (auto i = 0; i < table->size(); i++) {
		// 	auto board = indexToBoard<TB_MEN, 1>(bi);
		// }
	}
};