#pragma once

#include "poole/core.h"

namespace Poole
{
	class Collider2D
	{
	public:
		Collider2D();
		virtual ~Collider2D();
		virtual void DebugDraw() = 0;

		bool m_Colliding = false;
	};
}
