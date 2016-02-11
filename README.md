**DISCLAIMER**: I make no guarantees with regard to suitability of this library for cryptographic purposes. (well, tbh, if you're doing crypto, you should know better than to snoop on random GitHub projects)

seed11 [![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
------

This library provides extensions to C++11 `<random>`:

- `seed11::seed_device` - drop-in replacement for `std::random_device`, exists because of implementation bugs (I'm looking at you, MinGW)