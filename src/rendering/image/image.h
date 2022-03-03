#pragma once

#include "poole/core.h"

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
		const ivec2& GetSize() const { return m_Size; }
		i32 GetColorChannels() const { return m_NumColorChannels; }
		bool WasYFlippedWhenLoaded() const { return m_YFlippedWhenLoaded; }

		bool IsPowerOfTwo() const { return Math::IsPowerOfTwo((u64)m_Size.x) && Math::IsPowerOfTwo((u64)m_Size.y); }


		void DebugPrint() const;
	private:
		static bool s_YFlip;
		ivec2 m_Size = {-1,-1};
		i32 m_NumColorChannels = -1;
		u8* m_Bytes;
		bool m_YFlippedWhenLoaded;

		static constexpr u8 COLOR_DEPTH = 8;
	};
}