#pragma once

#include <new>

static void Fail()
{
#if defined(_HAS_EXCEPTIONS) || defined(__EXCEPTIONS)
	throw std::bad_alloc();
#else
	std::abort();
#endif
}

template <class... Types>
struct counter
{
	static const std::size_t value = sizeof...(Types);
};