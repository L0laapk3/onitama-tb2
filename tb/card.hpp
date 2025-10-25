#pragma once

#include "card.h"
#include "permutation.h"
#include "move.h"
#include "types.h"

#include <array>
#include <bit>



template<bool PLAYER>
constexpr auto CARDS_USED_IN = []{
	std::array<Perm, 5> res{};
	for (U32 cardI = 0; cardI < 5; cardI++) {
		for (U32 permI = 0; permI < CARD_PERMUTATIONS.size(); permI++) {
			const auto& perm = CARD_PERMUTATIONS[permI];
			if (perm.playerCards[PLAYER][0] == cardI || perm.playerCards[PLAYER][1] == cardI) {
				res[cardI] |= Perm::fromIndex(permI);
			}
		}
	}
	return res;
}();


template<bool REVERSE = false>
struct Cards : public std::array<Card<REVERSE>, 5> {
	template<bool PLAYER>
	Perm inline moveToPerms(Move<!PLAYER> move) const {
		static_assert(PLAYER == !REVERSE);
		Perm res = 0;
		#pragma unroll
		for (U32 i = 0; i < this->size(); i++)
			if ((*this)[i] & move)
				res |= CARDS_USED_IN<PLAYER>[i];
		return res;
	}

	constexpr Cards<!REVERSE> reverse() const {
		return {
			(*this)[0].reverse(),
			(*this)[1].reverse(),
			(*this)[2].reverse(),
			(*this)[3].reverse(),
			(*this)[4].reverse(),
		};
	}
};


struct CardPreCalc {
	CardPreCalc(const Cards<>& cards) : cards(cards), rCards(cards.reverse()), permsWhereCardMovesTo([&]{
		constexpr bool PLAYER = 0;
		std::array<std::array<Perm, 25>, 2> res{};
		for (U32 cardSlot = 0; cardSlot < 2; cardSlot++) {
			for (U32 permI = 0; permI < CARD_PERMUTATIONS.size(); permI++) {
				const auto& cardPerm = CARD_PERMUTATIONS[permI];
				U32 card = std::rotl(cards[cardPerm.playerCards[PLAYER][cardSlot]].bits, 12);
				for (U32 to = 0; to < 25; to++) {
					if (card & (1U << to))
						res[cardSlot][to] |= Perm::fromIndex(permI);
				}
			}
		}
		return res;
	}()) {}

	Cards<0> cards;
	Cards<1> rCards;

	std::array<std::array<Perm, 25>, 2> permsWhereCardMovesTo; // [player][to] -> permutations

	template<bool PLAYER>
	constexpr Cards<PLAYER>& get() {
		if constexpr (PLAYER)
			return rCards;
		else
			return cards;
	}
	template<bool PLAYER>
	constexpr const Cards<PLAYER>& get() const {
		return const_cast<CardPreCalc*>(this)->get<PLAYER>();
	}
};