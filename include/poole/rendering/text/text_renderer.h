#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class FontRenderer;

	class TextRenderer
	{
		friend class TextRendererFactory;
	public:
		enum class EHorizontalAlignment {
			Left, Center, Right
		};

		void SetSize(float size);
		void SetText(std::string_view text);

		void RenderText();

		ftransform2D m_Transform;

	private:
		TextRenderer() = default;
		std::shared_ptr<FontRenderer> m_Font;
		EHorizontalAlignment m_HorizontalAlign = EHorizontalAlignment::Left;
		std::string_view m_Text;
	};
}