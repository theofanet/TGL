#include "tglpch.h"
#include "ViewportPanel.h"

#include "TGL/Core/Math.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorEvents.h"


ViewportPanel::ViewportPanel()
	: m_ViewPortHovered(false), m_ViewportSize(glm::vec2(0.0f)) {
	
}

void ViewportPanel::InitFrameBuffer() {
	FrameBufferProps props;
	props.Width = 1280;
	props.Height = 720;
	props.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::DEPTH24STENCIL8 };
	m_FrameBuffer = FrameBuffer::Create(props); 
}

void ViewportPanel::OnUpdate(float ts){
	if (Mouse::IsButtonRelease(TGL_MOUSE_BUTTON_LEFT) && !ImGuizmo::IsUsing()) {
		auto [mx, my] = ImGui::GetMousePos();

		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		my = viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
			BindFrameBuffer();
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			UnBindFrameBuffer();

			if (pixelData > -1) {
				Entity e((entt::entity)pixelData, m_Context.get());
				TRIGGER_EVENT(EntitySelectedEvent, e);
			}
		}
	}
}

void ViewportPanel::OnImGuiRender(const EditorCamera& camera, const Entity& selectedEntity) {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");

	m_ViewPortHovered = ImGui::IsWindowFocused();
	
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	
	uint64_t textureID = m_FrameBuffer->GetColorAttachmentTextureID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	if (selectedEntity && m_GuizmoType != -1) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(
			m_ViewportBounds[0].x, m_ViewportBounds[0].y, 
			m_ViewportBounds[1].x - m_ViewportBounds[0].x, 
			m_ViewportBounds[1].y - m_ViewportBounds[0].y
		);
 
		const glm::mat4& cameraProjection = camera.GetProjection();
		glm::mat4 cameraView = camera.GetViewMatrix();

		auto& component = selectedEntity.GetComponent<TransformComponent>();
		glm::mat4 transform = component.GetTransform();

		ImGuizmo::Manipulate(
			glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
			(ImGuizmo::OPERATION)m_GuizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform)
		);

		if (ImGuizmo::IsUsing()) {
			glm::vec3 translation, rotation, scale;
			DecomposeTransform(transform, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - component.Rotation;
			component.Translation = translation;
			component.Rotation += deltaRotation;
			component.Scale = scale;
		}
	}
	
	ImGui::End();
	ImGui::PopStyleVar();
}
