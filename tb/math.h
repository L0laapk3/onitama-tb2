#pragma once

#include "types.h"

constexpr U64 fact(int from, int downto = 0) {
	return from <= downto ? 1 : from * fact(from - 1, downto);
}


constexpr U64 choose(int n, int k) {
	return fact(n, n - k) / fact(k);
}