#pragma once
#include <gl/glew.h>

namespace GL
{
	class VBO
	{
	private:
		static GLuint currentlyBoundVboID;

		GLuint ID;
		size_t dataSize;

		void SetBufferData(const void* data, size_t size);

	public:
		VBO(const void* data, size_t size);
		VBO::~VBO();

		void Bind();
		static void Unbind();

		GLuint GetID() const { return this->ID; }
		size_t GetDataSize() { return this->dataSize; }
	};
}
