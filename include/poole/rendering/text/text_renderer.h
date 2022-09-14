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
		fvec3 GetPosition() const { return m_Transform.position; }

		void SetSize(float size);
		void SetSize(fvec2 size);
		fvec2 GetSize() const { return m_Transform.scale; }

		void SetRotationRadians(float radians);
		float GetRotationRadians() const { return m_Transform.rotation; }

		void SetShadowOffset(fvec2 pos);
		fvec2 GetShadowOffset() const { return m_ShadowOffset; }

		void SetShadowColor(fcolor4 col);
		fcolor4 GetShadowColor() const { return m_ShadowTintColor; }

		void RenderText();
	private:
		void RenderText_Monospaced(ftransform2D& trans, fcolor4 col);
		void RenderText_VariableWidth(ftransform2D& trans, fcolor4 col);

		bool IsMonospaced() const;

		TextRenderer() = default;
		std::shared_ptr<FontRenderer> m_MonospacedFont;
		std::shared_ptr<SvgFontRenderer> m_VariableWidthFont;

		ftransform2D m_Transform;
		fvec2 m_ShadowOffset = { 0.05f, -0.05f };
		EHorizontalAlignment m_HorizontalAlign = EHorizontalAlignment::Left;
		std::string_view m_TextView;
		std::string m_Text;
		fcolor4 m_TintColor = Colors::White<fcolor4>;
		fcolor4 m_ShadowTintColor = Colors::Black<fcolor4>;
	};
}