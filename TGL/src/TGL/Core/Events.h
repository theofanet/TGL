#pragma once

#include <string>
#include <functional>
#include <memory>
#include <map>
#include "TGL/Core/Core.h"


// Callback Types
template <typename T>
using FuncType = std::function<bool(T&)>;
template <typename T>
using Func = Ref<FuncType<T>>;
template <typename T>
using SubscriberList = std::vector<Func<T>>;


///////////////////////////////////// EVENTS /////////////////////////////////////
enum EventCategory {
	Core,
	Keyboard,
	Mouse
};

// ===== ABSTRACT ===
class Event {
public:
	EventCategory Category = EventCategory::Core;
};

class KeyboardEvent : public Event {
public:
	KeyboardEvent(int keycode) : Keycode(keycode) {
		Category = EventCategory::Keyboard;
	}
	int Keycode;
};

class MouseEvent : public Event {
public:
	MouseEvent() {
		Category = EventCategory::Mouse;
	}
};

// ===== APP ========

class EventApplicationQuit : public Event {};

// ===== WINDOWS ======

class EventWindowClose : public Event {};

class EventWindowResize : public Event {
public:
	EventWindowResize(int width, int height) 
		: Width(width), Height(height) {}
	int Width, Height;
};

// ======= IOs ============

class EventKeyPressed : public KeyboardEvent {
public:
	EventKeyPressed(int keycode, bool repeated = false) 
		: KeyboardEvent(keycode), Repeated(repeated) {}
	bool Repeated;
};

class EventKeyReleased : public KeyboardEvent {
public:
	EventKeyReleased(int keycode) : KeyboardEvent(keycode) {}
};

class EventMouseMove : public MouseEvent {
public:
	EventMouseMove(float x, float y) : X(x), Y(y) {}
	float X, Y;
};

class EventMouseButtonPress : public MouseEvent {
public:
	EventMouseButtonPress(int button) : Button(button) {}
	int Button;
};

class EventMouseButtonRelease : public MouseEvent {
public:
	EventMouseButtonRelease(int button) : Button(button) {}
	int Button;
};

class EventMouseScroll : public MouseEvent {
public:
	EventMouseScroll(float x, float y) : X(x), Y(y) {}
	float X, Y;
};




// Dispatcher
template<typename T>
class EventDispatcher {
public:
	template <typename ... Args>
	inline static bool Trigger(Args&& ... args) {
		T e = T(std::forward<Args>(args)...);
		if (std::is_same<T, Event>::value || EventDispatcher<Event>::Trigger(e)) {
			for (const auto& fn : s_Subscribers) {
				if (!(*fn)(e))
					return false;
			}
			return true;
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


// MACROS
#define EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define SUB_EVENT(e, fn) EventDispatcher<e>::Sub(EVENT_FN(fn))
#define ON_EVENT(fn) EventDispatcher<Event>::Sub(EVENT_FN(fn));
#define TRIGGER_EVENT(e, ...) EventDispatcher<e>::Trigger(__VA_ARGS__)