#pragma once

#include "poole/core.h"

#include "poole/rendering/image/image.h"
#include "poole/rendering/image/sub_image.h"

namespace Poole::Rendering
{
	class SvgFontRenderer
	{
	public:
		SvgFontRenderer(const char* fontLocation);
		void CacheSize(f32 fontSize) const;

		std::shared_ptr<SubImage> Convert(const char c, float fontSize, float& xoff, float& yoff, float& xadvance);
		std::shared_ptr<Image> GetImageForSize(float fontSize);
	private:

		static constexpr ivec2 k_Size = { 1024,1024 };
		static constexpr i32 k_FontBuffer = k_Size.x * k_Size.y;
		static constexpr i32 k_StartChar = 32;
		static constexpr i32 k_NumChars = 96;
		static constexpr i32 k_LastChars = k_StartChar + k_NumChars;

		//Clone of stbtt_bakedchar
		struct TT_BakedChar
		{
			unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
			float xoff, yoff, xadvance;
		};

		struct RasterInfo
		{
			TT_BakedChar* m_CharMaps = nullptr;
			std::shared_ptr<Image> m_Image = nullptr;
		};

		mutable std::unordered_map<float, RasterInfo> m_Sizes;
		u8* m_TtfBuffer = nullptr;
	};
}