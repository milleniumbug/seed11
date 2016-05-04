#ifndef SEED11_HPP_E3A87608D1BE11E5A14C96B0531BF5E9
#define SEED11_HPP_E3A87608D1BE11E5A14C96B0531BF5E9

#include <cassert>
#include <climits>
#include <limits>
#include <initializer_list>
#include <memory>
#include <string>
#include <iterator>
#include <vector>
#include <cstddef>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <algorithm>

#include <seed11/seed_device.hpp>

namespace seed11
{
	namespace detail
	{
		template<typename T>
		class acquire_seed_size_seed_seq
		{
			std::size_t* s;

		public:
			typedef T result_type;

			acquire_seed_size_seed_seq(std::size_t* s = nullptr) :
				s(s)
			{

			}

			template<typename InputIterator>
			acquire_seed_size_seed_seq(InputIterator, InputIterator) :
				s(nullptr)
			{

			}

			template<typename V>
			acquire_seed_size_seed_seq(std::initializer_list<V>) :
				s(nullptr)
			{

			}

			template<typename RandomAccessIterator>
			void generate(RandomAccessIterator outb, RandomAccessIterator oute)
			{
				if(s) *s = std::distance(outb, oute);
				std::fill(outb, oute, 0);
			}

			std::size_t size() const
			{
				return 0;
			}

			template<typename OutputIterator>
			void param(OutputIterator)
			{

			}
		};

		template<typename T>
		std::size_t get_seed_size_for()
		{
			std::size_t seed_size;
			acquire_seed_size_seed_seq<unsigned> acq_seed_size(&seed_size);
			T gen(acq_seed_size);
			return seed_size;
		}
	}

	template<typename SequenceContainer>
	class seed_seq_container_adapter
	{
		SequenceContainer seed;

		void ensure_not_empty()
		{
			if(seed.empty())
			{
				seed.push_back(1);
				seed.push_back(2);
			}
		}

	public:
		typedef unsigned result_type;

		seed_seq_container_adapter()
		{
			ensure_not_empty();
		}

		template<typename InputIterator>
		seed_seq_container_adapter(InputIterator b, InputIterator e) :
			seed(b, e)
		{
			ensure_not_empty();
		}

		template<typename V>
		seed_seq_container_adapter(std::initializer_list<V> in) :
			seed(in.begin(), in.end())
		{
			ensure_not_empty();
		}

		seed_seq_container_adapter(const SequenceContainer& s) :
			seed(s)
		{
			ensure_not_empty();
		}

		seed_seq_container_adapter(SequenceContainer&& s) :
			seed(std::move(s))
		{
			ensure_not_empty();
		}

		template<typename RandomAccessIterator>
		void generate(RandomAccessIterator outb, RandomAccessIterator oute)
		{
			auto in = seed.begin();
			auto inb = seed.begin();
			auto ine = seed.end();
			for(auto& o = outb; o != oute; ++o)
			{
				*o = *in;
				++in;
				if(in == ine)
					in = inb;
			}
		}

		std::size_t size() const
		{
			return seed.size();
		}

		template<typename OutputIterator>
		void param(OutputIterator out)
		{
			std::copy(seed.begin(), seed.end(), out);
		}

		SequenceContainer steal_storage()
		{
			return SequenceContainer(std::move(seed));
		}
	};

	/// create seeded random engine using the generator passed as an argument
	template<typename T, typename U>
	T make_seeded(U&& urng)
	{
		typedef typename std::remove_cv<typename std::remove_reference<U>::type>::type UnqualifiedU;
		// get the size of the seed from the URNG by employing
		// a dummy SeedSequence object.
		const std::size_t seed_size = detail::get_seed_size_for<T>();
		// fill the vector with random data
		std::vector<typename UnqualifiedU::result_type> seed_data;
		seed_data.reserve(seed_size);
		std::generate_n(std::back_inserter(seed_data), seed_size, std::ref(urng));
		// use it to initialize a seed sequence
		seed_seq_container_adapter<decltype(seed_data)> true_seed_seq(std::move(seed_data));
		return T(true_seed_seq);
	}

	namespace detail
	{
		seed_device& thread_local_seed_device();
		std::mt19937_64& thread_local_random();
	}

	/// create seeded random engine
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
