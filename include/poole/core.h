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
//Wont be using char16_t, char32_t
using char8 = char8_t;
using wchar = wchar_t;

using fvec2 = glm::vec2;
using fvec3 = glm::vec3;
using fvec4 = glm::vec4;
using fquat = glm::quat;
using ivec2 = glm::i32vec2;
using ivec3 = glm::i32vec3;
using ivec4 = glm::i32vec4;
using uvec2 = glm::uvec2;
using uvec3 = glm::uvec3;
using uvec4 = glm::uvec4;

using fmat2 = glm::mat2;
using fmat3 = glm::mat3;
using fmat4 = glm::mat4;

using fcolor3 = glm::vec3;
using fcolor4 = glm::vec4;
using ucolor3 = glm::uvec3;
using ucolor4 = glm::uvec4;

namespace Colors
{
#define AddColor(Name, R, G, B, A) \
	template<class T> inline constexpr T Name; \
	template<> inline constexpr fcolor4 Name <fcolor4> = { R, G, B, A }; \
	template<> inline constexpr fcolor3 Name <fcolor3> = { Name <fcolor4> }; \
	template<> inline constexpr ucolor4 Name <ucolor4> = { Name <fcolor4> * 255.f }; \
	template<> inline constexpr ucolor3 Name <ucolor3> = { Name <ucolor4> }; 

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


