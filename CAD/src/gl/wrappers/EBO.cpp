#include "EBO.h"
#include <stdexcept>
#include <format>
#include "VAO.h"

namespace GL
{
	EBO::EBO(const GL::VAO& vao)
		: nrOfElements(0),
		vao(vao)
	{
		this->vao.Bind();
		glGenBuffers(1, &this->ID);
	}

	void EBO::Bind() const
	{
		this->vao.Bind();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
	}

	void EBO::Unbind() const
	{
		this->vao.Bind();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void EBO::SetBufferData(const void* const data, DataType dataType, size_t nrOfElements)
	{
		this->Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfElements * GetSizeOf(dataType), data, GL_STATIC_DRAW);
		this->dataType = dataType;
		this->nrOfElements = nrOfElements;
	}

	size_t EBO::GetSizeOf(DataType dataType)
	{
		switch (dataType)
		{
		case GL::EBO::DataType::UBYTE:
			return sizeof(GLubyte);
		case GL::EBO::DataType::USHORT:
			return sizeof(GLushort);
		case GL::EBO::DataType::UINT:
			return sizeof(GLuint);
		default:
			throw std::invalid_argument(std::format("EBO:GetSizeof - invalid dataType argument: {}.", static_cast<int>(dataType)));
		}
	}
}
