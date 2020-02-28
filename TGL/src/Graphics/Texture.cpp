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

	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture::~Texture(){
	glDeleteTextures(1, &m_ID);
}

void Texture::Bind(uint32_t slot) const{
	glBindTextureUnit(slot, m_ID);
}

void Texture::CreateGLImage(){
	ASSERT(m_InternalFormat & m_DataFormat, "Texture format not supported");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height);
	
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
