#pragma once

#include "collider_2D.h"

namespace Poole
{
	class BoxCollider2D : public Collider2D
	{
	public:
		BoxCollider2D(fvec2 Position = {}, fvec2 Size = {1,1});

		fvec2 m_Position;
		fvec2 m_Size;

		fvec2 GetMin() const;
		fvec2 GetMax() const;

		virtual void DebugDraw() override;
	};
}
