#include "tglpch.h"
#include "Texture.h"
#include "stb_image.h"


Ref<Texture> Texture::Create(const std::string& filePath){
	return CreateRef<Texture>(filePath);
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

	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

Texture::~Texture(){
	glDeleteTextures(1, &m_ID);
}

void Texture::Bind(uint32_t slot) const{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::CreateGLImage(){
	ASSERT(m_InternalFormat & m_DataFormat, "Texture format not supported");

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	//glTexStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
