#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{

	class RenderText;
	class RenderFont;
	
	class RenderTextFactory
	{
	public:
		static void Init();

		static std::shared_ptr<RenderText> MakeRenderText();
	
		//TODO: Make private with getter
		static std::shared_ptr<RenderFont> DefaultFont;
	private:
		
	};

}
