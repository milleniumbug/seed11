#include <seed11/deterministic_unsafe_seed_device.hpp>
#include <seed11/seed11.hpp>
#include <random>
#include <thread>
#include <mutex>
#include <ctime>
#include <chrono>
#include <functional>

namespace seed11
{
	namespace detail
	{
		struct deterministic_unsafe_seed_device_impl
		{
			typedef deterministic_unsafe_seed_device::result_type int_type;
			std::mt19937_64 mt;
			std::uniform_int_distribution<int_type> dist;
			std::random_device rd;

			deterministic_unsafe_seed_device_impl(std::mt19937_64 mt, int_type min, int_type max) :
				mt(std::move(mt)),
				dist(min, max)
			{

			}
		};

		void* deterministic_unsafe_seed_device_init(const std::string& token)
		{
			using namespace std;
			namespace c = std::chrono;
			std::random_device rd;
			// TODO: sources could be used in a more effective way
			// hopefully adding stuff won't hurt
			std::seed_seq ss = {
				// thread id
				hash<thread::id>()(this_thread::get_id()),
				// time
				// needs to be hashed for portability
				hash<time_t>()(time(nullptr)),
				hash<c::steady_clock::rep>()(
					c::steady_clock::now().time_since_epoch().count()
				),
				hash<c::high_resolution_clock::rep>()(
					c::high_resolution_clock::now().time_since_epoch().count()
				),
				hash<c::system_clock::rep>()(
					c::system_clock::now().time_since_epoch().count()
				),
				// hash code of some type_info object
				// unspecified value that can have different value
				// across executions
				typeid(deterministic_unsafe_seed_device_impl).hash_code(),
				// getting the address of a local variable, exploiting 
				// adress-space layout randomization where it's available
				std::hash<void*>()(static_cast<void*>(&ss)),
				// get randomness from user token
				std::hash<std::string>()(token)
			};
			return new deterministic_unsafe_seed_device_impl(
				std::mt19937_64(ss),
				deterministic_unsafe_seed_device::min(),
				deterministic_unsafe_seed_device::max()
			);
		}
	}

	void deterministic_unsafe_seed_device::seed_impl_deleter::operator()(void* ptr)
	{
		delete static_cast<detail::deterministic_unsafe_seed_device_impl*>(ptr);
	}

	deterministic_unsafe_seed_device::deterministic_unsafe_seed_device() :
		impl(detail::deterministic_unsafe_seed_device_init(""))
	{
		
	}

	deterministic_unsafe_seed_device::deterministic_unsafe_seed_device(const std::string& token) :
		impl(detail::deterministic_unsafe_seed_device_init(token))
	{
		
	}

	deterministic_unsafe_seed_device::result_type deterministic_unsafe_seed_device::operator()()
	{
		auto& state = *static_cast<detail::deterministic_unsafe_seed_device_impl*>(impl.get());
		// get randomness from random_device mixed with output of deterministic
		// RNG seeded with sources accessible from pure C++11 code
		return static_cast<result_type>(state.dist(state.mt) ^ state.rd());
	}

	double deterministic_unsafe_seed_device::entropy() const
	{
		return 0;
	}
}