#include "tglpch.h"
#include "TGL.h"

#include <glad/glad.h>



class TestLayer : public Layer {
public:
	TestLayer() : Layer("Test layer") {
		m_Cam = CreateRef<Camera2D>(800 / 600);
		m_Pos = { 0.0f, 0.0f };
		Registry::SetTexturePathPrefix("assets/textures/");
	}

	virtual inline void OnUpdate(float ts) {
		if (Keyboard::IsPressed(GLFW_KEY_ESCAPE))
			Application::Quit();

		if (Keyboard::IsHeld(GLFW_KEY_W))
			m_Pos.y += 0.002;
		else if (Keyboard::IsHeld(GLFW_KEY_S))
			m_Pos.y -= 0.002;

		if (Keyboard::IsPressed(GLFW_KEY_A))
			m_Pos.x -= 0.002;
		else if (Keyboard::IsPressed(GLFW_KEY_D))
			m_Pos.x += 0.002;

		std::ostringstream title;
		title << Application::GetInstance()->GetWindowProps().Title << " - FPS : " << (60 / ts);
		Application::GetInstance()->GetWindow()->SetTitle(title.str());
	}

	virtual inline void OnDraw() {
		Renderer::Begin(Renderer::R2D, m_Cam);
		Renderer2D::DrawQuad("rock.jpg", m_Pos, { .5f, .5f });
		Renderer2D::DrawQuad("rock.jpg", { .7f, .7f }, { .2f, .2f }, 45);
		Renderer::End();
	}

private:
	Ref<Camera2D> m_Cam;
	glm::vec2 m_Pos;
};


int main(int argc, char* argv[]) {
	Application app(WindowProps("Test TGL", 800, 600));
	app.PushLayer(CreateRef<TestLayer>());
	app.Run();

	return 0;
}