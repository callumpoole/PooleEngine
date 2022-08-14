#include "poole/rendering/text/render_text_factory.h"

#include "poole/rendering/image/image.h"
#include "poole/rendering/text/render_text.h"

namespace Poole::Rendering
{
	/*static*/ std::unique_ptr<RenderFont> RenderTextFactory::DefaultFont = nullptr;

	/*static*/ void RenderTextFactory::Init()
	{
#define IMAGE_PATH "../../poole_engine/content/font_atlases/"
		DefaultFont.reset(new RenderFont());
		DefaultFont->m_ImageAtlas.reset(new Poole::Rendering::Image(IMAGE_PATH "8x8ASCIIBitmapFont.png"));
		DefaultFont->m_GridSize = { 16, 8 };
		DefaultFont->GenerateSubImages();
#undef IMAGE_PATH
	}

}