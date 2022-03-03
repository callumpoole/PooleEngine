#include "image.h"

#include "stb/stb_image.h"
#include <sstream>

namespace Poole
{
	bool Image::s_YFlip = false;
	
	Image::Image(const char* path, bool* out_Successful, bool printLogWhenLoaded)
	{
		m_YFlippedWhenLoaded = s_YFlip;
		m_Bytes = stbi_load(path, &m_Size[0], &m_Size[1], &m_NumColorChannels, 0);
	
		if (!m_Bytes || m_Size == ivec2{-1,-1} || m_NumColorChannels == -1)
		{
			LOG_ERROR("Failed to load image: {}", path);
	
			if (out_Successful)
				*out_Successful = false;

			return;
		}
	
		if (out_Successful)
			*out_Successful = true;

		if (printLogWhenLoaded)
		{
			LOG("Loaded: {}, W: {}, H: {}, Channels: {}", path, m_Size[0], m_Size[1], m_NumColorChannels);
		}
	}
	
	Image::~Image()
	{
		stbi_image_free(m_Bytes);
	}
	
	/*static*/ void Image::SetYFlipBeforeLoad(bool bottomToTop)
	{
		//Stb reads top to bottom, openGL is the opposite, so change stb mode.
		stbi_set_flip_vertically_on_load(bottomToTop);
		s_YFlip = bottomToTop;
	}

	void Image::DebugPrint() const
	{
		if (!m_Bytes)
			return;

		std::ostringstream out;

		const size_t ROW_BYTES = m_Size.x * m_NumColorChannels;

		size_t i			= m_YFlippedWhenLoaded ? (ROW_BYTES * (m_Size.y - 1)) : 0;
		const size_t yDelta = m_YFlippedWhenLoaded ? -(ROW_BYTES * 2 - 1) : 1;

		for (size_t y = 0; y < m_Size.y; y++, i += yDelta)
		{
			for (size_t x = 0; x < m_Size.x; x++, i++)
			{
				for (size_t c = 0; c < m_NumColorChannels; c++, i++)
				{
					const auto chars = Math::ToHex(m_Bytes[i]);
					out << chars[0] << chars[1];
				}
				i--;
				out << ' ';
			}
			i--;
#ifdef _WINDOWS
			out << "\r\n";
#else
			out << '\n';
#endif
		}

		std::string s(out.str());
		LOG("IMAGE: \r\n{}", s);
	}
}