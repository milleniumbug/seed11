#include <climits>
#include <memory>
#include <string>

namespace seed11
{

	// hardcoded_sequence_generator
	// seeded<gen>()

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