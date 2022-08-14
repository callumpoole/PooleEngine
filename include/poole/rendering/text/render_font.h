#pragma once

#include "poole/core.h"

#include <string_view>

namespace Poole::Rendering
{
	class Image;
	class SubImage;

	class RenderFont
	{
		friend class RenderTextFactory;
	private:
		RenderFont() = default;
		void GenerateSubImages(u32 reserve = 128);

	public:
		std::shared_ptr<SubImage> Convert(const char c);

	private:
		bool HasCustomGlyphMappings() const { return m_GlyphMappings.size() > 0; }

		std::shared_ptr<Image> m_ImageAtlas = nullptr;
		uvec2 m_GridSize = { 0,0 };
		//Will fast track if pointing to nullptr
		std::string_view m_GlyphMappings;
		std::vector<std::shared_ptr<SubImage>> m_SubImages;
	};
}
