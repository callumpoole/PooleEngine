#include "poole/rendering/text/text_renderer.h"

#include "poole/rendering/text/font_renderer.h"
#include "poole/rendering/text/svg_font_renderer.h"
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
		ftransform2D trans = m_Transform;

		LOG("===============");

		if (m_ShadowOffset != fvec2{0})
		{
			trans.position.x += m_ShadowOffset.x * (trans.scale.x * cos(trans.rotation) + trans.scale.y * sin(trans.rotation));
			trans.position.y += m_ShadowOffset.y * (trans.scale.x * sin(-trans.rotation) + trans.scale.y * cos(trans.rotation));

			IsMonospaced() ? RenderText_Monospaced(trans, m_ShadowTintColor) : RenderText_VariableWidth(trans, m_ShadowTintColor);

			trans = m_Transform;
		}

		//LOG("===============");

		IsMonospaced() ? RenderText_Monospaced(trans, m_TintColor) : RenderText_VariableWidth(trans, m_TintColor);
	}

	void TextRenderer::RenderText_Monospaced(ftransform2D& trans, fcolor4 col)
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

			std::shared_ptr<SubImage> Sub = m_MonospacedFont->Convert(c);

			Renderer2D::DrawSubTexturedQuad(trans, *Sub, /*tiling*/ 1, col);

			//Offset for the next char
			trans.position.x += trans.scale.x * cos(trans.rotation);
			trans.position.y += trans.scale.x * sin(trans.rotation);
		}
	}
	void TextRenderer::RenderText_VariableWidth(ftransform2D& trans, fcolor4 col)
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

			float xoff;
			float yoff;
			float xadvance;

			constexpr float fontSize = 70.f;

			std::shared_ptr<SubImage> Sub = m_VariableWidthFont->Convert(c, fontSize, xoff, yoff, xadvance);

			const auto prevPos = trans.position;
			const auto prevScale = trans.scale;

			trans.scale.x *= (Sub->GetSize().x * 1024) / fontSize;
			trans.scale.y *= (Sub->GetSize().y * 1024) / fontSize;

			const float downBy = yoff / 1024;
			const float rightBy = (xoff + xadvance) / 1024;
			trans.position.x += downBy * sin(-trans.rotation) + rightBy * cos(trans.rotation);
			trans.position.y += downBy * cos(trans.rotation)  + rightBy * sin(trans.rotation);

			LOG("Letter {} : {} {} {}   y={}", c, xoff, yoff, xadvance, Sub->GetSize().y);

			Renderer2D::DrawSubTexturedQuad(trans, *Sub, /*tiling*/ 1, col);

			if (c != ' ')
			{
				trans.position = prevPos;
			}

			//Offset for the next char
			trans.position.x += trans.scale.x * cos(trans.rotation);// + rightBy * cos(trans.rotation);
			trans.position.y += trans.scale.x * sin(trans.rotation);// + rightBy * sin(trans.rotation);

			//Reset the fixed width
			trans.scale = prevScale;
		}
	}

	bool TextRenderer::IsMonospaced() const
	{
		return (bool)m_MonospacedFont.get();
	}
}
