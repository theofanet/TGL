#pragma once

#include "TGL/Graphics/Camera.h"


class SceneCamera : public RawCamera {
public:
	SceneCamera() { ReacalculateProjection(); };
	virtual ~SceneCamera() = default;

	void SetOrthographic(float size, float nearClip, float farClip);
	void SetViewportSize(uint32_t width, uint32_t height);

	inline float GetOrthographicSize() const { return m_OrthoGraphicSize; }
	inline void SetOrthographicSize(float size) { m_OrthoGraphicSize = size; ReacalculateProjection(); }

	inline const float GetAspectRatio() const { return m_AspectRatio; }
	inline const uint32_t GetViewportWidth() const { return m_ViewportWidth; }
	inline const uint32_t GetViewportHeight() const { return m_ViewportHeight; }

private:
	void ReacalculateProjection();

private:
	float m_OrthoGraphicSize = 10.0f;
	float m_OrthographicNear = -1.0f;
	float m_OrthographicFar = 1.0f;

	float m_AspectRatio = 0.0f;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};