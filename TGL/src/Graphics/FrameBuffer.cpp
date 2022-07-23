#include "tglpch.h"
#include "FrameBuffer.h"

#include "Core/Application.h"


FrameBuffer::FrameBuffer(const FrameBufferProps& props) : m_Props(props) {
	Create();
}

FrameBuffer::~FrameBuffer(){
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(1, &m_TextureID);
	glDeleteTextures(1, &m_DeptTextID);
}

void const FrameBuffer::Bind() { 
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0,m_Props.Width, m_Props.Height);
}

void const FrameBuffer::Unbind() { 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Application* app = Application::GetInstance();
	glViewport(0, 0, app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight());
}

void FrameBuffer::Create(){
	if (m_ID) {
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_TextureID);
		glDeleteTextures(1, &m_DeptTextID);
	}

	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	// Creating and attaching color texture
	glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Props.Width, m_Props.Height, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

	// Creating and attaching depth texture
	glCreateTextures(GL_TEXTURE_2D, 1, &m_DeptTextID);
	glBindTexture(GL_TEXTURE_2D, m_DeptTextID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Props.Width, m_Props.Height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DeptTextID, 0);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer init failed");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize(uint32_t width, uint32_t height){
	m_Props.Width = width;
	m_Props.Height = height;
	Create();
}
