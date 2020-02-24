#include "tglpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() 
	: m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)),
	  m_ProjectionMatrix(glm::mat4(1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_ViewProjectionMatrix(glm::mat4(1.0f)) {
}

Camera::~Camera(){
}

void Camera::RecalculateViewMatrix(){
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), { 1, 0, 0 })
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), { 0, 1, 0 })
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), { 0, 0, 1 });

	m_ViewMatrix = glm::inverse(transform);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

Camera2D::Camera2D(float aspectRatio, float zoomLevel) : Camera(), m_AspectRatio(aspectRatio), m_ZoomLevel(zoomLevel){
	SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

Camera2D::~Camera2D(){
}

void Camera2D::SetProjection(float left, float right, float bottom, float top){
	m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}


