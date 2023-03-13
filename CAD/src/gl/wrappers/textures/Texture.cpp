#include "Texture.h"
#include <glm/gtc/type_ptr.hpp>
#include <format>

namespace GL
{
	Texture::Texture(
		Type textureType,
		std::unordered_map<unsigned int, GLuint>& boundTextureUnitsMap,
		InternalFormat internalFormat,
		MinFilter minFilter, MagFilter magFilter,
		Wrap sWrap, Wrap tWrap,
		bool hasMipmaps)
		: textureType(textureType), boundTextureUnitsMap(boundTextureUnitsMap),
		internalFormat(internalFormat),
		minFilter(minFilter), magFilter(magFilter),
		sWrap(sWrap), tWrap(tWrap),
		hasMipmaps(hasMipmaps)
	{
		glGenTextures(1, &this->ID);
		this->ExecuteTextureOperation(
			[&]()
			{
				GLenum texTypeEnum = static_cast<GLenum>(this->textureType);
				glTexParameteri(texTypeEnum, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(this->minFilter));
				glTexParameteri(texTypeEnum, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(this->magFilter));
				glTexParameteri(texTypeEnum, GL_TEXTURE_WRAP_S, static_cast<GLint>(this->sWrap));
				glTexParameteri(texTypeEnum, GL_TEXTURE_WRAP_T, static_cast<GLint>(this->tWrap));
			});
	}

	Texture::~Texture()
	{
		this->Unbind();
		glDeleteTextures(1, &this->ID);
	}

	void Texture::Bind(unsigned int textureUnitIdx)
	{
		auto currentTextureUnitIndices = this->GetBoundTextureUnitIndices();
		for (unsigned int currentTextureUnitIdx : currentTextureUnitIndices)
		{
			if (currentTextureUnitIdx == textureUnitIdx)
			{
				return;
			}
		}
		this->Unbind(textureUnitIdx);
		glActiveTexture(GL_TEXTURE0 + textureUnitIdx);
		glBindTexture(static_cast<GLenum>(this->textureType), this->ID);
		this->boundTextureUnitsMap.insert_or_assign(textureUnitIdx, this->ID);
	}

	void Texture::Unbind()
	{
		auto boundTextureUnitIndices = this->GetBoundTextureUnitIndices();
		for (unsigned int textureUnitIdx : boundTextureUnitIndices)
		{
			glActiveTexture(GL_TEXTURE0 + textureUnitIdx);
			glBindTexture(GL_TEXTURE_2D, 0);
			this->boundTextureUnitsMap.erase(textureUnitIdx);
		}
	}

	void Texture::Unbind(unsigned int textureUnitIdx)
	{
		auto currentTextureUnitIdxIt = this->boundTextureUnitsMap.find(textureUnitIdx);
		if (currentTextureUnitIdxIt == this->boundTextureUnitsMap.end() || currentTextureUnitIdxIt->second != this->ID)
		{
			return;
		}
		glActiveTexture(GL_TEXTURE0 + textureUnitIdx);
		glBindTexture(GL_TEXTURE_2D, 0);
		this->boundTextureUnitsMap.erase(textureUnitIdx);
	}

	std::vector<unsigned int> Texture::GetBoundTextureUnitIndices()
	{
		std::vector<unsigned int> boundTextureUnitIndices;
		for (const auto& entry : this->boundTextureUnitsMap)
		{
			if (entry.second == this->ID)
			{
				boundTextureUnitIndices.push_back(entry.first);
			}
		}
		return boundTextureUnitIndices;
	}

	void Texture::ExecuteTextureOperation(std::function<void()> operation)
	{
		auto boundTextureUnitIndices = this->GetBoundTextureUnitIndices();
		if (boundTextureUnitIndices.size() > 0)
		{
			glActiveTexture(GL_TEXTURE0 + boundTextureUnitIndices[0]);
			operation();
		}
		else
		{
			auto it = this->boundTextureUnitsMap.find(0);
			GLuint oldTextureId = it == this->boundTextureUnitsMap.end() ? 0 : it->second;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(static_cast<GLenum>(this->textureType), this->ID);
			operation();
			glBindTexture(static_cast<GLenum>(this->textureType), oldTextureId);
		}
	}

