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
		const i32 GetColorChannels() const { return m_NumColorChannels; }
	
	private:
		static bool s_YFlip;
		ivec2 m_Size = {-1,-1};
		i32 m_NumColorChannels = -1;
		u8* m_Bytes;
	};
}