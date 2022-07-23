#include "tglpch.h"
#include "MyLayer.h"

#include "imgui.h"


MyLayer::MyLayer() : Layer("3D Tests") {
	float aspectRatio = Application::GetInstance()->GetWindowProps().Width / Application::GetInstance()->GetWindowProps().Height;
	m_Cam = CreateRef<Camera2D>(aspectRatio);
	m_StatsFPS = 0.0f;
}

MyLayer::~MyLayer() {

}

void MyLayer::OnAttach() {
	Registry::SetTexturePathPrefix("assets/textures/");

	SUB_EVENT(EventMouseScroll, MyLayer::OnScroll);
	m_MousePosition = Mouse::GetPosition();
}

void MyLayer::OnUpdate(float ts) {

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
}

void MyLayer::OnGuiDraw() {
	ImGui::Begin("Renderer2D Stats");
	ImGui::Text("FPS : %.2f", m_StatsFPS);
	ImGui::Text("Draw calls : %d", Renderer2D::s_StatsDrawCalls);
	ImGui::Text("Quads : %d", Renderer2D::s_StatsQuads);
	ImGui::Text("Vertices : %d", Renderer2D::s_StatsQuads * 4);
	ImGui::Text("Indices : %d", Renderer2D::s_StatsQuads * 6);
	ImGui::End();
}

bool MyLayer::OnScroll(EventMouseScroll& e) {
	float z = m_Cam->GetZoomLevel();
	z -= e.Y * 0.25;
	z = std::max(z, 0.25f);
	m_Cam->SetZoomLevel(z);
	return true;
}