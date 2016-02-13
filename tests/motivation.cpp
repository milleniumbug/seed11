#include <iostream>
#include <iterator>
#include <random>
#include <algorithm>
#include <functional>

int main()
{
	std::random_device rd;
	std::generate_n(
		std::ostream_iterator<unsigned>(std::cout, " "),
		4,
		std::ref(rd));
	std::cout << "\n";
}
