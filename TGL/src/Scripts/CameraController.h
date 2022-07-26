#pragma once

#include "TGL/TGL.h"
#include <glm/gtc/type_ptr.hpp>


class CameraController : public ScriptableEntity {
public:
	void OnCreate() {
		SUB_EVENT(EventMouseScroll, CameraController::OnScroll);
		SUB_EVENT(EventMouseButtonRelease, CameraController::OnMouseReleased);
	}

	void OnUpdate(float ts) {
		if (m_MouseDown) {
			auto& transform = GetComponent<TransformComponent>().Translation;
			auto& camera = GetComponent<CameraComponent>().Camera;

			glm::vec2 pos = Mouse::GetPosition() - m_MousePosition;
			glm::vec3 translation = glm::vec3(-pos.x / (camera.GetViewportWidth() / camera.GetAspectRatio()), pos.y / camera.GetViewportHeight(), 0.0f)
				* camera.GetOrthographicSize();

			transform += translation;
		}

		m_MousePosition = Mouse::GetPosition();

		if (Mouse::IsButtonHeld(MOUSE_BUTTON_LEFT))
			m_MouseDown = true;
		else
			m_MouseDown = false;
	}

	bool OnMouseReleased(EventMouseButtonRelease& e) {
		if (e.Button == MOUSE_BUTTON_LEFT)
			m_MouseDown = false;
		return true;
	}

	bool OnScroll(EventMouseScroll& e) {
		auto& camera = GetComponent<CameraComponent>().Camera;
		float z = camera.GetOrthographicSize();
		z -= e.Y * 0.25;
		z = std::max(z, 0.25f);
		camera.SetOrthographicSize(z);

		return true;
	}

private:
	bool m_MouseDown = false;
	glm::vec2 m_MousePosition = glm::vec2(0.0f);
};