#include "tglpch.h"
#include "Mouse.h"


MouseEventHandler Mouse::s_Handler;
std::map<int, bool> Mouse::s_ButtonHeld;
std::map<int, bool> Mouse::s_ButtonPress;
std::map<int, bool> Mouse::s_ButtonRelease;
glm::vec2 Mouse::s_Position;
glm::vec2 Mouse::s_Scroll;


MouseEventHandler::MouseEventHandler(){
	SUB_EVENT(EventMouseButtonPress, MouseEventHandler::OnMouseButtonPress);
	SUB_EVENT(EventMouseButtonRelease, MouseEventHandler::OnMouseButtonRelease);
	SUB_EVENT(EventMouseMove, MouseEventHandler::OnMouseMove);
	SUB_EVENT(EventMouseScroll, MouseEventHandler::OnMouseScroll);
}

bool MouseEventHandler::OnMouseMove(EventMouseMove& e){
	Mouse::s_Position.x = e.X;
	Mouse::s_Position.y = e.Y;
	return true;
}

bool MouseEventHandler::OnMouseButtonPress(EventMouseButtonPress& e){
	Mouse::s_ButtonPress[e.Button] = true;
	Mouse::s_ButtonHeld[e.Button] = true;
	Mouse::s_ButtonRelease[e.Button] = false;
	return true;
}

bool MouseEventHandler::OnMouseButtonRelease(EventMouseButtonRelease& e){
	Mouse::s_ButtonHeld[e.Button] = false;
	Mouse::s_ButtonPress[e.Button] = false;
	Mouse::s_ButtonRelease[e.Button] = true;
	return true;
}

bool MouseEventHandler::OnMouseScroll(EventMouseScroll& e){
	Mouse::s_Scroll.x = e.X;
	Mouse::s_Scroll.y = e.Y;
	return true;
}



void Mouse::Init() {

}

bool Mouse::IsButtonPress(int button){
	bool state = KeyState(s_ButtonPress, button);
	s_ButtonPress[button] = false;
	return state;
}

bool Mouse::IsButtonRelease(int button){
	bool state = KeyState(s_ButtonRelease, button);
	s_ButtonRelease[button] = false;
	return state;
}

bool Mouse::IsButtonHeld(int button){
	return KeyState(s_ButtonHeld, button);
}

bool Mouse::KeyState(const std::map<int, bool>& map, int key){
	if (map.find(key) != map.end())
		return map.at(key);
	return false;
}
