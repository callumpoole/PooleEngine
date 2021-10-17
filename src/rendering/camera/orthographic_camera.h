

#pragma once

#include "core.h"

namespace Poole
{
	//Mostly copied from: https://youtu.be/NjKv-HWstxA

	class OrthographicCamera
	{
	public:
		struct Bounds
		{
			f32 left;
			f32 right;
			f32 bottom;
			f32 top;
		};

		OrthographicCamera(const Bounds& bounds);
		OrthographicCamera(float left, float right, float bottom, float top);

		const Bounds& GetBounds() const { return m_bounds; }
		void SetBounds(const Bounds& bounds);
		void SetBounds(float left, float right, float bottom, float top);

		const fvec3& GetPosition() const { return m_position; }
		void SetPosition(const fvec3& position) { m_position = position; RecalculateViewMatrix(); }

		const f32& GetRotation() const { return m_rotation; }
		void SetPosition(const f32 rotation) { m_rotation = rotation; RecalculateViewMatrix(); }

		const fmat4& GetViewMatrix() const { return m_viewMatrix; }
		const fmat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		const fmat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();
	private:
		fmat4 m_viewMatrix;
		fmat4 m_projectionMatrix;
		fmat4 m_viewProjectionMatrix;

		Bounds m_bounds;
		fvec3 m_position = {};
		f32 m_rotation = 0.f;
	};
}