#pragma once

#include <optional>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <string_view>
#include <format>
#include <source_location>
#include <concepts>
#include <iostream>
#include <type_traits>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

//Effectively escapes , in macros
#define ARG(...) __VA_ARGS__

#ifdef _WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#else
	#error Other Platforms not supported
#endif

#include "core/core_types.h"
#include "core/core_logging.h"

