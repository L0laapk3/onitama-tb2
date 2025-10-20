#pragma once

#include "move.h"

#include <array>
#include <string_view>
#include <stdexcept>
#include <ranges>
#include <bit>
#include <algorithm>


template<bool REVERSE = false>
struct Card : public Move<REVERSE> {
	using Move<REVERSE>::Move;

	constexpr Card<!REVERSE> reverse() const {
		return { Move<REVERSE>::reverse() };
	}
};

constexpr Card<> BOAR     = 0b00000'00100'01010'00000'00000U;
constexpr Card<> COBRA    = 0b00000'01000'00010'01000'00000U;
constexpr Card<> CRAB     = 0b00000'00100'10001'00000'00000U;
constexpr Card<> CRANE    = 0b00000'00100'00000'01010'00000U;
constexpr Card<> DRAGON   = 0b00000'10001'00000'01010'00000U;
constexpr Card<> EEL      = 0b00000'00010'01000'00010'00000U;
constexpr Card<> ELEPHANT = 0b00000'01010'01010'00000'00000U;
constexpr Card<> FROG     = 0b00000'00010'00001'01000'00000U;
constexpr Card<> GOOSE    = 0b00000'00010'01010'01000'00000U;
constexpr Card<> HORSE    = 0b00000'00100'00010'00100'00000U;
constexpr Card<> MANTIS   = 0b00000'01010'00000'00100'00000U;
constexpr Card<> MONKEY   = 0b00000'01010'00000'01010'00000U;
constexpr Card<> OX       = 0b00000'00100'01000'00100'00000U;
constexpr Card<> RABBIT   = 0b00000'01000'10000'00010'00000U;
constexpr Card<> ROOSTER  = 0b00000'01000'01010'00010'00000U;
constexpr Card<> TIGER    = 0b00100'00000'00000'00100'00000U;



constexpr std::array<Card<>, 16> ALL_CARDS { BOAR, COBRA, CRAB, CRANE, DRAGON, EEL, ELEPHANT, FROG, GOOSE, HORSE, MANTIS, MONKEY, OX, RABBIT, ROOSTER, TIGER };

Card<> inline strToCard(std::string_view str) {
	if (str == "boar")		return BOAR;
	if (str == "cobra")		return COBRA;
	if (str == "crab")		return CRAB;
	if (str == "crane")		return CRANE;
	if (str == "dragon")	return DRAGON;
	if (str == "eel")		return EEL;
	if (str == "elephant")	return ELEPHANT;
	if (str == "frog")		return FROG;
	if (str == "goose")		return GOOSE;
	if (str == "horse")		return HORSE;
	if (str == "mantis")	return MANTIS;
	if (str == "monkey")	return MONKEY;
	if (str == "ox")		return OX;
	if (str == "rabbit")	return RABBIT;
	if (str == "rooster")	return ROOSTER;
	if (str == "tiger")		return TIGER;
	throw std::runtime_error("Unknown card: " + std::string(str));
}



struct CardPermutation {
	std::array<std::array<U8, 2>, 2> playerCards;
	U8 sideCard;
};

constexpr std::array<CardPermutation, 30> CARD_PERMUTATIONS = {{
	{ 2, 3, 0, 1, 4 },
	{ 1, 2, 0, 4, 3 },
	{ 0, 3, 1, 2, 4 },
	{ 2, 4, 0, 1, 3 },
	{ 0, 2, 1, 3, 4 },
	{ 1, 4, 0, 2, 3 },
	{ 0, 3, 1, 4, 2 },
	{ 1, 2, 0, 3, 4 },
	{ 0, 4, 1, 2, 3 },
	{ 1, 4, 0, 3, 2 },
	{ 0, 1, 2, 3, 4 },
	{ 0, 2, 1, 4, 3 },
	{ 1, 3, 0, 2, 4 },
	{ 0, 1, 2, 4, 3 },
	{ 0, 1, 3, 4, 2 },
	{ 0, 2, 3, 4, 1 },
	{ 1, 2, 3, 4, 0 },
	{ 2, 4, 0, 3, 1 },
	{ 1, 3, 2, 4, 0 },
	{ 0, 4, 1, 3, 2 },
	{ 2, 3, 0, 4, 1 },
	{ 1, 4, 2, 3, 0 },
	{ 2, 3, 1, 4, 0 },
	{ 0, 4, 2, 3, 1 },
	{ 1, 3, 0, 4, 2 },
	{ 3, 4, 0, 2, 1 },
	{ 2, 4, 1, 3, 0 },
	{ 3, 4, 0, 1, 2 },
	{ 0, 3, 2, 4, 1 },
	{ 3, 4, 1, 2, 0 },
}};

template<bool PLAYER>
constexpr auto CARDS_USED_IN = []{
	std::array<U32, 5> res{};
	for (U32 cardI = 0; cardI < 5; cardI++) {
		for (U32 permI = 0; permI < CARD_PERMUTATIONS.size(); permI++) {
			const auto& perm = CARD_PERMUTATIONS[permI];
			if (perm.playerCards[PLAYER][0] == cardI || perm.playerCards[PLAYER][1] == cardI) {
				res[cardI] |= 1U << permI;
			}
		}
	}
	return res;
}();


template<bool REVERSE = false>
struct Cards : public std::array<Card<REVERSE>, 5> {

	template<bool PLAYER>
	U32 inline validPerms(Move<!PLAYER> move) const {
		static_assert(PLAYER == !REVERSE);
		U32 res = 0;
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