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
		//Cache Size (If Needed)
		if (!m_cachedRenderArea)
		{
			m_cachedRenderArea = { 0.f, 0.f };
			f32 currentLineLength = 0.f;
			for (const char c : GetTextOrView())
			{
				if (c == '\n')
				{
					m_cachedRenderArea->x = std::max(m_cachedRenderArea->x, currentLineLength);
					m_cachedRenderArea->y += trans.scale.y;
					currentLineLength = 0;
				}
				else if (c != '\r')
				{
					currentLineLength += trans.scale.x;
				}
			}
			m_cachedRenderArea->x = std::max(m_cachedRenderArea->x, currentLineLength);
		}

		const fmat4 rotMat = trans.MakeRotationMatrix();

		//Apply Pivot
		const fvec2 pivotalOffset = GetRealPivotValues();

		//Debug Draw Pivot
		DebugShowPivot(trans.position);

		//Fine to compare floats == in this case, alternatives would be more expensive and defeat the point of the if
		if (pivotalOffset.x != 0.f && pivotalOffset.y != 0.f) //Can skip calculation if both are 0.
		{
			trans.position -= fvec3(rotMat * fvec4(pivotalOffset.x, -pivotalOffset.y, 0, 1));
		}

		//Draw
		fvec3 newLinePos = trans.position;
		for (const char c : GetTextOrView())
		{
			if (c == '\r')
				continue;
			if (c == '\n')
			{
				newLinePos += fvec3(rotMat * fvec4(0, -trans.scale.y, 0, 1));
				trans.position = newLinePos;
			}
			else if (std::shared_ptr<SubImage> Sub = m_MonospacedFont->Convert(c))
			{
				Renderer2D::DrawSubTexturedQuad(trans, *Sub, /*tiling*/ 1, col);

				//Offset for the next char
				trans.position += fvec3(rotMat * fvec4(trans.scale.x, 0, 0, 1));
			}
		}
	}
	void TextRenderer::RenderText_VariableWidth(ftransform2D& trans, const fcolor4& col)
	{
		//Push Scale Div
		trans.scale /= m_FontSize;

		fvec2 xy{};
		std::array<fvec4, 4> coords;
		std::array<fvec2, 4> uv;

		//Cache Size (If Needed)
		if (!m_cachedRenderArea)
		{
			f32 xOffset = 0;
			f32 yOffset = 0;
			m_cachedRenderArea = { 0.f, 0.f };
			for (const char c : GetTextOrView())
			{
				if (c == '\n')
				{
					xOffset = xy.x;
					yOffset += m_FontSize;
				}
				else
				{
					//TODO: Check with lots of trailing \n 
					//coords[1] has the highest in both x & y
					m_cachedRenderArea->x = std::max(m_cachedRenderArea->x, coords[1].x - xOffset);
					m_cachedRenderArea->y = std::max(m_cachedRenderArea->y, coords[1].y + yOffset);

					m_VariableWidthFont->Convert(c, m_FontSize, xy, coords, uv);
				}
			}
			m_cachedRenderArea->y -= m_FontSize;
			*m_cachedRenderArea = *m_cachedRenderArea * trans.scale;
		}

		//Apply Pivot
		{
			const fmat4 rotMat = trans.MakeRotationMatrix();
			const fvec2 pivotalOffset = GetRealPivotValues();

			//Debug Draw Pivot
			DebugShowPivot(trans.position);

			//Fine to compare floats == in this case, alternatives would be more expensive and defeat the point of the if
			if (pivotalOffset.x != 0.f && pivotalOffset.y != 0.f) //Can skip calculation if both are 0.
			{
				trans.position -= fvec3(rotMat * fvec4(pivotalOffset.x, -pivotalOffset.y, 0, 1));
			}
		}

		//Reset XY
		xy = fvec2{};

		//Draw
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

		//Pop Scale Div
		trans.scale *= m_FontSize;
	}

	fvec2 TextRenderer::GetRealPivotValues() const
	{
		fvec2 out; //No need to initalize, will be done below for sure.

		switch (m_HorizontalPivot)
		{
		case EHorizontal::Left:	  out.x = 0.f; break;
		case EHorizontal::Center: out.x = m_cachedRenderArea->x / 2.f; break;
		case EHorizontal::Right:  out.x = m_cachedRenderArea->x; break;
		default: return fvec2{};
		}
		switch (m_VerticalPivot)
		{
		case EVertical::Top:	out.y = 0.f;  break;
		case EVertical::Middle: out.y = m_cachedRenderArea->y / 2.f; break;
		case EVertical::Bottom: out.y = m_cachedRenderArea->y; break;
		default: return fvec2{};
		}

		return out;
	}

	bool TextRenderer::IsMonospaced() const
	{
		return (bool)m_MonospacedFont.get();
	}

	void TextRenderer::DebugShowPivot(fvec3 pos) const
	{
		if (s_DebugPivotCrossSize > SMALL_NUMBER)
		{
			constexpr f32 s = s_DebugPivotCrossSize;
			Renderer2D::DrawLine(pos - fvec3(s,  s, 0), pos + fvec3(s,  s, 0), s_DebugPivotColor);
			Renderer2D::DrawLine(pos - fvec3(s, -s, 0), pos + fvec3(s, -s, 0), s_DebugPivotColor);
		}
	}
}
