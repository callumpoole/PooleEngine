#pragma once

#include "poole/core.h"
#include <span>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

namespace Poole
{
	class Image
	{
	public:
		Image(const char* path, bool* out_Successful = nullptr, bool printLogWhenLoaded = true);
		~Image();
	
		//Stb reads top to bottom, openGL is bottom to top
		static void SetYFlipBeforeLoad(bool bottomToTop);
		static bool GetYFlipBeforeLoad() { return s_YFlip; }
	
		const u8* GetBytes() const { return m_Bytes; }
		const std::vector<u8> YFlipBytes() const;
		const ivec2& GetSize() const { return m_Size; }
		i32 GetColorChannels() const { return m_NumColorChannels; }
		bool WasYFlippedWhenLoaded() const { return m_YFlippedWhenLoaded; }

		bool IsPowerOfTwo() const { return Math::IsPowerOfTwo((u64)m_Size.x) && Math::IsPowerOfTwo((u64)m_Size.y); }


		void DebugPrint() const;

	private:
		static bool s_YFlip;
		ivec2 m_Size = { -1,-1 };
		i32 m_NumColorChannels = -1;
		u8* m_Bytes;
		bool m_YFlippedWhenLoaded;

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

		IteratorGenerator<u8> GetIterPerChannel() const			  { return IteratorGenerator<u8>(m_Bytes, m_Size.x * m_NumColorChannels, m_Size.y, m_YFlippedWhenLoaded); }
		IteratorGenerator<u8> GetIterPerChannelDontUnFlip() const { return IteratorGenerator<u8>(m_Bytes, m_Size.x * m_NumColorChannels, m_Size.y, false); }
	};
}