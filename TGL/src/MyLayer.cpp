#include "tglpch.h"
#include "MyLayer.h"

#include "imgui.h"


MyLayer::MyLayer() : Layer("3D Tests") {
	float aspectRatio = Application::GetInstance()->GetWindowProps().Width / Application::GetInstance()->GetWindowProps().Height;
	m_Cam = CreateRef<Camera2D>(aspectRatio);
	m_StatsFPS = 0.0f;
	m_ViewPortHovered = false;
	m_ViewportSize = { 1280, 720 };
}

MyLayer::~MyLayer() {

}

void MyLayer::OnAttach() {
	Registry::SetTexturePathPrefix("assets/textures/");

	SUB_EVENT(EventMouseScroll, MyLayer::OnScroll);
	ON_EVENT(MyLayer::OnEvent);

	m_MousePosition = Mouse::GetPosition();

	m_Cam->SetPosition(glm::vec3(0.0f));
	m_Cam->SetZoomLevel(6.0f);

	m_FrameBuffer = FrameBuffer::Create(1280, 720);
}

void MyLayer::OnUpdate(float ts) {
	FrameBufferProps props = m_FrameBuffer->GetProps();
	if (props.Height != m_ViewportSize.y || props.Width != m_ViewportSize.x) {
		m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_Cam->OnResize(m_ViewportSize.x, m_ViewportSize.y);
	}

	if (Mouse::IsButtonHeld(MOUSE_LEFT_BUTTON))
		m_MouseDown = true;
	else
		m_MouseDown = false;

	if (m_MouseDown) {
		glm::vec2 pos = Mouse::GetPosition() - m_MousePosition;
		m_Cam->SetPosition(m_Cam->GetPosition() + glm::vec3(-pos.x / 600.0, pos.y / 400.0f, 0.0f) * m_Cam->GetZoomLevel());
	}

	m_MousePosition = Mouse::GetPosition();

	m_StatsFPS = (1 / ts);
}

void MyLayer::OnDraw() {
	m_FrameBuffer->Bind();

	Renderer::Clear();
	Renderer2D::ResetStats();

	Renderer2D::Begin(m_Cam);

	Renderer2D::DrawLine({ 0.0f, 0.0f, 0.1f }, { 4.0, 4.0, 0.1f }, { 0.0, 0.2, 0.8, 1.0 });

	Renderer2D::DrawQuad({ 1.0, 0.0 }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
	Renderer2D::DrawQuad({ -1.0, 0.0 }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, 45.0f);
	Renderer2D::DrawQuad("Checkerboard.png", { .0f, .0f, -0.1f }, { 20.0f, 20.0f }, glm::vec4(1.0f), 0, 10.0f);

	for (float y = -5.0f; y < 5.0f; y += 0.5f) {
		for (float x = -5.0f; x < 5.0f; x += 0.5f) {
			glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
			Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}

	Renderer2D::End();
	m_FrameBuffer->Unbind();
}

void MyLayer::OnGuiDraw() {
	bool workspaceOpen = true;
	ImGuiRenderer::StartWorkspace(workspaceOpen);

	ImGui::Begin("Renderer2D Stats");
	ImGui::Text("FPS : %.2f", m_StatsFPS);
	ImGui::Text("Draw calls : %d", Renderer2D::s_StatsDrawCalls);
	ImGui::Text("Quads : %d", Renderer2D::s_StatsQuads);
	ImGui::Text("Vertices : %d", Renderer2D::s_StatsQuads * 4);
	ImGui::Text("Indices : %d", Renderer2D::s_StatsQuads * 6);
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

bool MyLayer::OnScroll(EventMouseScroll& e) {
	float z = m_Cam->GetZoomLevel();
	z -= e.Y * 0.25;
	z = std::max(z, 0.25f);
	m_Cam->SetZoomLevel(z);
	return true;
}

bool MyLayer::OnEvent(Event& e) {
	ImGuiIO& io = ImGui::GetIO();
	
	if (io.WantCaptureMouse && e.Category == EventCategory::Mouse)
		return m_ViewPortHovered;
	else if (io.WantCaptureKeyboard && e.Category == EventCategory::Keyboard)
		return m_ViewPortHovered;

	return true;
}