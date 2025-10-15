#pragma once

#include <array>
#include <cstdint>



struct Cell {
	uint8_t color;
	uint8_t animal;
};

struct Game {
	std::array<Cell, 49> cells;

};