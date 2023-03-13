#pragma once
#include <GL/glew.h>
#include "textures/Texture2D.h"
#include <memory>

namespace GL
{
	class FBO
	{
	private:
		static GLuint currentlyBoundFboId;
		
		GLuint ID;
		unsigned int width, height;

	public:
		FBO(unsigned int width, unsigned int height);
		~FBO();

		void Bind();
		static void Unbind();

		bool ValidateColorAttachmentIdx(unsigned int idx) const;
		void SetColorAttachment(unsigned int attachmentIdx, std::shared_ptr<Texture2D> texture);
	};
}
