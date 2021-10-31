
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
		};

		static ivec2 GetMousePosition(ECursorClamping clamping = ECursorClamping::Clamp);
		static uvec2 GetMousePositionUnsigned(ECursorClamping clamping = ECursorClamping::Clamp);
		static fvec2 GetMousePositionFloat(ECursorClamping clamping = ECursorClamping::Clamp, ECursorNormalization norm = ECursorNormalization::Absolute);
	private:
		static ivec2 m_LastMousePos;
		static ivec2 m_LastMousePosInWindow;
	};
}
