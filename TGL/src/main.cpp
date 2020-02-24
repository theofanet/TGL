#include "tglpch.h"
#include "TGL.h"

#include <glad/glad.h>


class TestLayer : public Layer {
public:
	TestLayer() : Layer("Test layer") {
		m_Cam = CreateRef<Camera2D>(800 / 600);
	}

	virtual inline void OnDraw() {
		Renderer::Begin(Renderer::R2D, m_Cam);
		Renderer2D::DrawQuad({ .0f, .0f }, { .5f, .5f });
		Renderer::End();
	}

private:
	Ref<Camera2D> m_Cam;
};


int main(int argc, char* argv[]) {
	Application app(WindowProps("Test TGL", 800, 600));
	app.PushLayer(CreateRef<TestLayer>());
	app.Run();

	return 0;
}