#include "poole/rendering/image/image_utils.h"

#include "poole/rendering/image/image.h"

namespace Poole::Rendering
{
	Image* ImageUtils::ToGreyscale(const Image* src, GreyScaleFunc func)
	{
		if (src->GetNumChannels() == 1)
		{
			LOG_WARNING("Greyscale to Greyscale, useless call!")
			return &Image::s_Invalid;
		}
		if (src->GetNumChannels() == 3 || src->GetNumChannels() == 4)
		{
			auto Do = [src, &func]<typename T>() -> Image*
			{
				u8* newBytes = new u8[src->GetNumPixels()];
				u32 iter = 0;
				for (T color : src->GetIterDontUnFlip<T>())
				{
					const u8 mix = (u8)(func((f32)color.r, (f32)color.g, (f32)color.b));
					newBytes[iter++] = mix;
				}
				return new Image(newBytes, src->GetSize(), 1, src->WasYFlippedWhenLoaded());
			};
			return (src->GetNumChannels() == 3) ? InvokeTemplatedLambda<u8color3>(Do) : InvokeTemplatedLambda<u8color4>(Do);
		}
		else
		{
			LOG_ERROR("Cannot Convert Image With {} Channels to Greyscale.", src->GetNumChannels());
			return &Image::s_Invalid;
		}
	}
	Image* ImageUtils::GreyscaleToRGB(const Image* src)
	{
		if (src->GetNumChannels() != 1)
		{
			LOG_ERROR("Current image is not greyscale, it has {} channels.", src->GetNumChannels());
			return &Image::s_Invalid;
		}

		u8* newBytes = new u8[src->GetNumPixels() * 3];
		u32 iter = 0;
		for (u8 grey : src->GetIterPerChannelDontUnFlip())
		{
			newBytes[iter++] = grey; //R
			newBytes[iter++] = grey; //G
			newBytes[iter++] = grey; //B
		}
		return new Image(newBytes, src->GetSize(), 3, src->WasYFlippedWhenLoaded());
	}
	Image* ImageUtils::GreyscaleToRGBA(const Image* src)
	{
		if (src->GetNumChannels() != 1)
		{
			LOG_ERROR("Current image is not greyscale, it has {} channels.", src->GetNumChannels());
			return &Image::s_Invalid;
		}

		u8* newBytes = new u8[src->GetNumPixels() * 4];
		u32 iter = 0;
		for (u8 grey : src->GetIterPerChannelDontUnFlip())
		{
			LOG("{:x}", grey);

			newBytes[iter++] = grey; //R
			newBytes[iter++] = grey; //G
			newBytes[iter++] = grey; //B
			newBytes[iter++] = 255;  //A
		}
		return new Image(newBytes, src->GetSize(), 4, src->WasYFlippedWhenLoaded());
	}
	Image* ImageUtils::ReplaceBlackWithAlpha(const Image* src)
	{
		u8color4* newBytes = new u8color4[src->GetNumBytes()];
		u32 iter = 0;

		if (src->GetNumChannels() == 1)
		{
			for (u8 grey : src->GetIterDontUnFlip<u8>())
			{
				if (grey == 0)
				{
					newBytes[iter++] = u8color4{ 0 };
				}
				else
				{
					newBytes[iter++] = u8color4{ grey, grey, grey, 255 };
				}
			}
		}
		else if (src->GetNumChannels() == 2)
		{
			for (u8color3 rg : src->GetIterDontUnFlip<u8color3>())
			{
				if (rg.r + rg.g == 0)
				{
					newBytes[iter++] = u8color4{ 0 };
				}
				else
				{
					newBytes[iter++] = u8color4{ rg.r, rg.g, 0, 255 };
				}
			}
		}
		else if (src->GetNumChannels() == 3)
		{
			for (u8color3 rgb : src->GetIterDontUnFlip<u8color3>())
			{
				if (rgb.r + rgb.g + rgb.b == 0)
				{
					newBytes[iter++] = u8color4{ 0 };
				}
				else
				{
					newBytes[iter++] = u8color4{ rgb.r, rgb.g, rgb.b, 255 };
				}
			}
		}
		else if (src->GetNumChannels() == 4)
		{
			for (u8color4 rgba : src->GetIterDontUnFlip<u8color4>())
			{
				if (rgba.r + rgba.g + rgba.b == 0)
				{
					newBytes[iter++] = u8color4{ 0 };
				}
				else
				{
					newBytes[iter++] = rgba;
				}
			}
		}

		return new Image((u8*)newBytes, src->GetSize(), 4, src->WasYFlippedWhenLoaded());
	}

}