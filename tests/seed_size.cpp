#include <random>
#include <type_traits>
#include <cstdint>
#include <cassert>
#include <seed11/seed11.hpp>
#include <iostream>

int main()
{
	assert(seed11::detail::get_seed_size_for<std::mt19937>() == 624);
	assert(seed11::detail::get_seed_size_for<std::mt19937_64>() == 624);
}