#include "tglpch.h"
#include "MyLayer.h"

#include "imgui.h"
#include "Scripts/CameraController.h"


MyLayer::MyLayer() : Layer("3D Tests") {
	float aspectRatio = Application::GetInstance()->GetWindowProps().Width / Application::GetInstance()->GetWindowProps().Height;
	m_StatsFPS = 0.0f;
	m_ViewPortHovered = false;
	m_ViewportSize = { 1280, 720 };
}

MyLayer::~MyLayer() {

}

void MyLayer::OnAttach() {
	Registry::SetTexturePathPrefix("assets/textures/");

	ON_EVENT(MyLayer::OnEvent);

	m_MousePosition = Mouse::GetPosition();

	m_FrameBuffer = FrameBuffer::Create(1280, 720);

	m_ActiveScene = CreateRef<Scene>();
	
	m_Square = m_ActiveScene->CreateEntity("square");
	m_Square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

	m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
	m_CameraEntity.AddComponent<CameraComponent>();
	m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

	m_EntitiesPanel.SetContext(m_ActiveScene);
}

void MyLayer::OnUpdate(float ts) {
	FrameBufferProps props = m_FrameBuffer->GetProps();
	if (props.Height != m_ViewportSize.y || props.Width != m_ViewportSize.x) {
		m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_ActiveScene->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
	}

	m_ActiveScene->OnUpdate(ts);

	m_StatsFPS = (1 / ts);
}

void MyLayer::OnDraw() {
	m_FrameBuffer->Bind();

	Renderer::Clear({ 0.15f, 0.15f, 0.15f });
	Renderer2D::ResetStats();

	m_ActiveScene->OnDraw();

	m_FrameBuffer->Unbind();
}

void MyLayer::OnGuiDraw() {
	bool workspaceOpen = true;
	ImGuiRenderer::StartWorkspace(workspaceOpen);

	m_EntitiesPanel.OnImGuiRender();

	ImGui::Begin("Renderer2D Stats");
	ImGui::Text("FPS : %.2f", m_StatsFPS);
	ImGui::Text("Draw calls : %d", Renderer2D::s_StatsDrawCalls);
	ImGui::Text("Quads : %d", Renderer2D::s_StatsQuads);
	ImGui::Text("Vertices : %d", Renderer2D::s_StatsQuads * 4);
	ImGui::Text("Indices : %d", Renderer2D::s_StatsQuads * 6);
	ImGui::End();

	ImGui::Begin("Settings");
	auto& color = m_Square.GetComponent<SpriteRendererComponent>().Color;
	ImGui::ColorEdit4("Square Color", glm::value_ptr(color));
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");
	m_ViewPortHovered = ImGui::IsWindowFocused();
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	uint64_t textureID = m_FrameBuffer->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGuiRenderer::EndWorkSpace();
}

bool MyLayer::OnEvent(Event& e) {
	ImGuiIO& io = ImGui::GetIO();
	
	if (io.WantCaptureMouse && e.Category == EventCategory::Mouse)
		return m_ViewPortHovered;
	else if (io.WantCaptureKeyboard && e.Category == EventCategory::Keyboard)
		return m_ViewPortHovered;

	return true;
}