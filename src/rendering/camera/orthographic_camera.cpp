
#include "poole/rendering/camera/orthographic_camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "../../window/window.h"
#include "poole/rendering/renderer.h"

namespace Poole::Rendering
{
	OrthographicCamera::OrthographicCamera() : OrthographicCamera({ -1.f, 1.f, -1.f, 1.f }) {}

	OrthographicCamera::OrthographicCamera(const Bounds& bounds)
		: m_viewMatrix(1.f)
	{
		SetBounds(bounds);
	}

	OrthographicCamera::OrthographicCamera(f32 left, f32 right, f32 bottom, f32 top)
		: m_viewMatrix(1.f)
	{
		SetBounds(left, right, bottom, top);
	}

	static void OnResize_OrthographicCamera(i32 width, i32 height)
	{
		Renderer::GetCamera().UpdateAdaptiveCamera(width, height);
	}

	void OrthographicCamera::UpdateAdaptiveCamera(i32 width, i32 height)
	{
		std::optional<f32> optScale = GetZoomScale();
		if (optScale)
		{
			const f32 scale = optScale.value();
			const f32 scaledAspectRatio = scale * (width / (f32)height);
			SetBounds(-scaledAspectRatio, scaledAspectRatio, -scale, scale);
		}
	}

	void OrthographicCamera::EnableResizeWithWindow()
	{
		//No way to dynamically reset right now...
		static bool bSet = false;
		if (!bSet)
		{
			Window::m_OnResize.emplace_back(&OnResize_OrthographicCamera);
			bSet = true;
		}
	}

	void OrthographicCamera::SetZoomScale(f32 scale)
	{
		m_AdaptiveCameraScale = scale;
		const uvec2 v = Window::GetWindowSize();
		UpdateAdaptiveCamera(v.x, v.y);
	}

	void OrthographicCamera::SetBounds(const Bounds& bounds)
	{
		m_bounds = bounds;
		m_projectionMatrix = glm::ortho(bounds.left, bounds.right, bounds.bottom, bounds.top, -1.f, 1.f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
	void OrthographicCamera::SetBounds(f32 left, f32 right, f32 bottom, f32 top)
	{
		SetBounds({ left, right, bottom, top });
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		const fmat4 transform = 
			glm::translate(fmat4(1.f), m_position) * glm::rotate(fmat4(1.f), m_rotation, fvec3(0, 0, 1));

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}