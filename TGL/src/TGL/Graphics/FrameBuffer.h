#pragma once


#include <glad/glad.h>
#include "Texture.h"
#include "TGL/Core/Events.h"


struct FrameBufferProps {
	uint32_t Width, Height;
};


class FrameBuffer {
public:
	FrameBuffer(const FrameBufferProps& props);
	virtual ~FrameBuffer();

	virtual const void Bind();
	virtual const void Unbind();

	inline const GLuint GetTextureID() { return m_TextureID; }
	inline const FrameBufferProps& GetProps() { return m_Props; }

	void Create();
	void Resize(uint32_t width, uint32_t height);

protected:
	GLuint m_ID, m_TextureID, m_DeptTextID;
	FrameBufferProps m_Props;
};