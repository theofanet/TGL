#pragma once

#include "TGL/Core/Core.h"
#include <glad/glad.h>


class Texture {
public:
	static Ref<Texture> Create(uint32_t width, uint32_t height);
	static Ref<Texture> Create(const std::string& filePath);

	Texture(uint32_t width, uint32_t height);
	Texture(const std::string& filePath);
	~Texture();

	void SetSize(uint32_t width, uint32_t height);
	void SetData(void* data, uint32_t size);

	inline const uint32_t GetWidth() { return m_Width; }
	inline const uint32_t GetHeight() { return m_Height; }

	void Bind(uint32_t slot) const;

	inline const GLuint GetID() { return m_ID; }

	virtual bool operator==(const Texture& other) const { return m_ID == other.m_ID; }

private:
	void CreateGLImage();

	uint32_t m_Width, m_Height;
	GLuint m_ID;
	GLenum m_InternalFormat, m_DataFormat;
};