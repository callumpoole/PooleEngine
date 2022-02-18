#include "poole/input/input.h"
#include "rendering/renderer.h"
#include "rendering/camera/orthographic_camera.h"
#include <iostream>
#include <window/window.h>

namespace Poole
{
	ivec2 Input::m_DeltaMousePos{};
	ivec2 Input::m_DeltaMousePosInWindow{};
	ivec2 Input::m_LastMousePos{};
	ivec2 Input::m_LastMousePosInWindow{};
	fvec2 Input::m_LastScrollDelta{};
	u64 Input::m_KeepLastScrollDataTickID{};

	void ProcessKeyEvent(GLFWwindow* /*window*/, i32 /*key*/, i32 /*scancode*/, i32 action, i32 /*mods*/)
	{
		static int i = 0;
		if (action == GLFW_PRESS)
		{
			LOG("key = {}", i++);
		}
	}

	void ProcessMouseButtonEvent(GLFWwindow* /*window*/, i32 /*mouseButton*/, i32 action, i32 /*mods*/)
	{
		static int i = 0;
		if (action == GLFW_PRESS)
		{
			LOG("mouse = {}", i++);
		}
	}

	void ProcessScrollEvent(GLFWwindow* /*window*/, double xOffset, double yOffset)
	{
		Input::m_LastScrollDelta = { (float)xOffset, (float)yOffset };
		Input::m_KeepLastScrollDataTickID = EngineLogTime::GetTickCount();
		//LOG("Scroll X: {}, Y: {}", xOffset, yOffset);
	}

	/*static*/ void Input::Init(GLFWwindow* window)
	{
		//Listen to input events
		glfwSetKeyCallback(window, ProcessKeyEvent);
		glfwSetMouseButtonCallback(window, ProcessMouseButtonEvent);
		glfwSetScrollCallback(window, ProcessScrollEvent);
	}

