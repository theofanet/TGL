#pragma once


#ifdef ENABLE_ASSERTS
#define ASSERT(x, ...) { if(!(x)) { ERROR(__VA_ARGS__); __debugbreak(); } }
#else
#define ASSERT(x, ...)
#endif


template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}