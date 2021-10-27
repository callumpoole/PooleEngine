#pragma once

#include "core.h"

namespace Poole::Rendering
{
	class RendererAPI
	{
	public:
		virtual ~RendererAPI() {}
		static RendererAPI* Get();
	};
}
