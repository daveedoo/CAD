#pragma once
#include <gl/glew.h>

namespace GL
{
	class EBO
	{
	public:
		enum class DataType
		{
			UBYTE = GL_UNSIGNED_BYTE,
			USHORT = GL_UNSIGNED_SHORT,
			UINT = GL_UNSIGNED_INT
		};

	private:
		static GLuint currentlyBoundEboID;
		GLuint ID;
		DataType dataType;
		size_t nrOfElements;

		size_t GetSizeof(DataType dataType);

	public:
		EBO();
		void Bind();
		void Unbind();
		void SetBufferData(const void* const data, DataType dataType, size_t nrOfElements);

		DataType GetDataType() const { return this->dataType; }
		size_t GetNrOfElements() const { return this->nrOfElements; }
	};
}
