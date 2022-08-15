#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class TextRenderer;
	class FontRenderer;
	
	class TextRendererFactory
	{
	public:
		static void Init();

		static std::shared_ptr<TextRenderer> MakeRenderText();

		static void RenderAllTextRenderers();
	private:
		static std::shared_ptr<FontRenderer> s_DefaultFont;
		static std::vector<std::weak_ptr<TextRenderer>> s_TextRenderers;
	};
}
