#include "tglpch.h"
#include "Window.h"

#include <glad/glad.h>

#include "TGL/Core/Core.h"
#include "TGL/Core/Application.h"


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
	: m_Window(nullptr), m_Props(props), m_CursorState(true) {
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

	glfwWindowHint(GLFW_SAMPLES, 4);
	
	m_Context = CreateRef<GLContext>(m_Window);
	m_Context->Init();
	
	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		TRIGGER_EVENT(EventWindowClose);
	});

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		Application::GetInstance()->GetWindow()->SetPropSize(width, height);
		TRIGGER_EVENT(EventWindowResize, width, height);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		switch (action) {
		case GLFW_PRESS:
			TRIGGER_EVENT(EventKeyPressed, key);
			break;
		case GLFW_RELEASE:
			TRIGGER_EVENT(EventKeyReleased, key);
			break;
		case GLFW_REPEAT:
			TRIGGER_EVENT(EventKeyPressed, key, true);
			break;
		}
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
		switch (action){
		case GLFW_PRESS:
			TRIGGER_EVENT(EventMouseButtonPress, button);
			break;
		case GLFW_RELEASE:
			TRIGGER_EVENT(EventMouseButtonRelease, button);
			break;
		}
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
		TRIGGER_EVENT(EventMouseMove, (float)x, (float)y);
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
		TRIGGER_EVENT(EventMouseScroll, (float)xOffset, (float)yOffset);
	});

}


void Window::SetTitle(const std::string& title){
	glfwSetWindowTitle(m_Window, title.c_str());
}

void Window::SetCursorPos(const glm::vec2& position){
	glfwSetCursorPos(m_Window, position.x, position.y);
}

void Window::SetCursorMiddle() {
	glfwSetCursorPos(m_Window, m_Props.Width / 2, m_Props.Height / 2);
}

void Window::Update() {
	glfwPollEvents();
	m_Context->SwapBuffers();
}

void Window::ToggleCursor() {
	SetCursorState(!m_CursorState);
}

void Window::SetCursorState(bool state) {
	m_CursorState = state;
	glfwSetInputMode(m_Window, GLFW_CURSOR, m_CursorState ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}