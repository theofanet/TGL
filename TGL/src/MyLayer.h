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
	bool OnEvent(Event& e);
private:
	Ref<Camera2D> m_Cam;
	Ref<FrameBuffer> m_FrameBuffer;
	glm::vec2 m_ViewportSize;

	bool m_MouseDown, m_ViewPortHovered;
	glm::vec2 m_MousePosition;
	
	float m_StatsFPS;
};