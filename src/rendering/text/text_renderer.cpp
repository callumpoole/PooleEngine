#include "poole/rendering/text/text_renderer.h"

#include "poole/rendering/text/font_renderer.h"
#include "poole/rendering/renderer2D.h"

namespace Poole::Rendering
{
	void TextRenderer::SetTextView(std::string_view textView)
	{
		m_TextView = textView;
		m_Text.clear();
	}
	void TextRenderer::SetText(const std::string& text)
	{
		m_TextView = {};
		m_Text = text;
	}
	void TextRenderer::SetText(std::string&& text)
	{
		m_TextView = {};
		m_Text = std::move(text);
	}

	void TextRenderer::SetColor(const fcolor4& tintColor)
	{
		m_TintColor = tintColor;
	}
	
	void TextRenderer::SetPosition(fvec3 pos)
	{
		m_Transform.position = pos;
	}

	void TextRenderer::SetSize(float size)
	{
		m_Transform.scale = { size,size };
	}

	void TextRenderer::RenderText()
	{
		ftransform2D Trans = m_Transform;

		for (const char c : (m_TextView.empty() ? m_Text : m_TextView))
		{
			Renderer2D::DrawSubTexturedQuad(Trans, *m_Font->Convert(c), /*tiling*/ 1, m_TintColor);
			Trans.position.x += Trans.scale.x;
		}
	}
}
