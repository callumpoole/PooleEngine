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
		const fmat4 rotMat = trans.MakeRotationMatrix();

		if (m_ShadowOffset != fvec2{0})
		{
			const fvec3 shadow3 = { m_ShadowOffset.x, m_ShadowOffset.y, 0 };
			trans.position += shadow3 * fvec3(rotMat * fvec4(trans.scale.x, trans.scale.y, 0, 1));

			IsMonospaced() ? RenderText_Monospaced(trans, m_ShadowTintColor) : RenderText_VariableWidth(trans, m_ShadowTintColor);

			trans = m_Transform;
		}

		IsMonospaced() ? RenderText_Monospaced(trans, m_TintColor) : RenderText_VariableWidth(trans, m_TintColor);
	}

	void TextRenderer::RenderText_Monospaced(ftransform2D& trans, fcolor4 col)
	{
		fvec3 newLinePos = trans.position;
		const fmat4 rotMat = trans.MakeRotationMatrix();
		for (const char c : (m_TextView.empty() ? m_Text : m_TextView))
		{
			if (c == '\r')
				continue;
			if (c == '\n')
			{
				newLinePos += fvec3(rotMat * fvec4(0, -trans.scale.y, 0, 1));
				trans.position = newLinePos;
				continue;
			}

			if (std::shared_ptr<SubImage> Sub = m_MonospacedFont->Convert(c))
			{
				Renderer2D::DrawSubTexturedQuad(trans, *Sub, /*tiling*/ 1, col);

				//Offset for the next char
				trans.position += fvec3(rotMat * fvec4(trans.scale.x, 0, 0, 1));
			}
		}
	}
	void TextRenderer::RenderText_VariableWidth(ftransform2D& trans, fcolor4 col)
	{
		fvec3 newLinePos = trans.position;
		const fmat4 rotMat = trans.MakeRotationMatrix();

		for (const char c : (m_TextView.empty() ? m_Text : m_TextView))
		{
			if (c == '\r')
				continue;
			if (c == '\n')
			{
				newLinePos += fvec3(rotMat * fvec4(0, -trans.scale.y, 0, 1));
				trans.position = newLinePos;
				continue;
			}

			float xoff;
			float yoff;
			float xadvance;

			constexpr float fontSize = 70.f;

			if (std::shared_ptr<SubImage> Sub = m_VariableWidthFont->Convert(c, fontSize, xoff, yoff, xadvance))
			{
				const auto prevPos = trans.position;
				const auto prevScale = trans.scale;

				trans.scale *= Sub->GetSize() * 1024.f / fontSize;

				const float downBy = yoff / 1024;
				const float rightBy = xoff / 1024;
				const float advanceBy = xadvance / 1024;

				trans.position += fvec3(rotMat * fvec4(rightBy + advanceBy, downBy, 0, 1));

				//LOG("Letter {} : {} {} {}", c, xoff, yoff, xadvance);

				Renderer2D::DrawSubTexturedQuad(trans, *Sub, /*tiling*/ 1, col);

				if (c != ' ')
				{
					trans.position = prevPos;
				}

				//Offset for the next char
				trans.position += fvec3(rotMat * fvec4(trans.scale.x, 0, 0, 1));

				//Reset the fixed width
				trans.scale = prevScale;
			}
		}
	}

	bool TextRenderer::IsMonospaced() const
	{
		return (bool)m_MonospacedFont.get();
	}
}