	/*static*/ void Input::Tick(GLFWwindow* window)
	{
		//TEMP
		if (IsButtonDown(EInputButton::KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}

		TickMouse(window);

		//LOG("Gamepad: {}, Guid: {}", GetJoystickName(EJoystickID::JOYSTICK_1), GetJoystickGUID(EJoystickID::JOYSTICK_1));

		//TEMP
		if constexpr (AllowCameraMovement)
		{
			MoveCamera();
		}
		if constexpr (AllowCameraZooming)
		{
			ZoomCamera();
		}

		//TO MOVE:
		//Reset Scroll Delta if it wasn't just applied this tick
		if (Input::m_KeepLastScrollDataTickID != EngineLogTime::GetTickCount())
		{
			Input::m_LastScrollDelta = { 0,0 };
		}
	}

	/*static*/ void Input::TickMouse(GLFWwindow* window)
	{
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		const ivec2 intPos = { floor(xPos), floor(yPos) };

		bool isInWindow;
		{
			const ivec2 windowSize = (ivec2)Window::GetWindowSize();
			isInWindow = (m_LastMousePos.x >= 0 && m_LastMousePos.x <= windowSize.x &&
						  m_LastMousePos.y >= 0 && m_LastMousePos.y <= windowSize.y);
		}

		if (EngineLogTime::GetTickCount() != 0)
		{
			m_DeltaMousePos = intPos - m_LastMousePos;

			if (isInWindow)
			{
				m_DeltaMousePosInWindow = m_DeltaMousePos;
			}
		}

		//Cache the cursor position this frame
		m_LastMousePos = intPos;
		if (isInWindow)
		{
			m_LastMousePosInWindow = m_LastMousePos;
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
		return GetMouseFloatLogic(i2, norm);
	}


	/*static*/ ivec2 Input::GetMouseDelta(bool invertY, bool withinScreen)
	{
		auto MaybeInvert = [invertY](const ivec2& in) -> ivec2
		{
			return invertY ? ivec2{ in.x, - in.y } : in;
		};

		return MaybeInvert(withinScreen ? m_DeltaMousePosInWindow : m_DeltaMousePos);
	}
	/*static*/ fvec2 Input::GetMouseDeltaFloat(bool invertY, bool withinScreen, ECursorNormalization norm)
	{
		const ivec2 i2 = GetMouseDelta(invertY, withinScreen);
		return GetMouseFloatLogic(i2, norm);
	}


	/*static*/ fvec2 Input::GetMouseFloatLogic(ivec2 in, ECursorNormalization norm)
	{
		const fvec2 abs = { (float)in.x, (float)in.y };

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


	/*static*/ bool Input::IsButton(EInputButton button, EInputPress press)
	{
		if (button == EInputButton::NONE)
			return false;

		if (button >= EInputButton::KEY_FIRST_PRINTABLE && button <= EInputButton::KEY_LAST_FUNCTIONAL)
		{
			return glfwGetKey(Window::m_WindowInstance, ToGLFWKey(button)) == u8(press);
		}

		if (button >= EInputButton::MOUSE_BUTTON_FIRST && button <= EInputButton::MOUSE_BUTTON_LAST)
		{
			return glfwGetMouseButton(Window::m_WindowInstance, ToGLFWMouseButton(button)) == u8(press);
		}

		if ((button & EInputButton::IS_JOYSTICK_FLAG) > 0)
		{
			ASSERT(press != EInputPress::REPEAT); //Not yet supported

			const u32 joy = ToGLFWJoystick(button);
			if ((button & EInputButton::GAMEPAD_BUTTON_FLAGS) > 0) //If GAMEPAD
			{
				GLFWgamepadstate state;
				if (glfwGetGamepadState(joy, &state))
				{
					const u32 glfw_button = ToGLFWGamepadButton(button);
					return state.buttons[glfw_button] == u8(press);
				}
			}
			else //If HAT (VERY MUCH GUESS WORK, UNTESTED)
			{
				const u32 hat = ToGLFWJoystickHat(button);
				int count;
				const unsigned char* hats = glfwGetJoystickButtons(joy, &count);

				ASSERT(count == 4);

				if (hat == 0)					return (1 - (hats[0] | hats[1] | hats[2] | hats[3])) == u8(press);
				if (IsPowerOfTwo<false>(hat))   return hats[hat] == u8(press);
				if (hat == GLFW_HAT_LEFT_DOWN)  return (hats[GLFW_HAT_LEFT] | hats[GLFW_HAT_DOWN]) == u8(press);
				if (hat == GLFW_HAT_LEFT_UP)    return (hats[GLFW_HAT_LEFT] | hats[GLFW_HAT_UP]) == u8(press);
				if (hat == GLFW_HAT_RIGHT_DOWN) return (hats[GLFW_HAT_RIGHT] | hats[GLFW_HAT_DOWN]) == u8(press);
				if (hat == GLFW_HAT_RIGHT_UP)   return (hats[GLFW_HAT_RIGHT] | hats[GLFW_HAT_UP]) == u8(press);

				LOG_ERROR("Invalid HAT");
			}
		}

		return false;
	}


	/*static*/ f32 Input::GetAxisRaw(EInputAxis axis)
	{
		if (axis == EInputAxis::NONE)
		{
			return 0.f;
		}

		if ((axis & EInputAxis::IS_JOYSTICK_FLAG) > 0) //Gamepad
		{
			const u32 joy = ToGLFWJoystick(axis);
			GLFWgamepadstate state;
			if (glfwGetGamepadState(joy, &state))
			{
				const u32 glfw_axis = ToGLFWGamepadAxis(axis);
				return state.axes[glfw_axis];
			}
		}
		else //Mouse
		{
			switch (axis)
			{
				case EInputAxis::MOUSE_POSITION_X: return GetMousePositionFloat().x;
				case EInputAxis::MOUSE_POSITION_Y: return GetMousePositionFloat().y;
				case EInputAxis::MOUSE_DELTA_X:	return GetMouseDeltaFloat().x;
				case EInputAxis::MOUSE_DELTA_Y:	return GetMouseDeltaFloat().y;
				case EInputAxis::MOUSE_SCROLL_DELTA_Y: return GetMouseScrollDelta();
				[[unlikely]] 
				case EInputAxis::MOUSE_SCROLL_DELTA_X: return GetMouseScrollDelta2D().x;
			}
		}

		return 0.f;
	}



	/*static*/ EJoystickType Input::GetJoystickType(EJoystickID ID)
	{
		const int glfwID = ToGLFWJoystick(ID);

		if (glfwJoystickPresent(glfwID))
		{
			return glfwJoystickIsGamepad(glfwID) ? EJoystickType::Gamepad : EJoystickType::Hat;
		}
		else
		{
			return EJoystickType::Disconnected;
		}
	}

	/*static*/ bool Input::IsAnyJoystickTypePresent(EJoystickType type)
	{
		for (EJoystickID i = EJoystickID::JOYSTICK_1; i < EJoystickID::JOYSTICK_LAST; ++i)
		{
			if (GetJoystickType(i) == type)
			{
				return true;
			}
		}
		return false;
	}

	/*static*/ const char* Input::GetJoystickGUIDRaw(EJoystickID ID)
	{
		const int glfwID = ToGLFWJoystick(ID);
		return glfwGetJoystickGUID(glfwID);
	}
	/*static*/ std::string Input::GetJoystickGUID(EJoystickID ID)
	{
		const char* raw = GetJoystickGUIDRaw(ID);
		return raw ? std::string(raw) : std::string{};
	}

	/*static*/ const char* Input::GetJoystickNameRaw(EJoystickID ID)
	{
		const int glfwID = ToGLFWJoystick(ID);
		return glfwGetJoystickName(glfwID);
	}
	/*static*/ std::string Input::GetJoystickName(EJoystickID ID)
	{
		const char* raw = GetJoystickNameRaw(ID);
		return raw ? std::string(raw) : std::string{};
	}






	/*static*/ void Input::MoveCamera()
	{
		constexpr size_t numDirs = 4;
		constexpr std::array<EInputButton, numDirs> keyCodes = { 
			EInputButton::KEY_LEFT, 
			EInputButton::KEY_RIGHT, 
			EInputButton::KEY_DOWN, 
			EInputButton::KEY_UP };
		constexpr f32 speed = 0.002f;
		constexpr std::array<fvec3, numDirs> keyDirs = { fvec3(-speed,    0.f, 0.f),
														 fvec3(speed,    0.f, 0.f),
														 fvec3(0.f, -speed, 0.f),
														 fvec3(0.f,  speed, 0.f) };

		for (size_t i = 0; i < numDirs; i++)
		{
			if (IsButtonDown(keyCodes[i]))
			{
				Rendering::OrthographicCamera& camera = Rendering::Renderer::GetCamera();
				const fvec3 pos = camera.GetPosition();
				camera.SetPosition(pos + keyDirs[i]); //todo: * deltaTime
			}
		}

		//Reset camera position
		if (IsButtonDown(EInputButton::KEY_0) || IsButtonDown(EInputButton::KEY_KP_0))
		{
			Rendering::OrthographicCamera& camera = Rendering::Renderer::GetCamera();
			camera.SetPosition(fvec3(0.f));
		}
	}

	/*static*/ void Input::ZoomCamera()
	{
		f32 scrollY = GetMouseScrollDelta();
		if (scrollY == 0.f)
		{
			constexpr f32 buttonScalar = 0.01f;
			if (IsButtonDown(EInputButton::KEY_MINUS) || IsButtonDown(EInputButton::G1_LEFT_BUMPER))
			{
				scrollY = -buttonScalar;
			}
			else if (IsButtonDown(EInputButton::KEY_EQUAL) || IsButtonDown(EInputButton::G1_RIGHT_BUMPER))
			{
				scrollY = buttonScalar;
			}
			else
			{
				return;
			}
		}

		Rendering::OrthographicCamera& camera = Rendering::Renderer::GetCamera();

		const std::optional<f32> opt_zoom = camera.GetZoomScale();
		const f32 zoom = opt_zoom.has_value() ? *opt_zoom : 1;

		constexpr float scaler = 0.1f;
		camera.SetZoomScale(zoom - scaler * scrollY);
	}
}

