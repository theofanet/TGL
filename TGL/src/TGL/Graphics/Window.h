#pragma once

#include <GLFW/glfw3.h>
#include "TGL/Core/Core.h"
#include <glm/glm.hpp>
#include "TGL/Core/Events.h"


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

	inline Ref<GLContext> GetContext() { return m_Context; }
	inline GLFWwindow* GetNativeWindow() { return m_Window; }
	
	void SetTitle(const std::string& title);
	void SetCursorPos(const glm::vec2& position);
	void SetCursorMiddle();
	void ToggleCursor();
	void SetCursorState(bool state);

	inline int GetWidth() { return m_Props.Width; }
	inline int GetHeight() { return m_Props.Height; }
	inline void SetPropSize(int width, int height) {
		m_Props.Width = width;
		m_Props.Height = height;
	};

	void Update();

private:
	void Init();

	static bool s_GLFWInitialized;
	bool m_CursorState;

	GLFWwindow *m_Window;
	WindowProps m_Props;
	Ref<GLContext> m_Context;
};