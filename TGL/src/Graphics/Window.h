#pragma once

#include <GLFW/glfw3.h>
#include "Core/Core.h"


struct WindowProps {
	std::string Title;
	int Width;
	int Height;

	WindowProps(const std::string& title, int width, int height) 
		: Title(title), Width(width), Height(height) {
	}
};

class GLContext {
public:
	GLContext(GLFWwindow* w);
	void Init();
	void SwapBuffers();

private:
	GLFWwindow* m_Window;
};


class Window {
public:
	Window(const WindowProps& props);
	~Window();

	Ref<GLContext> GetContext() { return m_Context; }
	
	void SetTitle(const std::string& title);

	void Update();

private:
	void Init();

	static bool s_GLFWInitialized;

	GLFWwindow *m_Window;
	WindowProps m_Props;
	Ref<GLContext> m_Context;
};