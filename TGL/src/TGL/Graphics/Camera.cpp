#include "tglpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(const glm::vec3& position)
	: m_Position(glm::vec3(position)), m_Rotation(glm::vec3(0.0f)),
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

Camera2D::Camera2D(float aspectRatio, float zoomLevel) 
	: Camera(), m_AspectRatio(aspectRatio), m_ZoomLevel(zoomLevel){
	SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	SUB_EVENT(EventWindowResize, Camera2D::OnWindowResize);
}

Camera2D::~Camera2D(){
}

bool Camera2D::OnWindowResize(EventWindowResize& e){
	m_AspectRatio = (float)e.Width / (float)e.Height;
	RecalculateProjection();
	return true;
}

void Camera2D::RecalculateProjection(){
	SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

void Camera2D::SetProjection(float left, float right, float bottom, float top){
	m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

Camera3D::Camera3D(const glm::vec3& position) 
	: Camera(position) {
	m_AspectRatio = 4.0f / 3.0f;
	m_Up = { 0.0f, 1.0f, 0.0f };
	m_Target = { 0.0f, 0.0f, 0.0f };
	RecalculateProjection();
	RecalculateViewMatrix();
	SUB_EVENT(EventWindowResize, Camera3D::OnWindowResize);
}

Camera3D::~Camera3D() {
}

void Camera3D::SetAspectRatio(uint32_t width, uint32_t height) {
	m_AspectRatio = (float)width / (float)height;
	RecalculateProjection();
}

bool Camera3D::OnWindowResize(EventWindowResize& e) {
	SetAspectRatio(e.Width, e.Height);
	return true;
}

void Camera3D::RecalculateProjection() {
	m_ProjectionMatrix = glm::perspective((float)glm::radians(90.0f), m_AspectRatio, 0.1f, 100.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera3D::RecalculateViewMatrix(){
	m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

