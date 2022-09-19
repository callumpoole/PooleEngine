#include "poole/rendering/image/image.h"

#include "stb/stb_image.h"
#include <sstream>

namespace Poole::Rendering
{
	Image Image::s_White1x1rgba = Image(new u8[]{ 255u, 255u, 255u, 255u }, { 1, 1 }, /*rgba*/ 4, /*isYFlipped*/ false, /*ownsMemory*/ true);

	u32 Image::s_IdCounter = 1;
	bool Image::s_YFlip = false;
	bool Image::s_PrintWhenLoadedFromFile = true;

	Image::Image(const char* path, EImageFormat format, bool* out_Successful)
		: m_OwnsMemory(true)
		, m_YFlippedWhenLoaded(s_YFlip)
		, m_Format(format)
		, m_Id(path ? s_IdCounter++ : 0)
	{
		i32 width = -1, height = -1, channels = -1;
		m_Data = stbi_load(path, &width, &height, &channels, 0);
	
		if (!m_Data || width <= 0 || height <= 0 || channels <= 0)
		{
			LOG_ERROR("Failed to load image: {}", path);
	
			if (out_Successful)
				*out_Successful = false;

			return;
		}
		m_Size.x = u32(width);
		m_Size.y = u32(height);
		m_NumChannels = u32(channels);

		switch (format)
		{
		case EImageFormat::Bytes:
			m_TotalBytesAllocated = GetBytesForWholeImage();

			break; //Do nothing
		case EImageFormat::Floats:

			//TODO

			m_TotalBytesAllocated = GetBytesForWholeImage();

			break; 
		default:
			if (out_Successful)
				*out_Successful = false;
			return;
		}
	
		if (out_Successful)
			*out_Successful = true;

		if (s_PrintWhenLoadedFromFile)
		{
			LOG("Loaded Image: {}, W: {}, H: {}, Channels: {}", path, m_Size.x, m_Size.y, m_NumChannels);
		}
	}

	Image::Image(void* data, uvec2 size, u32 channels, bool isYFlipped, bool ownsMemory, EImageFormat format)
		: m_Data(data)
		, m_Size(size)
		, m_NumChannels(channels)
		, m_YFlippedWhenLoaded(isYFlipped)
		, m_OwnsMemory(ownsMemory)
		, m_Format(format)
		, m_Id(s_IdCounter++)
	{ }

	Image::Image(const Image& src)
		: m_OwnsMemory(true) //It is an owner since it's memcpy'd
		, m_Size(src.m_Size)
		, m_NumChannels(src.m_NumChannels)
		, m_YFlippedWhenLoaded(src.m_YFlippedWhenLoaded)
		, m_Id(s_IdCounter++)
	{
		const u32 NumBytes = src.GetBytesForWholeImage();
		m_Data = new u8[NumBytes];
		std::memcpy(m_Data, src.m_Data, NumBytes);
	}

	Image& Image::operator=(const Image& rhs)
	{
		if (this == &rhs)
			return *this;

		const u32 NumBytes = rhs.GetBytesForWholeImage();

		if (m_OwnsMemory && GetBytesForWholeImage() != NumBytes)
		{
			//If STBI free is needed, it's: stbi_image_free(m_Bytes);
			delete[] m_Data;
			m_Data = new u8[NumBytes];	
		}
		m_OwnsMemory = true;
		std::memcpy(m_Data, rhs.m_Data, NumBytes);

		//Even if the "GetNumBytes() == NumBytes" was true previously, the size & channels might have been different
		m_Size = rhs.m_Size;
		m_NumChannels = rhs.m_NumChannels;
		m_YFlippedWhenLoaded = rhs.m_YFlippedWhenLoaded;
		m_Id = s_IdCounter++;

		return *this;
	}
	
	Image::Image(Image&& src)
		: m_OwnsMemory(src.m_OwnsMemory)
		, m_Size(src.m_Size)
		, m_NumChannels(src.m_NumChannels)
		, m_YFlippedWhenLoaded(src.m_YFlippedWhenLoaded)
		, m_Data(src.m_Data)
		, m_Id(src.m_Id)
	{ 
		src.m_OwnsMemory = false;
		src.m_Size = { 0,0 };
		src.m_NumChannels = 0;
		//Don't need to reset m_YFlippedWhenLoaded
		src.m_Data = nullptr;
		src.m_Id = 0;
		src.~Image();
	}

	Image& Image::operator=(Image&& rhs)
	{
		if (this == &rhs)
			return *this;

		m_OwnsMemory = rhs.m_OwnsMemory;
		m_Size = rhs.m_Size;
		m_NumChannels = rhs.m_NumChannels;
		m_YFlippedWhenLoaded = rhs.m_YFlippedWhenLoaded;
		m_Data = rhs.m_Data;
		m_Id = rhs.m_Id;

		rhs.m_OwnsMemory = false;
		rhs.m_Size = { 0,0 };
		rhs.m_NumChannels = 0;
		//Don't need to reset m_YFlippedWhenLoaded
		rhs.m_Data = nullptr;
		rhs.m_Id = 0;
		rhs.~Image();

		return *this;
	}

	Image::~Image()
	{
		if (m_Data && m_OwnsMemory)
		{
			//If STBI free is needed, it's: stbi_image_free(m_Bytes);
			delete[] m_Data;
		}
	}
	
	/*static*/ void Image::SetYFlipBeforeLoad(bool bottomToTop)
	{
		//Stb reads top to bottom, openGL is the opposite, so change stb mode.
		stbi_set_flip_vertically_on_load(bottomToTop);
		s_YFlip = bottomToTop;
	}

	void Image::DebugPrint() const
	{
		if (!m_Data)
			return;

		std::ostringstream out;

		u32 colorCounter = 0, widthCounter = 0;

		switch (m_Format)
		{
		case Poole::Rendering::Image::EImageFormat::Bytes:
			for (const u8 c : GetIterPerChannel<u8>())
			{
				out << Math::ToHexStr(c);

				if (++colorCounter == m_NumChannels)
				{
					colorCounter = 0;
					out << ' ';

					if (++widthCounter == m_Size.x)
					{
						widthCounter = 0;
						out << NEWLINE;
					}
				}
			}
			break;
		case Poole::Rendering::Image::EImageFormat::Floats:
			for (const f32 c : GetIterPerChannel<f32>())
			{
				out << std::format("{:.3f}", c);

				if (++colorCounter == m_NumChannels)
				{
					colorCounter = 0;
					out << ' ';

					if (++widthCounter == m_Size.x)
					{
						widthCounter = 0;
						out << NEWLINE;
					}
				}
				else
				{
					out << ',';
				}
			}
			break;
		default:
			out << "INVALID";
			break;
		}


		std::string s(out.str());
		LOG("IMAGE: " NEWLINE "{}", s);
	}
}