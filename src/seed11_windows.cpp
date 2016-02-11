#include <seed11/seed11.hpp>
#include <cstddef>
#include <cassert>
#include <windows.h>
#include <winerror.h>

namespace seed11
{
	namespace detail
	{
		struct seed_impl
		{
			HCRYPTPROV crypt_provider;

			seed_impl(const seed_impl&) = delete;
			seed_impl(seed_impl&&) = delete;
			seed_impl& operator=(const seed_impl&) = delete;
			seed_impl& operator=(seed_impl&&) = delete;

			seed_impl()
			{
				if(CryptAcquireContext(
					&crypt_provider,
					nullptr,
					nullptr,
					PROV_RSA_FULL,
					CRYPT_VERIFYCONTEXT))
					return;

				throw "FUCK";
			}

			~seed_impl()
			{
				CryptReleaseContext(crypt_provider, 0);
			}
		};
	}

	void seed_device::seed_impl_deleter::operator()(void* ptr)
	{
		delete static_cast<detail::seed_impl*>(ptr);
	}

	seed_device::seed_device() : 
		impl(new detail::seed_impl())
	{
		
	}

	seed_device::result_type seed_device::operator()()
	{
		detail::seed_impl& i = *static_cast<detail::seed_impl*>(impl.get());
		result_type res;
		if(!CryptGenRandom(i.crypt_provider, sizeof res, reinterpret_cast<BYTE*>(&res)))
			assert(false);
		return res;
	}

	double seed_device::entropy() const
	{
		return 42;
	}
}