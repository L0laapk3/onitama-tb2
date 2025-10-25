#pragma once

#include "move.h"
#include "types.h"

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

constexpr Card<> BOAR     = std::rotr(0b00000'00100'01010'00000'00000U, 12);
constexpr Card<> COBRA    = std::rotr(0b00000'01000'00010'01000'00000U, 12);
constexpr Card<> CRAB     = std::rotr(0b00000'00100'10001'00000'00000U, 12);
constexpr Card<> CRANE    = std::rotr(0b00000'00100'00000'01010'00000U, 12);
constexpr Card<> DRAGON   = std::rotr(0b00000'10001'00000'01010'00000U, 12);
constexpr Card<> EEL      = std::rotr(0b00000'00010'01000'00010'00000U, 12);
constexpr Card<> ELEPHANT = std::rotr(0b00000'01010'01010'00000'00000U, 12);
constexpr Card<> FROG     = std::rotr(0b00000'00010'00001'01000'00000U, 12);
constexpr Card<> GOOSE    = std::rotr(0b00000'00010'01010'01000'00000U, 12);
constexpr Card<> HORSE    = std::rotr(0b00000'00100'00010'00100'00000U, 12);
constexpr Card<> MANTIS   = std::rotr(0b00000'01010'00000'00100'00000U, 12);
constexpr Card<> MONKEY   = std::rotr(0b00000'01010'00000'01010'00000U, 12);
constexpr Card<> OX       = std::rotr(0b00000'00100'01000'00100'00000U, 12);
constexpr Card<> RABBIT   = std::rotr(0b00000'01000'10000'00010'00000U, 12);
constexpr Card<> ROOSTER  = std::rotr(0b00000'01000'01010'00010'00000U, 12);
constexpr Card<> TIGER    = std::rotr(0b00100'00000'00000'00100'00000U, 12);



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