#include <seed11/seed11.hpp>
#include <cstddef>
#include <cstdio>
#include <cassert>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/random.h>

namespace seed11
{
	namespace detail
	{
		void* seed_device_init(const std::string&)
		{
			return nullptr;
		}
	}

	void seed_device::seed_impl_deleter::operator()(void*)
	{
		
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
		void* buf = &res;
		std::size_t buflen = sizeof res;
		unsigned int flags = 0;
		int count_read = syscall(SYS_getrandom, buf, buflen, flags);
		if(count_read != static_cast<int>(buflen))
			throw seed_device_read_error("FUCK");
		return res;
	}
}