#pragma once

#include "poole/core.h"
#include <span>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

namespace Poole::Rendering
{
	class Image
	{
	public:
		static Image s_Invalid;
		static Image s_White1x1rgba;

		Image(const char* path, bool* out_Successful = nullptr, bool printLogWhenLoaded = true);
		Image(u8* bytes, uvec2 size, u32 channels, bool isYFlipped, bool ownsMemory = true);
		Image(u8* bytes, u32 width, u32 height, u32 channels, bool isYFlipped, bool ownsMemory = true) : Image(bytes, {width, height}, channels, isYFlipped, ownsMemory) {}
		Image(const Image& src);
		Image& operator=(const Image& rhs);
		Image(Image&& src);
		Image& operator=(Image&& rhs);
		Image() : Image(nullptr, { 0,0 }, 0, false, false) { }
		~Image();
	
		//Stb reads top to bottom, openGL is bottom to top
		static void SetYFlipBeforeLoad(bool bottomToTop);
		static bool GetYFlipBeforeLoad() { return s_YFlip; }
	
		u32 GetId() const { return m_Id; }
		const bool IsValid() const { return m_Bytes != nullptr && m_Id > 0; }
		operator bool() { return IsValid(); }
		const u8* GetBytes() const { return m_Bytes; }
		const std::vector<u8> YFlipBytes() const;
		const uvec2& GetSize() const { return m_Size; }
		u32 GetWidth() const { return m_Size.x; }
		u32 GetHeight() const { return m_Size.y; }
		u32 GetNumChannels() const { return m_NumChannels; }
		u32 GetNumBytesPerRow() const { return m_Size.x * m_NumChannels; }
		u32 GetNumBytes() const { return GetNumBytesPerRow() * m_Size.y; }
		u32 GetNumPixels() const { return m_Size.x * m_Size.y; }
		bool WasYFlippedWhenLoaded() const { return m_YFlippedWhenLoaded; }

		bool IsPowerOfTwo() const { return Math::IsPowerOfTwo((u64)m_Size.x) && Math::IsPowerOfTwo((u64)m_Size.y); }

		//Source: https://tannerhelland.com/2011/10/01/grayscale-image-algorithm-vb6.html
		static f32 GreyscaleAveraged(f32 r, f32 g, f32 b)		{ return (r + g + b) / 3; }
		static f32 GreyscaleColorAdj(f32 r, f32 g, f32 b)		{ return (r * 0.3f    + g * 0.59f   + b * 0.11f); }
		static f32 GreyscaleColorAdj_BT709(f32 r, f32 g, f32 b) { return (r * 0.2126f + g * 0.7152f + b * 0.0722f); }
		static f32 GreyscaleColorAdj_BT601(f32 r, f32 g, f32 b) { return (r * 0.299f  + g * 0.587f  + b * 0.114f); }

		using GreyScaleFunc = f32(*)(f32 r, f32 g, f32 b);
		
		Image ToGreyscale(GreyScaleFunc func = &GreyscaleColorAdj) const;
		Image GreyscaleToRGB() const;
		Image GreyscaleToRGBA() const;
		Image ReplaceBlackWithAlpha() const;

		void DebugPrint() const;

	private:
		static u32 s_IdCounter;
		u32 m_Id = 0;

		static bool s_YFlip;
		uvec2 m_Size = { 0, 0 };
		u32 m_NumChannels = 0;
		u8* m_Bytes = nullptr;
		bool m_YFlippedWhenLoaded;
		bool m_AllocatedWithSTBI = false;
		bool m_OwnsMemory = false;

		static constexpr u8 COLOR_DEPTH = 8;

	public:

		//Help from: https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
		template<typename TColor>
		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type	= std::ptrdiff_t;
			using value_type		= TColor;
			using pointer			= TColor*;
			using reference			= TColor&;

			Iterator(pointer bytes, u32 width, u32 height, bool flip) : m_Bytes(bytes), m_Width(width), m_Height(height), m_Flip(flip) {}

			reference operator*() const { return *m_Bytes; }
			pointer operator->() { return m_Bytes; }
			pointer GetPointer() { return m_Bytes; }

			// Prefix increment
			Iterator& operator++() 
			{ 
				m_Bytes++; 
				if (m_Flip)
				{
					if (++m_WidthCounter == m_Width)
					{
						if (++m_HeightCounter == m_Height)
						{
							m_Bytes += m_Width * (m_Height - 1); //(Pointer arithmetic)
							m_HeightCounter = 0; //Just for completeness / resetting
						}
						else
						{
							m_Bytes -= m_Width * 2; //(Pointer arithmetic)
						}
						m_WidthCounter = 0;
					}
				}
				return *this;
			}

			// Postfix increment
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_Bytes == b.m_Bytes; };
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_Bytes != b.m_Bytes; };
		private:
			pointer m_Bytes;
			u32 m_Width;
			u32 m_Height;
			bool m_Flip;
			u32 m_WidthCounter = 0;
			u32 m_HeightCounter = 0;
		};

		template<typename TColor>
		struct IteratorGenerator
		{
			IteratorGenerator(u8* bytes, u32 width, u32 height, bool flip) : m_Bytes((TColor*)bytes), m_Width(width), m_Height(height), m_Flip(flip) {}

			using Iter = Iterator<TColor>;

			Iter begin()
			{ 
				if (m_Flip)
				{
					return Iter(&m_Bytes[m_Width * (m_Height - 1)], m_Width, m_Height, m_Flip);
				}
				else
				{
					return Iter(&m_Bytes[0], m_Width, m_Height, m_Flip);
				}
			}
			Iter end()
			{ 
				return Iter(&m_Bytes[m_Width * m_Height], m_Width, m_Height, m_Flip);
			}
		private:
			TColor* m_Bytes;
			u32 m_Width;
			u32 m_Height;
			bool m_Flip;
		};

		template<typename TColor>
		IteratorGenerator<TColor> GetIter() const				  { return IteratorGenerator<TColor>(m_Bytes, m_Size.x, m_Size.y, m_YFlippedWhenLoaded); }

		template<typename TColor>
		IteratorGenerator<TColor> GetIterDontUnFlip() const		  { return IteratorGenerator<TColor>(m_Bytes, m_Size.x, m_Size.y, false); }

		IteratorGenerator<u8> GetIterPerChannel() const			  { return IteratorGenerator<u8>(m_Bytes, m_Size.x * m_NumChannels, m_Size.y, m_YFlippedWhenLoaded); }
		IteratorGenerator<u8> GetIterPerChannelDontUnFlip() const { return IteratorGenerator<u8>(m_Bytes, m_Size.x * m_NumChannels, m_Size.y, false); }
	};
}