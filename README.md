**DISCLAIMER**: I make no guarantees with regard to suitability of this library for cryptographic purposes. (well, tbh, if you're doing crypto, you should know better than to snoop on random GitHub projects)

seed11 [![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
------

This library provides extensions to C++11 `<random>`:

- `seed11::seed_device` - drop-in replacement for `std::random_device`, exists because of implementation bugs (I'm looking at you, MinGW)
- `seed11::seed_size` - metafunction for determining maximum usable seed size for a random number engine. You can specialize it for your engines. It's already specialized for C++11 `<random>` random number engines. If not specialized, the size is determined using the `sizeof`.
- `seed11::make_seeded<RandomNumberEngine>()` - create the engine seeded with thread local `seed_device`, with the seed size equal to `seed_size<RandomNumberEngine>::value`.
- `seed11::make_seeded<RandomNumberEngine>(URNG&& urng)` - create the engine seeded with the uniform random number generator provided as an argument, with the seed size equal to `seed_size<RandomNumberEngine>::value`.