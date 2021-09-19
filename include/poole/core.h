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

using fColor3 = glm::vec3;
using fColor4 = glm::vec4;
using uColor3 = glm::uvec3;
using uColor4 = glm::uvec4;

namespace Colors
{
#define AddColor(Name, R, G, B, A) \
	template<class T> inline constexpr T Name; \
	template<> inline constexpr fColor4 Name <fColor4> = { R, G, B, A }; \
	template<> inline constexpr fColor3 Name <fColor3> = { Name <fColor4> }; \
	template<> inline constexpr uColor4 Name <uColor4> = { Name <fColor4> * 255.f }; \
	template<> inline constexpr uColor3 Name <uColor3> = { Name <uColor4> }; 

	AddColor(Red,		1.00f, 0.00f, 0.00f, 1.00f);
	AddColor(Green,		0.00f, 1.00f, 0.00f, 1.00f);
	AddColor(Blue,		0.00f, 0.00f, 1.00f, 1.00f);
	AddColor(Yellow,	1.00f, 1.00f, 0.00f, 1.00f);
	AddColor(Cyan,		0.00f, 1.00f, 1.00f, 1.00f);
	AddColor(Purple,	1.00f, 0.00f, 1.00f, 1.00f);
	AddColor(Orange,	1.00f, 0.50f, 0.00f, 1.00f);
	AddColor(Pink,		1.00f, 0.50f, 0.50f, 1.00f);
	AddColor(White,		1.00f, 1.00f, 1.00f, 1.00f);
	AddColor(Grey,		0.50f, 0.50f, 0.50f, 1.00f);
	AddColor(Black,		0.00f, 0.00f, 0.00f, 1.00f);
#undef AddColor
}


