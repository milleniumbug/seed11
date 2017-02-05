#ifndef DETERMINISTIC_UNSAFE_SEED_DEVICE_HPP_9AB11E38D8E811E5A83BC0A2531BF5E9
#define DETERMINISTIC_UNSAFE_SEED_DEVICE_HPP_9AB11E38D8E811E5A83BC0A2531BF5E9

#include <memory>
#include <string>
#include <climits>

namespace seed11
{
	class deterministic_unsafe_seed_device
	{
		struct seed_impl_deleter
		{
			void operator()(void* ptr);
		};
		std::unique_ptr<void, seed_impl_deleter> impl;
	public:
		typedef unsigned result_type;

		static constexpr result_type min()
		{
			return 0;
		}

		static constexpr result_type max()
		{
			return UINT_MAX;
		}

		double entropy() const;

		result_type operator()();

		deterministic_unsafe_seed_device();
		explicit deterministic_unsafe_seed_device(const std::string& token);
		deterministic_unsafe_seed_device(const deterministic_unsafe_seed_device&) = delete;
		deterministic_unsafe_seed_device(deterministic_unsafe_seed_device&&) = delete;
		deterministic_unsafe_seed_device& operator=(const deterministic_unsafe_seed_device&) = delete;
		deterministic_unsafe_seed_device& operator=(deterministic_unsafe_seed_device&&) = delete;
		~deterministic_unsafe_seed_device() = default;
	};
}

#endif