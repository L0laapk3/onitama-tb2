#pragma once

#include "types.h"
#include <array>
#include <memory>
#include <algorithm>



template<U32 N>
constexpr inline auto multByPreviousArea(U32 i) {
	auto index = i;
	for (U32 j = 1; j < N; j++)
		index *= (i - j) / (j + 1);
	return index;
}

// my king cannot be on his base
// his king cannot be on my base
// kings cannot overlap
constexpr U32 KINGSMULT = 23*24+1;


template<U32 p0, U32 p1>
constexpr auto SIZE_FIXED_PIECECOUNT = KINGSMULT * static_cast<U64>(multByPreviousArea<p0>(23)) * multByPreviousArea<p1>(23 - p0);

constexpr auto SIZES = std::array<std::array<U64, 5>, 5>{
		std::array<U64, 5>{SIZE_FIXED_PIECECOUNT<0, 0>, SIZE_FIXED_PIECECOUNT<0, 1>, SIZE_FIXED_PIECECOUNT<0, 2>, SIZE_FIXED_PIECECOUNT<0, 3>, SIZE_FIXED_PIECECOUNT<0, 4>},
		std::array<U64, 5>{SIZE_FIXED_PIECECOUNT<1, 0>, SIZE_FIXED_PIECECOUNT<1, 1>, SIZE_FIXED_PIECECOUNT<1, 2>, SIZE_FIXED_PIECECOUNT<1, 3>, SIZE_FIXED_PIECECOUNT<1, 4>},
		std::array<U64, 5>{SIZE_FIXED_PIECECOUNT<2, 0>, SIZE_FIXED_PIECECOUNT<2, 1>, SIZE_FIXED_PIECECOUNT<2, 2>, SIZE_FIXED_PIECECOUNT<2, 3>, SIZE_FIXED_PIECECOUNT<2, 4>},
		std::array<U64, 5>{SIZE_FIXED_PIECECOUNT<3, 0>, SIZE_FIXED_PIECECOUNT<3, 1>, SIZE_FIXED_PIECECOUNT<3, 2>, SIZE_FIXED_PIECECOUNT<3, 3>, SIZE_FIXED_PIECECOUNT<3, 4>},
		std::array<U64, 5>{SIZE_FIXED_PIECECOUNT<4, 0>, SIZE_FIXED_PIECECOUNT<4, 1>, SIZE_FIXED_PIECECOUNT<4, 2>, SIZE_FIXED_PIECECOUNT<4, 3>, SIZE_FIXED_PIECECOUNT<4, 4>},
};

template<U32 TB_MEN>
constexpr auto TOTAL_SIZE = []{
	U64 total = 0;
	for (auto i = 0; i < TB_MEN/2; i++)
		for (auto j = 0; j < TB_MEN/2; j++)
			total += SIZES[i][j];
	return total;
}();

template<U32 TB_MEN>
struct TableBase {
	TableBase() : table(std::make_unique<Table>()) {
		std::ranges::fill(*table, 0);
	}

	using Table = std::array<U32, TOTAL_SIZE<TB_MEN>>; // 1 bit per card combination - 30 combinations
	std::unique_ptr<Table> table;
};