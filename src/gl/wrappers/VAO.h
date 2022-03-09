#pragma once
#include <gl/glew.h>
#include "VBO.h"

namespace GL
{
	class VAO
	{
	private:
		static GLuint currentlyBoundVaoID;

		GLuint ID;

	public:
		enum class FloatAttribute
		{
			BYTE = GL_BYTE,
			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			SHORT = GL_SHORT,
			UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
			INT = GL_INT,
			UNSIGNED_INT = GL_UNSIGNED_INT,
			HALF_FLOAT = GL_HALF_FLOAT,
			FLOAT = GL_FLOAT,
			DOUBLE = GL_DOUBLE
		};

	public:
		VAO();

		GLuint GetID() { return this->ID; }

		void Bind();
		static void Unbind();

		void DefineFloatAttribute(VBO& vbo,
			GLuint attribIdx,
			GLint nrOfCompPerAttribute,
			FloatAttribute attributeDataType,
			GLsizei strideInBytes,
			unsigned int offsetInBytes);
	};
}
