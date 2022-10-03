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
		enum class EHorizontalAlignment : u8 {
			Left, Center, Right
		};

		void SetTextView(std::string_view textView);
		void SetText(const std::string& text);
		void SetText(std::string&& text);
		std::string_view GetTextOrView() const { return m_TextView; }

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

		void RenderText();
	private:
		void RenderText_Monospaced(ftransform2D& trans, const fcolor4& col);
		void RenderText_VariableWidth(ftransform2D& trans, const fcolor4& col);

		bool IsMonospaced() const;

		TextRenderer() = default;
		std::shared_ptr<FontRenderer> m_MonospacedFont;
		std::shared_ptr<SvgFontRenderer> m_VariableWidthFont;

		ftransform2D m_Transform;
		fvec2 m_ShadowOffset = { 0.02f, -0.02f };
		EHorizontalAlignment m_HorizontalAlign = EHorizontalAlignment::Left;
		std::string_view m_TextView;
		std::string m_Text;
		fcolor4 m_TintColor = Colors::White<fcolor4>;
		fcolor4 m_ShadowTintColor = Colors::Black<fcolor4>;
		f32 m_FontSize = 70.f;
	};
}