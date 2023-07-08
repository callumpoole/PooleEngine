#pragma once

#include "collider_2D.h"

namespace Poole
{
	class CircleCollider2D : public Collider2D
	{
	public:
		CircleCollider2D(fvec2 position = { 0, 0 }, f32 radius = 1);

		void SetPosition(fvec2 position) { m_Position = position; }
		void SetRadius(f32 radius) { m_Radius = radius; }
		void SetValues(fvec2 position, f32 radius) { m_Position = position; m_Radius = radius; }
		fvec2 GetPosition() const { return m_Position; }
		f32 GetRadius() const { return m_Radius; }
	private:
		fvec2 m_Position;
		f32 m_Radius;
	public:
		virtual void DebugDraw() override;
	};
}