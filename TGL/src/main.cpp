#include "tglpch.h"
#include "TGL.h"

#include <glad/glad.h>


class TestLayer : public Layer {
public:
	TestLayer() : Layer("Test layer") {}

	virtual inline void OnDraw() {
		Renderer::Begin(Renderer::R2D);
		Renderer2D::DrawQuad({ .0f, .0f }, { .5f, .5f });
		Renderer::End();
	}
};


int main(int argc, char* argv[]) {
	Application app(WindowProps("Test TGL", 800, 600));
	app.PushLayer(CreateRef<TestLayer>());
	app.Run();

	return 0;
}