#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#ifdef _WINDOWS
	#ifdef PooleEngine_EXPORTS
		#define POOLE_API __declspec(dllexport)
	#else
		#define POOLE_API __declspec(dllimport)
	#endif
#else
	#error Other Platforms not supported
#endif