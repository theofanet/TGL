#include "tglpch.h"
#include "Shaders.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>



Ref<Shader> Shader::Create(const std::string& sourceFilePath){
	return CreateRef<Shader>(sourceFilePath);
}

Shader::Shader(const std::string& sourcePath){
	m_ID = glCreateProgram();

	m_Srcs = ParseSourceFile(sourcePath);

	GLuint vs = CompileShader(GL_VERTEX_SHADER, m_Srcs[Types::VERTEX]);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, m_Srcs[Types::FRAGMENT]);

	glAttachShader(m_ID, vs);
	glAttachShader(m_ID, fs);
	glLinkProgram(m_ID);
	glValidateProgram(m_ID);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader(){
	glDeleteProgram(m_ID);
}

void Shader::Bind(){
	glUseProgram(m_ID);
}

void Shader::Unbind(){
	glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value){
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value1){
	glUniform1f(GetUniformLocation(name), value1);
}

void Shader::SetFloat(const std::string& name, float value1, float value2){
	glUniform2f(GetUniformLocation(name), value1, value2);
}

void Shader::SetFloat(const std::string& name, float value1, float value2, float value3){
	glUniform3f(GetUniformLocation(name), value1, value2, value3);
}

void Shader::SetFloat(const std::string& name, float value1, float value2, float value3, float value4){
	glUniform4f(GetUniformLocation(name), value1, value2, value3, value4);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value){
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

int Shader::GetUniformLocation(const std::string& name){
	if (m_UniformLocation.find(name) != m_UniformLocation.end())
		return m_UniformLocation[name];
	int location = glGetUniformLocation(m_ID, name.c_str());
	m_UniformLocation[name] = location;
	return location;
}

Shader::Sources Shader::ParseSourceFile(const std::string& filePath){
	std::fstream stream(filePath);

	ASSERT(stream, "Shader file not found : '{0}'", filePath);
		
	std::string line;
	std::stringstream ss[Types::NB_TYPES];
	Types type = Types::VERTEX;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = Types::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = Types::FRAGMENT;
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	Sources srcs;

	for (int i = 0; i < Types::NB_TYPES; i++)
		srcs.push_back(ss[i].str());

	return srcs;
}

uint32_t Shader::CompileShader(uint32_t type, const std::string& source){
	uint32_t id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int results;
	glGetShaderiv(id, GL_COMPILE_STATUS, &results);
	if (results == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> message(length);
		glGetShaderInfoLog(id, length, &length, &message[0]);
		ASSERT(false, "Failed to compile {0} shader: {1}", ShaderTypeToString(type), message.data());
	}

	return id;
}
