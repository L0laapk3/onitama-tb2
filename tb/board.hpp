#pragma once

#include "types.h"
#include "move.h"

#include <array>
#include <bit>



constexpr std::array<U64, 2> PTEMPLE = { 22, 2 };

class Board {
public:
	std::array<U32, 2> bbp; // bitboard of player pieces
	std::array<U32, 2> ik; // index of kings


	// series of helper functions that calculate what move board
	template <bool PLAYER>
	Move inline isTempleKingInRange() {
		// player king can move to temple
		return std::rotl(1U, PTEMPLE[PLAYER] - ik[PLAYER]);
	}

	// no player piece is blocking the temple.
	template <bool PLAYER>
	bool inline isTempleFree() {
		return !(bbp[PLAYER] & (1U << PTEMPLE[PLAYER]));
	}


	template <bool PLAYER>
	Move inline isTempleWinInOne() {
		return isTempleFree<PLAYER>() ? isTempleKingInRange<PLAYER>() : 0;
	}

	// is !player king safe?
	// is player attacking !players king?
	template <bool PLAYER>
	Move inline isTakeWinInOne() {
		return std::rotl(bbp[PLAYER], ik[!PLAYER]);
	}

	template <bool PLAYER>
	Move inline isWinInOne() {
		return isTempleWinInOne<PLAYER>() | isTakeWinInOne<PLAYER>();
	}

	// debug utils
	void print() const;
	Board invert() const;
};