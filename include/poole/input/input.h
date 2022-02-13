
#pragma once

#include "poole/core.h"
#include "input_types.h"

struct GLFWwindow;

namespace Poole
{
	/*
		TODO: 
		 - Make a generic enum for mappings for the GLFW ones, with just a GetKey() instead of: 
		   glfwGetKey, glfwGetMouseButton, etc. AND supply a callback method.
		 - Make a generic read axis for delta-mouse, delta-scroll, joysticks & delta-joysticks to be read each tick AND supply callback method.
	*/

	class Input
	{
	public:
		static void Init(GLFWwindow* window);
		static void Tick(GLFWwindow* window);
	private:
		static void TickMouse(GLFWwindow* window);

	public:
		static ivec2 GetMousePosition(bool invertY = false, ECursorClamping clamping = ECursorClamping::Clamp);
		static uvec2 GetMousePositionUnsigned(bool invertY = false, ECursorClamping clamping = ECursorClamping::Clamp);
		static fvec2 GetMousePositionFloat(bool invertY = false, ECursorClamping clamping = ECursorClamping::Clamp, ECursorNormalization norm = ECursorNormalization::Absolute);
		static ivec2 GetMouseDelta(bool invertY = false, bool withinScreen = true);
		static fvec2 GetMouseDeltaFloat(bool invertY = false, bool withinScreen = true, ECursorNormalization norm = ECursorNormalization::Absolute);
	private:
		static fvec2 GetMouseFloatLogic(ivec2 in, ECursorNormalization norm);
	public:
		static f32 GetMouseScrollDelta() { return m_LastScrollDelta.y; }
		static fvec2 GetMouseScrollDelta2D() { return m_LastScrollDelta; }


		static bool IsButton(EInputButton button, EInputPress press);
		static bool IsButtonDown(EInputButton button)   { return IsButton(button, EInputPress::PRESS); }
		static bool IsButtonUp(EInputButton button)		{ return IsButton(button, EInputPress::RELEASE); }
		static bool IsButtonRepeat(EInputButton button) { return IsButton(button, EInputPress::REPEAT); }

		static float GetAxisRaw(EInputAxis axis);
	private:
		static constexpr bool AllowCameraMovement = true;
		static constexpr bool AllowCameraZooming = true;
		static void MoveCamera();
		static void ZoomCamera();

		static ivec2 m_DeltaMousePos;
		static ivec2 m_DeltaMousePosInWindow;
		static ivec2 m_LastMousePos;
		static ivec2 m_LastMousePosInWindow;

		static fvec2 m_LastScrollDelta;
		static u64 m_KeepLastScrollDataTickID;

		friend void ProcessScrollEvent(GLFWwindow* window, double xOffset, double yOffset);
	};
}

