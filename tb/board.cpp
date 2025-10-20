#include "board.hpp"

#include <iostream>



void Board::print() const {
	U32 bbk[2] = { 1U << ik[0], 1U << ik[1] };
	for (int r = 5; r-- > 0;) {
		for (int c = 0; c < 5; c++) {
			const int mask = 1 << (5 * r + c);
			if (bbp[0] & mask) {
				if ((bbp[1] | bbk[1]) & mask)    std::cout << '?';
				else if (bbk[0] & mask)          std::cout << '0';
				else                             std::cout << 'o';
			} else if (bbp[1] & mask) {
				if (bbk[1] & mask)               std::cout << 'X';
				else                             std::cout << '+';
			} else if ((bbk[0] | bbk[1]) & mask) std::cout << 'F';
			else                                 std::cout << '.';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}