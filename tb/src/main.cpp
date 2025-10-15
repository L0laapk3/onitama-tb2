#include "index.hpp"

#include <iostream>



int main() {
	std::cout << "TB Size: " << sizeof(TableBase<10>::Table) / 1024 / 1024 / 1024 << " GiB" << std::endl;
	return 0;
}