#include "poole/core.h"

namespace Poole::Rendering
{
	class Image;

	class ImageUtils
	{
	public:
		//Source: https://tannerhelland.com/2011/10/01/grayscale-image-algorithm-vb6.html
		static f32 GreyscaleAveraged(f32 r, f32 g, f32 b) { return (r + g + b) / 3; }
		static f32 GreyscaleColorAdj(f32 r, f32 g, f32 b) { return (r * 0.3f + g * 0.59f + b * 0.11f); }
		static f32 GreyscaleColorAdj_BT709(f32 r, f32 g, f32 b) { return (r * 0.2126f + g * 0.7152f + b * 0.0722f); }
		static f32 GreyscaleColorAdj_BT601(f32 r, f32 g, f32 b) { return (r * 0.299f + g * 0.587f + b * 0.114f); }

		using GreyScaleFunc = f32(*)(f32 r, f32 g, f32 b);

		static Image* ToGreyscale(const Image* src, GreyScaleFunc func = &GreyscaleColorAdj);
		static Image* ToGreyscaleRG(const Image* src, GreyScaleFunc func = &GreyscaleColorAdj);
		static Image* ToGreyscaleRGB(const Image* src, GreyScaleFunc func = &GreyscaleColorAdj);
		static Image* ToGreyscaleRGBA(const Image* src, GreyScaleFunc func = &GreyscaleColorAdj);
		static Image* ToGreyscaleInline(Image* src, GreyScaleFunc func = &GreyscaleColorAdj);
		static Image* ToGreyscaleRGInline(Image* src, GreyScaleFunc func = &GreyscaleColorAdj);
		static Image* ToGreyscaleRGBInline(Image* src, GreyScaleFunc func = &GreyscaleColorAdj);
		static Image* ToGreyscaleRGBAInline(Image* src, GreyScaleFunc func = &GreyscaleColorAdj);

		static Image* GreyscaleToRGB(const Image* src);
		static Image* GreyscaleToRGBA(const Image* src);
		static Image* GreyscaleToRGBInline(Image* src);
		static Image* GreyscaleToRGBAInline(Image* src);

		static void ReplaceBlackWithAlphaImpl(const Image* src, u8* bytes);
		static Image* ReplaceBlackWithAlpha(const Image* src);
		static Image* ReplaceBlackWithAlphaInline(Image* src);

		static Image* YFlip(const Image* src, bool bToggleWasYFlippedWhenLoaded = true);
		static Image* YFlipInline(Image* src, bool bToggleWasYFlippedWhenLoaded = true);

		static Image* XFlip(const Image* src);
		static Image* XFlipInline(Image* src);
	};
}
