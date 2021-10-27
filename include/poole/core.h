#pragma once

#include <optional>
#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#ifdef _WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
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

//Effectively escapes , in macros
#define ARG(...) __VA_ARGS__

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


#define VertexWithColorDefinition(Name, TVertexPart, TColorPart, Extra, ...)			\
	struct Name																			\
	{																					\
		using TVertex = TVertexPart;													\
		using TColor = TColorPart;														\
		Name() = default;																\
		Name(TVertex position, TColor color) : m_position(position), m_color(color) {}	\
		Extra																			\
		union {																			\
			struct																		\
			{																			\
				TVertex m_position;														\
				TColor m_color;															\
			};																			\
			struct { f32 __VA_ARGS__; }; /*I feel no need for m_ here */				\
		};																				\
	};

#define Vertex3Color4Ctr Vertex3Color4(f32 x, f32 y, f32 z, f32 r, f32 g, f32 b, f32 a) : x(x), y(y), z(z), r(r), g(g), b(b), a(a) {}
#define Vertex3Color3Ctr Vertex3Color3(f32 x, f32 y, f32 z, f32 r, f32 g, f32 b)		: x(x), y(y), z(z), r(r), g(g), b(b)	   {}
#define Vertex2Color4Ctr Vertex2Color4(f32 x, f32 y,		f32 r, f32 g, f32 b, f32 a) : x(x), y(y),		r(r), g(g), b(b), a(a) {}
#define Vertex2Color3Ctr Vertex2Color3(f32 x, f32 y,		f32 r, f32 g, f32 b)		: x(x), y(y),		r(r), g(g), b(b)	   {}
#define Vertex3Color3Funcs //Vertex3Color4 ToColor4(f32 a = 1.f) const { return {x, y, z, r, g, b, a}; } Vertex2Color3 ToVertex2()			const { return {x, y,    r, g, b   }; }
#define Vertex3Color4Funcs //Vertex3Color3 ToColor3()			   const { return {x, y, z, r, g, b   }; } Vertex2Color4 ToVertex2()			const { return {x, y,    r, g, b, a}; }
#define Vertex2Color3Funcs //Vertex2Color4 ToColor4(f32 a = 1.f) const { return {x, y,    r, g, b, a}; } Vertex3Color3 ToVertex3(f32 z = 0.f) const { return {x, y, z, r, g, b   }; }
#define Vertex2Color4Funcs //Vertex2Color3 ToColor3()			   const { return {x, y,    r, g, b   }; } Vertex3Color4 ToVertex3(f32 z = 0.f) const { return {x, y, z, r, g, b, a}; }


VertexWithColorDefinition(Vertex3Color4, fvec3, fcolor4, Vertex3Color4Ctr Vertex3Color4Funcs, x, y, z, r, g, b, a);
VertexWithColorDefinition(Vertex3Color3, fvec3, fcolor3, Vertex3Color3Ctr Vertex3Color3Funcs, x, y, z, r, g, b   );
VertexWithColorDefinition(Vertex2Color4, fvec2, fcolor4, Vertex2Color4Ctr Vertex2Color4Funcs, x, y,    r, g, b, a);
VertexWithColorDefinition(Vertex2Color3, fvec2, fcolor3, Vertex2Color3Ctr Vertex2Color3Funcs, x, y,    r, g, b   );

#undef Vertex2Color3Funcs
#undef Vertex2Color4Funcs
#undef Vertex3Color3Funcs
#undef Vertex3Color4Funcs
#undef Vertex2Color3Ctr
#undef Vertex2Color4Ctr
#undef Vertex3Color3Ctr
#undef Vertex3Color4Ctr
#undef VertexWithColorDefinition

