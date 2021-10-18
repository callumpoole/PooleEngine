
#pragma once

#include "core.h"

struct GLFWwindow;

namespace Poole
{
	class Input
	{
	public:
		static void Init(GLFWwindow* window);
		static void Tick(GLFWwindow* window);
	private:
	};
}
