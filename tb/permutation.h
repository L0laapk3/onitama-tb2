#pragma once

#include "types.h"
#include "card.h"

#include <array>




struct CardPermutation {
	std::array<std::array<U8, 2>, 2> playerCards;
	U8 sideCard;
};

// P0 move : shift by 10 or 20
// P1 unmove ??: within groups of 10, no optimization is made yet for this
constexpr std::array<CardPermutation, 30> CARD_PERMUTATIONS = {{
	{ 2, 3, 0, 1, 4 },
	{ 1, 3, 0, 2, 4 },
	{ 1, 2, 0, 3, 4 },
	{ 1, 2, 0, 4, 3 },
	{ 0, 3, 1, 2, 4 },
	{ 0, 2, 1, 3, 4 },
	{ 0, 2, 1, 4, 3 },
	{ 0, 1, 2, 3, 4 },
	{ 0, 1, 2, 4, 3 },
	{ 0, 1, 3, 4, 2 },

	{ 3, 4, 0, 1, 2 },
	{ 3, 4, 0, 2, 1 },
	{ 2, 4, 0, 3, 1 },
	{ 2, 3, 0, 4, 1 },
	{ 3, 4, 1, 2, 0 },
	{ 2, 4, 1, 3, 0 },
	{ 2, 3, 1, 4, 0 },
	{ 1, 4, 2, 3, 0 },
	{ 1, 3, 2, 4, 0 },
	{ 1, 2, 3, 4, 0 },

	{ 4, 2, 0, 1, 3 },
	{ 4, 1, 0, 2, 3 },
	{ 4, 1, 0, 3, 2 },
	{ 3, 1, 0, 4, 2 },
	{ 4, 0, 1, 2, 3 },
	{ 4, 0, 1, 3, 2 },
	{ 3, 0, 1, 4, 2 },
	{ 4, 0, 2, 3, 1 },
	{ 3, 0, 2, 4, 1 },
	{ 2, 0, 3, 4, 1 },
}};


struct Perm {
	U32 bits;
	constexpr Perm(U32 value = 0) : bits(value) {}
	constexpr operator U32() const { return bits; }
	constexpr Perm& operator|=(U32 other) { bits |= other; return *this; }
	constexpr Perm& operator&=(U32 other) { bits &= other; return *this; }
	constexpr Perm& operator^=(U32 other) { bits ^= other; return *this; }

	constexpr static U32 FULL = 0b11111'11111'0'11111'11111'0'11111'11111U;
	constexpr inline Perm invert() const {
		return bits ^ FULL;
	}
	constexpr inline bool isFull() const {
		return bits == FULL;
	}

	constexpr inline static Perm fromIndex(int index) {
		return 1U << (index + (index / 10));
	}

	constexpr inline Perm useP0Card0() const {
		return bits << 11 | bits >> 22;
	}
	constexpr inline Perm useP0Card1() const {
		return bits << 22 | bits >> 11;
	}
	// TODO
	constexpr inline Perm unuseP1Card0() const;
	constexpr inline Perm unuseP1Card1() const;
};