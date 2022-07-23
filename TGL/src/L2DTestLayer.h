#pragma once
#pragma once

#include "TGL.h"


class L2DTestLayer : public Layer {
public:
	L2DTestLayer();
	~L2DTestLayer();

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