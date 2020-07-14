#pragma once

#include "glad/glad.h" //Includes OpenGL
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class PooleEngine
{
public:
	PooleEngine(const char* windowName, glm::uvec2 size);
};