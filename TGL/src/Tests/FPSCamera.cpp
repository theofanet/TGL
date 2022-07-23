#include "tglpch.h"
#include "FPSCamera.h"
#include "TGL.h"
#include <glm/gtc/matrix_transform.hpp>


FPSCamera::FPSCamera() 
	: m_Front({ .0f, .0f, -1.0f }), m_Pitch(.0f), m_Yaw(.0f),
    m_MouseSensitivity(.35f), m_Speed(2.5f), Camera3D() {
	m_LastMousePos = Mouse::GetPosition();
}

FPSCamera::~FPSCamera() {

}

void FPSCamera::OnUpdate(float ts) {
    HandleMouseUpdate(ts);
    HandleKeyboardUpdate(ts);
    m_Target = m_Position - m_Front;
}

void FPSCamera::HandleMouseUpdate(float ts) {
    glm::vec2 mousePos = Mouse::GetPosition();
    float xoffset = mousePos.x - m_LastMousePos.x;
    float yoffset = mousePos.y - m_LastMousePos.y;

    m_LastMousePos = mousePos;

    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    if (m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
        m_Pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
}

void FPSCamera::HandleKeyboardUpdate(float ts) {
    glm::vec3 pos = GetPosition();

    if (Keyboard::IsHeld(GLFW_KEY_S))
        pos += m_Speed * ts * m_Front;
    if (Keyboard::IsHeld(GLFW_KEY_W))
        pos -= m_Speed * ts * m_Front;
    if (Keyboard::IsHeld(GLFW_KEY_D))
        pos -= glm::normalize(glm::cross(m_Front, m_Up)) * (m_Speed * 1.8f) * ts;
    if (Keyboard::IsHeld(GLFW_KEY_A))
        pos += glm::normalize(glm::cross(m_Front, m_Up)) * (m_Speed * 1.8f) * ts;

    SetPosition(pos);
}

