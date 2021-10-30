#pragma once

#include "core.h"

namespace Poole::Rendering
{
	class RendererAPI
	{
	public:
		virtual ~RendererAPI() {}
		static RendererAPI* GetOrCreate();

		virtual void DrawIndexed(u32 count) = 0;
	};

	#define GetRendererAPI() RendererAPI::GetOrCreate()
}
