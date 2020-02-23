#include "tglpch.h"
#include "TGL.h"

#include <glad/glad.h>


class TestLayer : public Layer {
public:
	TestLayer() : Layer("Test layer") {}

	virtual inline void OnAttach() {
		const std::vector<float> vertices = {
			-0.5f,  0.5f, 0.0f, 0.8, 0.1, 0.1,
			 0.5f,  0.5f, 0.0f, 0.1, 0.8, 0.1,
			 0.5f, -0.5f, 0.0f, 0.1, 0.1, 0.8,
			-0.5f, -0.5f, 0.0f, 0.8, 0.8, 0.1
		};

		const std::vector<uint32_t> indices = { 0, 1, 3, 3, 1, 2 };

		va = CreateRef<VertexArray>();

		vb = CreateRef<VertexBuffer>(vertices);
		vb->AddAttrib(3, GL_FLOAT);
		vb->AddAttrib(3, GL_FLOAT);

		ib = CreateRef<IndexBuffer>(indices);
		
		va->AddVertexBuffer(vb);
		va->SetIndexBuffer(ib);

		shader = CreateRef<Shader>("assets/shaders/shader.glsl");
	}

	virtual inline void OnUpdate(float ts) {
		INFO("Update layer {1} : {0}", ts, m_Name);
	}

	virtual inline void OnDraw() {
		INFO("Draw Layer {0} !", m_Name);
		glClearColor(0.3, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Bind();
		va->Draw();
	}

private:
	Ref<VertexArray> va;
	Ref<VertexBuffer> vb;
	Ref<IndexBuffer> ib;
	Ref<Shader> shader;
};


int main(int argc, char* argv[]) {
	Log::Init();

	Application app(WindowProps("Test TGL", 800, 600));
	app.PushLayer(CreateRef<TestLayer>());
	app.Run();

	return 0;
}