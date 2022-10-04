#include "poole/rendering/text/text_renderer.h"

#include "poole/rendering/text/font_renderer.h"
#include "poole/rendering/text/svg_font_renderer.h"
#include "poole/rendering/renderer2D.h"

#include "glm/gtx/string_cast.hpp"

namespace Poole::Rendering
{
	void TextRenderer::SetTextView(std::string_view textView)
	{
		m_TextView = textView;
		m_Text.clear();
		m_cachedRenderArea = {};
	}
	void TextRenderer::SetText(const std::string& text)
	{
		m_TextView = {};
		m_Text = text;
		m_cachedRenderArea = {};
	}
	void TextRenderer::SetText(std::string&& text)
	{
		m_TextView = {};
		m_Text = std::move(text);
		m_cachedRenderArea = {};
	}

	void TextRenderer::SetColor(const fcolor4& tintColor)
	{
		m_TintColor = tintColor;
	}
	
	void TextRenderer::SetPosition(fvec3 pos)
	{
		m_Transform.position = pos;
	}

	void TextRenderer::SetFontSize(f32 fontSize)
	{
		if (IsMonospaced())
		{
			LOG_ERROR("Monospaced Fonts can't have a font size, rely on Scale only!");
		}
		else
		{
			m_FontSize = fontSize;
		}
		m_cachedRenderArea = {};
	}

	void TextRenderer::SetScale(f32 scale)
	{
		m_Transform.scale = { scale, scale };
	}
	void TextRenderer::SetScale(fvec2 scale)
	{
		m_Transform.scale = scale;
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

	void TextRenderer::RenderText_Monospaced(ftransform2D& trans, const fcolor4& col)
	{
		fvec3 newLinePos = trans.position;
		fmat4 rotMat = trans.MakeRotationMatrix();

		auto Do = [this, &trans, col, &newLinePos, &rotMat](bool cacheArea)
		{
			if (cacheArea)
			{
				m_cachedRenderArea = { 0,0 };
			}
			float currentLineLength = 0;
			for (const char c : GetTextOrView())
			{
				if (c == '\r')
					continue;
				if (c == '\n')
				{
					newLinePos += fvec3(rotMat * fvec4(0, -trans.scale.y, 0, 1));
					trans.position = newLinePos;

					if (cacheArea)
					{
						if (currentLineLength > m_cachedRenderArea->x)
						{
							m_cachedRenderArea->x = currentLineLength;
						}
						m_cachedRenderArea->y += trans.scale.y;
						currentLineLength = 0;
					}
					continue;
				}

				if (std::shared_ptr<SubImage> Sub = m_MonospacedFont->Convert(c))
				{
					if (cacheArea)
					{
						currentLineLength += trans.scale.x;
					}
					else
					{
						Renderer2D::DrawSubTexturedQuad(trans, *Sub, /*tiling*/ 1, col);
					}
					//Offset for the next char
					trans.position += fvec3(rotMat * fvec4(trans.scale.x, 0, 0, 1));
				}
			}
			if (cacheArea && currentLineLength > m_cachedRenderArea->x)
			{
				m_cachedRenderArea->x = currentLineLength;
			}
		};

		if (!m_cachedRenderArea.has_value())
		{
			ftransform2D prevTrans = trans;
			Do(true);
			trans = prevTrans;
		}

		float h = 0, v = 0;
		switch (m_HorizontalPivot)
		{
		case EHorizontal::Left: break;
		case EHorizontal::Center: h = m_cachedRenderArea->x / 2.f; break;
		case EHorizontal::Right:  h = m_cachedRenderArea->x; break;
		default: return;
		}
		switch (m_VerticalPivot)
		{
		case EVertical::Top: break;
		case EVertical::Middle: v = m_cachedRenderArea->y / 2.f; break;
		case EVertical::Bottom: v = m_cachedRenderArea->y; break;
		default: return;
		}

		trans.position -= fvec3(rotMat * fvec4(h, -v, 0, 1));
		newLinePos = trans.position;

		Do(false);
	}
	void TextRenderer::RenderText_VariableWidth(ftransform2D& trans, const fcolor4& col)
	{
		fvec2 xy{};

		trans.scale /= m_FontSize;

		std::array<fvec4, 4> coords;
		std::array<fvec2, 4> uv;

		for (const char c : GetTextOrView())
		{
			if (c == '\r')
				continue;
			if (c == '\n')
			{
				xy.x = 0;
				xy.y -= m_FontSize;
				continue;
			}

			m_VariableWidthFont->Convert(c, m_FontSize, xy, coords, uv);

			Renderer2D::DrawSubTexturedQuad(coords, trans, m_VariableWidthFont->GetImageForSize(m_FontSize), uv, /*tiling*/ 1, col);
		}

		trans.scale *= m_FontSize;
	}

	bool TextRenderer::IsMonospaced() const
	{
		return (bool)m_MonospacedFont.get();
	}
}
