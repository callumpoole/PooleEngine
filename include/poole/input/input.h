
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

		static ivec2 GetMousePosition(bool invertY = false, ECursorClamping clamping = ECursorClamping::Clamp);
		static ivec2 GetMousePosition(ECursorClamping clamping = ECursorClamping::Clamp) { return GetMousePosition(false, clamping); }
		static uvec2 GetMousePositionUnsigned(bool invertY = false, ECursorClamping clamping = ECursorClamping::Clamp);
		static uvec2 GetMousePositionUnsigned(ECursorClamping clamping = ECursorClamping::Clamp) { return GetMousePositionUnsigned(false, clamping); }
		static fvec2 GetMousePositionFloat(bool invertY = false, ECursorClamping clamping = ECursorClamping::Clamp, ECursorNormalization norm = ECursorNormalization::Absolute);
		static fvec2 GetMousePositionFloat(ECursorClamping clamping = ECursorClamping::Clamp, ECursorNormalization norm = ECursorNormalization::Absolute) { return GetMousePositionFloat(false, clamping, norm); }

		static f32 GetMouseScrollDelta() { return m_LastScrollDelta.y; }
		static fvec2 GetMouseScrollDelta2D() { return m_LastScrollDelta; }

		static bool GetKey(EInputKey key, EInputPress press);
		static bool GetKeyDown(EInputKey key) { return GetKey(key, EInputPress::PRESS); }
		static bool GetKeyUp(EInputKey key) { return GetKey(key, EInputPress::RELEASE); }
		static bool GetKeyRepeat(EInputKey key) { return GetKey(key, EInputPress::REPEAT); }
	private:
		static constexpr bool AllowCameraMovement = true;
		static constexpr bool AllowCameraZooming = true;
		static void MoveCamera();
		static void ZoomCamera();

		static ivec2 m_LastMousePos;
		static ivec2 m_LastMousePosInWindow;

		static fvec2 m_LastScrollDelta;
		static u64 m_KeepLastScrollDataTickID;

		friend void ProcessScrollEvent(GLFWwindow* window, double xOffset, double yOffset);
	};
}

