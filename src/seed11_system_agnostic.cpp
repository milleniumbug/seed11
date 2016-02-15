#include <seed11/seed11.hpp>
#include <cstddef>
#include <limits>
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

	seed_device_read_error::seed_device_read_error(const std::string& what_arg) :
		std::runtime_error(what_arg)
	{

	}
	seed_device_read_error::seed_device_read_error(const char* what_arg) :
		std::runtime_error(what_arg)
	{

	}

	double seed_device::entropy() const
	{
		return std::numeric_limits<seed_device::result_type>::digits;
	}

	void reseed()
	{
		detail::thread_local_random() = make_seeded<std::mt19937_64>();
	}

	void reseed(std::mt19937_64::result_type value)
	{
		detail::thread_local_random().seed(value);
	}

	namespace detail
	{
		seed_device& thread_local_seed_device()
		{
			thread_local seed_device s;
			return s;
		}

		std::mt19937_64& thread_local_random()
		{
			thread_local auto local_random = make_seeded<std::mt19937_64>();
			return local_random;
		}
	}
}