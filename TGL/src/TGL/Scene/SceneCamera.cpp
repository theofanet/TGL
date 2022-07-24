#include "tglpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>


void SceneCamera::SetOrthographic(float size, float nearClip, float farClip){
	m_OrthoGraphicSize = size;
	m_OrthographicNear = nearClip;
	m_OrthographicFar = farClip;

	ReacalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height){
	m_ViewportWidth = width;
	m_ViewportHeight = height;
	m_AspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;

	ReacalculateProjection();
}

void SceneCamera::ReacalculateProjection() {
	float orthLeft = -m_OrthoGraphicSize * m_AspectRatio * 0.5f;
	float orthRigth = m_OrthoGraphicSize * m_AspectRatio * 0.5f;
	float orthBottom = -m_OrthoGraphicSize * 0.5f;
	float orthTop = m_OrthoGraphicSize * 0.5f;

	m_Projection = glm::ortho(orthLeft, orthRigth, orthBottom, orthTop, m_OrthographicNear, m_OrthographicFar);
}
