
#pragma once

#include "poole/core.h"

struct GLFWwindow;

namespace Poole
{
	enum class ECursorClamping : u8
	{
		None,
		Clamp,
		LastValidPosition,
	};

	enum class ECursorNormalization : u8
	{
		Absolute = 0,
		ZeroToOne = 1,
		NegativeOneToOne = 2,

		FLAG_Aspect = 4, //Scaled for aspect ratio
		FLAG_Camera = 8, //Move relative to camera
		ALL_FLAGS = FLAG_Aspect | FLAG_Camera,

		ZeroToOneAspect		         = ZeroToOne		| FLAG_Aspect,
		NegativeOneToOneAspect       = NegativeOneToOne | FLAG_Aspect,
		CameraNegativeOneToOneAspect = NegativeOneToOne | FLAG_Aspect | FLAG_Camera,
	};
	ENUM_FLAGS(ECursorNormalization)

	
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
	private:
		static constexpr bool AllowCameraMovement = true;
		static constexpr bool AllowCameraZooming = true;
		static void MoveCamera(GLFWwindow* window);
		static void ZoomCamera(GLFWwindow* window);

		static ivec2 m_LastMousePos;
		static ivec2 m_LastMousePosInWindow;
		static fvec2 m_LastScrollDelta;

		friend void ProcessScrollEvent(GLFWwindow* window, double xOffset, double yOffset);
	};
}

