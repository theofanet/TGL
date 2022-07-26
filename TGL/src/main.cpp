#include "tglpch.h"

#include "TGL/TGL.h"

#include "Layers/EditorLayer.h"


int main(int argc, char* argv[]) {
	Application app(WindowProps("Test TGL", 1280, 720));
	app.PushLayer(CreateRef<EditorLayer>());
	app.Run();

	return 0;
}