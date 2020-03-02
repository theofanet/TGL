#pragma once


#ifdef GL_PLATEFORM_WINDOWS
#define BREAK() { __debugbreak();  }
#elif GL_PLATEFORM_MACOSX
#define BREAK() { __builtin_trap();  }
#else 
#define BREAK()

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
