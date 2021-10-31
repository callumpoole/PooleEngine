#include "poole/input/input.h"
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
			std::cout << "1\n";
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

	/*static*/ ivec2 Input::GetMousePosition(ECursorClamping clamping)
	{
		switch (clamping)
		{
		case ECursorClamping::None: 
			return m_LastMousePos;
		case ECursorClamping::Clamp:
			const ivec2 windowSize = (ivec2)Window::GetWindowSize();
			return { std::clamp(m_LastMousePos.x, 0, windowSize.x),
					 std::clamp(m_LastMousePos.y, 0, windowSize.y) };
		case ECursorClamping::LastValidPosition:
			return m_LastMousePosInWindow;
		default:
			std::cerr << "Unhandled ECursorClamping: " << (u8)clamping << "\n";
			return m_LastMousePos;
		}
	}

	/*static*/ uvec2 Input::GetMousePositionUnsigned(ECursorClamping clamping)
	{
		const ivec2 i2 = GetMousePosition(clamping);
		return { i2.x < 0 ? 0u : i2.x, i2.y < 0 ? 0u : i2.y };
	}

	/*static*/ fvec2 Input::GetMousePositionFloat(ECursorClamping clamping, ECursorNormalization norm)
	{
		const ivec2 i2 = GetMousePosition(clamping);
		const fvec2 abs = { (float)i2.x, (float)i2.y };

		if (norm == ECursorNormalization::Absolute)
		{
			return abs;
		}
		else if (norm == ECursorNormalization::ZeroToOne || norm == ECursorNormalization::NegativeOneToOne)
		{
			const uvec2 windowSizeUnsigned = Window::GetWindowSize();
			const fvec2 windowSize = { (float)windowSizeUnsigned.x, (float)windowSizeUnsigned.y };

			if (norm == ECursorNormalization::ZeroToOne)
			{
				return abs / windowSize;
			}
			else //ECursorNormalizedMode::NegativeOneToOne
			{
				return ((abs / windowSize) - fvec2(0.5f)) * fvec2(2.f);
			}
		}
		else
		{
			std::cerr << "Unhandled CursorNormalizedMode: " << (u8)norm << "\n";
			return abs;
		}
	}
}

