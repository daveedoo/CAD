#include "VAO.h"
#include <cstdint>

namespace GL
{
	GLuint VAO::currentlyBoundVaoID = 0;

	VAO::VAO()
	{
		glGenVertexArrays(1, &this->ID);
		this->Bind();
	}

	void VAO::Bind()
	{
		if (currentlyBoundVaoID != this->ID)
		{
			glBindVertexArray(this->ID);
			currentlyBoundVaoID = this->ID;
		}
	}

	void VAO::Unbind()
	{
		if (currentlyBoundVaoID != 0)
		{
			glBindVertexArray(0);
			currentlyBoundVaoID = 0;
		}
	}
	void VAO::DefineFloatAttribute(VBO& vbo,
		GLuint attribIdx,
		GLint nrOfCompPerAttribute,
		FloatAttribute attributeDataType,
		GLsizei strideInBytes,
		unsigned int offsetInBytes)
	{
		this->Bind();
		vbo.Bind();

		const uint8_t* ptr = static_cast<const uint8_t*>(0) + offsetInBytes;
		glVertexAttribPointer(attribIdx, nrOfCompPerAttribute, static_cast<GLenum>(attributeDataType), GL_FALSE, strideInBytes, static_cast<const void*>(ptr));
		glEnableVertexAttribArray(attribIdx);
	}
}
