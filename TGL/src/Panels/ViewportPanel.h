#pragma once

#include "TGL/Graphics/FrameBuffer.h"

#include <glm/glm.hpp>


class ViewportPanel {
public:
	ViewportPanel();

	inline void InitFrameBuffer() { m_FrameBuffer = FrameBuffer::Create(1280, 720); }

	void OnImGuiRender();

	inline const Ref<FrameBuffer>& GetFrameBuffer() const { return m_FrameBuffer; }
	inline void ResizeFrameBuffer(float width, float height) { m_FrameBuffer->Resize((uint32_t)width, (uint32_t)height); }
	inline void BindFrameBuffer() const { m_FrameBuffer->Bind(); }
	inline void UnBindFrameBuffer() const { m_FrameBuffer->Unbind(); }

	inline const glm::vec2& GetViewportSize() const { return m_ViewportSize; }
	inline const bool IsHovered() const { return m_ViewPortHovered; }

private:
	Ref<FrameBuffer> m_FrameBuffer;
	glm::vec2 m_ViewportSize;
	bool m_ViewPortHovered;
};