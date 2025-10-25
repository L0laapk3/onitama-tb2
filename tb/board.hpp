#pragma once

#include "types.h"
#include "move.h"
#include "card.hpp"

#include <array>
#include <cassert>

constexpr U32 CENTER = 12;
// temple is the spot that when a player moves their king there, they win
constexpr std::array<int, 2> PTEMPLE = { 22, 2 };

inline constexpr std::array<U32, 25> MOVE_MASK{
	std::rotr(0b00000'00000'00111'00111'00111U, 12),
	std::rotr(0b00000'00000'01111'01111'01111U, 12),
	std::rotr(0b00000'00000'11111'11111'11111U, 12),
	std::rotr(0b00000'00000'11110'11110'11110U, 12),
	std::rotr(0b00000'00000'11100'11100'11100U, 12),
	std::rotr(0b00000'00111'00111'00111'00111U, 12),
	std::rotr(0b00000'01111'01111'01111'01111U, 12),
	std::rotr(0b00000'11111'11111'11111'11111U, 12),
	std::rotr(0b00000'11110'11110'11110'11110U, 12),
	std::rotr(0b00000'11100'11100'11100'11100U, 12),
	std::rotr(0b00111'00111'00111'00111'00111U, 12),
	std::rotr(0b01111'01111'01111'01111'01111U, 12),
	std::rotr(0b11111'11111'11111'11111'11111U, 12),
	std::rotr(0b11110'11110'11110'11110'11110U, 12),
	std::rotr(0b11100'11100'11100'11100'11100U, 12),
	std::rotr(0b00111'00111'00111'00111'00000U, 12),
	std::rotr(0b01111'01111'01111'01111'00000U, 12),
	std::rotr(0b11111'11111'11111'11111'00000U, 12),
	std::rotr(0b11110'11110'11110'11110'00000U, 12),
	std::rotr(0b11100'11100'11100'11100'00000U, 12),
	std::rotr(0b00111'00111'00111'00000'00000U, 12),
	std::rotr(0b01111'01111'01111'00000'00000U, 12),
	std::rotr(0b11111'11111'11111'00000'00000U, 12),
	std::rotr(0b11110'11110'11110'00000'00000U, 12),
	std::rotr(0b11100'11100'11100'00000'00000U, 12),
};

class Board {
public:
	std::array<U32, 2> bbp; // bitboard of player pieces
	std::array<int, 2> ik; // index of kings

	// series of helper functions that calculate what reverse move bits required to reach that situation

	// Which move bit is required for player to move the king to the temple?
	template<bool PLAYER>
	Move<!PLAYER> inline kingTemple() const {
		if constexpr (PLAYER) { // check if temple and king are too far away to avoid wrapping issues
			if (ik[PLAYER] - PTEMPLE[PLAYER] > 15)
				return 0;
		} else {
			if (PTEMPLE[PLAYER] - ik[PLAYER] > 15)
				return 0;
		}
		return std::rotr(1U, PTEMPLE[PLAYER] - ik[PLAYER]); // No row wrapping can happen here because the temples are always in the middle of a row
	}

	// Is a player piece in the way of its own temple?
	template<bool PLAYER>
	bool inline isTempleFree() const {
		return !(bbp[PLAYER] & (1U << PTEMPLE[PLAYER]));
	}

	template<bool PLAYER>
	Move<!PLAYER> inline templeWinInOne() const {
		return isTempleFree<PLAYER>() ? kingTemple<PLAYER>() : Move<!PLAYER>{0U};
	}

	// is !players king safe?
	// What move bits can player use to move a piece to take !player's king?
	template<bool PLAYER>
	Move<!PLAYER> inline takeWinInOne() const {
		auto& mask = MOVE_MASK[ik[!PLAYER]];
		return std::rotr(bbp[PLAYER], ik[!PLAYER]) & mask;
	}

	template<bool PLAYER>
	Move<!PLAYER> inline winInOne() const {
		return templeWinInOne<PLAYER>() | takeWinInOne<PLAYER>();
	}


	void print() const;
};