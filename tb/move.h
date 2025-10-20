#pragma once

#include "types.h"

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
		Move<!REVERSE> res = 0;
		for (U32 i = 0; i < 25; i++)
			res |= ((bits >> i) & 1U) << (24 - i);
		return res;
	}
};