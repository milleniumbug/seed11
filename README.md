**DISCLAIMER**: I make no guarantees with regard to suitability of this library for cryptographic purposes. (well, tbh, if you're doing crypto, you should know better than to snoop on random GitHub projects)

seed11 [![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
======

Motivation
----------

```
$ cat tests/motivation.cpp
#include <iostream>
#include <iterator>
#include <random>
#include <algorithm>
#include <functional>

int main()
{
	std::random_device rd;
	std::generate_n(
		std::ostream_iterator<unsigned>(std::cout, " "),
		4,
		std::ref(rd));
	std::cout << "\n";
}
$ x86_64-w64-mingw32-g++ -std=c++11 -Wall tests/motivation.cpp -o fail
$ for i in `seq 1 3`; do ./fail; done
3499211612 581869302 3890346734 3586334585
3499211612 581869302 3890346734 3586334585
3499211612 581869302 3890346734 3586334585
```

Well, that's just terrible... How could we make it better?

Synopsis
--------

This library provides extensions to C++11 `<random>`:

- `seed11::seed_device` - drop-in replacement for `std::random_device`, exists because of implementation bugs (I'm looking at you, MinGW)
- `seed11::seed_size` - metafunction for determining maximum usable seed size for a random number engine. You can specialize it for your engines. It's already specialized for C++11 `<random>` random number engines. If not specialized, the size is determined using the `sizeof`.
- `seed11::make_seeded<RandomNumberEngine>()` - create the engine seeded with thread local `seed_device`, with the seed size equal to `seed_size<RandomNumberEngine>::value`.
- `seed11::make_seeded<RandomNumberEngine>(URNG&& urng)` - create the engine seeded with the uniform random number generator provided as an argument, with the seed size equal to `seed_size<RandomNumberEngine>::value`.
- `seed11::choice(RandomAccessIterator begin, RandomAccessIterator end)` - returns an iterator to the random element (chosen using `std::uniform_int_distribution` and a thread local `std::mt19937_64`). If `begin == end`, `end` is returned.
- `seed11::choice(RandomAccessIterator begin, RandomAccessIterator end, URNG&& urng)` - returns an iterator to the random element (chosen using `std::uniform_int_distribution` on a provided uniform random number generator). If `begin == end`, `end` is returned.

Building
--------

```
cd build
cmake -G Ninja ..
ninja-build
```

You can as well choose any other generator, your choice...