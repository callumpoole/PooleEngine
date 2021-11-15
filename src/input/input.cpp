#include "poole/input/input.h"
#include "rendering/renderer.h"
#include "rendering/camera/orthographic_camera.h"
#include <iostream>
#include <window/window.h>

namespace Poole
{
	ivec2 Input::m_LastMousePos{ 0, 0 };
	ivec2 Input::m_LastMousePosInWindow{ 0, 0 };

	void TestProcessInputEvent(GLFWwindow* /*window*/, i32 key, i32 /*scancode*/, i32 action, i32 /*mods*/)
	{
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			LOG_LINE("1");
		}
	}

	/*static*/ void Input::Init(GLFWwindow* window)
	{
		//Listen to input events
		glfwSetKeyCallback(window, TestProcessInputEvent);
	}

	/*static*/ void Input::Tick(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		MoveCamera(window);

		//Cache the cursor position this frame
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_LastMousePos = { floor(xpos), floor(ypos) };
		const ivec2 windowSize = (ivec2)Window::GetWindowSize();
		if (m_LastMousePos.x >= 0 && m_LastMousePos.x <= windowSize.x &&
			m_LastMousePos.y >= 0 && m_LastMousePos.y <= windowSize.y)
		{
			m_LastMousePosInWindow = m_LastMousePos;
		}
	}

	void Input::MoveCamera(GLFWwindow* window)
	{
		constexpr size_t numDirs = 4;
		constexpr std::array<u16, numDirs> keyCodes = { GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_DOWN, GLFW_KEY_UP };
		constexpr f32 speed = 0.002f;
		constexpr std::array<fvec3, numDirs> keyDirs = { fvec3(-speed,    0.f, 0.f),
														 fvec3( speed,    0.f, 0.f),
														 fvec3(   0.f, -speed, 0.f),
														 fvec3(   0.f,  speed, 0.f) };

		for (size_t i = 0; i < numDirs; i++)
		{
			if (glfwGetKey(window, keyCodes[i]) == GLFW_PRESS)
			{
				Rendering::OrthographicCamera& Camera = Rendering::Renderer::GetCamera();
				const fvec3 pos = Camera.GetPosition();
				Camera.SetPosition(pos + keyDirs[i]); //todo: * deltaTime
			}
		}

		//Reset camera position
		if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS)
		{
			Rendering::OrthographicCamera& Camera = Rendering::Renderer::GetCamera();
			Camera.SetPosition(fvec3(0.f));
		}
	}

	/*static*/ ivec2 Input::GetMousePosition(bool invertY, ECursorClamping clamping)
	{
		const ivec2 windowSize = (ivec2)Window::GetWindowSize();

		auto MaybeInvert = [invertY, windowSize](const ivec2& in) -> ivec2
		{
			return invertY ? ivec2{ in.x, windowSize.y - in.y } : in;
		};

		switch (clamping)
		{
		case ECursorClamping::Clamp:
			return MaybeInvert({ std::clamp(m_LastMousePos.x, 0, windowSize.x),
								 std::clamp(m_LastMousePos.y, 0, windowSize.y) });
		case ECursorClamping::LastValidPosition:
			return MaybeInvert(m_LastMousePosInWindow);
		default:
			LOG_ERROR("Unhandled ECursorClamping: {}", (u8)clamping);
			[[fallthrough]];
		case ECursorClamping::None: 
			return MaybeInvert(m_LastMousePos);
		}
	}

	/*static*/ uvec2 Input::GetMousePositionUnsigned(bool invertY, ECursorClamping clamping)
	{
		const ivec2 i2 = GetMousePosition(invertY, clamping);
		return { i2.x < 0 ? 0u : i2.x, i2.y < 0 ? 0u : i2.y };
	}

	/*static*/ fvec2 Input::GetMousePositionFloat(bool invertY, ECursorClamping clamping, ECursorNormalization norm)
	{
		const ivec2 i2 = GetMousePosition(invertY, clamping);
		const fvec2 abs = { (float)i2.x, (float)i2.y };

		if (norm == ECursorNormalization::Absolute)
		{
			return abs;
		}

		const uvec2 windowSizeUnsigned = Window::GetWindowSize();
		const fvec2 windowSize = { (float)windowSizeUnsigned.x, (float)windowSizeUnsigned.y };

		fvec2 out;
		if ((norm & ECursorNormalization::ZeroToOne) > 0)
		{
			out = abs / windowSize;
		}
		else if ((norm & ECursorNormalization::NegativeOneToOne) > 0)
		{
			out = ((abs / windowSize) - fvec2(0.5f)) * fvec2(2.f);
		}
		else
		{
			LOG_ERROR("Unhandled CursorNormalizedMode: {}", (u8)norm);
			return abs;
		}

		if ((norm & ECursorNormalization::FLAG_Aspect) > 0)
		{
			out.x *= windowSize.x / windowSize.y;
		}
		if ((norm & ECursorNormalization::FLAG_Camera) > 0)
		{
			Rendering::OrthographicCamera& Camera = Rendering::Renderer::GetCamera();
			const fvec3 cameraPos = Camera.GetPosition();
			out.x += cameraPos.x;
			out.y += cameraPos.y;
		}
		return out;
	}
}

