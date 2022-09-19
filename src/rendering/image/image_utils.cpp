#include "poole/rendering/image/image_utils.h"

#include "poole/rendering/image/image.h"

namespace Poole::Rendering
{
	namespace 
	{
		template<u32 CHANNELS>
		Image* ToGreyscale(const Image* src, ImageUtils::GreyScaleFunc func)
		{
			static_assert(CHANNELS != 0 && CHANNELS <= 4);

			if (src->GetNumChannels() != 3 && src->GetNumChannels() != 4)
			{
				LOG_ERROR("Cannot Convert Image With {} Channels to Greyscale.", src->GetNumChannels());
				return nullptr;
			}

			auto Do = [src, &func]<typename BASE, typename GROUP>(BASE* newData) -> Image*
			{
				u32 i = 0;
				for (GROUP color : src->GetIterDontUnFlip<GROUP>())
				{
					const BASE mix = (BASE)(func((f32)color.r, (f32)color.g, (f32)color.b)); //f32 is not a mistake

					newData[i++] = mix;
					if constexpr (CHANNELS >= 2)
					{
						newData[i++] = mix;
					}
					if constexpr (CHANNELS >= 3)
					{
						newData[i++] = mix;
					}
					if constexpr (CHANNELS == 4)
					{
						newData[i++] = Image::Types<BASE>::MAX;
					}
				}
				return new Image(newData, src->GetSize(), CHANNELS, src->WasYFlippedWhenLoaded());
			};
			auto Impl = [src, Do]<typename BASE>() -> Image*
			{
				BASE* newData = new BASE[src->GetNumPixels() * CHANNELS];

				using Types = Image::Types<BASE>;
				using RGB = Types::RGB;
				using RGBA = Types::RGBA;

				return (src->GetNumChannels() == 3) ? InvokeTemplatedLambda<BASE, RGB>(Do, newData) : InvokeTemplatedLambda<BASE, RGBA>(Do, newData);
			};

			return src->InvokeForFormat(Impl);
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

			auto Impl = [src]<typename BASE>() -> Image*
			{
				BASE* newData = new BASE[src->GetNumPixels() * CHANNELS];
				u32 i = 0;
				for (BASE grey : src->GetIterPerChannelDontUnFlip<BASE>())
				{
					newData[i++] = grey; //R
					newData[i++] = grey; //G
					newData[i++] = grey; //B
					if constexpr (CHANNELS == 4)
					{
						newData[i++] = Image::Types<BASE>::MAX; 
					}
				}
				return new Image(newData, src->GetSize(), CHANNELS, src->WasYFlippedWhenLoaded());
			};

			return src->InvokeForFormat(Impl);
		}
	}

	Image* ImageUtils::GreyscaleToRGB(const Image* src)  { return ::Poole::Rendering::GreyscaleTo<3>(src); }
	Image* ImageUtils::GreyscaleToRGBA(const Image* src) { return ::Poole::Rendering::GreyscaleTo<4>(src); }

	Image* ImageUtils::ReplaceBlackWithAlpha(const Image* src)
	{
		auto Impl = [src]<typename BASE>() -> Image*
		{
			using Types = Image::Types<BASE>;
			using GREY = Types::GREY;
			using RG = Types::RG;
			using RGB = Types::RGB;
			using RGBA = Types::RGBA;

			RGBA* newBytes = new RGBA[src->GetBytesForWholeImage()];
			u32 iter = 0;

			//Downside for InvokeForFormat is I need to forward the RGBA, I can't access directly from outer body
			auto AddBytes = [newBytes, &iter, src]<typename GROUP, typename RGBA>(bool(*isBlack)(GROUP color), RGBA(*copyColor)(GROUP color))
			{
				for (const GROUP& color : src->GetIterDontUnFlip<GROUP>())
				{
					newBytes[iter++] = isBlack(color) ? RGBA{ 0 } : copyColor(color);
				}
			};

			switch (src->GetNumChannels())
			{
			case 1: InvokeTemplatedLambda<GREY, RGBA>(AddBytes,
				[](GREY) { return false; }, //On purpse			   
				[](GREY grey) { return RGBA{ Types::MAX, Types::MAX, Types::MAX, grey }; }); //Best results with greyscale image (no black fades)
				break;
			case 2: InvokeTemplatedLambda<RG, RGBA>(AddBytes,
				[](RG rg) { return rg.r + rg.g == Types::MIN; },
				[](RG rg) { return RGBA{ rg.r, rg.g, Types::MIN, Types::MAX }; });
				break;
			case 3: InvokeTemplatedLambda<RGB, RGBA>(AddBytes,
				[](RGB rgb) { return rgb.r + rgb.g + rgb.b == Types::MIN; },
				[](RGB rgb) { return RGBA{ rgb.r, rgb.g, rgb.b, Types::MAX }; });
				break;
			case 4: InvokeTemplatedLambda<RGBA, RGBA>(AddBytes,
				[](RGBA rgba) { return rgba.r + rgba.g + rgba.b == Types::MIN; },
				[](RGBA rgba) { return rgba; });
				break;
			default:
				return nullptr;
			}

			return new Image((BASE*)newBytes, src->GetSize(), 4, src->WasYFlippedWhenLoaded());
		};

		return src->InvokeForFormat(Impl);
	}

