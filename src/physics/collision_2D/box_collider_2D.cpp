#include "poole/physics/collision_2D/box_collider_2D.h"

#include "poole/rendering/renderer2D.h"

namespace Poole
{
	BoxCollider2D::BoxCollider2D(fvec2 Position, fvec2 Size)
		: m_Position(Position), m_Size(Size)
	{
	}

	void BoxCollider2D::DebugDraw()
	{
		Rendering::Renderer2D::DrawQuad(m_Position, m_Size, colliding ? Colors::Red<fcolor4> : Colors::Green<fcolor4>);
	}

	fvec2 BoxCollider2D::GetMin() const
	{
		return m_Position - m_Size / 2.f;
	}
	fvec2 BoxCollider2D::GetMax() const
	{
		return m_Position + m_Size / 2.f;
	}
}
