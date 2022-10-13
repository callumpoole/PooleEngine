#include "poole/rendering/text/text_renderer_factory.h"

#include "poole/rendering/image/image.h"
#include "poole/rendering/text/svg_font_renderer.h"
#include "poole/rendering/text/font_renderer.h"
#include "poole/rendering/text/text_renderer.h"

namespace Poole::Rendering
{
	/*static*/ std::vector<std::weak_ptr<TextRenderer>> TextRendererFactory::s_TextRenderers;
	/*static*/ std::shared_ptr<FontRenderer> TextRendererFactory::s_DefaultMonospacedFont = nullptr;
	/*static*/ std::shared_ptr<SvgFontRenderer> TextRendererFactory::s_DefaultVariableWidthFont = nullptr;

	/*static*/ void TextRendererFactory::Init()
	{
#define FONT_PATH "../../poole_engine/content/font_atlases/"
		s_DefaultMonospacedFont.reset(new FontRenderer());
		s_DefaultMonospacedFont->m_ImageAtlas.reset(new Poole::Rendering::Image(FONT_PATH "8x8ASCIIBitmapFont.png"));
		s_DefaultMonospacedFont->m_GridSize = { 16, 8 };
		s_DefaultMonospacedFont->ConvertBlackToAlpha();
		s_DefaultMonospacedFont->GenerateSubImages();
#undef FONT_PATH

		//TODO: Fix Windows Path for Mac & Linux
#define FONT_PATH "c:/windows/fonts/"
		s_DefaultVariableWidthFont.reset(new SvgFontRenderer(FONT_PATH "times.ttf"));
		//s_DefaultVariableWidthFont.reset(new SvgFontRenderer(FONT_PATH "calibri.ttf"));
#undef FONT_PATH
	}

	/*static*/ std::shared_ptr<TextRenderer> TextRendererFactory::MakeRenderText(bool Monospaced)
	{
		std::shared_ptr<TextRenderer> rt{ new TextRenderer() };
		if (Monospaced)
		{
			rt->m_MonospacedFont = s_DefaultMonospacedFont;
		}
		else
		{
			rt->m_VariableWidthFont = s_DefaultVariableWidthFont;
		}
		s_TextRenderers.emplace_back(rt);
		return rt;
	}

	/*static*/ void TextRendererFactory::RenderAllTextRenderers()
	{
		auto it = s_TextRenderers.begin();
		while (it != s_TextRenderers.end())
		{
			if (it->expired())
			{
				// `erase()` invalidates the iterator, use returned iterator
				it = s_TextRenderers.erase(it);
			}
			// Notice that the iterator is incremented only on the else part (why?)
			else 
			{
				it->lock()->RenderText();
				++it;
			}
		}
	}

	/*static*/ std::shared_ptr<Image> TextRendererFactory::GetVariableFontImage(float fontSize)
	{
		return s_DefaultVariableWidthFont->GetImageForSize(fontSize);
	}
}