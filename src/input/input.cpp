#include "poole/input/input.h"
#include <iostream>

namespace Poole
{
	void TestProcessInputEvent(GLFWwindow* /*window*/, i32 key, i32 /*scancode*/, i32 action, i32 /*mods*/)
	{
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			std::cout << "1\n";
		}
	}

	/*static*/ void Input::Init(GLFWwindow* window)
	{
		//Listen to input events
		glfwSetKeyCallback(window, TestProcessInputEvent);
	}

	/*static*/ void Input::Tick(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
	}
}

