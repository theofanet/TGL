#include "tglpch.h"
#include "Application.h"
#include "Core/Core.h"


Application* Application::s_Instance = nullptr;


Application* Application::GetInstance() {
	ASSERT(s_Instance, "I ain't got any Application instance");
	return s_Instance;
}

Application::Application(const WindowProps& props) 
	: m_Running(false), m_Minimized(false), m_LastTime(0.0f), m_Props(props) {
	Log::Init();
	s_Instance = this;
	m_Window = CreateRef<Window>(props);

	Keyboard::Init();
	Mouse::Init();
	Renderer::Init();
}

Application::~Application() {
}

void Application::Run() {
	m_Running = true;

	SUB_EVENT(EventWindowClose, Application::OnWindowClose);
	SUB_EVENT(EventWindowResize, Application::OnWindowResize);
	SUB_EVENT(EventApplicationQuit, Application::OnQuit);

	// main game loop
	while (m_Running) {
		float time = (float)glfwGetTime();
		float timestep = time - m_LastTime;
		m_LastTime = time;

		if (!m_Minimized) {
			Renderer::Clear();

			for (auto& layer : m_LayerStack) {
				layer->OnUpdate(timestep);
				layer->OnDraw();
			}
		}

		m_Window->Update();
	}

	Renderer::Shutdown();
}

void Application::PushLayer(Ref<Layer> layer){
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Ref<Layer> layer){
	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
}

bool Application::OnWindowResize(EventWindowResize& e) {
	if (e.Width == 0 || e.Height == 0) {
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;

	return true;
}

bool Application::OnWindowClose(EventWindowClose&) {
	StopApplication();
	return true;
}

bool Application::OnQuit(EventApplicationQuit&){
	StopApplication();
	return false;
}

void Application::StopApplication() {
	m_Running = false;
}