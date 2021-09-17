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

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float_t;
using f64 = double_t;
//Wont be using char8_t, char16_t, char32_t
using wchar = wchar_t;


