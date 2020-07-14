#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace Poole {

	class Engine
	{
	public:
		Engine(const char* windowName, glm::uvec2 size);
	};

}