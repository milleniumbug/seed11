#include <cassert>
#include <iostream>
#include <random>
#include <seed11/seed11.hpp>

int main()
{
	auto a = seed11::make_seeded<std::mt19937>();
	auto b = seed11::make_seeded<std::mt19937>();
	assert(a != b);

	std::mt19937 r;
	r.seed(std::mt19937::default_seed);
	auto c = seed11::make_seeded<std::minstd_rand>(r);
	r.seed(std::mt19937::default_seed);
	auto d = seed11::make_seeded<std::minstd_rand>(r);
	assert(c == d);
}