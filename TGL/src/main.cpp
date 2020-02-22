#include "tglpch.h"
#include "TGL.h"

#include <glad/glad.h>


class TestLayer : public Layer {
public:
	TestLayer() : Layer("Test layer") {}

	virtual inline void OnAttach() {
	}

	virtual inline void OnUpdate(float ts) {
		INFO("Update layer {1} : {0}", ts, m_Name);
		glClearColor(0.3, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	virtual inline void OnDraw() {
		INFO("Draw Layer {0} !", m_Name);
	}

private:
};


int main(int argc, char* argv[]) {
	Log::Init();

	Application app(WindowProps("Test TGL", 800, 600));
	app.PushLayer(CreateRef<TestLayer>());
	app.Run();

	return 0;
}