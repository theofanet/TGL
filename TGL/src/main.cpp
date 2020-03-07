#include "tglpch.h"
#include "TGL.h"

#include <glad/glad.h>


class TestLayer : public Layer {
public:
	TestLayer() : Layer("Test layer"), m_Speed(1.0f), m_Margin(0.0) {
		m_Cam = CreateRef<Camera2D>(800 / 600);
		m_MouseDown = false;
	}

	virtual void OnAttach() {
		Registry::SetTexturePathPrefix("assets/textures/");
		SUB_EVENT(EventMouseScroll, TestLayer::OnScroll);
		m_MousePosition = Mouse::GetPosition();
	}

	virtual inline void OnUpdate(float ts) {
		if (Keyboard::IsPressed(GLFW_KEY_ESCAPE))
			Application::Quit();

		if (Keyboard::IsHeld(GLFW_KEY_LEFT_SHIFT))
			m_Speed = 2.0;
		else
			m_Speed = 1.0f;

		if (Keyboard::IsHeld(GLFW_KEY_M))
			m_Margin = .01f;
		else
			m_Margin = .0f;

		glm::vec3 pos = m_Cam->GetPosition();
		if (Keyboard::IsHeld(GLFW_KEY_W))
			pos.y += m_Speed * ts;
		else if (Keyboard::IsHeld(GLFW_KEY_S))
			pos.y -= m_Speed * ts;

		if (Keyboard::IsHeld(GLFW_KEY_A))
			pos.x -= m_Speed * ts;
		else if (Keyboard::IsHeld(GLFW_KEY_D))
			pos.x += m_Speed * ts;

		if(pos != m_Cam->GetPosition())
			m_Cam->SetPosition(pos);

		std::ostringstream title;
		title << Application::GetInstance()->GetWindowProps().Title << " - FPS : " << (60 / ts);
		Application::GetInstance()->GetWindow()->SetTitle(title.str());

		if (Mouse::IsButtonPress(MOUSE_LEFT_BUTTON))
			INFO("MOUSE BUTTON LEFT PRESS");
		if (Mouse::IsButtonRelease(MOUSE_LEFT_BUTTON))
			INFO("MOUSE BUTTON LEFT RELEASE");
		if (Mouse::IsButtonHeld(MOUSE_LEFT_BUTTON)) {
			m_MouseDown = true;
			INFO("MOUSE BUTTON LEFT HELD");
		}
		else
			m_MouseDown = false;
	
		if (m_MouseDown) {
			glm::vec2 pos = Mouse::GetPosition() - m_MousePosition;
			m_Cam->SetPosition(m_Cam->GetPosition() + glm::vec3(-pos.x / 600.0, pos.y / 400.0f, 0.0f) * m_Cam->GetZoomLevel());
		}
		m_MousePosition = Mouse::GetPosition();
	}

	virtual inline void OnDraw() {
		Renderer::Begin(Renderer::R2D, m_Cam);

		for (int y = 0; y < 40; y++) {
			for (int x = 0; x < 40; x++) {
				int i = x * y + x;
				Renderer2D::DrawQuad(
					"grass.png",
					{ x * (0.1 + m_Margin), y * (0.1 + m_Margin) },
					{ 0.1, 0.1 }
				);

				if (i % 17 == 0) {
					Renderer2D::DrawQuad(
						"glass05red.png",
						{ x * (0.1 + m_Margin), y * (0.1 + m_Margin) },
						{ 0.1, 0.1 }
					);
				}
			}
		}

		glm::vec4 color = {0.0f, 0.0f, 1.0f, 1.0f};
		for (int y = 0; y < 40; y++) {
			color.r += 0.025;
			color.b -= 0.025;

			for (int x = -1; x > -41; x--) {
				color.g += 0.025;

				Renderer2D::DrawQuad(
					{ x * (0.1 + m_Margin), y * (0.1 + m_Margin) },
					{ 0.1, 0.1 },
					color
				);
			}
			color.g = 0;
		}

		Renderer::End();
	}

	inline bool OnScroll(EventMouseScroll& e) {
		float z = m_Cam->GetZoomLevel();
		z -= e.Y * 0.25;
		z = std::max(z, 0.25f);
		m_Cam->SetZoomLevel(z);
		return true;
	}

private:
	Ref<Camera2D> m_Cam;
	float m_Speed;
	float m_Margin;
	bool m_MouseDown;
	glm::vec2 m_MousePosition;
};


int main(int argc, char* argv[]) {
	Application app(WindowProps("Test TGL", 800, 600));
	app.PushLayer(CreateRef<TestLayer>());
	app.Run();

	return 0;
}