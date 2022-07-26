#pragma once


#include <glad/glad.h>
#include "Texture.h"
#include "TGL/Core/Events.h"


enum class FrameBufferTextureFormat {
	None = 0,

	// Color
	RGBA8,
	RED_INTEGER,

	// Depth / Stencil
	DEPTH24STENCIL8,

	Depth = DEPTH24STENCIL8
};


struct FrameBufferTextureSpecification {
	FrameBufferTextureSpecification() = default;
	FrameBufferTextureSpecification(FrameBufferTextureFormat format)
		: TextureFormat(format) {}

	FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
};


struct FrameBufferAttachmentSpecification {
	FrameBufferAttachmentSpecification() = default;
	FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
		: Attachments(attachments) {}

	std::vector<FrameBufferTextureSpecification> Attachments;
};


struct FrameBufferProps {
	uint32_t Width, Height;
	FrameBufferAttachmentSpecification Attachments;

	uint32_t Samples = 1;
	bool SwapChainTarget = false;
};


class FrameBuffer {
public:
	static Ref<FrameBuffer> Create(const FrameBufferProps& props);

	FrameBuffer(const FrameBufferProps& props);
	virtual ~FrameBuffer();

	virtual const void Bind();
	virtual const void Unbind();

	inline const GLuint GetColorAttachmentTextureID(int idx = 0) { 
		ASSERT(idx < m_ColorAttachments.size(), "Index Out Of Bounds");
		return m_ColorAttachments[idx];
	}
	inline const FrameBufferProps& GetProps() { return m_Props; }

	void CreateTextures();
	void Resize(uint32_t width, uint32_t height);
	void ClearAttachment(uint32_t attachmentIndex, int value);

	int ReadPixel(uint32_t attachmentIndex, int x, int y);

protected:
	GLuint m_ID;
	FrameBufferProps m_Props;

	std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
	FrameBufferTextureSpecification m_DepthAttachmentSpecification = FrameBufferTextureFormat::None;

	std::vector<uint32_t> m_ColorAttachments;
	uint32_t m_DepthAttachment = 0;
};