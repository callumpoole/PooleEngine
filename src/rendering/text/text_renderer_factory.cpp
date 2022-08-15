#include "poole/rendering/text/text_renderer_factory.h"

#include "poole/rendering/image/image.h"
#include "poole/rendering/text/font_renderer.h"
#include "poole/rendering/text/text_renderer.h"

namespace Poole::Rendering
{
	/*static*/ std::vector<std::weak_ptr<TextRenderer>> TextRendererFactory::s_TextRenderers;
	/*static*/ std::shared_ptr<FontRenderer> TextRendererFactory::s_DefaultFont = nullptr;

	/*static*/ void TextRendererFactory::Init()
	{
#define IMAGE_PATH "../../poole_engine/content/font_atlases/"
		s_DefaultFont.reset(new FontRenderer());
		s_DefaultFont->m_ImageAtlas.reset(new Poole::Rendering::Image(IMAGE_PATH "8x8ASCIIBitmapFont.png"));
		s_DefaultFont->m_GridSize = { 16, 8 };
		s_DefaultFont->GenerateSubImages();
#undef IMAGE_PATH
	}

	/*static*/ std::shared_ptr<TextRenderer> TextRendererFactory::MakeRenderText()
	{
		std::shared_ptr<TextRenderer> rt{ new TextRenderer() };
		rt->m_Font = s_DefaultFont;
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
}