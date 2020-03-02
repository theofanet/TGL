#pragma once

#include "Events.h"
#include "Graphics/Window.h"
#include "Graphics/Renderer.h"
#include "Layer.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"


class Application {
public:
	Application(const WindowProps& props);
	~Application();

	void Run();

	void PushLayer(Ref<Layer> layer);
	void PushOverlay(Ref<Layer> layer);

	inline Ref<Window> GetWindow() const { return m_Window; }

	// Events handlers
	bool OnWindowClose(EventWindowClose&);
	bool OnWindowResize(EventWindowResize& e);

	inline const WindowProps& GetWindowProps() const { return m_Props; }

	void StopApplication();
	inline static void Quit() { TRIGGER_EVENT(EventApplicationQuit); };
	bool OnQuit(EventApplicationQuit&);

	static Application* GetInstance();

private:
	float m_LastTime;
	bool m_Running, m_Minimized;
	LayerStack m_LayerStack;
	Ref<Window> m_Window;
	WindowProps m_Props;

	static Application *s_Instance;
};