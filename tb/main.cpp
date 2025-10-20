#include "card.hpp"
#include "generate.h"


#include <iostream>


int main(int argc, char** argv) {
	auto tb_men = argc > 1 ? std::atoi(argv[1]) : 8;
	Cards cards{{ BOAR, OX, ELEPHANT, HORSE, CRAB }};
	if (argc > 2) {
		if (argc < 7) {
			std::cerr << "Usage: tb [tb_men] [card1] [card2] [card3] [card4] [card5]" << std::endl;
			return 1;
		}
		for (int i = 0; i < 5; i++) {
			cards[i] = strToCard(argv[i + 2]);
		}
	}


	generate(cards);

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