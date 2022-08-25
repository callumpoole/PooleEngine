#include "poole/rendering/image/svg.h"

#include "stb/stb_truetype.h"

namespace Poole::Rendering
{
	/*static*/ void* SVG::GetBytes()
	{
		unsigned char* ttf_buffer = new unsigned char[1 << 20];
		unsigned char* temp_bitmap = new unsigned char[512 * 512];

		stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs

		fread(ttf_buffer, 1, 1 << 20, fopen("c:/windows/fonts/times.ttf", "rb"));
		stbtt_BakeFontBitmap(ttf_buffer, 0, 70.0, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!

		delete[] ttf_buffer;

		return temp_bitmap;
	}
}
