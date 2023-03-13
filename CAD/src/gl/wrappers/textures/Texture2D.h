#pragma once
#include "Texture.h"

namespace GL
{
	class Texture2D : public Texture
	{
	private:
		static std::unordered_map<unsigned int, GLuint> boundTexture2DUnits;

		unsigned int width, height;

	public:
		Texture2D(
			unsigned int width,
			unsigned int height,
			InternalFormat internalFormat = InternalFormat::RGB_32F,
			MinFilter minFilter = MinFilter::LINEAR,
			MagFilter magFilter = MagFilter::LINEAR,
			Wrap sWrap = Wrap::REPEAT,
			Wrap tWrap = Wrap::REPEAT,
			bool generateMipmaps = false);

		constexpr unsigned int GetWidth() const { return this->width; }
		constexpr unsigned int GetHeight() const { return this->height; }

		void SetSize(unsigned int width, unsigned int height);
		void SetData(const void* data, unsigned int width, unsigned int height, DataFormat format, DataType type);

		static std::optional<unsigned int> GetMaxTextureSize();
	};
}
