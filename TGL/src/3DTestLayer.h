#pragma once

#include "TGL.h"

#include "Tests/FPSCamera.h"


class L3DTestLayer : public Layer {
public:
	L3DTestLayer();
	~L3DTestLayer();

	virtual void OnAttach();
	virtual void OnUpdate(float ts);
	virtual void OnDraw();

private:
	Ref<Light> m_Light;
	Ref<Camera3D> m_Cam;
	Ref<Material> m_Material;
};