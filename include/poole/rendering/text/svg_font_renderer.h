#pragma once

#include "poole/core.h"

#include "poole/rendering/image/image.h"

namespace Poole::Rendering
{
	class SvgFontRenderer
	{
	public:
		SvgFontRenderer(const char* fontLocation);
		Image* GetSprintImage(f32 fontSize) { return m_Sizes[fontSize].m_Image; }
		void CacheSize(f32 fontSize) const;

	private:

		static constexpr i32 k_FontBuffer = 1 << 20;
		static constexpr i32 k_StartChar = 32;
		static constexpr i32 k_NumChars = 96;
		static constexpr i32 k_LastChars = k_StartChar + k_NumChars;
		static constexpr ivec2 k_Size = { 512,512 };

		//Clone of stbtt_bakedchar
		struct TT_BakedChar
		{
			unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
			float xoff, yoff, xadvance;
		};

		struct RasterInfo
		{
			TT_BakedChar* m_CharMaps = nullptr;
			Image* m_Image = nullptr;
		};

		mutable std::unordered_map<float, RasterInfo> m_Sizes;
		u8* m_TtfBuffer = nullptr;
	};
}