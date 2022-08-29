#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class Image;
	class TextRenderer;
	class FontRenderer;
	class SvgFontRenderer;
	
	class TextRendererFactory
	{
	public:
		static void Init();

		static std::shared_ptr<TextRenderer> MakeRenderText(bool Monospaced);

		static std::shared_ptr<Image> GetVariableFontImage(float fontSize);

		static void RenderAllTextRenderers();
	private:
		static std::shared_ptr<FontRenderer> s_DefaultMonospacedFont;
		static std::shared_ptr<SvgFontRenderer> s_DefaultVariableWidthFont;
		static std::vector<std::weak_ptr<TextRenderer>> s_TextRenderers;
	};
}
