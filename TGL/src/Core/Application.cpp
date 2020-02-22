#include "tglpch.h"
#include "Application.h"
#include "Core/Core.h"


Application::Application(const WindowProps& props) : m_Running(false), m_Minimized(false), m_LastTime(0.0f) {
	m_Window = new Window(props);
}

Application::~Application() {
	delete m_Window;
}

void Application::Run() {
	m_Running = true;

	EventDisptacher<EventWindowClose>::Sub(EVENT_FN(Application::OnWindowClose));
	EventDisptacher<EventWindowResize>::Sub(EVENT_FN(Application::OnWindowResize));

	// main game loop
	while (m_Running) {
		float time = (float)glfwGetTime();
		float timestep = time - m_LastTime;
		m_LastTime = time;

		if (!m_Minimized) {
			for (auto& layer : m_LayerStack) {
				layer->OnUpdate(timestep);
				layer->OnDraw();
			}
		}

		m_Window->Update();
	}
}

void Application::PushLayer(Ref<Layer> layer){
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Ref<Layer> layer){
	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
}

bool Application::OnWindowClose(EventWindowClose&) {
	INFO("Window Close Event");
	m_Running = false;
	return true;
}

bool Application::OnWindowResize(EventWindowResize& e) {
	TRACE("WIDTH: {0} | HEIGHT: {1}", e.Width, e.Height);

	if (e.Width == 0 || e.Height == 0) {
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;

	return true;
}
