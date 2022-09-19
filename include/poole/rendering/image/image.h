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
		enum class EImageFormat : u8
		{
			Bytes,
			Floats,
			//sRGB in future...
		};

		static Image s_White1x1rgba;

		Image(const char* path, EImageFormat format = EImageFormat::Bytes, bool* out_Successful = nullptr);
		Image(void* data, uvec2 size, u32 channels, bool isYFlipped, bool ownsMemory, EImageFormat format);
		Image(u8* bytes, uvec2 size, u32 channels, bool isYFlipped, bool ownsMemory = true) : Image((void*)bytes, size, channels, isYFlipped, ownsMemory, EImageFormat::Bytes) {}
		Image(f32* floats, uvec2 size, u32 channels, bool isYFlipped, bool ownsMemory = true) : Image((void*)floats, size, channels, isYFlipped, ownsMemory, EImageFormat::Floats) {}
		Image(const Image& src);
		Image& operator=(const Image& rhs);
		Image(Image&& src);
		Image& operator=(Image&& rhs);
		~Image();
	
		//Stb reads top to bottom, openGL is bottom to top
		static void SetYFlipBeforeLoad(bool bottomToTop);
		static bool GetYFlipBeforeLoad() { return s_YFlip; }
		bool WasYFlippedWhenLoaded() const { return m_YFlippedWhenLoaded; }

		u32 GetId() const { return m_Id; }
		const bool IsValid() const { return m_Data != nullptr && m_Id > 0; }
		operator bool() const { return IsValid(); }

		const void* GetData() const { return m_Data; }
		template<typename T>
		const T* GetData() const { return CheckStorageType<T>() ? (T*)m_Data : nullptr; }

		u8 GetDataElementSizeBytes() const { return m_Format == EImageFormat::Bytes ? 1 : 4; }
		u8 GetDataElementSizeBits() const { return GetDataElementSizeBytes() * 8; }

		const uvec2& GetSize() const { return m_Size; }
		u32 GetWidth() const { return m_Size.x; }
		u32 GetHeight() const { return m_Size.y; }
		u32 GetNumPixels() const { return m_Size.x * m_Size.y; }
		u32 GetNumChannels() const { return m_NumChannels; }
		u32 GetNumBytesPerPixel() const { return m_NumChannels * GetDataElementSizeBytes(); }
		EImageFormat GetFormat() const { return m_Format; }

		u32 GetPixelsByChannelsPerRow() const { return m_Size.x * m_NumChannels; }
		u32 GetPixelsByChannelsPerColumn() const { return m_Size.y * m_NumChannels; }
		u32 GetPixelsByChannelsForWholeImage() const { return m_Size.x * m_Size.y * m_NumChannels; }

		u32 GetBytesPerRow() const { return GetPixelsByChannelsPerRow() * GetDataElementSizeBytes(); }
		u32 GetBytesPerColumn() const { return GetPixelsByChannelsPerColumn() * GetDataElementSizeBytes(); }
		u32 GetBytesForWholeImage() const { return GetPixelsByChannelsForWholeImage() * GetDataElementSizeBytes(); }

		bool IsPowerOfTwo() const { return Math::IsPowerOfTwo((u64)m_Size.x) && Math::IsPowerOfTwo((u64)m_Size.y); }

		void DebugPrint() const;

		template<typename T>
		bool CheckStorageType() const
		{
			switch (m_Format)
			{
			case EImageFormat::Bytes:  return std::is_same_v(T, u8);
			case EImageFormat::Floats: return std::is_same_v(T, f32);
			default: return false;
			}
		}

	private:
		static u32 s_IdCounter;
		u32 m_Id = 0;

		static bool s_YFlip;
		static bool s_PrintWhenLoadedFromFile;
		uvec2 m_Size = { 0, 0 };
		u32 m_NumChannels = 0;
		void* m_Data = nullptr;
		bool m_YFlippedWhenLoaded;
		bool m_OwnsMemory = false;
		EImageFormat m_Format = EImageFormat::Bytes;

	public:

		//Help from: https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
		template<typename T>
		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type	= std::ptrdiff_t;
			using value_type		= T;
			using pointer			= T*;
			using reference			= T&;

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

		template<typename T>
		struct IteratorGenerator
		{
			IteratorGenerator(void* data, u32 width, u32 height, bool flip) : m_Data((T*)data), m_Width(width), m_Height(height), m_Flip(flip) {}

			using Iter = Iterator<T>;

			Iter begin()
			{ 
				if (m_Flip)
				{
					return Iter(&m_Data[m_Width * (m_Height - 1)], m_Width, m_Height, m_Flip);
				}
				else
				{
					return Iter(&m_Data[0], m_Width, m_Height, m_Flip);
				}
			}
			Iter end()
			{ 
				return Iter(&m_Data[m_Width * m_Height], m_Width, m_Height, m_Flip);
			}
		private:
			T* m_Data;
			u32 m_Width;
			u32 m_Height;
			bool m_Flip;
		};

		template<typename T>
		IteratorGenerator<T> GetIter() const			{ return IteratorGenerator<T>(m_Data, m_Size.x, m_Size.y, m_YFlippedWhenLoaded); }
		template<typename T>
		IteratorGenerator<T> GetIterDontUnFlip() const	{ return IteratorGenerator<T>(m_Data, m_Size.x, m_Size.y, false); }
		template<typename T>
		IteratorGenerator<T> GetIterFlip() const		{ return IteratorGenerator<T>(m_Data, m_Size.x, m_Size.y, true); }

		template<typename T>
		IteratorGenerator<T> GetIterPerChannel() const			  { return IteratorGenerator<T>(m_Data, m_Size.x * m_NumChannels, m_Size.y, m_YFlippedWhenLoaded); }
		template<typename T>
		IteratorGenerator<T> GetIterPerChannelDontUnFlip() const  { return IteratorGenerator<T>(m_Data, m_Size.x * m_NumChannels, m_Size.y, false); }
		template<typename T>
		IteratorGenerator<T> GetIterPerChannelFlip() const		  { return IteratorGenerator<T>(m_Data, m_Size.x * m_NumChannels, m_Size.y, true); }

		template<typename T>
		struct Types {};
		template<> struct Types<u8>
		{
			using GREY = u8;
			using RG = u8color2;
			using RGB = u8color3;
			using RGBA = u8color4;
			static constexpr u8 MIN = 0;
			static constexpr u8 MAX = 255;
		};
		template<> struct Types<f32>
		{
			using GREY = f32;
			using RG = fcolor2;
			using RGB = fcolor3;
			using RGBA = fcolor4;
			static constexpr f32 MIN = 0.f;
			static constexpr f32 MAX = 1.f;
		};








		template<typename ... TemplateParam, typename Lambda, typename ... FunctionParam>
		decltype(auto) InvokeForFormat(Lambda&& lambda, FunctionParam && ... functionParam) const
		{
			switch (m_Format)
			{
			case EImageFormat::Bytes:  return std::forward<Lambda>(lambda).template operator()<u8, TemplateParam...>(std::forward<FunctionParam>(functionParam)...);
			case EImageFormat::Floats: return std::forward<Lambda>(lambda).template operator()<f32, TemplateParam...>(std::forward<FunctionParam>(functionParam)...);
			default: throw; //TODO: Find a non-throw alternative
			}
		}
		//Expects first param to be: const Image*
		template<typename ... TemplateParam, typename Lambda, typename ... FunctionParam>
		decltype(auto) InvokeForFormatThis(Lambda&& lambda, FunctionParam && ... functionParam) const
		{
			switch (m_Format)
			{
			case EImageFormat::Bytes:  return std::forward<Lambda>(lambda).template operator()<u8, TemplateParam...>(this, std::forward<FunctionParam>(functionParam)...);
			case EImageFormat::Floats: return std::forward<Lambda>(lambda).template operator()<f32, TemplateParam...>(this, std::forward<FunctionParam>(functionParam)...);
			default: throw; //TODO: Find a non-throw alternative
			}
		}
		//Expects first param to be: Image* or const Image*
		template<typename ... TemplateParam, typename Lambda, typename ... FunctionParam>
		decltype(auto) InvokeForFormatThis(Lambda&& lambda, FunctionParam && ... functionParam) /*mutable*/
		{
			switch (m_Format)
			{
			case EImageFormat::Bytes:  return std::forward<Lambda>(lambda).template operator()<u8, TemplateParam...>(this, std::forward<FunctionParam>(functionParam)...);
			case EImageFormat::Floats: return std::forward<Lambda>(lambda).template operator()<f32, TemplateParam...>(this, std::forward<FunctionParam>(functionParam)...);
			default: throw; //TODO: Find a non-throw alternative
			}
		}
	};
}