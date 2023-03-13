#include "Texture2D.h"

#include <exception>
#include <format>

namespace GL
{
	std::unordered_map<unsigned int, GLuint> Texture2D::boundTexture2DUnits;

	Texture2D::Texture2D(
		unsigned int width,
		unsigned int height,
		InternalFormat internalFormat,
		MinFilter minFilter,
		MagFilter magFilter,
		Wrap sWrap,
		Wrap tWrap,
		bool generateMipmaps)
		: Texture(Type::TEXTURE_2D, boundTexture2DUnits, internalFormat, minFilter, magFilter, sWrap, tWrap, generateMipmaps),
		width(0), height(0)
	{
		this->SetSize(width, height);
	}

	void Texture2D::SetSize(unsigned int width, unsigned int height)
	{
		if (width == this->width && height == this->height)
		{
			return;
		}
		auto formatInfo = this->GetMatchingTextureFormatAndType(this->internalFormat);
		this->SetData(nullptr, width, height, formatInfo.first, formatInfo.second);
	}

	void Texture2D::SetData(const void* data, unsigned int width, unsigned int height, DataFormat format, DataType type)
	{
		if (this->ValidateTextureFormatCorrespondence(this->internalFormat, format, type) == false)
		{
			std::string excMsg = std::format("Texture2D::SetData - invalid argument values: format={} , type={}", static_cast<int>(format), static_cast<int>(type));
			throw std::invalid_argument(excMsg);
		}
		if (width == this->width && height == this->height && data == nullptr)
		{
			return;
		}

		this->ExecuteTextureOperation(
			[&]()
			{
				if (width == this->width && height == this->height)
				{
					glTexSubImage2D(
						GL_TEXTURE_2D,
						0,
						0,
						0,
						width,
						height,
						static_cast<GLenum>(format),
						static_cast<GLenum>(type),
						data);
				}
				else
				{
					glTexImage2D(
						GL_TEXTURE_2D,
						0,
						static_cast<GLint>(this->internalFormat),
						width,
						height,
						0,
						static_cast<GLenum>(format),
						static_cast<GLenum>(type),
						data);
				}
				if (this->hasMipmaps)
				{
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				this->width = width;
				this->height = height;
			});
	}

	std::optional<unsigned int> Texture2D::GetMaxTextureSize()
	{
		static std::optional<unsigned int> maxTextureSize;
		if (glGetIntegerv != NULL && maxTextureSize.has_value() == false)
		{
			int texSize;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
			maxTextureSize = texSize;
		}
		return maxTextureSize;
	}
}