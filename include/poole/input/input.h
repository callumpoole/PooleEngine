
#pragma once

#include "poole/core.h"

struct GLFWwindow;

namespace Poole
{
	class Input
	{
	public:
		static void Init(GLFWwindow* window);
		static void Tick(GLFWwindow* window);

		enum class ECursorClamping : u8
		{
			None,
			Clamp,
			LastValidPosition,
		};
		enum class ECursorNormalization : u8
		{
			Absolute,
			ZeroToOne,
			NegativeOneToOne,
			ZeroToOneAspect,		//Scaled for aspect ratio
			NegativeOneToOneAspect,	//Scaled for aspect ratio
		};

		static ivec2 GetMousePosition(bool invertY = false, ECursorClamping clamping = ECursorClamping::Clamp);
		static ivec2 GetMousePosition(ECursorClamping clamping = ECursorClamping::Clamp) { return GetMousePosition(false, clamping); }
		static uvec2 GetMousePositionUnsigned(bool invertY = false, ECursorClamping clamping = ECursorClamping::Clamp);
		static uvec2 GetMousePositionUnsigned(ECursorClamping clamping = ECursorClamping::Clamp) { return GetMousePositionUnsigned(false, clamping); }
		static fvec2 GetMousePositionFloat(bool invertY = false, ECursorClamping clamping = ECursorClamping::Clamp, ECursorNormalization norm = ECursorNormalization::Absolute);
		static fvec2 GetMousePositionFloat(ECursorClamping clamping = ECursorClamping::Clamp, ECursorNormalization norm = ECursorNormalization::Absolute) { return GetMousePositionFloat(false, clamping, norm); }
	private:
		static ivec2 m_LastMousePos;
		static ivec2 m_LastMousePosInWindow;
	};
}
