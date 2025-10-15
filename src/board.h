#pragma once

#include "types.h"
#include "card.hpp"

#include <array>
#include <x86intrin.h>
#include <xmmintrin.h>


constexpr std::array<U64, 2> PTEMPLE = { 22, 2 };

class Board {
public:
	std::array<U64, 2> bbp;
	std::array<U64, 2> bbk;

	static U64 inline isKingAttackedBy(U64 bbk, U64 bbp, const MoveBoard& reverseMoveBoard);
	template <bool player>
	U64 inline isKingAttacked(U64 bbk, const MoveBoard& reverseMoveBoard);
	template <bool player>
	bool inline isTempleKingInRange(const MoveBoard& reverseMoveBoard);
	template <bool player>
	bool inline isTempleFree();

	template <bool player>
	bool inline isTempleWinInOne(const MoveBoard& reverseMoveBoard);
	template <bool player>
	U64 inline isTakeWinInOne(const MoveBoard& reverseMoveBoard);
	template <bool player>
	bool inline isWinInOne(const MoveBoard& reverseMoveBoard);
	template <bool player>
	bool inline isWinInTwo(const MoveBoard& reverseMoveBoard, const MoveBoard& reverseOtherMoveBoard);
	template <bool player>
	bool inline isWinInThree(const MoveBoard& reverseMoveBoard, const MoveBoard& reverseMoveBoardcard0, const MoveBoard& reverseMoveBoardcard1, const MoveBoard& forwardOtherMoveBoard, const MoveBoard& reverseOtherMoveBoard);

	// debug utils
	void print() const;
	Board invert() const;
};


template <bool player>
bool inline Board::isTempleKingInRange(const MoveBoard& reverseMoveBoard) {
	// player king can move to temple
	return reverseMoveBoard[PTEMPLE[player]] & bbk[player];
}


template <bool player>
bool inline Board::isTempleFree() {
	// no player piece is blocking the temple.
	return !(bbp[player] & (1 << PTEMPLE[player]));
}


template <bool player>
bool inline Board::isTempleWinInOne(const MoveBoard& reverseMoveBoard) {
	return isTempleKingInRange<player>(reverseMoveBoard) && isTempleFree<player>();
}


U64 inline Board::isKingAttackedBy(U64 bbk, U64 bbp, const MoveBoard& reverseMoveBoard) {
	U64 pk = _tzcnt_u64(bbk);
	return reverseMoveBoard[pk] & bbp;
}

// is !player king safe?
// is player attacking !players king?
template <bool player>
U64 inline Board::isKingAttacked(U64 bbk, const MoveBoard& reverseMoveBoard) {
	return isKingAttackedBy(bbk, bbp[player], reverseMoveBoard);
}



template <bool player>
U64 inline Board::isTakeWinInOne(const MoveBoard& reverseMoveBoard) {
	return isKingAttacked<player>(bbk[!player], reverseMoveBoard);
}

template <bool player>
bool inline Board::isWinInOne(const MoveBoard& reverseMoveBoard) {
	if (isTempleWinInOne<player>(reverseMoveBoard))
		return true;
	return isTakeWinInOne<player>(reverseMoveBoard);
}