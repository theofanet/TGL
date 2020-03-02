#include "tglpch.h"
#include "Renderer.h"

#include "Renderer2D.h"


Ref<Renderer::Context> Renderer::s_Context = CreateRef<Renderer::Context>();
Ref<RendererEventHandler> Renderer::s_Handler = nullptr;
Renderer::DrawStack Renderer::s_DrawStack;


void Renderer::Init() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_DEPTH_TEST);

	Renderer2D::Init();
	s_Handler = CreateRef<RendererEventHandler>();
}

void Renderer::Shutdown() {
	Renderer2D::Shutdown();
}

void Renderer::Clear() {
	glClearColor(
		s_Context->ClearColor.r,
		s_Context->ClearColor.g,
		s_Context->ClearColor.b,
		s_Context->ClearColor.a
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Begin(Type mode, Ref<Camera> camera) {
	s_Context->Mode = mode;
	s_Context->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	s_DrawStack.Clear();
}

void Renderer::End() {
	s_DrawStack.Draw(s_Context);
	s_DrawStack.Clear();
}

void Renderer::Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const glm::mat4& model, Ref<Texture> texture, int slot, const glm::vec4& color) {
	s_DrawStack.PushStep(DrawStep(shader, vertexArray, model, texture, slot, color));
}

RendererEventHandler::RendererEventHandler() {
	SUB_EVENT(EventWindowResize, RendererEventHandler::OnWindowResize);
}

bool RendererEventHandler::OnWindowResize(EventWindowResize& e) {
	glViewport(0, 0, e.Width, e.Height);
	return true;
}