	void Texture::SetBorderColor(glm::vec4 color)
	{
		this->ExecuteTextureOperation(
			[&]()
			{
				glTexParameterfv(static_cast<GLenum>(this->textureType), GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
			});
	}

	bool Texture::IsDepthTexture() const
	{
		return this->internalFormat == InternalFormat::DEPTH_16NUI
			|| this->internalFormat == InternalFormat::DEPTH_24NUI
			|| this->internalFormat == InternalFormat::DEPTH_32F
			|| this->internalFormat == InternalFormat::DEPTH_24NUI_STENCIL_8NUI
			|| this->internalFormat == InternalFormat::DEPTH_32F_STENCIL_8NUI;
	}

	void Texture::SetDepthCompareModeFunc(DepthCompareFunc func)
	{
		if (this->IsDepthTexture() == false)
		{
			std::string excMsg = std::format("Texture::SetDepthCompareModeFunc - cannot set depth compare function when internal format is not of DEPTH type");
			throw std::exception(excMsg.c_str());
		}
		if ((func == DepthCompareFunc::NONE && this->isCompareModeEnabled == false)
			|| (func != DepthCompareFunc::NONE && this->compareModeFunc == func))
		{
			return;
		}
		this->ExecuteTextureOperation(
			[&]()
			{
				if (func == DepthCompareFunc::NONE)
				{
					glTexParameteri(static_cast<GLenum>(this->textureType), GL_TEXTURE_COMPARE_MODE, GL_NONE);
					this->isCompareModeEnabled = false;
				}
				else
				{
					if (this->isCompareModeEnabled == false)
					{
						glTexParameteri(static_cast<GLenum>(this->textureType), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
						this->isCompareModeEnabled = true;
					}
					glTexParameteri(static_cast<GLenum>(this->textureType), GL_TEXTURE_COMPARE_FUNC, static_cast<GLenum>(func));
					this->compareModeFunc = func;
				}
			});
	}

	void Texture::SetSwizzle(Swizzle red, Swizzle green, Swizzle blue, Swizzle alpha)
	{
		this->ExecuteTextureOperation(
			[&]()
			{
				const GLint swizzleMask[] =
				{
					static_cast<GLint>(red),
					static_cast<GLint>(green),
					static_cast<GLint>(blue),
					static_cast<GLint>(alpha)
				};
				glTexParameteriv(static_cast<GLenum>(this->textureType), GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
			}
		);
	}

	std::optional<Texture::DepthCompareFunc> Texture::GetDepthCompareModeFunc() const
	{
		return this->IsDepthTexture() ? std::make_optional<>(this->compareModeFunc) : std::nullopt;
	}

	std::pair<Texture::DataFormat, Texture::DataType> Texture::GetMatchingTextureFormatAndType(InternalFormat internalFormat)
	{
		switch (internalFormat)
		{
		case InternalFormat::DEPTH_24NUI_STENCIL_8NUI:
			return std::make_pair<>(DataFormat::DEPTH_STENCIL, DataType::UNSIGNED_INT_24_8);
		case InternalFormat::DEPTH_32F_STENCIL_8NUI:
			return std::make_pair<>(DataFormat::DEPTH_STENCIL, DataType::FLOAT_32_UNSIGNED_INT_24_8_REV);
		case InternalFormat::DEPTH_16NUI:
		case InternalFormat::DEPTH_24NUI:
		case InternalFormat::DEPTH_32F:
			return std::make_pair<>(DataFormat::DEPTH_COMPONENT, DataType::FLOAT);
		default:
			return std::make_pair<>(DataFormat::RGBA, DataType::FLOAT);
		}
	}

	bool Texture::ValidateTextureFormatCorrespondence(InternalFormat internalFormat, DataFormat dataFormat, DataType dataType)
	{
		bool isTextureFormatValid = true;
		if (internalFormat == InternalFormat::DEPTH_24NUI_STENCIL_8NUI
			|| internalFormat == InternalFormat::DEPTH_32F_STENCIL_8NUI)
		{
			if (dataFormat != DataFormat::DEPTH_STENCIL
				|| (dataType != DataType::UNSIGNED_INT_24_8 && dataType != DataType::FLOAT_32_UNSIGNED_INT_24_8_REV))
			{
				isTextureFormatValid = false;
			}
		}
		else if (internalFormat == InternalFormat::DEPTH_16NUI
			|| internalFormat == InternalFormat::DEPTH_24NUI
			|| internalFormat == InternalFormat::DEPTH_32F)
		{
			if (dataFormat != DataFormat::DEPTH_COMPONENT
				|| dataType == DataType::UNSIGNED_INT_24_8
				|| dataType == DataType::FLOAT_32_UNSIGNED_INT_24_8_REV)
			{
				isTextureFormatValid = false;
			}
		}
		else
		{
			if (dataFormat == DataFormat::DEPTH_COMPONENT
				|| dataFormat == DataFormat::DEPTH_STENCIL
				|| dataType == DataType::UNSIGNED_INT_24_8
				|| dataType == DataType::FLOAT_32_UNSIGNED_INT_24_8_REV)
			{
				isTextureFormatValid = false;
			}
		}
		return isTextureFormatValid;
	}

	std::optional<unsigned int> Texture::GetMaxTextureUnits()
	{
		static std::optional<unsigned int> maxTextureUnits;
		if (glGetIntegerv != NULL && !maxTextureUnits.has_value())
		{
			int texUnits;
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texUnits);
			maxTextureUnits = texUnits;
		}
		return maxTextureUnits;
	}
}