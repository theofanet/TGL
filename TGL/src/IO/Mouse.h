#pragma once

#include "Core/Events.h"
#include <glm/glm.hpp>


class MouseEventHandler {
public:
	MouseEventHandler();

	bool OnMouseMove(EventMouseMove &e);
	bool OnMouseButtonPress(EventMouseButtonPress& e);
	bool OnMouseButtonRelease(EventMouseButtonRelease& e);
	bool OnMouseScroll(EventMouseScroll& e);
};

class Mouse {
	friend MouseEventHandler;
public:
	static void Init();

	static bool IsButtonPress(int button);
	static bool IsButtonRelease(int button);
	static bool IsButtonHeld(int button);

	inline static const glm::vec2& GetPosition() { return s_Position; }
	inline static const glm::vec2& GetScroll() { return s_Scroll; }

protected:
	static std::map<int, bool> s_ButtonPress;
	static std::map<int, bool> s_ButtonRelease;
	static std::map<int, bool> s_ButtonHeld;

	static glm::vec2 s_Position;
	static glm::vec2 s_Scroll;

private:
	static bool KeyState(const std::map<int, bool>& map, int key);
	static MouseEventHandler s_Handler;
};