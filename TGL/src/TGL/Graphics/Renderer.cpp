#include "tglpch.h"
#include "Renderer.h"
#include "TGL/Core/Registry.h"
#include "Renderer2D.h"


Ref<RendererEventHandler> Renderer::s_Handler = nullptr;
bool Renderer::s_IsInitialized = false;
Ref<Texture> Renderer::s_WhiteTexture = nullptr;


void Renderer::Init() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	s_WhiteTexture = Texture::Create(1, 1);
	uint32_t WhiteTextureData = 0xffffffff;
	s_WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));

	Renderer2D::Init();

	s_Handler = CreateRef<RendererEventHandler>();

	s_IsInitialized = true;
}

void Renderer::Shutdown() {
	Renderer2D::Shutdown();
}

void Renderer::Clear(const glm::vec3& color) {
	glClearColor(
		color.r,
		color.g,
		color.b,
		1.0f
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

RendererEventHandler::RendererEventHandler() {
	SUB_EVENT(EventWindowResize, RendererEventHandler::OnWindowResize);
}

bool RendererEventHandler::OnWindowResize(EventWindowResize& e) {
	glViewport(0, 0, e.Width, e.Height);
	return true;
}