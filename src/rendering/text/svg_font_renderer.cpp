#include "poole/rendering/text/svg_font_renderer.h"

#include "stb/stb_truetype.h"

#include "poole/rendering/image/image_utils.h"
#include "poole/rendering/renderer.h"

namespace Poole::Rendering
{
	SvgFontRenderer::SvgFontRenderer(const char* fontLocation)
	{
		m_TtfBuffer.resize(k_FontBuffer);

		FILE* filepoint;
		errno_t err;

		if ((err = fopen_s(&filepoint, fontLocation, "rb")) == 0) 
		{
			fread(m_TtfBuffer.data(), 1, k_FontBuffer, filepoint);
			fclose(filepoint);

			LOG("Loaded Variable-Width Font: {}", fontLocation);
		}
		else
		{
#pragma warning( disable : 4996 ) //It's warning about strerror_s but that's only in c compiler
			LOG_ERROR("File {} couldn't be opened! {}", fontLocation, strerror(err));
#pragma warning( default : 4996 )
		}
	}

	void SvgFontRenderer::CacheSize(f32 fontSize) const
	{
		if (m_Sizes.contains(fontSize))
			return;

		u8* temp_bitmap = new u8[k_Size.x * k_Size.y];
		std::vector<stbtt_bakedchar> cdata; 
		cdata.resize(k_NumChars);// ASCII 32..127 is 96 glyphs

		stbtt_BakeFontBitmap(m_TtfBuffer.data(), 0, fontSize, temp_bitmap, k_Size.x, k_Size.y, k_StartChar, k_NumChars, cdata.data()); // no guarantee this fits!

		Image* temp = new Image(temp_bitmap, k_Size.x, k_Size.y, 1, false, true);

		std::shared_ptr<Image> img;

		//OpenGL requires data to be flipped vertically
		//TODO: Make a function return a bool if a flip is needed.
		if (Renderer::s_GraphicsAPI == EGraphicsAPI::OpenGL)
		{
			//Flip first, then convert Greyscale to RGBA (whilst replacing black to alpha) to save performance
			img.reset(ImageUtils::ReplaceBlackWithAlpha(/*ImageUtils::YFlip*/(temp)));

			for (i32 i = 0; i < k_NumChars; i++)
			{
			//	cdata[i].y0 = (u16)k_Size.y - cdata[i].y0;
			//	cdata[i].y1 = (u16)k_Size.y - cdata[i].y1;
			//	std::swap(cdata[i].y0, cdata[i].y1);
			//	cdata[i].yoff = ((f32)k_Size.y - cdata[i].yoff) - (f32)k_Size.y;
			}
		}
		else
		{
			img.reset(ImageUtils::ReplaceBlackWithAlpha(temp));
		}

		m_Sizes[fontSize] = RasterInfo{ std::move(*(std::vector<TT_BakedChar>*)& cdata), img };
	}

	std::shared_ptr<SubImage> SvgFontRenderer::Convert(const char c, const f32 fontSize, f32& xoff, f32& yoff, f32& xadvance)
	{
		if (c < k_StartChar || c - k_StartChar >= k_NumChars)
		{
			LOG_WARNING("Character {} ({}) out of bounds ({} to {}).", c, u32(c), k_StartChar, k_StartChar + k_NumChars - 1);
			return nullptr;
		}

		CacheSize(fontSize); 
		RasterInfo& info = m_Sizes[fontSize];

		TT_BakedChar& map = info.m_CharMaps[c - k_StartChar];

		const f32 w = (f32)info.m_Image->GetWidth();
		const f32 h = (f32)info.m_Image->GetHeight();
		const fvec2 minMax[2] = { fvec2{map.x0/w, map.y0/h}, fvec2{map.x1/w, map.y1/h} };

		std::shared_ptr<SubImage> sub(new SubImage(info.m_Image, minMax));

		xoff = map.xoff;
		yoff = map.yoff;
		xadvance = map.xadvance;

		return sub;
	}

	void SvgFontRenderer::Convert2(char c, f32 fontSize, fvec2& pos, std::array<fvec4, 4>& coords, std::array<fvec2, 4>& uv)
	{
		if (c < k_StartChar || c - k_StartChar >= k_NumChars)
		{
			LOG_WARNING("Character {} ({}) out of bounds ({} to {}).", c, u32(c), k_StartChar, k_StartChar + k_NumChars - 1);
			return;
		}

		CacheSize(fontSize);
		RasterInfo& info = m_Sizes[fontSize];

		stbtt_aligned_quad q;
		stbtt_GetBakedQuad((const stbtt_bakedchar*)info.m_CharMaps.data(), k_Size.x, k_Size.y, c - k_StartChar, &pos.x, &pos.y, &q, 1); //1=opengl & d3d10+, 0=d3d9

		coords[0] = { q.x0, q.y0, 0, 1 };
		coords[1] = { q.x1, q.y0, 0, 1 };
		coords[2] = { q.x1, q.y1, 0, 1 };
		coords[3] = { q.x0, q.y1, 0, 1 };
		uv[0] = { q.s0, q.t0 };
		uv[1] = { q.s1, q.t0 };
		uv[2] = { q.s1, q.t1 };
		uv[3] = { q.s0, q.t1 };
	}

	std::shared_ptr<Image> SvgFontRenderer::GetImageForSize(f32 fontSize)
	{
		CacheSize(fontSize);
		return m_Sizes[fontSize].m_Image;
	}
}
