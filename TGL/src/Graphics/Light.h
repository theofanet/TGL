#pragma once

#include <glm/glm.hpp>


class Light {
public:
	enum Type {
		None = 0,
		DIRECTIONAL = 1, POINT = 2, FLASH = 3
	};

	Light(Type type = Type::DIRECTIONAL);
	virtual ~Light();

	virtual void SetAttenuation(float constant, float linear, float quadratic);

	inline virtual void SetPosition(const glm::vec3& position) { m_Position = position; }
	inline virtual void SetDirection(const glm::vec3& direction) { m_Direction = direction; }
	inline virtual void SetAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
	inline virtual void SetDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
	inline virtual void SetSpecular(const glm::vec3& specular) { m_Specular = specular; }
	inline virtual void SetCutOff(const float cutOff) { m_CutOff = cutOff; }
	inline virtual void SetOuterCutOff(const float cutOff) { m_OuterCutOff = cutOff; }

	inline virtual float GetConstant() { return m_Constant; }
	inline virtual float GetLinear() { return m_Linear; }
	inline virtual float GetQuadratic() { return m_Quadratic; }

	inline const glm::vec3& GetPosition() { return m_Position; }
	inline const glm::vec3& GetDirection() { return m_Direction; }
	inline const float GetCutOff() { return m_CutOff; }
	inline const float GetOuterCutOff() { return m_OuterCutOff; }
	inline virtual const glm::vec3& GetAmbient() { return m_Ambient; }
	inline virtual const glm::vec3& GetDiffuse() { return m_Diffuse; }
	inline virtual const glm::vec3& GetSpecular() { return m_Specular; }

	inline virtual void SetType(const Type type) { m_Type = type; }
	inline virtual const Type GetType() { return m_Type; }

protected:
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_Ambient, m_Specular, m_Diffuse;

	Type m_Type;

	// Point Light Attenuation data
	float m_Constant;
	float m_Linear;
	float m_Quadratic;

	// Flashlight
	float m_CutOff, m_OuterCutOff;
};
