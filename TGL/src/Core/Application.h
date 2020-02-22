#pragma once

#include "Events.h"
#include "Graphics/Window.h"
#include "Layer.h"


class Application {
public:
	Application(const WindowProps& props);
	~Application();

	void Run();

	void PushLayer(Ref<Layer> layer);
	void PushOverlay(Ref<Layer> layer);

	// Events handlers
	bool OnWindowClose(EventWindowClose&);
	bool OnWindowResize(EventWindowResize& e);

private:
	float m_LastTime;
	bool m_Running, m_Minimized;
	LayerStack m_LayerStack;
	Window *m_Window;
};