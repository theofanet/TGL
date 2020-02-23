#pragma once

#include "Core/Core.h"

#include <glad/glad.h>
#include <vector>
#include <string>


class Shader {
public:
	enum Types{
		VERTEX = 0, FRAGMENT,
		NB_TYPES
	};

	typedef std::vector<std::string> Sources;

	Shader(const std::string& sourcePath);
	~Shader();

	void Bind();
	void Unbind();

private:
	Sources ParseSourceFile(const std::string& filePath);
	uint32_t CompileShader(uint32_t type, const std::string& source);

	GLuint m_ID;
	Sources m_Srcs;
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