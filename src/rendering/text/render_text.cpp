#include "poole/rendering/text/render_text.h"

#include "poole/rendering/image/image.h"
#include "poole/rendering/image/sub_image.h"

namespace Poole::Rendering
{
	//RenderFont::RenderFont(std::shared_ptr<Image> Atlas, uvec2 GridSize, std::string_view GlyphMappings)
	//	: m_GridSize(GridSize)
	//	, m_GlyphMappings(GlyphMappings)
	//{
	//	
	//
	//}

	void RenderFont::GenerateSubImages(u32 reserve)
	{
		ASSERT(m_GridSize.x * m_GridSize.y > 0);
		IF_ASSERT(HasCustomGlyphMappings(), m_GlyphMappings.size() == m_GridSize.x * m_GridSize.y);

		m_SubImages.reserve(reserve);

		const fvec2 cellSize = m_ImageAtlas->GetSize() / m_GridSize;

		for (u32 y = 0; y < m_GridSize.y; y++)
		{
			for (u32 x = 0; x < m_GridSize.x; x++)
			{
				std::shared_ptr<SubImage> subImage;
				subImage.reset(SubImage::Create(m_ImageAtlas, { (f32)x, (f32)y }, cellSize));
				//TODO: change to emplace
				m_SubImages.push_back(subImage);
			}
		}
	}

	std::shared_ptr<SubImage> RenderFont::Convert(const char c)
	{
		ASSERT(!m_SubImages.empty());
		ASSERT_MSG(size_t(c) < m_SubImages.size(), "Character {} ({}) out of bounds (0 to {}).", c, u32(c), m_SubImages.size() - 1);

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
