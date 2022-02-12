#include "poole/input/input.h"
#include "rendering/renderer.h"
#include "rendering/camera/orthographic_camera.h"
#include <iostream>
#include <window/window.h>

namespace Poole
{
	ivec2 Input::m_LastMousePos{};
	ivec2 Input::m_LastMousePosInWindow{};
	fvec2 Input::m_LastScrollDelta{};
	u64 Input::m_KeepLastScrollDataTickID{};

	void TestProcessInputEvent(GLFWwindow* /*window*/, i32 key, i32 /*scancode*/, i32 action, i32 /*mods*/)
	{
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			LOG_LINE("1 Key Pressed");
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
		glfwSetKeyCallback(window, TestProcessInputEvent);
		glfwSetScrollCallback(window, ProcessScrollEvent);
	}

	/*static*/ void Input::Tick(GLFWwindow* window)
	{
		if (GetKeyDown(EInputKey::KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}

		if constexpr (AllowCameraMovement)
		{
			MoveCamera();
		}
		if constexpr (AllowCameraZooming)
		{
			ZoomCamera();
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

		//Reset Scroll Delta if it wasn't just applied this tick
		if (Input::m_KeepLastScrollDataTickID != EngineLogTime::GetTickCount())
		{
			Input::m_LastScrollDelta = { 0,0 };
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



	/*static*/ bool Input::GetKey(EInputKey key, EInputPress press)
	{
		if (key == EInputKey::NONE)
			return false;

		if (key >= EInputKey::KEY_FIRST_PRINTABLE && key <= EInputKey::KEY_LAST_FUNCTIONAL)
		{
			return glfwGetKey(Window::m_WindowInstance, ToGLFWKey(key)) == u8(press);
		}

		if (key >= EInputKey::MOUSE_BUTTON_FIRST && key <= EInputKey::MOUSE_BUTTON_LAST)
		{
			return glfwGetMouseButton(Window::m_WindowInstance, ToGLFWMouseButton(key)) == u8(press);
		}

		if ((key & EInputKey::IS_JOYSTICK_FLAG) > 0)
		{
			ASSERT(press != EInputPress::REPEAT); //Not yet supported

			const u32 joy = ToGLFWJoystick(key);
			if ((key & EInputKey::IS_GAMEPAD_BUTTON_FLAG) > 0) //If GAMEPAD
			{
				GLFWgamepadstate state;
				if (glfwGetGamepadState(joy, &state))
				{
					const u32 button = ToGLFWGamepadButton(key);
					return state.buttons[button] == u8(press);
				}
			}
			else //If HAT (VERY MUCH GUESS WORK, UNTESTED)
			{
				const u32 hat = ToGLFWJoystickHat(key);
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




	void Input::MoveCamera()
	{
		constexpr size_t numDirs = 4;
		constexpr std::array<EInputKey, numDirs> keyCodes = { 
			EInputKey::KEY_LEFT, 
			EInputKey::KEY_RIGHT, 
			EInputKey::KEY_DOWN, 
			EInputKey::KEY_UP };
		constexpr f32 speed = 0.002f;
		constexpr std::array<fvec3, numDirs> keyDirs = { fvec3(-speed,    0.f, 0.f),
														 fvec3(speed,    0.f, 0.f),
														 fvec3(0.f, -speed, 0.f),
														 fvec3(0.f,  speed, 0.f) };

		for (size_t i = 0; i < numDirs; i++)
		{
			if (GetKeyDown(keyCodes[i]))
			{
				Rendering::OrthographicCamera& camera = Rendering::Renderer::GetCamera();
				const fvec3 pos = camera.GetPosition();
				camera.SetPosition(pos + keyDirs[i]); //todo: * deltaTime
			}
		}

		//Reset camera position
		if (GetKeyDown(EInputKey::KEY_0) || GetKeyDown(EInputKey::KEY_KP_0))
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
			if (GetKeyDown(EInputKey::KEY_MINUS) || GetKeyDown(EInputKey::G1_LEFT_BUMPER))
			{
				scrollY = -buttonScalar;
			}
			else if (GetKeyDown(EInputKey::KEY_EQUAL) || GetKeyDown(EInputKey::G1_RIGHT_BUMPER))
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

