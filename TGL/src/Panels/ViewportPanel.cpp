#include "tglpch.h"
#include "ViewportPanel.h"

#include "imgui.h"


ViewportPanel::ViewportPanel()
	: m_ViewPortHovered(false), m_ViewportSize(glm::vec2(0.0f)) {
	
}

void ViewportPanel::OnImGuiRender() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");
	
	m_ViewPortHovered = ImGui::IsWindowFocused();
	
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	
	uint64_t textureID = m_FrameBuffer->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	
	ImGui::End();
	ImGui::PopStyleVar();
}
