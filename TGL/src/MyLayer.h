#pragma once
#pragma once

#include "TGL/TGL.h"


class MyLayer : public Layer {
public:
	MyLayer();
	~MyLayer();

	virtual void OnAttach();
	virtual void OnUpdate(float ts);
	virtual void OnDraw();
	virtual void OnGuiDraw();

	bool OnScroll(EventMouseScroll& e);

private:
	Ref<Camera2D> m_Cam;
	bool m_MouseDown;
	glm::vec2 m_MousePosition;
	float m_StatsFPS;
};