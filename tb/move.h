#pragma once

#include "types.h"

#include <bit>
#include <iostream>



// moves are U32's, centered around bit 0. Meaning to Move 1 spot, Move{2} would do that. Move{1 << 31} moves -1 spot.
template<bool REVERSE = false>
struct Move {
	U32 bits;
	constexpr Move(U32 value = 0) : bits(value) {}
	constexpr operator U32() const { return bits; }
	constexpr Move& operator|=(U32 other) { bits |= other; return *this; }
	constexpr Move& operator&=(U32 other) { bits &= other; return *this; }
	constexpr Move& operator^=(U32 other) { bits ^= other; return *this; }

	constexpr auto reverse() const {
		Move<!REVERSE> res = bits & 1U;
		for (U32 i = 1; i < 32; i++)
			res |= ((bits >> i) & 1U) << (32 - i);
		return res;
	}


	void print() const {
		for (int r = 5; r-- > 0;) {
			for (int c = 0; c < 5; c++) {
				const int mask = std::rotr(1U << (5 * r + c), 12);
				if (bits & mask) std::cout << 'M';
				else             std::cout << '.';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
};