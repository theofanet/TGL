#include "tglpch.h"
#include "Texture.h"
#include "stb_image.h"


Ref<Texture> Texture::Create(const std::string& filePath){
	return CreateRef<Texture>(filePath);
}

Ref<Texture> Texture::Create(uint32_t width, uint32_t height) {
	return CreateRef<Texture>(width, height);
}

Texture::Texture(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {
	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	CreateGLImage();
}

Texture::Texture(const std::string& filePath)
	: m_InternalFormat(0), m_DataFormat(0) {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);

	stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	ASSERT(data, "Unable to load image {0}", filePath);

	m_Width = width;
	m_Height = height;

	if (channels == 4) {
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
	}
	else if (channels == 3) {
		m_InternalFormat = GL_RGB8;
		m_DataFormat = GL_RGB;
	}

	CreateGLImage();

	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture::~Texture(){
	glDeleteTextures(1, &m_ID);
}

void Texture::Bind(uint32_t slot) const{
	glBindTextureUnit(slot, m_ID);
}

void Texture::SetSize(uint32_t width, uint32_t height) {
	m_Width = width;
	m_Height = height;
	glTexImage2D(m_ID, 0, m_InternalFormat, m_Width, m_Height, 0, GL_RED, GL_FLOAT, NULL);
}

void Texture::CreateGLImage(){
	ASSERT(m_InternalFormat & m_DataFormat, "Texture format not supported");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	
	glTextureStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height);
	//glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetData(void* data, uint32_t size) {
	uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
	ASSERT(size == m_Width * m_Height * bpp, "Data must be entire Texture !");
	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}