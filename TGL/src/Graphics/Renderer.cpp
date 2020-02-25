#include "tglpch.h"
#include "Renderer.h"

#include "Renderer2D.h"


Ref<Renderer::Context> Renderer::s_Context = CreateRef<Renderer::Context>();
Ref<RendererEventHandler> Renderer::s_Handler = nullptr;

void Renderer::Init(){
	Renderer2D::Init();
	s_Handler = CreateRef<RendererEventHandler>();
}

void Renderer::Shutdown(){
	Renderer2D::Shutdown();
}

void Renderer::Clear(){
	glClearColor(
		s_Context->ClearColor.r, 
		s_Context->ClearColor.g, 
		s_Context->ClearColor.b, 
		s_Context->ClearColor.a
	);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Begin(Type mode, const Ref<Camera>& camera){
	s_Context->Mode = mode;
	s_Context->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
}

void Renderer::End(){

}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& model){
	shader->Bind();

	shader->SetMat4("u_ViewProjection", s_Context->ViewProjectionMatrix);
	shader->SetMat4("u_Model", model);

	vertexArray->Draw();
}

RendererEventHandler::RendererEventHandler() {
	SUB_EVENT(EventWindowResize, RendererEventHandler::OnWindowResize);
}

bool RendererEventHandler::OnWindowResize(EventWindowResize& e) {
	glViewport(0, 0, e.Width, e.Height);
	return true;
}