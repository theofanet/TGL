#pragma once

#include "Core/Core.h"
#include <glad/glad.h>


class Texture {
public:
	static Ref<Texture> Create(const std::string& filePath);

	Texture(const std::string& filePath);
	~Texture();

	inline const uint32_t GetWidth() { return m_Width; }
	inline const uint32_t GetHeight() { return m_Height; }

	void Bind(uint32_t slot) const;
	void Unbind() const;

private:
	void CreateGLImage();

	uint32_t m_Width, m_Height;
	GLuint m_ID;
	GLenum m_InternalFormat, m_DataFormat;
};