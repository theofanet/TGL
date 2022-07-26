#include "tglpch.h"
#include "EditorLayer.h"

#include "defines.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include "Scripts/CameraController.h"


EditorLayer::EditorLayer() 
	: Layer("3D Tests"), 
	  m_EditorCamera(EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f)) {
	float aspectRatio = (float)Application::GetInstance()->GetWindowProps().Width / (float)Application::GetInstance()->GetWindowProps().Height;
}

EditorLayer::~EditorLayer() {

}

void EditorLayer::OnAttach() {
	m_ActiveScene = CreateRef<Scene>();
	m_EntitiesPanel.SetContext(m_ActiveScene);
	m_ViewportPanel.SetContext(m_ActiveScene);
	m_ViewportPanel.InitFrameBuffer();
}

void EditorLayer::OnUpdate(float ts) {
	FrameBufferProps props = m_ViewportPanel.GetFrameBuffer()->GetProps();
	if (props.Height != m_ViewportPanel.GetViewportSize().y || props.Width != m_ViewportPanel.GetViewportSize().x) {
		glm::vec2 size = { m_ViewportPanel.GetViewportSize().x, m_ViewportPanel.GetViewportSize().y };
		if (size.x && size.y) {
			m_ViewportPanel.ResizeFrameBuffer(size.x, size.y);
			m_ActiveScene->SetViewportSize((uint32_t)size.x, (uint32_t)size.y);
			m_EditorCamera.SetViewportSize(size.x, size.y);
		}
	}

	m_ViewportPanel.OnUpdate(ts);
	if (m_ViewportPanel.IsHovered()) {
		m_ActiveScene->OnUpdate(ts);
		m_EditorCamera.OnUpdate(ts);
	}

	if (m_EntitiesPanel.GetSelectedEntity()) {
		if (Keyboard::IsPressed(TGL_KEY_Q))
			m_ViewportPanel.SetGuizmoType(-1);
		else if (Keyboard::IsPressed(TGL_KEY_G))
			m_ViewportPanel.SetGuizmoType(ImGuizmo::OPERATION::TRANSLATE);
		else if (Keyboard::IsPressed(TGL_KEY_R))
			m_ViewportPanel.SetGuizmoType(ImGuizmo::OPERATION::ROTATE);
		else if (Keyboard::IsPressed(TGL_KEY_S))
			m_ViewportPanel.SetGuizmoType(ImGuizmo::OPERATION::SCALE);
	}
}

void EditorLayer::OnDraw() {
	m_ViewportPanel.BindFrameBuffer();

	Renderer::Clear({ 0.15f, 0.15f, 0.15f });
	Renderer2D::ResetStats();

	m_ViewportPanel.GetFrameBuffer()->ClearAttachment(1, -1);

	m_ActiveScene->OnDrawEditor(m_EditorCamera);
	
	OnOverlayDraw();

	m_ViewportPanel.UnBindFrameBuffer();
}

void EditorLayer::OnOverlayDraw() {
	Renderer2D::Begin(m_EditorCamera);

	auto& entity = m_EntitiesPanel.GetSelectedEntity();
	if (entity) {
		auto& transform = entity.GetComponent<TransformComponent>().GetTransform();

		Renderer2D::DrawRect(transform, {1.0, 0.5, 0.0, 1.0f});
	}

	Renderer2D::End();
}

void EditorLayer::OnGuiDraw() {
	bool workspaceOpen = true;
	ImGuiRenderer::StartWorkspace(workspaceOpen);

	m_EntitiesPanel.OnImGuiRender();
	m_ContentBrowserPanel.OnImGuiRender();
	m_StatsPanel.OnImGuiRender();

	m_ViewportPanel.OnImGuiRender(m_EditorCamera, m_EntitiesPanel.GetSelectedEntity());

	ImGuiRenderer::EndWorkSpace();
}