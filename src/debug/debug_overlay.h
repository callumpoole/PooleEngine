#pragma once

#include "poole/core.h"

namespace Poole { class Engine; }

namespace Poole::Rendering { class TextRenderer; }

namespace Poole::Debug 
{
	class DebugOverlay
	{
	public:
		DebugOverlay();
		~DebugOverlay();
		void Update(const Poole::Engine& engine);

		std::shared_ptr<Poole::Rendering::TextRenderer> m_DebugText;
	};
}

