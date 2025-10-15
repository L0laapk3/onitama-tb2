#pragma once

#include "index.hpp"



template<U32 TB_MEN>
struct TableBase {
	using Table = std::array<U32, TOTAL_SIZE<TB_MEN>>; // 1 bit per card combination - 30 combinations
	std::unique_ptr<Table> table;

	TableBase() : table(std::make_unique<Table>()) {
		std::ranges::fill(*table, 0);

		initialize_win_in_1();
	}

	void initialize_win_in_1() {
		// Fill the table with winning positions for player 1
	}
};