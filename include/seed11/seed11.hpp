#ifndef SEED11_HPP_E3A87608D1BE11E5A14C96B0531BF5E9
#define SEED11_HPP_E3A87608D1BE11E5A14C96B0531BF5E9

#include <climits>
#include <memory>
#include <string>
#include <array>
#include <cstddef>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <algorithm>

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

	template<typename T>
	struct seed_size
	{
		static const std::size_t value = 1 + sizeof(T)/sizeof(seed_device::result_type);
	};

	template<class UIntType,
		size_t w, size_t n, size_t m, size_t r,
		UIntType a, size_t u, UIntType d, size_t s,
		UIntType b, size_t t,
		UIntType c, size_t l, UIntType f>
	struct seed_size<std::mersenne_twister_engine<UIntType, w, n, m, r, a, u, d, s, b, t, c, l, f>>
	{
		static const std::size_t value = std::mersenne_twister_engine<UIntType, w, n, m, r, a, u, d, s, b, t, c, l, f>::state_size;
	};

	template<class UIntType,
		UIntType a,
		UIntType c,
		UIntType m>
	struct seed_size<std::linear_congruential_engine<UIntType, a, c, m>>
	{
		static const std::size_t value = sizeof(UIntType)/sizeof(seed_device::result_type);
	};

	template<class UIntType,
		std::size_t w,
		std::size_t s,
		std::size_t r>
	struct seed_size<std::subtract_with_carry_engine<UIntType, w, s, r>>
	{
		static const std::size_t value = sizeof(UIntType)/sizeof(seed_device::result_type)*r;
	};

	template<typename T, typename U>
	T make_seeded(U&& urng)
	{
		typedef typename std::remove_cv<typename std::remove_reference<U>::type>::type UnqualifiedU;
		std::array<typename UnqualifiedU::result_type, seed_size<T>::value> seed;
		std::generate(seed.begin(), seed.end(), std::ref(urng));
		std::seed_seq seed_sequence(seed.begin(), seed.end());
		return T(seed_sequence);
	}

	namespace detail
	{
		inline seed_device& thread_local_seed_device()
		{
			thread_local seed_device s;
			return s;
		}
	}

	template<typename T>
	T make_seeded()
	{
		return make_seeded<T>(detail::thread_local_seed_device());
	}

	namespace detail
	{
		inline std::mt19937_64& thread_local_random()
		{
			thread_local auto local_random = make_seeded<std::mt19937_64>();
			return local_random;
		}
	}

	template<typename RandomAccessIterator>
	void shuffle(RandomAccessIterator first, RandomAccessIterator last)
	{
		shuffle(first, last, detail::thread_local_random());
	}

	template<typename RandomAccessIterator, typename URNG>
	RandomAccessIterator choice(RandomAccessIterator first, RandomAccessIterator last, URNG&& urng)
	{
		typedef typename std::iterator_traits<RandomAccessIterator>::difference_type df_t;
		std::uniform_int_distribution<df_t> dist(0, std::max(static_cast<df_t>(0), std::distance(first, last-1)));
		return first + dist(urng);
	}

	template<typename RandomAccessIterator>
	RandomAccessIterator choice(RandomAccessIterator first, RandomAccessIterator last)
	{
		return choice(first, last, detail::thread_local_random());
	}
}

#endif