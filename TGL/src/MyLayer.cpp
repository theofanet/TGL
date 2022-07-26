#include "tglpch.h"
#include "MyLayer.h"

#include "imgui.h"
#include "Scripts/CameraController.h"


MyLayer::MyLayer() 
	: Layer("3D Tests"), 
	  m_EditorCamera(EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f)) {
	float aspectRatio = Application::GetInstance()->GetWindowProps().Width / Application::GetInstance()->GetWindowProps().Height;
}

MyLayer::~MyLayer() {

}

void MyLayer::OnAttach() {
	m_ViewportPanel.InitFrameBuffer();
	m_ActiveScene = CreateRef<Scene>();
	m_EntitiesPanel.SetContext(m_ActiveScene);
}

void MyLayer::OnUpdate(float ts) {
	FrameBufferProps props = m_ViewportPanel.GetFrameBuffer()->GetProps();
	if (props.Height != m_ViewportPanel.GetViewportSize().y || props.Width != m_ViewportPanel.GetViewportSize().x) {
		glm::vec2 size = { m_ViewportPanel.GetViewportSize().x, m_ViewportPanel.GetViewportSize().y };
		if (size.x && size.y) {
			m_ViewportPanel.ResizeFrameBuffer(size.x, size.y);
			m_ActiveScene->SetViewportSize(size.x, size.y);
			m_EditorCamera.SetViewportSize(size.x, size.y);
		}
	}

	if (m_ViewportPanel.IsHovered()) {
		m_ActiveScene->OnUpdate(ts);
		m_EditorCamera.OnUpdate(ts);
	}
}

void MyLayer::OnDraw() {
	m_ViewportPanel.BindFrameBuffer();

	Renderer::Clear({ 0.15f, 0.15f, 0.15f });
	Renderer2D::ResetStats();

	m_ActiveScene->OnDrawEditor(m_EditorCamera);

	m_ViewportPanel.UnBindFrameBuffer();
}

void MyLayer::OnGuiDraw() {
	bool workspaceOpen = true;
	ImGuiRenderer::StartWorkspace(workspaceOpen);

	m_EntitiesPanel.OnImGuiRender();
	m_ContentBrowserPanel.OnImGuiRender();
	m_ViewportPanel.OnImGuiRender();
	m_StatsPanel.OnImGuiRender();

	ImGuiRenderer::EndWorkSpace();
}