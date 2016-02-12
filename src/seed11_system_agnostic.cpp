#include <seed11/seed11.hpp>
#include <cstddef>
#include <stdexcept>

namespace seed11
{
	seed_device_init_error::seed_device_init_error(const std::string& what_arg) :
		std::runtime_error(what_arg)
	{

	}
	seed_device_init_error::seed_device_init_error(const char* what_arg) :
		std::runtime_error(what_arg)
	{

	}

	double seed_device::entropy() const
	{
		return 42;
	}
}