	/*static*/ Image* ImageUtils::YFlip(const Image* src, bool bToggleWasYFlippedWhenLoaded)
	{
		u8* newBytes = new u8[src->GetBytesForWholeImage()]; //Okay if actually floats since just copying data
		const u8* srcBytes = (u8*)src->GetData(); //Can be floats, works in the same way
		const u32 bytesPerRow = src->GetBytesPerRow();
		for (u32 y = 0; y < src->GetHeight(); y++)
		{
			const u32 yRead = src->GetHeight() - y - 1;
			memcpy(newBytes + y * bytesPerRow, srcBytes + yRead * bytesPerRow, bytesPerRow);
		}

		return new Image(
			(void*)newBytes, 
			src->GetSize(), 
			src->GetNumChannels(), 
			bToggleWasYFlippedWhenLoaded ? !src->WasYFlippedWhenLoaded() : src->WasYFlippedWhenLoaded(), 
			/*ownsMemory*/ true, 
			src->GetFormat());
	}

	/*static*/ Image* ImageUtils::YFlipInline(Image* src, bool bToggleWasYFlippedWhenLoaded)
	{
		u8* srcBytes = (u8*)src->m_Data; //Can be floats, works in the same way
		const u32 bytesPerRow = src->GetBytesPerRow();

		std::vector<u8> buffer;
		buffer.resize(bytesPerRow);

		for (u32 yTop = 0, yBottom = src->GetHeight()-1; yTop < src->GetHeight() / 2; yTop++, yBottom--)
		{
			//Essentially MemSwap top and bottom by bytesPerRow
			u8* topRow    = srcBytes + yTop * bytesPerRow;
			u8* bottomRow = srcBytes + yBottom * bytesPerRow;

			memcpy(buffer.data(), topRow, bytesPerRow);
			memcpy(topRow, bottomRow, bytesPerRow);
			memcpy(bottomRow, buffer.data(), bytesPerRow);
		}
		if (bToggleWasYFlippedWhenLoaded)
		{
			src->m_YFlippedWhenLoaded = !src->m_YFlippedWhenLoaded;
		}
		return src;
	}

	/*static*/ Image* ImageUtils::XFlip(const Image* src)
	{
		u8* newBytes = new u8[src->GetBytesForWholeImage()]; //Okay if actually floats since just copying data
		const u8* srcBytes = (u8*)src->GetData(); //Can be floats, works in the same way
		const u32 bytesPerRow = src->GetBytesPerRow();
		const u32 bytesPerPixel = src->GetNumBytesPerPixel();

		for (u32 y = 0; y < src->GetHeight(); y++)
		{
			for (u32 x = 0, xWrite = src->GetWidth() - 1; x < src->GetWidth(); x++, xWrite--)
			{
				memcpy(newBytes + y * bytesPerRow + xWrite * bytesPerPixel, 
					   srcBytes + y * bytesPerRow + x      * bytesPerPixel,
					   bytesPerPixel);
			}
		}

		return new Image(
			(void*)newBytes,
			src->GetSize(),
			src->GetNumChannels(),
			src->WasYFlippedWhenLoaded(),
			/*ownsMemory*/ true,
			src->GetFormat());
	}

	/*static*/ Image* ImageUtils::XFlipInline(Image* src)
	{
		u8* srcBytes = (u8*)src->m_Data; //Can be floats, works in the same way
		const u32 bytesPerRow = src->GetBytesPerRow();
		const u32 bytesPerPixel = src->GetNumBytesPerPixel();

		std::array<u8, Image::MAX_BYTES_POSSIBLE_PER_PIXEL> buffer; //Cheaper to do this than heap alloc

		for (u32 y = 0; y < src->GetHeight(); y++)
		{
			for (u32 xLeft = 0, xRight = src->GetWidth() - 1; xLeft < (src->GetWidth() / 2); xLeft++, xRight--)
			{
				//Essentially MemSwap left and right by bytesPerPixel
				u8* leftSide = srcBytes + y * bytesPerRow + xLeft * bytesPerPixel;
				u8* rightSide = srcBytes + y * bytesPerRow + xRight * bytesPerPixel;
				memcpy(&buffer, leftSide, bytesPerPixel);
				memcpy(leftSide, rightSide, bytesPerPixel);
				memcpy(rightSide, &buffer, bytesPerPixel);
			}
		}
		return src;
	}
}