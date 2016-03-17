#ifndef SEED_DEVICE_HPP_D399E636D90F11E59E05DDD3531BF5E9
#define SEED_DEVICE_HPP_D399E636D90F11E59E05DDD3531BF5E9

#include <climits>
#include <memory>

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

	/// non-deterministic uniform random number generator
	class seed_device
	{
		struct seed_impl_deleter
		{
			void operator()(void* ptr);
		};
		std::unique_ptr<void, seed_impl_deleter> impl;
	public:
		/// 
		typedef unsigned result_type;

		/// lowest possible value that operator() can return
		static constexpr result_type min()
		{
			return 0;
		}

		/// highest possible value that operator() can return
		static constexpr result_type max()
		{
			return UINT_MAX;
		}

		/// returns "entropy" of the random device
		/// https://en.wikipedia.org/wiki/Entropy_%28information_theory%29
		///
		/// always non-deterministic
		double entropy() const;

		/// return a random value from range [min(), max()]
		///
		/// can throw exceptions derived from `seed_device_read_error`
		result_type operator()();

		/// create a seed_device
		/// can throw exceptions derived from `seed_device_init_error` on failure
		seed_device();
		/// create a seed device passing the token
		/// Exists for compatibility with `std::random_device`
		/// 
		/// currently the token is ignored
		explicit seed_device(const std::string& token);

		/// copy operations are deleted
		seed_device(const seed_device&) = delete;
		/// copy operations are deleted
		seed_device(seed_device&&) = delete;
		/// copy operations are deleted
		seed_device& operator=(const seed_device&) = delete;
		/// copy operations are deleted
		seed_device& operator=(seed_device&&) = delete;
		~seed_device() = default;
	};
}

#endif