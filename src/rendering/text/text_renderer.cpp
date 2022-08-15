#include "poole/rendering/text/text_renderer.h"

#include "poole/rendering/text/font_renderer.h"
#include "poole/rendering/renderer2D.h"

namespace Poole::Rendering
{
	void TextRenderer::SetSize(float size)
	{
		m_Transform.scale = { size,size };
	}

	void TextRenderer::SetText(std::string_view text)
	{
		m_Text = text;
	}
	
	void TextRenderer::RenderText()
	{
		ftransform2D Trans = m_Transform;

		for (const char c : m_Text)
		{
			Renderer2D::DrawSubTexturedQuad(Trans, *m_Font->Convert(c));
			Trans.position.x += Trans.scale.x;
		}
	}
}
