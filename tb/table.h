#pragma once

#include "types.h"
#include "rank.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <tuple>
#include <utility>


template<U32 HALFMEN>
struct Table;

template<U32 P0, U32 P1>
using Cell = std::array<U32, P0 * P1>;
template<U32 P0, U32 P1>
using Row = CombArray<Cell<P0, P1>, 25 - P0, P1>;
template<U32 P0, U32 P1>
struct FixedMenTable : CombArray<Row<P0, P1>, 25, P0> {
	using CombArray<Row<P0, P1>, 25, P0>::CombArray;

	template<bool P, int incr, U32 HALFMEN>
	auto& neighbor(Table<HALFMEN>& table);
	template<bool P, int incr, U32 HALFMEN>
	const auto& neighbor(const Table<HALFMEN>& table) const;
};


// Generate PIECECOUNTS in sorted order by P0+P1 at compile time
template<U32 HALFMEN>
constexpr auto PIECECOUNTS = []{
	std::array<std::pair<U32, U32>, HALFMEN * HALFMEN> res;
	auto entry = res.begin();
	for (U32 sum = 2; sum <= 2 * HALFMEN; ++sum) { // Iterate through sums from 2 to 2*HALFMEN
		for (U32 p0 = 1; p0 < sum; p0++) {          // Iterate through combinations that produce that sum
			U32 p1 = sum - p0;
			if (p0 <= HALFMEN && p1 <= HALFMEN)
				*entry++ = {p0, p1};
		}
	}
	return res;
}();

template<U32 HALFMEN, std::size_t... Is>
static auto makeTable(std::index_sequence<Is...>) -> std::tuple<FixedMenTable<PIECECOUNTS<HALFMEN>[Is].first, PIECECOUNTS<HALFMEN>[Is].second>...>;


template<U32 HALFMEN>
struct Table : public decltype(makeTable<HALFMEN>(std::make_index_sequence<HALFMEN*HALFMEN>{})) {
	using Base = decltype(makeTable<HALFMEN>(std::make_index_sequence<HALFMEN*HALFMEN>{}));
	using Base::Base;

	template<U32 P0, U32 P1>
	auto& get() {
		return std::get<std::ranges::find_if(PIECECOUNTS<HALFMEN>, [&](const auto& p){ return p.first == P0 && p.second == P1; })>(static_cast<const Base&>(*this));
	}
	template<U32 P0, U32 P1>
	const auto& get() const {
		return const_cast<Table*>(this)->get<P0, P1>();
	}

	void iterateFixedMenTables(auto&& func) {
		[&]<std::size_t... Is>(std::index_sequence<Is...>) {
			([&]() {
				auto& fixedMenTable = std::get<Is>(static_cast<Base&>(*this));
				func.template operator()<PIECECOUNTS<HALFMEN>[Is].first, PIECECOUNTS<HALFMEN>[Is].second>(fixedMenTable);
			}(), ...);
		}(std::make_index_sequence<HALFMEN * HALFMEN>{});
	}
};


template<U32 P0, U32 P1>
template<bool P, int incr, U32 HALFMEN>
auto& FixedMenTable<P0, P1>::neighbor(Table<HALFMEN>& table) {
	return table.template get<P0 + (P ? 0 : incr), P1 + (P ? incr : 0)>();
}
template<U32 P0, U32 P1>
template<bool P, int incr, U32 HALFMEN>
const auto& FixedMenTable<P0, P1>::neighbor(const Table<HALFMEN>& table) const {
	return const_cast<FixedMenTable<P0, P1>*>(this)->neighbor<P, incr>(const_cast<Table<HALFMEN>&>(table));
}



static_assert(sizeof(Table<5>) == 138472072800);