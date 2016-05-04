#include <seed11/seed11.hpp>
#include <cstddef>
#include <cstdio>
#include <cassert>

namespace seed11
{
	namespace detail
	{
		void* seed_device_init(const std::string&)
		{
			FILE* f = std::fopen("/dev/urandom", "rb");
			if(!f)
				throw seed_device_init_error("FUCK");
			else
				return f;
		}
	}

	void seed_device::seed_impl_deleter::operator()(void* ptr)
	{
		std::fclose(static_cast<FILE*>(ptr));
	}

	seed_device::seed_device() :
		impl(detail::seed_device_init(""))
	{
		
	}

	seed_device::seed_device(const std::string& token) :
		impl(detail::seed_device_init(token))
	{
		
	}

	seed_device::result_type seed_device::operator()()
	{
		result_type res;
		std::size_t count_read = fread(&res, sizeof res, 1, static_cast<FILE*>(impl.get()));
		if(count_read < 1)
			throw seed_device_read_error("FUCK");
		return res;
	}
}