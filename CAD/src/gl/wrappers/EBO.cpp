#include "EBO.h"
#include <stdexcept>
#include <format>

namespace GL
{
	GLuint EBO::currentlyBoundEboID = 0;

	EBO::EBO()
		: nrOfElements(0)
	{
		glGenBuffers(1, &this->ID);
	}

	void EBO::Bind()
	{
		if (currentlyBoundEboID != this->ID)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
			currentlyBoundEboID = this->ID;
		}
	}

	void EBO::Unbind()
	{
		if (currentlyBoundEboID != 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			currentlyBoundEboID = 0;
		}
	}

	void EBO::SetBufferData(const void* const data, DataType dataType, size_t nrOfElements)
	{
		this->Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrOfElements * GetSizeof(dataType), data, GL_STATIC_DRAW);
		this->dataType = dataType;
		this->nrOfElements = nrOfElements;
	}

	size_t EBO::GetSizeof(DataType dataType)
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
