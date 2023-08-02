#pragma once
#include <gl/glew.h>

namespace GL
{
	class VAO;

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
		const GL::VAO& vao;
		GLuint ID;
		DataType dataType;
		size_t nrOfElements;

		static size_t GetSizeOf(DataType dataType);

	public:
		EBO(const GL::VAO& vao);
		void Bind() const;
		void Unbind() const;
		void SetBufferData(const void* const data, DataType dataType, size_t nrOfElements);

		DataType GetDataType() const { return this->dataType; }
		size_t GetNrOfElements() const { return this->nrOfElements; }
	};
}
