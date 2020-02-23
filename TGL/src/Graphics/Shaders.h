#pragma once

#include "Core/Core.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>


class Shader {
public:
	enum Types{
		VERTEX = 0, FRAGMENT,
		NB_TYPES
	};

	typedef std::vector<std::string> Sources;

	static Ref<Shader> Create(const std::string& sourceFilePath);

	Shader(const std::string& sourcePath);
	~Shader();

	void Bind();
	void Unbind();

	void SetInt(const std::string& name, int value);

	void SetFloat(const std::string& name, float value1);
	void SetFloat(const std::string& name, float value1, float value2);
	void SetFloat(const std::string& name, float value1, float value2, float value3);
	void SetFloat(const std::string& name, float value1, float value2, float value3, float value4);

	void SetMat4(const std::string& name, glm::mat4 value);

private:
	int GetUniformLocation(const std::string& name);

	Sources ParseSourceFile(const std::string& filePath);
	uint32_t CompileShader(uint32_t type, const std::string& source);

	GLuint m_ID;
	Sources m_Srcs;

	std::unordered_map<std::string, int> m_UniformLocation;
};

inline std::string ShaderTypeToString(uint32_t type) {
	switch (type) {
	case Shader::Types::FRAGMENT:
		return "Fragment";
	case GL_FRAGMENT_SHADER:
		return "Fragment";
	case Shader::Types::VERTEX:
		return "Vertex";
	case GL_VERTEX_SHADER:
		return "Vertex";
	}

	ASSERT(false, "ShaderTypeToString: Shader type not usable");
}