#include "poole/rendering/text/svg_font_renderer.h"

#include "stb/stb_truetype.h"

#include "poole/rendering/image/image_utils.h"

namespace Poole::Rendering
{
	SvgFontRenderer::SvgFontRenderer(const char* fontLocation)
	{
		m_TtfBuffer = new u8[k_FontBuffer];
		fread(m_TtfBuffer, 1, k_FontBuffer, fopen("c:/windows/fonts/times.ttf", "rb"));
	}

	void SvgFontRenderer::CacheSize(f32 fontSize) const
	{
		if (m_Sizes.contains(fontSize))
			return;

		u8* temp_bitmap = new u8[k_Size.x * k_Size.y];
		stbtt_bakedchar* cdata = new stbtt_bakedchar[k_NumChars]; // ASCII 32..126 is 95 glyphs
		stbtt_BakeFontBitmap(m_TtfBuffer, 0, fontSize, temp_bitmap, k_Size.x, k_Size.y, k_StartChar, k_NumChars, cdata); // no guarantee this fits!

		std::shared_ptr<Image> img (ImageUtils::GreyscaleToRGBA(ImageUtils::YFlip(new Image(temp_bitmap, k_Size.x, k_Size.y, 1, true, true))));

		for (i32 i = 0; i < k_NumChars; i++)
		{
			cdata[i].y0 = k_Size.y - cdata[i].y0;
			cdata[i].y1 = k_Size.y - cdata[i].y1;
			std::swap(cdata[i].y0, cdata[i].y1);
			cdata[i].yoff = k_Size.y - cdata[i].yoff;
		}

		m_Sizes[fontSize] = RasterInfo{(TT_BakedChar*)(void*)cdata, img };
	}

	std::shared_ptr<SubImage> SvgFontRenderer::Convert(const char c, float fontSize)
	{
		CacheSize(fontSize);
		RasterInfo& info = m_Sizes[fontSize];

		TT_BakedChar& map = info.m_CharMaps[c - k_StartChar];

		LOG("{},{} and {},{}", map.x0, map.y0, map.x1, map.y1);

		f32 w = info.m_Image->GetWidth();
		f32 h = info.m_Image->GetHeight();
		fvec2 minMax[2] = { fvec2{map.x0/w, map.y0/h}, fvec2{map.x1/w, map.y1/h} };

		std::shared_ptr<SubImage> sub(new SubImage(info.m_Image, minMax));

		return sub;
	}
}
