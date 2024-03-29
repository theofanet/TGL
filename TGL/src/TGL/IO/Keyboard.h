#pragma once

#include "TGL/Core/Events.h"


class KeyboardEventHandler {
public:
	KeyboardEventHandler();

	bool KeyPressed(EventKeyPressed& e);
	bool KeyReleased(EventKeyReleased& e);
};


class Keyboard {
	friend KeyboardEventHandler;
public:
	static void Init();

	static bool IsPressed(int keycode);
	static bool IsReleased(int keycode);
	static bool IsHeld(int keycode);

protected:
	static std::map<int, bool> s_KeyPressed;
	static std::map<int, bool> s_KeyReleased;
	static std::map<int, bool> s_KeyHeld;

private:
	static bool KeyState(const std::map<int, bool>& map, int key);

	static KeyboardEventHandler s_Handler;
};