#include "tglpch.h"
#include "Light.h"

#include <stdlib.h>


Light::Light(Type type)
	: m_Type(type),
	m_Ambient(glm::vec3(1.0f)), m_Specular(glm::vec3(1.0)), m_Diffuse(glm::vec3(1.0)),
	m_Position(glm::vec3(.0f)), m_Direction(glm::vec3(.0f)) {

}

Light::~Light(){

}

void Light::SetAttenuation(float constant, float linear, float quadratic) {
	m_Constant = constant;
	m_Linear = linear;
	m_Quadratic = quadratic;
}
