#include "tglpch.h"
#include "Window.h"

#include <glad/glad.h>

#include "Core/Core.h"
#include "Core/Events.h"


static void GLFWErrorCallback(int error, const char* desc) {
	ERROR("GLFW Error ({0}): {1}", error, desc);
}

GLContext::GLContext(GLFWwindow* window) : m_Window(window) {
	ASSERT(m_Window, "GLContext: Window is null");
}

void GLContext::Init() {
	glfwMakeContextCurrent(m_Window);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ASSERT(status, "GLContext: Failed to initialize GLAD");
}

void GLContext::SwapBuffers() {
	glfwSwapBuffers(m_Window);
}

bool Window::s_GLFWInitialized = false;

Window::Window(const WindowProps& props) 
	: m_Window(nullptr), m_Props(props) {
	Init();
}

Window::~Window() {
	glfwTerminate();
}

void Window::Init() {
	if (!s_GLFWInitialized) {
		int success = glfwInit();
		ASSERT(success, "Unable to initialize GLFW");
		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWInitialized = true;
	}

	m_Window = glfwCreateWindow(m_Props.Width, m_Props.Height, m_Props.Title.c_str(), nullptr, nullptr);

	m_Context = CreateRef<GLContext>(m_Window);
	m_Context->Init();
	
	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		TRIGGER_EVENT(EventWindowClose);
	});

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		TRIGGER_EVENT(EventWindowResize, width, height);
	});
}


void Window::Update() {
	glfwPollEvents();
	m_Context->SwapBuffers();
}