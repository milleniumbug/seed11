#include <seed11/seed_device.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <stdexcept>
#include <functional>

template<typename T, typename Function>
void print_gen(
	T&& urng,
	Function fun,
	int count = -1)
{
	if(count < 0)
	{
		while(true)
		{
			fun(urng());
		}
	}
	else
	{
		for(int i = 0; i < count; ++i)
		{
			fun(urng());
		}
	}
	std::cout << "\n";
}

int main(int argc, char** argv)
{
	std::vector<std::string> argsvec(argv, argv+argc);
	std::map<int, std::string> args;
	for(std::size_t i = 0; i < argsvec.size(); ++i)
	{
		args.emplace(i, argsvec[i]);
	}
	auto value_or = [&](int key, std::string value)
	{
		return args.insert(std::make_pair(key, value)).first->second;
	};
	const std::string type = value_or(1, "ascii_spaces");
	const std::string dev = value_or(2, "seed_device");
	const int count = std::stoi(value_or(3, "-1"));
	std::random_device random_device_gen;
	seed11::seed_device seed_device_gen;
	std::function<void()> at_end = [](){};
	std::function<void(unsigned)> out;
	std::function<unsigned()> rd;
	if(dev == "random_device")
	{
		rd = std::ref(random_device_gen);
	}
	else if(dev == "seed_device")
	{
		rd = std::ref(seed_device_gen);
	}
	else
		throw std::runtime_error("no such device");

	if(type == "ascii_newlines")
	{
		out = [](unsigned i){ std::cout << i << "\n"; };
	}
	else if(type == "ascii_spaces")
	{
		out = [](unsigned i){ std::cout << i << " "; };
		at_end = [](){ std::cout << "\n"; };
	}
	else if(type == "raw")
	{
		out = [](unsigned i){ std::cout.write(reinterpret_cast<char*>(&i), sizeof i); };
	}
	else
		throw std::runtime_error("no such output type");

	print_gen(rd, out, count);
	at_end();
}