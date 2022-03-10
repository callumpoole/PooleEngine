#include "image.h"

#include "stb/stb_image.h"
#include <sstream>

namespace Poole
{
	Image Image::s_Invalid = Image();
	bool Image::s_YFlip = false;
	
	Image::Image(const char* path, bool* out_Successful, bool printLogWhenLoaded)
	{
		m_YFlippedWhenLoaded = s_YFlip;
		i32 width = -1, height = -1, channels = -1;
		m_Bytes = stbi_load(path, &width, &height, &channels, 0);
		m_AllocatedWithSTBI = true;
	
		if (!m_Bytes || width <= 0 || height <= 0 || channels <= 0)
		{
			LOG_ERROR("Failed to load image: {}", path);
	
			if (out_Successful)
				*out_Successful = false;

			return;
		}
		m_Size.x = u32(width);
		m_Size.y = u32(height);
		m_NumChannels = u32(channels);
	
		if (out_Successful)
			*out_Successful = true;

		if (printLogWhenLoaded)
		{
			LOG("Loaded: {}, W: {}, H: {}, Channels: {}", path, m_Size[0], m_Size[1], m_NumChannels);
		}
	}

	Image::Image(u8* bytes, uvec2 size, u32 channels, bool isYFlipped)
		: m_AllocatedWithSTBI(false)
		, m_Bytes(bytes)
		, m_Size(size)
		, m_NumChannels(channels)
		, m_YFlippedWhenLoaded(isYFlipped)
	{ }

	Image::Image(const Image& src)
		: m_AllocatedWithSTBI(false)
		, m_Size(src.m_Size)
		, m_NumChannels(src.m_NumChannels)
		, m_YFlippedWhenLoaded(src.m_YFlippedWhenLoaded)
	{
		const u32 NumBytes = src.GetNumBytes();
		m_Bytes = new u8[NumBytes];
		std::memcpy(m_Bytes, src.m_Bytes, NumBytes);
	}

	Image& Image::operator=(const Image& rhs)
	{
		if (this == &rhs)
			return *this;

		const u32 NumBytes = rhs.GetNumBytes();

		if (m_AllocatedWithSTBI)
		{
			stbi_image_free(m_Bytes);
			m_AllocatedWithSTBI = false;
			m_Bytes = new u8[NumBytes];
		}
		else if (GetNumBytes() != NumBytes)
		{
			delete[] m_Bytes;
			m_Bytes = new u8[NumBytes];
		}
		std::memcpy(m_Bytes, rhs.m_Bytes, NumBytes);

		//Even if the "GetNumBytes() == NumBytes" was true previously, the size & channels might have been different
		m_Size = rhs.m_Size;
		m_NumChannels = rhs.m_NumChannels;
		m_YFlippedWhenLoaded = rhs.m_YFlippedWhenLoaded;

		return *this;
	}
	
	Image::Image(Image&& src)
		: m_AllocatedWithSTBI(src.m_AllocatedWithSTBI)
		, m_Size(src.m_Size)
		, m_NumChannels(src.m_NumChannels)
		, m_YFlippedWhenLoaded(src.m_YFlippedWhenLoaded)
		, m_Bytes(src.m_Bytes)
	{ 
		src.m_Bytes = nullptr;
	}

	Image& Image::operator=(Image&& rhs)
	{
		if (this == &rhs)
			return *this;

		m_AllocatedWithSTBI = rhs.m_AllocatedWithSTBI;
		m_Size = rhs.m_Size;
		m_NumChannels = rhs.m_NumChannels;
		m_YFlippedWhenLoaded = rhs.m_YFlippedWhenLoaded;
		m_Bytes = rhs.m_Bytes;

		rhs.m_Bytes = nullptr;

		return *this;
	}

	Image::~Image()
	{
		if (m_Bytes == nullptr)
		{
			return;
		}
		else if (m_AllocatedWithSTBI)
		{
			stbi_image_free(m_Bytes);
		}
		else
		{
			delete[] m_Bytes;
		}
	}
	
	/*static*/ void Image::SetYFlipBeforeLoad(bool bottomToTop)
	{
		//Stb reads top to bottom, openGL is the opposite, so change stb mode.
		stbi_set_flip_vertically_on_load(bottomToTop);
		s_YFlip = bottomToTop;
	}

	Image Image::ToGreyscale(GreyScaleFunc func) const
	{
		if (m_NumChannels == 1)
		{
			LOG_WARNING("Greyscale to Greyscale, useless call!")
			return *this; //Copy
		}
		if (m_NumChannels == 3 || m_NumChannels == 4)
		{
			auto Do = [this, &func]<typename T>(T)
			{
				u8* newBytes = new u8[GetNumPixels()];
				u32 iter = 0;
				for (T color : GetIterDontUnFlip<T>())
				{
					const u8 mix = (u8)(func((f32)color.r, (f32)color.g, (f32)color.b));
					newBytes[iter++] = mix;
				}
				return Image(newBytes, m_Size, 1, m_YFlippedWhenLoaded);
			};
			return (m_NumChannels == 3) ? Do(u8color3{}) : Do(u8color4{});
		}
		else
		{
			LOG_ERROR("Cannot Convert Image With {} Channels to Greyscale.", m_NumChannels);
			return s_Invalid;
		}
	}
	Image Image::GreyscaleToRGB() const
	{
		if (m_NumChannels != 1)
		{
			LOG_ERROR("Current image is not greyscale, it has {} channels.", m_NumChannels);
			return s_Invalid;
		}

		u8* newBytes = new u8[GetNumPixels() * 3];
		u32 iter = 0;
		for (u8 grey : GetIterPerChannelDontUnFlip())
		{
			newBytes[iter++] = grey; //R
			newBytes[iter++] = grey; //G
			newBytes[iter++] = grey; //B
		}
		return Image(newBytes, m_Size, 3, m_YFlippedWhenLoaded);
	}
	Image Image::GreyscaleToRGBA() const
	{
		if (m_NumChannels != 1)
		{
			LOG_ERROR("Current image is not greyscale, it has {} channels.", m_NumChannels);
			return s_Invalid;
		}

		u8* newBytes = new u8[GetNumPixels() * 4];
		u32 iter = 0;
		for (u8 grey : GetIterPerChannelDontUnFlip())
		{
			LOG("{:x}", grey);

			newBytes[iter++] = grey; //R
			newBytes[iter++] = grey; //G
			newBytes[iter++] = grey; //B
			newBytes[iter++] = 255;  //A
		}
		return Image(newBytes, m_Size, 4, m_YFlippedWhenLoaded);
	}

	void Image::DebugPrint() const
	{
		if (!m_Bytes)
			return;

		std::ostringstream out;

		u32 colorCounter = 0, widthCounter = 0;

		for (const u8 c : GetIterPerChannel())
		{
			out << Math::ToHexStr(c);

			if (++colorCounter == m_NumChannels)
			{
				colorCounter = 0;
				out << ' ';

				if (++widthCounter == m_Size.x)
				{
					widthCounter = 0;
#ifdef _WINDOWS
					out << "\r\n";
#else
					out << '\n';
#endif
				}
			}
		}

		std::string s(out.str());
		LOG("IMAGE: \r\n{}", s);
	}
}