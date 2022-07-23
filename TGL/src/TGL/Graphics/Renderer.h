#pragma once


#include <glm/glm.hpp>

#include "TGL/Core/Core.h"
#include "Buffers.h"
#include "Arrays.h"
#include "Shaders.h"
#include "Texture.h"
#include "Camera.h"
#include "TGL/Core/Events.h"

class RendererEventHandler {
public:
	RendererEventHandler();
	bool OnWindowResize(EventWindowResize& e);
};


class Renderer {
public:
	static void Init();
	static void Shutdown();
	inline static bool IsIntialized() { return s_IsInitialized; }
	static inline const Ref<Texture>GetWhiteTexture() { return s_WhiteTexture; }
	static void Clear(const glm::vec3& color = glm::vec3(0.0f));

private:
	static Ref<RendererEventHandler> s_Handler;
	static Ref<Texture> s_WhiteTexture;
	static bool s_IsInitialized;
};