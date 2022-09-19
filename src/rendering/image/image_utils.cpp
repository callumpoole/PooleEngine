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
				return nullptr;
			}
		}
	}

	Image* ImageUtils::ToGreyscale(const Image* src, GreyScaleFunc func)	 { return ::Poole::Rendering::ToGreyscale<1>(src, func); }
	Image* ImageUtils::ToGreyscaleRG(const Image* src, GreyScaleFunc func)   { return ::Poole::Rendering::ToGreyscale<2>(src, func); }
	Image* ImageUtils::ToGreyscaleRGB(const Image* src, GreyScaleFunc func)  { return ::Poole::Rendering::ToGreyscale<3>(src, func); }
	Image* ImageUtils::ToGreyscaleRGBA(const Image* src, GreyScaleFunc func) { return ::Poole::Rendering::ToGreyscale<4>(src, func); }

	namespace 
	{
		template<u32 CHANNELS>
		Image* GreyscaleTo(const Image* src)
		{
			static_assert(CHANNELS == 3 || CHANNELS == 4);

			if (src->GetNumChannels() != 1)
			{
				LOG_ERROR("Current image is not greyscale, it has {} channels.", src->GetNumChannels());
				return nullptr;
			}

			u8* newBytes = new u8[src->GetNumPixels() * CHANNELS];
			u32 iter = 0;
			for (u8 grey : src->GetIterPerChannelDontUnFlip<u8>())
			{
				newBytes[iter++] = grey; //R
				newBytes[iter++] = grey; //G
				newBytes[iter++] = grey; //B
				if constexpr (CHANNELS == 4)
				{
					newBytes[iter++] = 255;  //A
				}
			}
			return new Image(newBytes, src->GetSize(), CHANNELS, src->WasYFlippedWhenLoaded());
		}
	}

	Image* ImageUtils::GreyscaleToRGB(const Image* src)  { return ::Poole::Rendering::GreyscaleTo<3>(src); }
	Image* ImageUtils::GreyscaleToRGBA(const Image* src) { return ::Poole::Rendering::GreyscaleTo<4>(src); }

	namespace {
		template<typename BASE>
		Image* ReplaceBlackWithAlphaUNUSED(const Image* src)
		{
			using Types = Image::Types<BASE>;
			using GREY = Types::GREY;
			using RG = Types::RG;
			using RGB = Types::RGB;
			using RGBA = Types::RGBA;

			RGBA* newBytes = new RGBA[src->GetBytesForWholeImage()];
			u32 iter = 0;

			auto AddBytes = [newBytes, &iter, src]<typename GROUP>(bool(*isBlack)(GROUP color), RGBA(*copyColor)(GROUP color))
			{
				for (const GROUP& color : src->GetIterDontUnFlip<GROUP>())
				{
					newBytes[iter++] = isBlack(color) ? RGBA{ 0 } : copyColor(color);
				}
			};

			switch (src->GetNumChannels())
			{
			case 1: InvokeTemplatedLambda<GREY>(AddBytes,
				[](GREY) { return false; }, //On purpse			   
				[](GREY grey) { return RGBA{ Types::MAX, Types::MAX, Types::MAX, grey }; }); //Best results with greyscale image (no black fades)
				break;
			case 2: InvokeTemplatedLambda<RG>(AddBytes,
				[](RG rg) { return rg.r + rg.g == Types::MIN; },
				[](RG rg) { return RGBA{ rg.r, rg.g, Types::MIN, Types::MAX }; });
				break;
			case 3: InvokeTemplatedLambda<RGB>(AddBytes,
				[](RGB rgb) { return rgb.r + rgb.g + rgb.b == Types::MIN; },
				[](RGB rgb) { return RGBA{ rgb.r, rgb.g, rgb.b, Types::MAX }; });
				break;
			case 4: InvokeTemplatedLambda<RGBA>(AddBytes,
				[](RGBA rgba) { return rgba.r + rgba.g + rgba.b == Types::MIN; },
				[](RGBA rgba) { return rgba; });
				break;
			default:
				return nullptr;
			}

			return new Image((BASE*)newBytes, src->GetSize(), 4, src->WasYFlippedWhenLoaded());
		}
	}

	Image* ImageUtils::ReplaceBlackWithAlpha(const Image* src)
	{
		auto ReplaceBlackWithAlpha = []<typename BASE>(const Image* src) -> Image*
		{
			using Types = Image::Types<BASE>;
			using GREY = Types::GREY;
			using RG = Types::RG;
			using RGB = Types::RGB;
			using RGBA = Types::RGBA;

			RGBA* newBytes = new RGBA[src->GetBytesForWholeImage()];
			u32 iter = 0;

			auto AddBytes = [newBytes, &iter, src]<typename GROUP>(bool(*isBlack)(GROUP color), RGBA(*copyColor)(GROUP color))
			{
				for (const GROUP& color : src->GetIterDontUnFlip<GROUP>())
				{
					newBytes[iter++] = isBlack(color) ? RGBA{ 0 } : copyColor(color);
				}
			};

			switch (src->GetNumChannels())
			{
			case 1: InvokeTemplatedLambda<GREY>(AddBytes,
				[](GREY) { return false; }, //On purpse			   
				[](GREY grey) { return RGBA{ Types::MAX, Types::MAX, Types::MAX, grey }; }); //Best results with greyscale image (no black fades)
				break;
			case 2: InvokeTemplatedLambda<RG>(AddBytes,
				[](RG rg) { return rg.r + rg.g == Types::MIN; },
				[](RG rg) { return RGBA{ rg.r, rg.g, Types::MIN, Types::MAX }; });
				break;
			case 3: InvokeTemplatedLambda<RGB>(AddBytes,
				[](RGB rgb) { return rgb.r + rgb.g + rgb.b == Types::MIN; },
				[](RGB rgb) { return RGBA{ rgb.r, rgb.g, rgb.b, Types::MAX }; });
				break;
			case 4: InvokeTemplatedLambda<RGBA>(AddBytes,
				[](RGBA rgba) { return rgba.r + rgba.g + rgba.b == Types::MIN; },
				[](RGBA rgba) { return rgba; });
				break;
			default:
				return nullptr;
			}

			return new Image((BASE*)newBytes, src->GetSize(), 4, src->WasYFlippedWhenLoaded());
		};


		return src->InvokeForFormat(ReplaceBlackWithAlpha, src);

		//switch (src->GetFormat())
		//{
		//case Image::EImageFormat::Bytes:
		//	return ::Poole::Rendering::ReplaceBlackWithAlpha<u8>(src);
		//case Image::EImageFormat::Floats:
		//	return ::Poole::Rendering::ReplaceBlackWithAlpha<f32>(src);
		//default:
		//	return nullptr;
		//}
	}

	/*static*/ Image* ImageUtils::YFlip(const Image* src)
	{
		u8* newBytes = new u8[src->GetBytesForWholeImage()]; //Okay if actually floats since just copying data
		const u8* srcBytes = (u8*)src->GetData(); //Can be floats, works in the same way
		const u32 bytesPerRow = src->GetBytesPerRow();
		for (u32 y = 0; y < src->GetHeight(); y++)
		{
			const u32 yRead = src->GetHeight() - y - 1;
			memcpy(newBytes + (y * bytesPerRow), srcBytes + (yRead * bytesPerRow), bytesPerRow);
		}

		return new Image((void*)newBytes, src->GetSize(), src->GetNumChannels(), !src->WasYFlippedWhenLoaded(), /*ownsMemory*/ true, src->GetFormat());
	}
}