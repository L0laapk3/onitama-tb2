#pragma once

#include "card.hpp"
#include "types.h"
#include "math.h"

#include <array>
#include <tuple>
#include <utility>



template<U32 P0, U32 P1>
using Cell = std::array<U32, P0 * P1>;
template<U32 P0, U32 P1>
using Row = std::array<Cell<P0, P1>, choose(25 - P0, P1)>;
template<U32 P0, U32 P1>
using FixedMenTable = std::array<Row<P0, P1>, choose(25, P0)>;

template<U32 P0, std::size_t... Is>
static auto makeTableRow(std::index_sequence<Is...>) -> std::tuple<FixedMenTable<P0, Is + 1>...>;
template<U32 HALFMEN, U32 P0>
using TableRow = decltype(makeTableRow<P0>(std::make_index_sequence<HALFMEN>{}));

template<U32 HALFMEN, std::size_t... Is>
static auto makeTable(std::index_sequence<Is...>) -> std::tuple<TableRow<HALFMEN, Is + 1>...>;
template<U32 HALFMEN>
using Table = decltype(makeTable<HALFMEN>(std::make_index_sequence<HALFMEN>{}));


static_assert(sizeof(Table<5>) == 138472072800);