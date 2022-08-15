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

		static Image* ToGreyscale(const Image* Src, GreyScaleFunc func = &GreyscaleColorAdj);
		static Image* GreyscaleToRGB(const Image* Src);
		static Image* GreyscaleToRGBA(const Image* Src);
		static Image* ReplaceBlackWithAlpha(const Image* Src);
	};
}
