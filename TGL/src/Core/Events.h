#pragma once

#include <string>
#include <functional>
#include <memory>
#include <map>
#include "Core/Core.h"


// Callback Types
template <typename T>
using FuncType = std::function<bool(T&)>;
template <typename T>
using Func = Ref<FuncType<T>>;
template <typename T>
using SubscriberList = std::vector<Func<T>>;


// Dispatcher
template<typename T>
class EventDispatcher {
public:
	template <typename ... Args>
	inline static void Trigger(Args&& ... args) {
		T e = T(std::forward<Args>(args)...);
		for (const auto& fn : s_Subscribers) {
			if (!(*fn)(e))
				break;
		}
	}

	inline static void Sub(FuncType<T> f) {
		Func<T> func(CreateRef<FuncType<T>>(f));
		s_Subscribers.push_back(func);
	}

	static SubscriberList<T> s_Subscribers;
};

template<typename T>
SubscriberList<T> EventDispatcher<T>::s_Subscribers;


///////////////////////////////////// EVENTS /////////////////////////////////////

// ===== WINDOWS ======

class EventWindowClose {};

class EventWindowResize {
public:
	EventWindowResize(int width, int height) 
		: Width(width), Height(height) {}
	int Width, Height;
};


// MACROS
#define EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define SUB_EVENT(e, fn) EventDispatcher<e>::Sub(EVENT_FN(fn))
#define TRIGGER_EVENT(e, ...) EventDispatcher<e>::Trigger(__VA_ARGS__)