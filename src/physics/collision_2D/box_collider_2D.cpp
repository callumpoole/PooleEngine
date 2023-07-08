#include "poole/physics/collision_2D/box_collider_2D.h"

#include "poole/rendering/renderer2D.h"

namespace Poole
{
	BoxCollider2D::BoxCollider2D(fvec2 position, fvec2 size, f32 radians)
		: m_Position(position), m_Size(size), m_Radians(Poole::Math::NormalizeRadiansNegPiAndPi(radians))
	{
		CacheCorners();
	}

	void BoxCollider2D::SetPosition(fvec2 position)
	{ 
		if (m_Position != position)
		{
			m_Position = position; 
			CacheCorners();
		}
	}
	void BoxCollider2D::SetSize(fvec2 size) 
	{
		if (m_Size != size)
		{
			m_Size = size; 
			CacheCorners(); 
		}
	}
	void BoxCollider2D::SetRadians(f32 radians) 
	{
		if (m_Radians != radians)
		{
			m_Radians = Poole::Math::NormalizeRadiansNegPiAndPi(radians);
			CacheCorners();
		}
	}
	void BoxCollider2D::SetValues(fvec2 position, fvec2 size, f32 radians)
	{
		if (m_Position != position || m_Size != size || m_Radians != radians)
		{
			m_Position = position; 
			m_Size = size; 
			m_Radians = Poole::Math::NormalizeRadiansNegPiAndPi(radians);
			CacheCorners();
		}
	}
	void BoxCollider2D::CacheCorners()
	{
		m_CachedCorners = CalculateCorners();
	}

	auto BoxCollider2D::CalculateCorners() const -> Corners
	{
		if (m_Radians == 0.0f)
			return CalculateCornersUnrotated();

		Corners out;
		const fmat2 mat = CalculateRotationMatrix();
		out.BL = (fvec2{ -m_Size.x, -m_Size.y } / 2.f) * mat + m_Position;
		out.BR = (fvec2{  m_Size.x, -m_Size.y } / 2.f) * mat + m_Position;
		out.TR = (fvec2{  m_Size.x,  m_Size.y } / 2.f) * mat + m_Position;
		out.TL = (fvec2{ -m_Size.x,  m_Size.y } / 2.f) * mat + m_Position;

		//Makes sure the names of the corners get swapped when the shape is rotated, probably not needed ever.
		//const float absRads = abs(m_Radians);
		//if (absRads > PI * 3 / 4.f)
		//{
		//	swap(out.BL, out.TR);
		//	swap(out.TL, out.BR);
		//}
		//else if (absRads > PI / 4)
		//{
		//	if (m_Radians > 0.f) 
		//	{
		//		const fvec2 temp = out.TL;
		//		out.TL = out.TR;
		//		out.TR = out.BR;
		//		out.BR = out.BL;
		//		out.BL = temp;
		//	}
		//	else
		//	{
		//		const fvec2 temp = out.BL;
		//		out.BL = out.BR;
		//		out.BR = out.TR;
		//		out.TR = out.TL;
		//		out.TL = temp;
		//	}
		//}

		return out;
	}
	auto BoxCollider2D::CalculateCornersUnrotated() const -> Corners
	{
		Corners out;
		out.BL = (fvec2{ -m_Size.x, -m_Size.y } / 2.f) + m_Position;
		out.BR = (fvec2{  m_Size.x, -m_Size.y } / 2.f) + m_Position;
		out.TL = (fvec2{ -m_Size.x,  m_Size.y } / 2.f) + m_Position;
		out.TR = (fvec2{  m_Size.x,  m_Size.y } / 2.f) + m_Position;
		return out;
	}

	void BoxCollider2D::DebugDraw()
	{
		Rendering::Renderer2D::DrawQuad(m_Position, m_Size, m_Colliding ? Colors::Red<fcolor4> : Colors::Green<fcolor4>, m_Radians);

		//const Corners c = GetCorners();
		//
		//Rendering::Renderer2D::DrawQuad(c.BL,  fvec2{ 0.05f, 0.05f }, Colors::Blue<fcolor4>, m_Radians);
		//Rendering::Renderer2D::DrawQuad(c.BR, fvec2{ 0.05f, 0.05f }, Colors::Pink<fcolor4>, m_Radians);
		//Rendering::Renderer2D::DrawQuad(c.TL,  fvec2{ 0.05f, 0.05f }, Colors::Purple<fcolor4>, m_Radians);
		//Rendering::Renderer2D::DrawQuad(c.TR, fvec2{ 0.05f, 0.05f }, Colors::Cyan<fcolor4>, m_Radians);
	}
}
