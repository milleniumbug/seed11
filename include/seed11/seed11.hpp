#ifndef SEED11_HPP_E3A87608D1BE11E5A14C96B0531BF5E9
#define SEED11_HPP_E3A87608D1BE11E5A14C96B0531BF5E9

#include <climits>
#include <limits>
#include <memory>
#include <string>
#include <array>
#include <cstddef>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <algorithm>

#include <seed11/seed_device.hpp>

namespace seed11
{
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
		seed_device& thread_local_seed_device();
		std::mt19937_64& thread_local_random();
	}

	template<typename T>
	T make_seeded()
	{
		return make_seeded<T>(detail::thread_local_seed_device());
	}

	void reseed();
	void reseed(std::mt19937_64::result_type value);

	template<typename T>
	T randint(T a, T b)
	{
		return std::uniform_int_distribution<T>(a,b)(detail::thread_local_random());
	}

	template<typename RandomAccessIterator>
	void shuffle(RandomAccessIterator first, RandomAccessIterator last)
	{
		std::shuffle(first, last, detail::thread_local_random());
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