#pragma once


#ifdef GL_PLATFORM_WINDOWS
#define BREAK() { __debugbreak();  }
#elif GL_PLATFORM_MACOSX
#define BREAK() { __builtin_trap(); }
#else 
#define BREAK()
#endif

#ifdef ENABLE_ASSERTS
#define ASSERT(x, ...) { if(!(x)) { ERROR(__VA_ARGS__); BREAK(); } }
#else
#define ASSERT(x, ...)
#endif


template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}
