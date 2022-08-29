#pragma once

#include "poole/core.h"

#include "poole/rendering/image/image.h"

namespace Poole::Rendering
{

	class SubImage
	{
	public:
		//Kinda ugly having the array[2], but it's due to overload resolution conflict, and this one is less likely to be used
		SubImage(std::shared_ptr<Image> image, const fvec2 minMax[2]); 

		SubImage(std::shared_ptr<Image> image, fvec2 coords, fvec2 cellSize, fvec2 spriteSize = { 1,1 });

		std::shared_ptr<Image> GetImage() const { return m_Image; }
		const std::array<fvec2, 4>& GetTexCoords() const { return m_TexCoords; }
		fvec2 GetSize() const;
		float GetAspectRatio() const;
	private:
		void InitTexChoords(fvec2 min, fvec2 max);
		std::shared_ptr<Image> m_Image;
		std::array<fvec2, 4> m_TexCoords;
		std::array<fvec2, 2> m_MinMax;
	};
}