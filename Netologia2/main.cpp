#include <iostream>
#include "Table.h"
#include "Table.cpp"

int main()
{
	auto test = table<int>(2, 3);
	test[0][0] = 4;
	std::cout << test[0][0] << std::endl; // выводит 4
	std::cout << test.Size() << std::endl;

	return EXIT_SUCCESS;
}