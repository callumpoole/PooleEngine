#include "poole/rendering/image/sub_image.h"



namespace Poole::Rendering
{
	SubImage::SubImage(std::shared_ptr<Image> image, fvec2 minMax[2])
		: m_Image(image)
	{
		InitTexChoords(minMax[0], minMax[1]);
	}

	SubImage::SubImage(std::shared_ptr<Image> image, fvec2 coords, fvec2 cellSize, fvec2 spriteSize)
		: m_Image(image)
	{
		fvec2 min = {  (coords.x * cellSize.x)				   / image->GetWidth(),  (coords.y * cellSize.y)				 / image->GetHeight() };
		fvec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / image->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / image->GetHeight() };

		if (image->WasYFlippedWhenLoaded())
		{
			min.y = image->GetHeight() - min.y;
			max.y = image->GetHeight() - max.y;
			std::swap(min.y, max.y);
		}

		InitTexChoords(min, max);
	}

	void SubImage::InitTexChoords(fvec2 min, fvec2 max)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}
}