#pragma once
#include <gl/glew.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <optional>
#include <functional>

namespace GL
{
	class Texture
	{
	public:
		enum class Type
		{
			TEXTURE_2D = GL_TEXTURE_2D,
			TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
			CUBE_MAP = GL_TEXTURE_CUBE_MAP
		};

		// N - normalized
		// U - unsigned
		// I - integer
		// F - float
		enum class InternalFormat
		{
			DEPTH_24NUI_STENCIL_8NUI = GL_DEPTH24_STENCIL8, // DataFormat must be DEPTH_STENCIL and type must be UNSIGNED_INT_24_8 or FLOAT_32_UNSIGNED_INT_24_8_REV
			DEPTH_32F_STENCIL_8NUI = GL_DEPTH32F_STENCIL8, // DataFormat must be DEPTH_STENCIL and type must be UNSIGNED_INT_24_8 or FLOAT_32_UNSIGNED_INT_24_8_REV
			DEPTH_16NUI = GL_DEPTH_COMPONENT16, // DataFormat must be DEPTH_COMPONENT
			DEPTH_24NUI = GL_DEPTH_COMPONENT24, // DataFormat must be DEPTH_COMPONENT
			DEPTH_32F = GL_DEPTH_COMPONENT32, // DataFormat must be DEPTH_COMPONENT
			RGBA_8NUI = GL_RGBA8,
			RGBA_16NUI = GL_RGBA16,
			RGBA_8I = GL_RGBA8I,
			RGBA_16I = GL_RGBA16I,
			RGBA_32I = GL_RGBA32I,
			RGBA_16F = GL_RGBA16F,
			RGBA_32F = GL_RGBA32F,
			RGB_8NUI = GL_RGB8,
			RGB_16NUI = GL_RGB16,
			RGB_8I = GL_RGB8I,
			RGB_16I = GL_RGB16I,
			RGB_32I = GL_RGB32I,
			RGB_16F = GL_RGB16F,
			RGB_32F = GL_RGB32F,
			SRGB_NUI = GL_SRGB,
			SRGBA_NUI = GL_SRGB_ALPHA,
			R_8NUI = GL_R8,
			R_8I = GL_R8I,
			R_16I = GL_R16I,
			R_32I = GL_R32I,
			R_16F = GL_R16F,
			R_32F = GL_R32F
		};

		enum class Wrap
		{
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			REPEAT = GL_REPEAT
		};

		enum class MinFilter
		{
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
		};

		enum class MagFilter
		{
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR
		};

		enum class DataFormat
		{
			RGB = GL_RGB,
			RGBA = GL_RGBA,
			RED = GL_RED,
			DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
			DEPTH_STENCIL = GL_DEPTH_STENCIL
		};

		enum class DataType
		{
			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			BYTE = GL_BYTE,
			UNSIGNED_INT = GL_UNSIGNED_INT,
			UNSIGNED_INT_24_8 = GL_UNSIGNED_INT_24_8,
			INT = GL_INT,
			FLOAT = GL_FLOAT,
			FLOAT_32_UNSIGNED_INT_24_8_REV = GL_FLOAT_32_UNSIGNED_INT_24_8_REV
		};

		enum class DepthCompareFunc
		{
			LESS_OR_EQUAL = GL_LEQUAL,
			GREATER_OR_EQUAL = GL_GEQUAL,
			LESS = GL_LESS,
			GREATER = GL_GREATER,
			EQUAL = GL_EQUAL,
			NOT_EQUAL = GL_NOTEQUAL,
			ALWAYS = GL_ALWAYS,
			NEVER = GL_NEVER,
			NONE = GL_NONE,
		};

		enum class Swizzle
		{
			RED_COMP = GL_RED,
			GREEN_COMP = GL_GREEN,
			BLUE_COMP = GL_BLUE,
			ALPHA_COMP = GL_ALPHA,
			ZERO = GL_ZERO,
			ONE = GL_ONE,
		};

	private:
		std::unordered_map<unsigned int, GLuint>& boundTextureUnitsMap;

		bool isCompareModeEnabled = false;
		DepthCompareFunc compareModeFunc = DepthCompareFunc::LESS_OR_EQUAL;

	protected:
		Texture(
			Type textureType,
			std::unordered_map<unsigned int, GLuint>& boundTextureUnitsMap,
			InternalFormat internalFormat,
			MinFilter minFilter, MagFilter magFilter,
			Wrap sWrap, Wrap tWrap,
			bool hasMipmaps);

		GLuint ID;
		const InternalFormat internalFormat;

		MinFilter minFilter = MinFilter::NEAREST_MIPMAP_LINEAR;
		MagFilter magFilter = MagFilter::LINEAR;
		Wrap sWrap = Wrap::REPEAT;
		Wrap tWrap = Wrap::REPEAT;
		bool hasMipmaps = false;

		glm::vec4 borderColor = glm::vec4(0.0, 0.0, 0.0, 0.0);

		bool IsDepthTexture() const;

		std::pair<DataFormat, DataType> GetMatchingTextureFormatAndType(InternalFormat internalFormat);
		bool ValidateTextureFormatCorrespondence(InternalFormat internalFormat, DataFormat dataFormat, DataType dataType);

		void ExecuteTextureOperation(std::function<void()> operation);

	public:
		const Type textureType;

		virtual ~Texture();

		constexpr GLuint GetID() const { return this->ID; }

		constexpr InternalFormat GetInternalFormat() const { return this->internalFormat; }
		constexpr MinFilter GetMinFilter() const { return this->minFilter; }
		constexpr MagFilter GetMagFilter() const { return this->magFilter; }
		constexpr Wrap GetSWrap() const { return this->sWrap; }
		constexpr Wrap GetTWrap() const { return this->tWrap; }
		constexpr bool HasMipmaps() const { return this->hasMipmaps; }

		constexpr glm::dvec4 GetBorderColor() const { return this->borderColor; };
		void SetBorderColor(glm::vec4 color);

		std::optional<DepthCompareFunc> GetDepthCompareModeFunc() const;
		void SetDepthCompareModeFunc(DepthCompareFunc func);

		void SetSwizzle(Swizzle red, Swizzle green, Swizzle blue, Swizzle alpha);

		void Bind(unsigned int textureUnitIdx);
		void Unbind();
		void Unbind(unsigned int textureUnitIdx);
		std::vector<unsigned int> GetBoundTextureUnitIndices();

		static std::optional<unsigned int> GetMaxTextureUnits();
	};
}