#include "poole/rendering/image/sub_image.h"



namespace Poole::Rendering
{
	SubImage::SubImage(std::shared_ptr<Image> image, fvec2 min, fvec2 max)
		: m_Image(image)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	/*static*/ SubImage* SubImage::Create(std::shared_ptr<Image> image, fvec2 coords, fvec2 cellSize, fvec2 spriteSize)
	{
		fvec2 min = { (coords.x * cellSize.x) / image->GetWidth(), (coords.y * cellSize.y) / image->GetHeight() };
		fvec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / image->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / image->GetHeight() };

		if (image->WasYFlippedWhenLoaded())
		{
			min.y = image->GetHeight() - min.y;
			max.y = image->GetHeight() - max.y;
			std::swap(min.y, max.y);
		}

		return new SubImage(image, min, max);
	}
}