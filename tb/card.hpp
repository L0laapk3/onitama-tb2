#pragma once

#include "move.h"

#include <array>
#include <string_view>
#include <stdexcept>
#include <ranges>
#include <bit>
#include <algorithm>


using Card = Move;

constexpr Card BOAR     = std::rotr(0b00000'00100'01010'00000'00000U, 12);
constexpr Card COBRA    = std::rotr(0b00000'01000'00010'01000'00000U, 12);
constexpr Card CRAB     = std::rotr(0b00000'00100'10001'00000'00000U, 12);
constexpr Card CRANE    = std::rotr(0b00000'00100'00000'01010'00000U, 12);
constexpr Card DRAGON   = std::rotr(0b00000'10001'00000'01010'00000U, 12);
constexpr Card EEL      = std::rotr(0b00000'00010'01000'00010'00000U, 12);
constexpr Card ELEPHANT = std::rotr(0b00000'01010'01010'00000'00000U, 12);
constexpr Card FROG     = std::rotr(0b00000'00010'00001'01000'00000U, 12);
constexpr Card GOOSE    = std::rotr(0b00000'00010'01010'01000'00000U, 12);
constexpr Card HORSE    = std::rotr(0b00000'00100'00010'00100'00000U, 12);
constexpr Card MANTIS   = std::rotr(0b00000'01010'00000'00100'00000U, 12);
constexpr Card MONKEY   = std::rotr(0b00000'01010'00000'01010'00000U, 12);
constexpr Card OX       = std::rotr(0b00000'00100'01000'00100'00000U, 12);
constexpr Card RABBIT   = std::rotr(0b00000'01000'10000'00010'00000U, 12);
constexpr Card ROOSTER  = std::rotr(0b00000'01000'01010'00010'00000U, 12);
constexpr Card TIGER    = std::rotr(0b00100'00000'00000'00100'00000U, 12);

constexpr std::array<Card, 16> ALL_CARDS { BOAR, COBRA, CRAB, CRANE, DRAGON, EEL, ELEPHANT, FROG, GOOSE, HORSE, MANTIS, MONKEY, OX, RABBIT, ROOSTER, TIGER };

Card inline strToCard(std::string_view str) {
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


struct CardPlayerMoves : public std::array<Move, 30> {
	using std::array<Move, 30>::array;

	template<bool PLAYER>
	static CardPlayerMoves make(std::array<Card, 5> cards) {
		CardPlayerMoves result;
		auto view = CARD_PERMUTATIONS | std::views::transform([&](const CardPermutation& perm) {
			return cards[perm.playerCards[PLAYER][0]] | cards[perm.playerCards[PLAYER][1]];
		});
		std::ranges::copy(view, result.begin());
		return result;
	}

	U32 inline validPerms(Move move) const {
		U32 res = 0;
		for (U32 i = 0; i < size(); i++) {
			if ((*this)[i] & move) {
				res |= (1U << i);
			}
		}
		return res;
	}
};