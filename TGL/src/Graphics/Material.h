#pragma once

#include <glm/glm.hpp>
#include "Texture.h"


class Material {
public: 
	Material();
	virtual ~Material();

	inline void SetAmbientMap(Ref<Texture> texture) { m_AmbientMap = texture; }
	inline const Ref<Texture> GetAmbientMap() { return m_AmbientMap; }

	inline void SetSpecularMap(Ref<Texture> texture) { m_SpecularMap = texture; }
	inline const Ref<Texture> GetSpecularMap() { return m_SpecularMap; }

	inline void SetColor(const glm::vec4& color) { m_Color = color; }
	inline void SetAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
	inline void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
	inline void SetSpecular(const glm::vec3& specular) { m_Specular = specular; }
	inline void SetShininess(float shininess) { m_Shininess = shininess; }

	inline const glm::vec4& GetColor() { return m_Color; }
	inline const glm::vec3& GetAmbient() { return m_Ambient; }
	inline const glm::vec3& GetDiffuse() { return m_Diffuse; }
	inline const glm::vec3& GetSpecular() { return m_Specular; }
	inline float GetShininess() { return m_Shininess; }

protected:
	glm::vec4 m_Color;
	glm::vec3 m_Ambient, m_Diffuse, m_Specular;
	Ref<Texture> m_AmbientMap, m_SpecularMap;
	float m_Shininess;
};