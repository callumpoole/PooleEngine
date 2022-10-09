#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class FontRenderer;
	class SvgFontRenderer;

	class TextRenderer
	{
		friend class TextRendererFactory;
	public:
		void SetTextView(std::string_view textView);
		void SetText(const std::string& text);
		void SetText(std::string&& text);
		std::string_view GetTextOrView() const { return (m_TextView.empty() ? m_Text : m_TextView); }

		void SetColor(const fcolor4& tintColor);
		const fcolor4& GetColor() const { return m_TintColor; }

		void SetPosition(fvec3 pos);
		const fvec3& GetPosition() const { return m_Transform.position; }

		//Variable Width Fonts Only!
		void SetFontSize(f32 fontSize);
		f32 GetFontSize() const { return IsMonospaced() ? -1.f : m_FontSize; }

		void SetScale(f32 scale);
		void SetScale(fvec2 scale);
		const fvec2& GetScale() const { return m_Transform.scale; }

		void SetRotationRadians(float radians);
		f32 GetRotationRadians() const { return m_Transform.rotation; }

		void SetShadowOffset(fvec2 pos);
		const fvec2& GetShadowOffset() const { return m_ShadowOffset; }

		void SetShadowColor(fcolor4 col);
		const fcolor4& GetShadowColor() const { return m_ShadowTintColor; }

		void SetHorizontalPivot(EHorizontal h) { m_HorizontalPivot = h; }
		void SetVerticalPivot(EVertical v) { m_VerticalPivot = v; }
		void SetPivot(EHorizontal h, EVertical v) { m_HorizontalPivot = h; m_VerticalPivot = v; }
		EHorizontal GetHorizontalPivot() const { return m_HorizontalPivot; }
		EVertical GetVerticalPivot() const { return m_VerticalPivot; }
		std::tuple<EHorizontal, EVertical> GetPivot() const { return { m_HorizontalPivot, m_VerticalPivot }; }

		void RenderText();
	private:
		void RenderText_Monospaced(ftransform2D& trans, const fcolor4& col);
		void RenderText_VariableWidth(ftransform2D& trans, const fcolor4& col);

		void DebugShowPivot(fvec3 pos) const;
		fvec2 GetRealPivotValues() const;

		bool IsMonospaced() const;

		TextRenderer() = default;
		std::shared_ptr<FontRenderer> m_MonospacedFont;
		std::shared_ptr<SvgFontRenderer> m_VariableWidthFont;

		ftransform2D m_Transform;
		fvec2 m_ShadowOffset = { 0.02f, -0.02f };
		EHorizontal m_HorizontalPivot = EHorizontal::Left;
		EVertical m_VerticalPivot = EVertical::Top;
		std::string_view m_TextView;
		std::string m_Text;
		fcolor4 m_TintColor = Colors::White<fcolor4>;
		fcolor4 m_ShadowTintColor = Colors::Black<fcolor4>;
		f32 m_FontSize = 70.f;
		mutable std::optional<fvec2> m_cachedRenderArea;
		static constexpr f32 s_DebugPivotCrossSize = 0.1f;
		static constexpr fcolor4 s_DebugPivotColor = Colors::Yellow<fcolor4>;
	};
}