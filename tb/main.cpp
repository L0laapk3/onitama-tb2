#include "helper.h"
#include "tablebase.h"
#include "card.hpp"


#include <iostream>


int main(int argc, char** argv) {
	auto tb_men = argc > 1 ? std::atoi(argv[1]) : 8;
	std::array<U32, 5> cards;
	if (argc > 2) {
		if (argc < 7) {
			std::cerr << "Usage: tb [tb_men] [card1] [card2] [card3] [card4] [card5]" << std::endl;
			return 1;
		}
		for (int i = 0; i < 5; i++) {
			cards[i] = strToCard(argv[i + 2]);
		}
	} else {
		cards = { BOAR, OX, ELEPHANT, HORSE, CRAB };
	}

	const CardsInfo cardsInfo{ cards };



	U64 cells = 0;
	for (U32 p0c = 5; p0c <= 5; p0c++)
		for (U32 p1c = 5; p1c <= 5; p1c++)
			cells += p0c * p1c * fact(25, 25 - p0c - p1c) / fact(p0c) / fact(p1c);

	std::cout << "Size: " << cells * 32/8 / 1024 / 1024 / 1024.0 << " GiB" << std::endl;




	U64 cellsKingSeparated = 0;
	for (U32 p0c = 5; p0c <= 5; p0c++)
		for (U32 p1c = 5; p1c <= 5; p1c++)
			cellsKingSeparated += (24*23+1) * fact(23, 23 - p0c - p1c + 2) / fact(p0c - 1) / fact(p1c - 1);
	std::cout << "Size: " << cellsKingSeparated * 32/8 / 1024 / 1024 / 1024.0 << " GiB" << std::endl;



	// if (1)
    //     testIndexing(cardsInfo);
	// if (1) {
	// 	std::cout << "start" << std::endl;
	// 	exhaustiveIndexTest(cardsInfo);
	// 	std::cout << "done" << std::endl;
	// }


	// if (tb_men == 4)
	// 	TableBase<4> tb(cardsInfo);
	// else if (tb_men == 6)
	// 	TableBase<6> tb(cardsInfo);
	// else if (tb_men == 8)
	// 	TableBase<8> tb(cardsInfo);
	// else if (tb_men == 10)
	// 	TableBase<10> tb(cardsInfo);
	// else {
	// 	std::cerr << "Unsupported size: " << tb_men << std::endl;
	// 	return 1;
	// }
	return 0;
}