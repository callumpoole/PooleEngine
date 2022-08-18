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
		m_Transform.scale = { size, size };
	}
	void TextRenderer::SetSize(fvec2 size)
	{
		m_Transform.scale = size;
	}

	void TextRenderer::SetRotationRadians(float radians)
	{
		m_Transform.rotation = radians;
	}

	void TextRenderer::SetShadowOffset(fvec2 pos)
	{
		m_ShadowOffset = pos;
	}

	void TextRenderer::SetShadowColor(fcolor4 col)
	{
		m_ShadowTintColor = col;
	}

	void TextRenderer::RenderText()
	{
		auto Render = [this](ftransform2D& trans, fcolor4 col)
		{
			fvec3 newLinePos = trans.position;
			for (const char c : (m_TextView.empty() ? m_Text : m_TextView))
			{
				if (c == '\r')
					continue;
				if (c == '\n')
				{
					newLinePos.x += -trans.scale.y * sin(-trans.rotation);
					newLinePos.y += -trans.scale.y * cos(trans.rotation);
					trans.position = newLinePos;
					continue;
				}

				Renderer2D::DrawSubTexturedQuad(trans, *m_Font->Convert(c), /*tiling*/ 1, col);
				trans.position.x += trans.scale.x * cos(trans.rotation);
				trans.position.y += trans.scale.x * sin(trans.rotation);
			}
		};

		ftransform2D trans = m_Transform;

		if (m_ShadowOffset != fvec2{0})
		{
			trans.position.x += m_ShadowOffset.x * (trans.scale.x * cos(trans.rotation) + trans.scale.y * sin(trans.rotation));
			trans.position.y += m_ShadowOffset.y * (trans.scale.x * sin(-trans.rotation) + trans.scale.y * cos(trans.rotation));

			Render(trans, m_ShadowTintColor);

			trans = m_Transform;
		}

		Render(trans, m_TintColor);
	}
}
