#include "tglpch.h"
#include "Keyboard.h"


std::map<int, bool> Keyboard::s_KeyPressed;
std::map<int, bool> Keyboard::s_KeyReleased;
std::map<int, bool> Keyboard::s_KeyHeld;
Ref<KeyboardEventHandler> Keyboard::s_Handler = nullptr;


KeyboardEventHandler::KeyboardEventHandler(){
	SUB_EVENT(EventKeyPressed, KeyboardEventHandler::KeyPressed);
	SUB_EVENT(EventKeyReleased, KeyboardEventHandler::KeyReleased);
}

bool KeyboardEventHandler::KeyPressed(EventKeyPressed& e){
	if(!e.Repeated)
		Keyboard::s_KeyPressed[e.Keycode] = true;
	Keyboard::s_KeyHeld[e.Keycode] = true;
	Keyboard::s_KeyReleased[e.Keycode] = false;
	return true;
}

bool KeyboardEventHandler::KeyReleased(EventKeyReleased& e){
	Keyboard::s_KeyHeld[e.Keycode] = false;
	Keyboard::s_KeyPressed[e.Keycode] = false;
	Keyboard::s_KeyReleased[e.Keycode] = true;
	return true;
}


void Keyboard::Init(){
	s_Handler = CreateRef<KeyboardEventHandler>();
}

bool Keyboard::IsPressed(int keycode){
	bool state = KeyState(s_KeyPressed, keycode);
	s_KeyPressed[keycode] = false;
	return state;
}

bool Keyboard::IsReleased(int keycode){
	bool state = KeyState(s_KeyReleased, keycode);
	s_KeyReleased[keycode] = false;
	return state;
}

bool Keyboard::IsHeld(int keycode){
	return KeyState(s_KeyHeld, keycode);
}

bool Keyboard::KeyState(const std::map<int, bool>& map, int key){
	if (map.find(key) != map.end())
		return map.at(key);
	return false;
}
