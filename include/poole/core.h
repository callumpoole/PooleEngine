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

using LinearColor3 = glm::vec3;
using Color3 = glm::uvec3;
using LinearColor4 = glm::vec4;
using Color4 = glm::uvec4;

namespace Colors
{
	template<class T> inline constexpr T Red;
	template<class T> inline constexpr T Green;
	template<class T> inline constexpr T Blue;
	template<class T> inline constexpr T Yellow;
	template<class T> inline constexpr T Cyan;
	template<class T> inline constexpr T Purple;
	template<class T> inline constexpr T White;
	template<class T> inline constexpr T Black;
	
	template<> inline constexpr LinearColor3 Red<LinearColor3>		= { 1.0f, 0.0f, 0.0f };
	template<> inline constexpr LinearColor3 Green<LinearColor3>	= { 0.0f, 1.0f, 0.0f };
	template<> inline constexpr LinearColor3 Blue<LinearColor3>		= { 0.0f, 0.0f, 1.0f };
	template<> inline constexpr LinearColor3 Yellow<LinearColor3>	= { 1.0f, 1.0f, 0.0f };
	template<> inline constexpr LinearColor3 Cyan<LinearColor3>		= { 0.0f, 1.0f, 1.0f };
	template<> inline constexpr LinearColor3 Purple<LinearColor3>	= { 1.0f, 0.0f, 1.0f };
	template<> inline constexpr LinearColor3 White<LinearColor3>	= { 1.0f, 1.0f, 1.0f };
	template<> inline constexpr LinearColor3 Black<LinearColor3>	= { 0.0f, 0.0f, 0.0f };

	template<> inline constexpr LinearColor4 Red<LinearColor4>		= { 1.0f, 0.0f, 0.0f, 0.0f };
	template<> inline constexpr LinearColor4 Green<LinearColor4>	= { 0.0f, 1.0f, 0.0f, 0.0f };
	template<> inline constexpr LinearColor4 Blue<LinearColor4>		= { 0.0f, 0.0f, 1.0f, 0.0f };
	template<> inline constexpr LinearColor4 Yellow<LinearColor4>	= { 1.0f, 1.0f, 0.0f, 0.0f };
	template<> inline constexpr LinearColor4 Cyan<LinearColor4>		= { 0.0f, 1.0f, 1.0f, 0.0f };
	template<> inline constexpr LinearColor4 Purple<LinearColor4>	= { 1.0f, 0.0f, 1.0f, 0.0f };
	template<> inline constexpr LinearColor4 White<LinearColor4>	= { 1.0f, 1.0f, 1.0f, 0.0f };
	template<> inline constexpr LinearColor4 Black<LinearColor4>	= { 0.0f, 0.0f, 0.0f, 0.0f };

	template<> inline constexpr Color3 Red<Color3>		= { 255, 0, 0 };
	template<> inline constexpr Color3 Green<Color3>	= { 0, 255, 0 };
	template<> inline constexpr Color3 Blue<Color3>		= { 0, 0, 255 };
	template<> inline constexpr Color3 Yellow<Color3>	= { 255, 255, 0 };
	template<> inline constexpr Color3 Cyan<Color3>		= { 0, 255, 255 };
	template<> inline constexpr Color3 Purple<Color3>	= { 255, 0, 255 };
	template<> inline constexpr Color3 White<Color3>	= { 255, 255, 255 };
	template<> inline constexpr Color3 Black<Color3>	= { 0, 0, 0 };

	template<> inline constexpr Color4 Red<Color4>		= { 255, 0, 0, 0 };
	template<> inline constexpr Color4 Green<Color4>	= { 0, 255, 0, 0 };
	template<> inline constexpr Color4 Blue<Color4>		= { 0, 0, 255, 0 };
	template<> inline constexpr Color4 Yellow<Color4>	= { 255, 255, 0, 0 };
	template<> inline constexpr Color4 Cyan<Color4>		= { 0, 255, 255, 0 };
	template<> inline constexpr Color4 Purple<Color4>	= { 255, 0, 255, 0 };
	template<> inline constexpr Color4 White<Color4>	= { 255, 255, 255, 0 };
	template<> inline constexpr Color4 Black<Color4>	= { 0, 0, 0, 0 };
}


