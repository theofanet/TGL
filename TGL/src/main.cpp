#include "tglpch.h"
#include "TGL.h"

#include <glad/glad.h>



class TestLayer : public Layer {
public:
	TestLayer() : Layer("Test layer"), m_Speed(1.0f), m_Margin(0.0) {
		m_Cam = CreateRef<Camera2D>(800 / 600);
		Registry::SetTexturePathPrefix("assets/textures/");
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
	}

	virtual inline void OnDraw() {
		Renderer::Begin(Renderer::R2D, m_Cam);

		for (int y = 0; y < 40; y++) {
			for (int x = 0; x < 40; x++) {
				int i = x * y + x;
				Renderer2D::DrawQuad(
					i % 2 == 0 ? "rock.jpg" : "paper.jpg",
					{ x * (0.1 + m_Margin), y * (0.1 + m_Margin) },
					{ 0.1, 0.1 }
				);
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

private:
	Ref<Camera2D> m_Cam;
	float m_Speed;
	float m_Margin;
};


int main(int argc, char* argv[]) {
	Application app(WindowProps("Test TGL", 800, 600));
	app.PushLayer(CreateRef<TestLayer>());
	app.Run();

	return 0;
}