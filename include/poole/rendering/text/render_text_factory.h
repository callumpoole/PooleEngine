#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{

	class RenderFont;
	
	class RenderTextFactory
	{
	public:
		static void Init();
	
		//TODO: Make private with getter
		static std::unique_ptr<RenderFont> DefaultFont;
	private:
		
	};

}
