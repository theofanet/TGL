#include "tglpch.h"
#include "FrameBuffer.h"

#include "TGL/Core/Application.h"


static bool IsDepthFormat(FrameBufferTextureFormat format){
	switch (format) {
		case FrameBufferTextureFormat::DEPTH24STENCIL8:  return true;
	}

	return false;
}

static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
	bool multisample = samples > 1;

	if (multisample) 
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, id, 0);
}

static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
{
	bool multisample = samples > 1;
	if (multisample)
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
	else{
		glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, id, 0);
}

static GLenum TextureFormatToGL(FrameBufferTextureFormat format){
	switch (format) {
		case FrameBufferTextureFormat::RGBA8:       return GL_RGBA8;
		case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
	}

	ASSERT(false, "Texture Format not available");
	return 0;
}

Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferProps& props){
	return CreateRef<FrameBuffer>(props);
}

FrameBuffer::FrameBuffer(const FrameBufferProps& props) : m_Props(props) {
	for (auto format : m_Props.Attachments.Attachments) {
		if (!IsDepthFormat(format.TextureFormat))
			m_ColorAttachmentSpecifications.emplace_back(format);
		else
			m_DepthAttachmentSpecification = format;
	}

	CreateTextures();
}

FrameBuffer::~FrameBuffer(){
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
	glDeleteTextures(1, &m_DepthAttachment);
}

void const FrameBuffer::Bind() { 
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0, m_Props.Width, m_Props.Height);
}

void const FrameBuffer::Unbind() { 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Application* app = Application::GetInstance();
	//glViewport(0, 0, app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight());
}

void FrameBuffer::CreateTextures(){
	if (m_ID) {
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);

		m_ColorAttachments.clear();
		m_DepthAttachment = 0;
	}

	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	bool multisample = m_Props.Samples > 1;
	GLenum textureTarget = multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

	if (m_ColorAttachmentSpecifications.size()) {
		m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
		glCreateTextures(textureTarget, (GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());

		for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
			glBindTexture(textureTarget, m_ColorAttachments[i]);

			switch (m_ColorAttachmentSpecifications[i].TextureFormat) {
				case FrameBufferTextureFormat::RGBA8:
					AttachColorTexture(m_ColorAttachments[i], m_Props.Samples, GL_RGBA8, GL_RGBA, m_Props.Width, m_Props.Height, (int)i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER:
					AttachColorTexture(m_ColorAttachments[i], m_Props.Samples, GL_R32I, GL_RED_INTEGER, m_Props.Width, m_Props.Height, (int)i);
					break;
			}
		}
	}

	if (m_DepthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None) {
		glCreateTextures(textureTarget, 1, &m_DepthAttachment);
		glBindTexture(textureTarget, m_DepthAttachment);

		switch (m_DepthAttachmentSpecification.TextureFormat) {
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(m_DepthAttachment, m_Props.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Props.Width, m_Props.Height);
				break;
		}
	}

	if (m_ColorAttachments.size() > 1) {
		ASSERT(m_ColorAttachments.size() <= 4, "Too many attachments");
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
	}
	else if (m_ColorAttachments.empty())
		glDrawBuffer(GL_NONE);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer init failed");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize(uint32_t width, uint32_t height){
	m_Props.Width = width;
	m_Props.Height = height;
	CreateTextures();
}

void FrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value){
	ASSERT(attachmentIndex < m_ColorAttachments.size(), "Index out of bounds");

	auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
	glClearTexImage(m_ColorAttachments[attachmentIndex], 0, TextureFormatToGL(spec.TextureFormat), GL_INT, &value);
}

int FrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y){
	ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment Index Out Of Bounds");

	int pixelData;

	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

	return pixelData;
}
