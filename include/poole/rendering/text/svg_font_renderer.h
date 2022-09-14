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

		std::shared_ptr<SubImage> Convert(char c, f32 fontSize, f32& xoff, f32& yoff, f32& xadvance);
		void Convert2(char c, f32 fontSize, fvec2& pos, std::array<fvec4, 4>& coords, std::array<fvec2, 4>& uv);
		std::shared_ptr<Image> GetImageForSize(f32 fontSize);

	private:

		static constexpr ivec2 k_Size = { 1024,1024 };
		static constexpr i32 k_FontBuffer = k_Size.x * k_Size.y;
		static constexpr i32 k_StartChar = 32;
		static constexpr i32 k_NumChars = 96;
		static constexpr i32 k_LastChars = k_StartChar + k_NumChars;

		//Clone of stbtt_bakedchar
		struct TT_BakedChar
		{
			u16 x0, y0, x1, y1; // coordinates of bbox in bitmap
			f32 xoff, yoff, xadvance;
		};

		struct RasterInfo
		{
			std::vector<TT_BakedChar> m_CharMaps;
			std::shared_ptr<Image> m_Image = nullptr;
		};

		mutable std::unordered_map<f32, RasterInfo> m_Sizes;
		std::vector<u8> m_TtfBuffer;
	};
}