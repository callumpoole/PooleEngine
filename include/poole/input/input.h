
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
	private:
		static void MoveCamera(GLFWwindow* window);

		static ivec2 m_LastMousePos;
		static ivec2 m_LastMousePosInWindow;
	};
}

