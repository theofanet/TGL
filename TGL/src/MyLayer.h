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
	Ref<FrameBuffer> m_FrameBuffer;
	glm::vec2 m_ViewportSize;

	Ref<Scene> m_ActiveScene;

	bool m_MouseDown, m_ViewPortHovered;
	glm::vec2 m_MousePosition;

	Entity m_Square, m_CameraEntity;
	
	float m_StatsFPS;
};