#include "poole/physics/collision_2D/circle_collider_2D.h"

#include "poole/rendering/renderer2D.h"

namespace Poole
{
	CircleCollider2D::CircleCollider2D(fvec2 position, f32 radius)
		: m_Position(position), m_Radius(radius)
	{ }

	void CircleCollider2D::DebugDraw()
	{
		Rendering::Renderer2D::DrawCircle(m_Position, fvec2{ m_Radius, m_Radius } * 2.f, m_Colliding ? Colors::Red<fcolor4> : Colors::Green<fcolor4>);
	}
}