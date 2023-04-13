#include "VBO.h"

namespace GL
{
	GLuint VBO::currentlyBoundVboID = 0;

	VBO::VBO(const void* data, size_t size) : dataSize(size)
	{
		glGenBuffers(1, &this->ID);
		SetBufferData(data, size);
	}

	VBO::~VBO()
	{
		glDeleteBuffers(1, &this->ID);
		if (currentlyBoundVboID == this->ID)
			currentlyBoundVboID = 0;
	}

	void VBO::Bind()
	{
		if (currentlyBoundVboID != this->ID)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->ID);
			currentlyBoundVboID = this->ID;
		}
	}

	void VBO::Unbind()
	{
		if (currentlyBoundVboID != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			currentlyBoundVboID = 0;
		}
	}

	void VBO::SetBufferData(const void* data, size_t size)
	{
		this->dataSize = size;
		if (size == 0)
			return;

		this->Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, this->dataSize, data);
	}
}
