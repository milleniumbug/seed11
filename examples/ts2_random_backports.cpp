#include <array>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <seed11/seed11.hpp>

int main()
{
	std::array<int, 10> arr = { 1, 4, 5, 7, 9, 11, 14, 15, 16, 20 };
	seed11::shuffle(arr.begin(), arr.end());
	std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << "\n";

	std::generate_n(std::ostream_iterator<int>(std::cout, " "), 5, [](){ return seed11::randint(1, 6); });
	std::cout << "\n";

	seed11::reseed(42);
	std::generate_n(std::ostream_iterator<int>(std::cout, " "), 5, [](){ return seed11::randint(1, 6); });
	std::cout << "\n";
	seed11::reseed(42);
	std::generate_n(std::ostream_iterator<int>(std::cout, " "), 5, [](){ return seed11::randint(1, 6); });
	std::cout << "\n";
}