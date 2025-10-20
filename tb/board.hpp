#pragma once

#include "types.h"
#include "move.h"
#include "pdep.hpp"

#include <array>

constexpr U32 CENTER = 12;
// temple is the spot that when a player moves their king there, they win
constexpr std::array<U32, 2> PTEMPLE = { 22, 2 };

template<bool REVERSE>
constexpr auto COL_MASKS = []{
	std::array<Move<REVERSE>, 25> res;
	for (U32 i = 0; i < 25; i += 5) {
		res[i    ] = REVERSE ? 0b11100'11100'11100'11100'11100U : 0b00111'00111'00111'00111'00111U;
		res[i + 1] = REVERSE ? 0b11110'11110'11110'11110'11110U : 0b01111'01111'01111'01111'01111U;
		res[i + 2] = REVERSE ? 0b11111'11111'11111'11111'11111U : 0b11111'11111'11111'11111'11111U;
		res[i + 3] = REVERSE ? 0b01111'01111'01111'01111'01111U : 0b11110'11110'11110'11110'11110U;
		res[i + 4] = REVERSE ? 0b00111'00111'00111'00111'00111U : 0b11100'11100'11100'11100'11100U;
	}
	return res;
}();

class Board {
public:
	std::array<U32, 2> bbp; // bitboard of player pieces
	std::array<U32, 2> ik; // index of kings

	// series of helper functions that calculate what reverse move bits required to reach that situation

	// Which move bit is required for player to move the king to the temple?
	template<bool PLAYER>
	Move<!PLAYER> inline kingTemple() {
		return 1U << (CENTER + ik[PLAYER] - PTEMPLE[PLAYER]); // No row wrapping can happen here because the temples are always in the middle of a row
	}

	// Is a player piece in the way of its own temple?
	template<bool PLAYER>
	bool inline isTempleFree() {
		return !(bbp[PLAYER] & (1U << PTEMPLE[PLAYER]));
	}

	template<bool PLAYER>
	Move<!PLAYER> inline templeWinInOne() {
		return isTempleFree<PLAYER>() ? kingTemple<PLAYER>() : Move<!PLAYER>{0U};
	}

	// is !players king safe?
	// What move bits can player use to move a piece to take !player's king?
	template<bool PLAYER>
	Move<!PLAYER> inline takeWinInOne() {
		return signed_shl(bbp[PLAYER] & COL_MASKS<!PLAYER>[ik[!PLAYER]], ik[!PLAYER] - CENTER);
	}

	template<bool PLAYER>
	Move<!PLAYER> inline winInOne() {
		return templeWinInOne<PLAYER>() | takeWinInOne<PLAYER>();
	}
};