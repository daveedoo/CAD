#include "FBO.h"

namespace GL
{
	GLuint FBO::currentlyBoundFboId = 0;

	FBO::FBO(unsigned int width, unsigned int height)
	{
		glGenFramebuffers(1, &this->ID);
		this->width = width;
		this->height = height;
	}

	FBO::~FBO()
	{
		glDeleteFramebuffers(1, &this->ID);
	}

	void FBO::Bind()
	{
		if (FBO::currentlyBoundFboId != this->ID)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, this->ID);
			FBO::currentlyBoundFboId = this->ID;
		}
	}

	void FBO::Unbind()
	{
		if (FBO::currentlyBoundFboId != 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			FBO::currentlyBoundFboId = 0;
		}
	}

	void FBO::SetColorAttachment(unsigned int attachmentIdx, std::shared_ptr<Texture2D> texture)
	{
		if (!this->ValidateColorAttachmentIdx(attachmentIdx))
			throw std::exception("FBO::SetColorAttachment - invalid attachment index");

		this->Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIdx, GL_TEXTURE_2D, texture->GetID(), 0);
	}

	bool FBO::ValidateColorAttachmentIdx(unsigned int idx) const
	{
		GLint maxAttachments;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttachments);
		
		return idx >= 0 && idx < maxAttachments;
	}
}
