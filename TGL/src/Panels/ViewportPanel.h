#pragma once

#include "TGL/TGL.h"

#include <glm/glm.hpp>


class ViewportPanel {
public:
	ViewportPanel();

	inline void SetContext(const Ref<Scene>& scene) { m_Context = scene; }
	void InitFrameBuffer();

	void OnUpdate(float ts);
	void OnImGuiRender(const EditorCamera& camera, const Entity& selectedEntity = {});

	inline const Ref<FrameBuffer>& GetFrameBuffer() const { return m_FrameBuffer; }
	inline void ResizeFrameBuffer(float width, float height) { 
		m_FrameBuffer->Resize((uint32_t)width, (uint32_t)height);	
	}
	inline void BindFrameBuffer() const { m_FrameBuffer->Bind(); }
	inline void UnBindFrameBuffer() const { m_FrameBuffer->Unbind(); }

	inline const glm::vec2& GetViewportSize() const { return m_ViewportSize; }
	inline const bool IsHovered() const { return m_ViewPortHovered; }

	inline void SetGuizmoType(int type) { m_GuizmoType = type; }

private:
	Ref<Scene> m_Context;
	Ref<FrameBuffer> m_FrameBuffer;

	glm::vec2 m_ViewportSize;
	glm::vec2 m_ViewportBounds[2];

	bool m_ViewPortHovered;

	int m_GuizmoType = -1;
};