#pragma once

#include "collider_2D.h"

namespace Poole
{
	class BoxCollider2D : public Collider2D
	{
	public:
		BoxCollider2D(fvec2 position = {0, 0}, fvec2 halfSize = {1, 1}, f32 radians = 0);

		struct Corners
		{
			union {
				struct {
					fvec2 BL; //Bottom Left
					fvec2 BR; //Bottom Right
					fvec2 TL; //Top Left
					fvec2 TR; //Top Right
				};
				std::array<fvec2, 4> corners;
			};
		};
		
		void SetPosition(fvec2 position);
		void SetHalfSize(fvec2 halfSize);
		void SetRadians(f32 radians);
		void SetValues(fvec2 position, fvec2 halfSize, f32 radians);
		fvec2 GetPosition() const { return m_Position; }
		fvec2 GetHalfSize() const { return m_HalfSize; }
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
		fvec2 m_HalfSize;
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
				fvec2{-m_HalfSize.x, -m_HalfSize.y} * mat + m_Position,
				fvec2{ m_HalfSize.x,  m_HalfSize.y} * mat + m_Position
			};
		}
		inline std::array<fvec2, 2> GetMinMaxUnrotated() const
		{
			return { m_CachedCorners.BL, m_CachedCorners.TR };
		}

		virtual void DebugDraw() override;
	};
}
