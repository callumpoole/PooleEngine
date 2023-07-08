#include "poole/physics/collision_2D/box_collider_2D.h"

#include "poole/rendering/renderer2D.h"

namespace Poole
{
	BoxCollider2D::BoxCollider2D(fvec2 Position, fvec2 Size, f32 Radians)
		: m_Position(Position), m_Size(Size), m_Radians(Poole::Math::NormalizeRadiansNegPiAndPi(Radians))
	{
		CacheCorners();
	}

	void BoxCollider2D::SetPosition(fvec2 Position)
	{ 
		if (m_Position != Position)
		{
			m_Position = Position; 
			CacheCorners();
		}
	}
	void BoxCollider2D::SetSize(fvec2 Size) 
	{
		if (m_Size != Size)
		{
			m_Size = Size; 
			CacheCorners(); 
		}
	}
	void BoxCollider2D::SetRadians(f32 Radians) 
	{
		if (m_Radians != Radians)
		{
			m_Radians = Poole::Math::NormalizeRadiansNegPiAndPi(Radians);
			CacheCorners();
		}
	}
	void BoxCollider2D::SetValues(fvec2 Position, fvec2 Size, f32 Radians)
	{
		if (m_Position != Position || m_Size != Size || m_Radians != Radians)
		{
			m_Position = Position; 
			m_Size = Size; 
			m_Radians = Poole::Math::NormalizeRadiansNegPiAndPi(Radians);
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
		out.LL = (fvec2{ -m_Size.x, -m_Size.y } / 2.f) * mat + m_Position;
		out.LR = (fvec2{  m_Size.x, -m_Size.y } / 2.f) * mat + m_Position;
		out.UR = (fvec2{  m_Size.x,  m_Size.y } / 2.f) * mat + m_Position;
		out.UL = (fvec2{ -m_Size.x,  m_Size.y } / 2.f) * mat + m_Position;

		//Makes sure the names of the corners get swapped when the shape is rotated, probably not needed ever.
		//const float absRads = abs(m_Radians);
		//if (absRads > PI * 3 / 4.f)
		//{
		//	swap(out.LL, out.UR);
		//	swap(out.UL, out.LR);
		//}
		//else if (absRads > PI / 4)
		//{
		//	if (m_Radians > 0.f) 
		//	{
		//		const fvec2 temp = out.UL;
		//		out.UL = out.UR;
		//		out.UR = out.LR;
		//		out.LR = out.LL;
		//		out.LL = temp;
		//	}
		//	else
		//	{
		//		const fvec2 temp = out.LL;
		//		out.LL = out.LR;
		//		out.LR = out.UR;
		//		out.UR = out.UL;
		//		out.UL = temp;
		//	}
		//}

		return out;
	}
	auto BoxCollider2D::CalculateCornersUnrotated() const -> Corners
	{
		Corners out;
		out.LL = (fvec2{ -m_Size.x, -m_Size.y } / 2.f) + m_Position;
		out.LR = (fvec2{  m_Size.x, -m_Size.y } / 2.f) + m_Position;
		out.UL = (fvec2{ -m_Size.x,  m_Size.y } / 2.f) + m_Position;
		out.UR = (fvec2{  m_Size.x,  m_Size.y } / 2.f) + m_Position;
		return out;
	}

	void BoxCollider2D::DebugDraw()
	{
		Rendering::Renderer2D::DrawQuad(m_Position, m_Size, colliding ? Colors::Red<fcolor4> : Colors::Green<fcolor4>, m_Radians);

		//const Corners c = GetCorners();
		//
		//Rendering::Renderer2D::DrawQuad(c.LL,  fvec2{ 0.05f, 0.05f }, Colors::Blue<fcolor4>, m_Radians);
		//Rendering::Renderer2D::DrawQuad(c.LR, fvec2{ 0.05f, 0.05f }, Colors::Pink<fcolor4>, m_Radians);
		//Rendering::Renderer2D::DrawQuad(c.UL,  fvec2{ 0.05f, 0.05f }, Colors::Purple<fcolor4>, m_Radians);
		//Rendering::Renderer2D::DrawQuad(c.UR, fvec2{ 0.05f, 0.05f }, Colors::Cyan<fcolor4>, m_Radians);
	}
}
