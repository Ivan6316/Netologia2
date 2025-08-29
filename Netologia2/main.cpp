#include <iostream>
#include <set>
#include <list>
#include <vector>

template<typename T>
void print_container(T arr);

int main()
{
	std::set<std::string> test_set = { "one", "two", "three", "four" };
	print_container(test_set); // four one three two. помните почему такой порядок? :)

	std::list<std::string> test_list = { "one", "two", "three", "four" };
	print_container(test_list); // one, two, three, four

	std::vector<std::string> test_vector = { "one", "two", "three", "four" };
	print_container(test_vector); // one, two, three, four

	return EXIT_SUCCESS;
}

template<typename T>
void print_container(T arr)
{
	for (const auto& element : arr)
	{
		std::cout << element << " ";
	}
	std::cout << std::endl;
}