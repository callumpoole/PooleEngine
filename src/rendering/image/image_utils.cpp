#include "poole/rendering/image/image_utils.h"

#include "poole/rendering/image/image.h"

namespace Poole::Rendering
{
	namespace 
	{
		template<u32 C>
		Image* ToGreyscale(const Image* src, ImageUtils::GreyScaleFunc func)
		{
			static_assert(C != 0 && C <= 4);

			if (src->GetNumChannels() == 3 || src->GetNumChannels() == 4)
			{
				auto Do = [src, &func]<typename T>() -> Image*
				{
					u8* newBytes = new u8[src->GetNumPixels() * C];
					u32 iter = 0;
					for (T color : src->GetIterDontUnFlip<T>())
					{
						const u8 mix = (u8)(func((f32)color.r, (f32)color.g, (f32)color.b));
						newBytes[iter++] = mix;
						if constexpr (C >= 2)
							newBytes[iter++] = mix;
						if constexpr (C >= 3)
							newBytes[iter++] = mix;
						if constexpr (C == 4)
							newBytes[iter++] = 255;
					}
					return new Image(newBytes, src->GetSize(), C, src->WasYFlippedWhenLoaded());
				};
				return (src->GetNumChannels() == 3) ? InvokeTemplatedLambda<u8color3>(Do) : InvokeTemplatedLambda<u8color4>(Do);
			}
			else
			{
				LOG_ERROR("Cannot Convert Image With {} Channels to Greyscale.", src->GetNumChannels());
				return &Image::s_Invalid;
			}
		}
	}

	Image* ImageUtils::ToGreyscale(const Image* src, GreyScaleFunc func)	 { return ::Poole::Rendering::ToGreyscale<1>(src, func); }
	Image* ImageUtils::ToGreyscaleRG(const Image* src, GreyScaleFunc func)   { return ::Poole::Rendering::ToGreyscale<2>(src, func); }
	Image* ImageUtils::ToGreyscaleRGB(const Image* src, GreyScaleFunc func)  { return ::Poole::Rendering::ToGreyscale<3>(src, func); }
	Image* ImageUtils::ToGreyscaleRGBA(const Image* src, GreyScaleFunc func) { return ::Poole::Rendering::ToGreyscale<4>(src, func); }

	namespace 
	{
		template<u32 C>
		Image* GreyscaleTo(const Image* src)
		{
			static_assert(C == 3 || C == 4);

			if (src->GetNumChannels() != 1)
			{
				LOG_ERROR("Current image is not greyscale, it has {} channels.", src->GetNumChannels());
				return &Image::s_Invalid;
			}

			u8* newBytes = new u8[src->GetNumPixels() * C];
			u32 iter = 0;
			for (u8 grey : src->GetIterPerChannelDontUnFlip())
			{
				newBytes[iter++] = grey; //R
				newBytes[iter++] = grey; //G
				newBytes[iter++] = grey; //B
				if constexpr (C == 4)
				{
					newBytes[iter++] = 255;  //A
				}
			}
			return new Image(newBytes, src->GetSize(), C, src->WasYFlippedWhenLoaded());
		}
	}

	Image* ImageUtils::GreyscaleToRGB(const Image* src)  { return ::Poole::Rendering::GreyscaleTo<3>(src); }
	Image* ImageUtils::GreyscaleToRGBA(const Image* src) { return ::Poole::Rendering::GreyscaleTo<4>(src); }

	Image* ImageUtils::ReplaceBlackWithAlpha(const Image* src)
	{
		u8color4* newBytes = new u8color4[src->GetNumBytes()];
		u32 iter = 0;

		auto AddBytes = [newBytes, &iter, src]<typename T>(bool(*isBlack)(T color), u8color4(*copyColor)(T color))
		{
			for (T color : src->GetIterDontUnFlip<T>())
			{
				newBytes[iter++] = isBlack(color) ? u8color4{ 0 } : copyColor(color);
			}
		};

		switch (src->GetNumChannels())
		{
		case 1: InvokeTemplatedLambda<u8>(AddBytes, 
			[](u8 grey) { return grey == 0; },					   
			[](u8 grey) { return u8color4{ grey, grey, grey, 255 }; }); 
			break;
		case 2: InvokeTemplatedLambda<u8color2>(AddBytes, 
			[](u8color2 rg) { return rg.r + rg.g == 0; },				   
			[](u8color2 rg) { return u8color4{ rg.r, rg.g, 0, 255 }; }); 
			break;
		case 3: InvokeTemplatedLambda<u8color3>(AddBytes, 
			[](u8color3 rgb) { return rgb.r + rgb.g + rgb.b == 0; },	   
			[](u8color3 rgb) { return u8color4{ rgb.r, rgb.g, rgb.b, 255 }; }); 
			break;
		case 4: InvokeTemplatedLambda<u8color4>(AddBytes, 
			[](u8color4 rgba) { return rgba.r + rgba.g + rgba.b == 0; }, 
			[](u8color4 rgba) { return rgba; }); 
			break;
		default:
			return &Image::s_Invalid;
		}

		return new Image((u8*)newBytes, src->GetSize(), 4, src->WasYFlippedWhenLoaded());
	}

	/*static*/ Image* ImageUtils::YFlip(const Image* src)
	{
		u8* newBytes = new u8[src->GetNumBytes()];
		const u8* srcBytes = src->GetBytes();
		const i32 bytesPerRow = src->GetNumBytesPerRow();
		for (i32 y = 0; y < src->GetHeight(); y++)
		{
			const i32 yRead = src->GetHeight() - y - 1;
			memcpy(newBytes + (y * bytesPerRow), srcBytes + (yRead * bytesPerRow), bytesPerRow);
		}

		return new Image((u8*)newBytes, src->GetSize(), src->GetNumChannels(), src->WasYFlippedWhenLoaded());
	}
}