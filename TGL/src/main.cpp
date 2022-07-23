#include "tglpch.h"

#include "TGL/TGL.h"

#include "MyLayer.h"


int main(int argc, char* argv[]) {
	Application app(WindowProps("Test TGL", 800, 600));
	app.PushLayer(CreateRef<MyLayer>());
	app.Run();

	return 0;
}