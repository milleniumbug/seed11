#ifndef SEED_DEVICE_HPP_D399E636D90F11E59E05DDD3531BF5E9
#define SEED_DEVICE_HPP_D399E636D90F11E59E05DDD3531BF5E9

namespace seed11
{
	class seed_device_init_error : public std::runtime_error
	{
	public:
		explicit seed_device_init_error(const std::string& what_arg);
		explicit seed_device_init_error(const char* what_arg);
	};

	class seed_device_read_error : public std::runtime_error
	{
	public:
		explicit seed_device_read_error(const std::string& what_arg);
		explicit seed_device_read_error(const char* what_arg);
	};

	class seed_device
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

		seed_device();
		explicit seed_device(const std::string& token);
		seed_device(const seed_device&) = delete;
		seed_device(seed_device&&) = delete;
		seed_device& operator=(const seed_device&) = delete;
		seed_device& operator=(seed_device&&) = delete;
		~seed_device() = default;
	};
}

#endif