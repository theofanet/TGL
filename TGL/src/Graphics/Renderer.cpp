#include "tglpch.h"
#include "Renderer.h"
#include "Core/Registry.h"
#include "Renderer2D.h"
#include "Renderer3D.h"


Ref<Renderer::Context> Renderer::s_Context = CreateRef<Renderer::Context>();
Ref<RendererEventHandler> Renderer::s_Handler = nullptr;
Renderer::DrawStack Renderer::s_DrawStack;
std::vector<Ref<Light>> Renderer::s_Lights;
int Renderer::s_LightsCount;
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
	Renderer3D::Init();

	s_LightsCount = 0;

	s_Handler = CreateRef<RendererEventHandler>();

	s_IsInitialized = true;
}

void Renderer::Shutdown() {
	Renderer2D::Shutdown();
	Renderer3D::Shutdown();
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

void Renderer::AddLight(Ref<Light> light){
	ASSERT(s_LightsCount < 10, "Max Light number reached !");
	s_Lights.push_back(light);
	s_LightsCount++;
}

void Renderer::Begin(Type mode, Ref<Camera> camera) {
	s_Context->Mode = mode;
	s_Context->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	s_Context->ViewPosition = camera->GetPosition();
	s_DrawStack.Clear();
	s_Lights.clear();
	s_LightsCount = 0;
}

void Renderer::End() {
	Renderer::Clear();
	s_DrawStack.Draw(s_Context, s_Lights);
	s_DrawStack.Clear();
}

void Renderer::Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const glm::mat4& model, const Ref<Material> material) {
	s_DrawStack.PushStep(DrawStep(shader, vertexArray, model, material));
}

RendererEventHandler::RendererEventHandler() {
	SUB_EVENT(EventWindowResize, RendererEventHandler::OnWindowResize);
}

bool RendererEventHandler::OnWindowResize(EventWindowResize& e) {
	glViewport(0, 0, e.Width, e.Height);
	return true;
}