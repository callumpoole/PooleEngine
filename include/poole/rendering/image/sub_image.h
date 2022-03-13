#pragma once

#include "poole/core.h"

#include "poole/rendering/image/image.h"

namespace Poole::Rendering
{

	class SubImage
	{
	public:
		SubImage(std::shared_ptr<Image> image, fvec2 min, fvec2 max);

		static SubImage* Create(std::shared_ptr<Image> texture, fvec2 coords, fvec2 cellSize, fvec2 spriteSize = { 1,1 });

		std::shared_ptr<Image> GetImage() const { return m_Image; }
		const std::array<fvec2, 4>& GetTexCoords() const { return m_TexCoords; }
	private:
		std::shared_ptr<Image> m_Image;
		std::array<fvec2, 4> m_TexCoords;
	};
}