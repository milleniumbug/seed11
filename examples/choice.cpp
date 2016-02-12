#include <cassert>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <seed11/seed11.hpp>

int main()
{
	{
		std::array<int, 0> empty_arr;
		auto it = seed11::choice(empty_arr.begin(), empty_arr.end());
		assert(it == empty_arr.end());
	}
	{
		std::array<int, 4> arr = { 1, 2, 3, 45 };
		std::array<int, 4> counts;
		counts.fill(0);
		std::mt19937_64 mt;
		for(int i = 0; i < 10000; i++)
		{
			auto it = seed11::choice(arr.begin(), arr.end(), mt);
			assert(it >= arr.begin() && it < arr.end());
			assert(it != arr.end());
			++counts[std::distance(arr.begin(), it)];
		}
		for(auto&& x : counts)
		{
			std::cout << x << "\n";
		}
		assert(std::all_of(counts.begin(), counts.end(), [&](int x)
		{
			return x - 200 < counts[0] && x + 200 > counts[0];
		}));
	}
}