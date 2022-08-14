#pragma once

#include "poole/core.h"

#include <string_view>

namespace Poole::Rendering
{
	class RenderFont;

	class RenderText
	{
		friend class RenderTextFactory;
	public:
		enum class EHorizontalAlignment {
			Left, Center, Right
		};

		void SetSize(float size);
		void SetText(std::string_view text);

		ftransform2D m_Transform;

	private:
		RenderText() = default;
		std::shared_ptr<RenderFont> m_Font;
		EHorizontalAlignment m_HorizontalAlign = EHorizontalAlignment::Left;
	};
}