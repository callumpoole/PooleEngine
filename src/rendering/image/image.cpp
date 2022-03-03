#include "image.h"

#include "stb/stb_image.h"

namespace Poole
{
	bool Image::s_YFlip = false;
	
	Image::Image(const char* path, bool* out_Successful, bool printLogWhenLoaded)
	{
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
}