#include "tglpch.h"
#include "Material.h"
#include "Renderer.h"

Material::Material() 
	: m_Ambient(glm::vec3(1.0f)), m_Diffuse(glm::vec3(1.0f)), m_Specular(glm::vec3(1.0f)), m_Shininess(.01f), 
	m_Color(glm::vec4(1.0f)), m_AmbientMap(nullptr), m_SpecularMap(nullptr) {
	ASSERT(Renderer::IsIntialized(), "Renderer must be initialized prior to create Materials");
	m_AmbientMap = Renderer::GetWhiteTexture();
	m_SpecularMap = Renderer::GetWhiteTexture();
}

Material::~Material() {

}