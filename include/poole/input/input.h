
#pragma once

#include "poole/core.h"
#include "input_types.h"

struct GLFWwindow;

namespace Poole
{
	class Input
	{
	public:
		static void Init(GLFWwindow* window);
		static void Tick(GLFWwindow* window);



		/* Mouse */
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
		friend void ProcessScrollEvent(GLFWwindow* window, double xOffset, double yOffset);
	public:
		static f32 GetMouseScrollDelta() { return m_LastScrollDelta.y; }
		static fvec2 GetMouseScrollDelta2D() { return m_LastScrollDelta; }
	private:
		static ivec2 m_DeltaMousePos;
		static ivec2 m_DeltaMousePosInWindow;
		static ivec2 m_LastMousePos;
		static ivec2 m_LastMousePosInWindow;
		static fvec2 m_LastScrollDelta;
		static u64 m_KeepLastScrollDataTickID;



		/* Buttons & Axes */
		static bool IsButton(EInputButton button, EInputPress press);
		static bool IsButtonDown(EInputButton button)   { return IsButton(button, EInputPress::PRESS); }
		static bool IsButtonUp(EInputButton button)		{ return IsButton(button, EInputPress::RELEASE); }
		static bool IsButtonRepeat(EInputButton button) { return IsButton(button, EInputPress::REPEAT); }

		static f32 GetAxisRaw(EInputAxis axis);



		/* Joysticks / Gamepad */
		static EJoystickType GetJoystickType(EJoystickID ID);
		static bool IsAnyJoystickTypePresent(EJoystickType type);
		static bool IsGamepadConnected() { return IsAnyJoystickTypePresent(EJoystickType::Gamepad); }
		static const char* GetJoystickGUIDRaw(EJoystickID ID); //Will return nullptr if disconnected
		static const char* GetJoystickNameRaw(EJoystickID ID); //Will return nullptr if disconnected
		static std::string GetJoystickGUID(EJoystickID ID); //Will return empty string if disconnected
		static std::string GetJoystickName(EJoystickID ID); //Will return empty string if disconnected

	private:
		//CAMERA STUFF IS TEMP
		static constexpr bool AllowCameraMovement = true;
		static constexpr bool AllowCameraZooming = true;
		static void MoveCamera();
		static void ZoomCamera();
	};
}

