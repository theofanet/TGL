#pragma once

#include "Graphics/Camera.h"
#include "IO/Keyboard.h"


class FPSCamera : public Camera3D {
public:
	FPSCamera();
	~FPSCamera();

	void OnUpdate(float ts);

protected:
	void HandleMouseUpdate(float ts);
	void HandleKeyboardUpdate(float ts);

	glm::vec3 m_Front;
	glm::vec2 m_LastMousePos;
	float m_Pitch, m_Yaw;
	float m_MouseSensitivity, m_Speed;
};