#include <seed11/seed11.hpp>
#include <cstddef>
#include <cstdio>
#include <cassert>

namespace seed11
{

	void seed_device::seed_impl_deleter::operator()(void* ptr)
	{
		std::fclose(static_cast<FILE*>(ptr));
	}

	seed_device::seed_device() :
		impl(std::fopen("/dev/urandom", "rb"))
	{

	}

	seed_device::result_type seed_device::operator()()
	{
		result_type res;
		std::size_t count_read = fread(&res, sizeof res, 1, static_cast<FILE*>(impl.get()));
		assert(count_read == 1);
		return res;
	}

	double seed_device::entropy() const
	{
		return 42;
	}
}