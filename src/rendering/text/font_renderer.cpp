#include "poole/rendering/text/font_renderer.h"

#include "poole/rendering/image/image.h"
#include "poole/rendering/image/image_utils.h"
#include "poole/rendering/image/sub_image.h"

namespace Poole::Rendering
{
	void FontRenderer::ConvertBlackToAlpha()
	{
		if (m_ImageAtlas)
		{
			ImageUtils::ReplaceBlackWithAlphaInline(m_ImageAtlas.get());
		}
	}

	void FontRenderer::GenerateSubImages(u32 reserve)
	{
		ASSERT(m_GridSize.x * m_GridSize.y > 0);
		IF_ASSERT(HasCustomGlyphMappings(), m_GlyphMappings.size() == m_GridSize.x * m_GridSize.y);

		m_SubImages.reserve(reserve);

		const fvec2 cellSize = m_ImageAtlas->GetSize() / m_GridSize;

		for (u32 y = 0; y < m_GridSize.y; y++)
		{
			for (u32 x = 0; x < m_GridSize.x; x++)
			{
				//No need to emplace_back since make_shared is already emplacing
				m_SubImages.push_back(std::make_shared<SubImage>(m_ImageAtlas, fvec2{ (f32)x, (f32)y }, cellSize));
			}
		}
	}

	std::shared_ptr<SubImage> FontRenderer::Convert(const char c)
	{
		ASSERT(!m_SubImages.empty());

		if (size_t(c) >= m_SubImages.size())
		{
			LOG_WARNING("Character {} ({}) out of bounds (0 to {}).", c, u32(c), m_SubImages.size() - 1);
			return nullptr;
		}

		if (HasCustomGlyphMappings())
		{
			LOG_NOT_IMPL();
			return nullptr;
		}
		else
		{
			return m_SubImages[u32(c)];
		}
	}
}