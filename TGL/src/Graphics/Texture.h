#pragma once

#include "Core/Core.h"
#include <glad/glad.h>


class Texture {
public:
	static Ref<Texture> Create(uint32_t width, uint32_t height);
	static Ref<Texture> Create(const std::string& filePath);

	Texture(uint32_t width, uint32_t height);
	Texture(const std::string& filePath);
	~Texture();

	void SetData(void* data, uint32_t size);

	inline const uint32_t GetWidth() { return m_Width; }
	inline const uint32_t GetHeight() { return m_Height; }

	void Bind(uint32_t slot) const;

private:
	void CreateGLImage();

	uint32_t m_Width, m_Height;
	GLuint m_ID;
	GLenum m_InternalFormat, m_DataFormat;
};