#pragma once

#include "collider_2D.h"

namespace Poole
{
	class BoxCollider2D : public Collider2D
	{
	public:
		BoxCollider2D(fvec2 Position = {0, 0}, fvec2 Size = {1, 1}, f32 Radians = 0);

		struct Corners
		{
			fvec2 LL; //Lower Left
			fvec2 LR; //Lower Right
			fvec2 UL; //Upper Left
			fvec2 UR; //Upper Right
		};
		
		void SetPosition(fvec2 Position);
		void SetSize(fvec2 Size);
		void SetRadians(f32 Radians);
		void SetValues(fvec2 Position, fvec2 Size, f32 Radians);
		fvec2 GetPosition() const { return m_Position; }
		fvec2 GetSize() const { return m_Size; }
		f32 GetRadians() const { return m_Radians; }
		const Corners& GetCorners() const { return m_CachedCorners; }
		inline fmat2 CalculateRotationMatrix() const
		{
			const f32 s = sin(m_Radians);
			const f32 c = cos(m_Radians);
			return fmat2{ c, -s, s, c };
		}

	private:
		fvec2 m_Position;
		fvec2 m_Size;
		f32 m_Radians;
		Corners m_CachedCorners;

		void CacheCorners();
		Corners CalculateCorners() const;
		Corners CalculateCornersUnrotated() const;
	public:
		inline std::array<fvec2, 2> CalculateMinMax() const
		{
			if (m_Radians == 0.0f)
				return GetMinMaxUnrotated();

			const fmat2 mat = CalculateRotationMatrix();
			return {
				(fvec2{-m_Size.x, -m_Size.y} / 2.f) * mat + m_Position,
				(fvec2{ m_Size.x,  m_Size.y} / 2.f) * mat + m_Position
			};
		}
		inline std::array<fvec2, 2> GetMinMaxUnrotated() const
		{
			return { m_CachedCorners.LL, m_CachedCorners.UR };
		}

		virtual void DebugDraw() override;
	};
}
