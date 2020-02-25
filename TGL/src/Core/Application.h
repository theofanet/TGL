#pragma once

#include "Events.h"
#include "Graphics/Window.h"
#include "Graphics/Renderer.h"
#include "Layer.h"
#include "IO/Keyboard.h"


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

	void StopApplication();
	inline static void Quit() { TRIGGER_EVENT(EventApplicationQuit); };
	bool OnQuit(EventApplicationQuit&);


private:
	float m_LastTime;
	bool m_Running, m_Minimized;
	LayerStack m_LayerStack;
	Window *m_Window;
};