#pragma once

#include "core.h"

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
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

using fcolor2 = glm::vec2;  //Could be useful for UV maps
using fcolor3 = glm::vec3;
using fcolor4 = glm::vec4;
using u8color2 = glm::u8vec2; //Could be useful for UV maps
using u8color3 = glm::u8vec3;
using u8color4 = glm::u8vec4;

struct ftransform2D
{
	ftransform2D() = default;
	ftransform2D(const fvec2& position,
		const fvec2& scale = fvec2(1.0f),
		const f32 rotation = 0,
		const fvec2& shear = fvec2(0.0f))
		: position(fvec3{ position.x, position.y, 0.f })
		, scale(scale)
		, rotation(rotation)
		, shear(shear)
	{}
	ftransform2D(const fvec2& position,
		fvec2&& scale = fvec2(1.0f),
		f32&& rotation = 0,
		fvec2&& shear = fvec2(0.0f))
		: position(fvec3{ position.x, position.y, 0.f })
		, scale(std::move(scale))
		, rotation(rotation)
		, shear(std::move(shear))
	{}
	ftransform2D(fvec2&& position,
		fvec2&& scale = fvec2(1.0f),
		f32&& rotation = 0,
		fvec2&& shear = fvec2(0.0f))
		: position(fvec3{ position.x, position.y, 0.f })
		, scale(std::move(scale))
		, rotation(rotation)
		, shear(std::move(shear))
	{}

	fmat4 MakeTransformMatrix() const;
	fmat4 MakeRotationMatrix() const;

	fvec3 position = fvec3(0.0f);
	fvec2 scale = fvec2(1.0f);
	f32 rotation = 0;
	fvec2 shear = fvec2(0.0f);
};

enum class EHorizontal : u8 {
	Left, Center, Right
};
enum class EVertical : u8 {
	Bottom, Middle, Top
};

//Copied from https://youtu.be/INn3xa4pMfg
template<typename _K, typename _V, size_t _Size>
struct ConstexprMap
{
	using K = _K;
	using V = _V;
	static constexpr size_t Size = _Size;

	std::array<std::pair<K, V>, Size> m_Data;
	
	[[nodiscard]]
	constexpr const V& at(const K& key) const {
		const auto itr =
			std::find_if(begin(m_Data), end(m_Data),
				[&key](const auto& pair) { return pair.first == key; });
		if (itr != end(m_Data))
		{
			return itr->second;
		}
		else
		{
			throw std::range_error("Not Found");
		}
	}
};

namespace Colors
{
#define AddColor(Name, R, G, B, A) \
	template<class T> inline constexpr T Name; \
	template<> inline constexpr fcolor4 Name <fcolor4> = { R, G, B, A }; \
	template<> inline constexpr fcolor3 Name <fcolor3> = { Name <fcolor4> }; \
	template<> inline constexpr u8color4 Name <u8color4> = { Name <fcolor4> * 255.f }; \
	template<> inline constexpr u8color3 Name <u8color3> = { Name <u8color4> }; 

	AddColor(Red, 1.00f, 0.00f, 0.00f, 1.00f);
	AddColor(Green, 0.00f, 1.00f, 0.00f, 1.00f);
	AddColor(Blue, 0.00f, 0.00f, 1.00f, 1.00f);
	AddColor(Yellow, 1.00f, 1.00f, 0.00f, 1.00f);
	AddColor(Cyan, 0.00f, 1.00f, 1.00f, 1.00f);
	AddColor(Purple, 0.50f, 0.00f, 1.00f, 1.00f);
	AddColor(Orange, 1.00f, 0.50f, 0.00f, 1.00f);
	AddColor(Pink, 1.00f, 0.00f, 1.00f, 1.00f);
	AddColor(White, 1.00f, 1.00f, 1.00f, 1.00f);
	AddColor(Grey, 0.50f, 0.50f, 0.50f, 1.00f);
	AddColor(Black, 0.00f, 0.00f, 0.00f, 1.00f);

	//TODO: Find a cleaner solution
	AddColor(Alpha90, 1.00f, 1.00f, 1.00f, 0.90f);
	AddColor(Alpha80, 1.00f, 1.00f, 1.00f, 0.80f);
	AddColor(Alpha75, 1.00f, 1.00f, 1.00f, 0.75f);
	AddColor(Alpha70, 1.00f, 1.00f, 1.00f, 0.70f);
	AddColor(Alpha67, 1.00f, 1.00f, 1.00f, 0.67f);
	AddColor(Alpha60, 1.00f, 1.00f, 1.00f, 0.60f);
	AddColor(Alpha50, 1.00f, 1.00f, 1.00f, 0.50f);
	AddColor(Alpha40, 1.00f, 1.00f, 1.00f, 0.40f);
	AddColor(Alpha33, 1.00f, 1.00f, 1.00f, 0.33f);
	AddColor(Alpha30, 1.00f, 1.00f, 1.00f, 0.30f);
	AddColor(Alpha25, 1.00f, 1.00f, 1.00f, 0.25f);
	AddColor(Alpha20, 1.00f, 1.00f, 1.00f, 0.20f);
	AddColor(Alpha10, 1.00f, 1.00f, 1.00f, 0.10f);
	AddColor(Alpha5 , 1.00f, 1.00f, 1.00f, 0.05f);
	AddColor(Alpha1 , 1.00f, 1.00f, 1.00f, 0.01f);
	AddColor(Alpha0 , 1.00f, 1.00f, 1.00f, 0.00f);
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

VertexWithColorDefinition(Vertex3Color4, fvec3, fcolor4, Vertex3Color4Ctr, x, y, z, r, g, b, a);
VertexWithColorDefinition(Vertex3Color3, fvec3, fcolor3, Vertex3Color3Ctr, x, y, z, r, g, b);
VertexWithColorDefinition(Vertex2Color4, fvec2, fcolor4, Vertex2Color4Ctr, x, y, r, g, b, a);
VertexWithColorDefinition(Vertex2Color3, fvec2, fcolor3, Vertex2Color3Ctr, x, y, r, g, b);

#undef Vertex2Color3Ctr
#undef Vertex2Color4Ctr
#undef Vertex3Color3Ctr
#undef Vertex3Color4Ctr
#undef VertexWithColorDefinition

//For: Vertex3Color4
static Vertex3Color3 ToVertex3Color3(const Vertex3Color4& s) { return { s.x, s.y, s.z, s.r, s.g, s.b }; }
static Vertex2Color4 ToVertex2Color4(const Vertex3Color4& s) { return { s.x, s.y,      s.r, s.g, s.b, s.a }; }
static Vertex2Color3 ToVertex2Color3(const Vertex3Color4& s) { return { s.x, s.y,      s.r, s.g, s.b }; }
static Vertex3Color3 ToColor3(const Vertex3Color4& s) { return ToVertex3Color3(s); }
static Vertex2Color4 ToVertex2(const Vertex3Color4& s) { return ToVertex2Color4(s); }

//For: Vertex3Color3								  
static Vertex3Color4 ToVertex3Color4(const Vertex3Color3& s, f32 a = 1.f) { return { s.x, s.y, s.z, s.r, s.g, s.b, a }; }
static Vertex2Color4 ToVertex2Color4(const Vertex3Color3& s, f32 a = 1.f) { return { s.x, s.y,      s.r, s.g, s.b, a }; }
static Vertex2Color3 ToVertex2Color3(const Vertex3Color3& s) { return { s.x, s.y,      s.r, s.g, s.b }; }
static Vertex3Color4 ToColor4(const Vertex3Color3& s, f32 a = 1.f) { return ToVertex3Color4(s, a); }
static Vertex2Color3 ToVertex2(const Vertex3Color3& s) { return ToVertex2Color3(s); }

//For: Vertex2Color4								  
static Vertex3Color4 ToVertex3Color4(const Vertex2Color4& s, f32 z = 0.f) { return { s.x, s.y, z, s.r, s.g, s.b, s.a }; }
static Vertex3Color3 ToVertex3Color3(const Vertex2Color4& s, f32 z = 0.f) { return { s.x, s.y, z, s.r, s.g, s.b }; }
static Vertex2Color3 ToVertex2Color3(const Vertex2Color4& s) { return { s.x, s.y,    s.r, s.g, s.b }; }
static Vertex3Color4 ToVertex3(const Vertex2Color4& s, f32 z = 0.f) { return ToVertex3Color4(s, z); }
static Vertex2Color3 ToColor3(const Vertex2Color4& s) { return ToVertex2Color3(s); }

//For: Vertex2Color3								  
static Vertex3Color4 ToVertex3Color4(const Vertex2Color3& s, f32 z = 0.f, f32 a = 1.f) { return { s.x, s.y, z, s.r, s.g, s.b, a }; }
static Vertex3Color3 ToVertex3Color3(const Vertex2Color3& s, f32 z = 0.f) { return { s.x, s.y, z, s.r, s.g, s.b, }; }
static Vertex2Color4 ToVertex2Color4(const Vertex2Color3& s, f32 a = 1.f) { return { s.x, s.y,    s.r, s.g, s.b, a }; }
static Vertex3Color3 ToVertex3(const Vertex2Color3& s, f32 z = 0.f) { return ToVertex3Color3(s, z); }
static Vertex2Color4 ToColor4(const Vertex2Color3& s, f32 a = 1.f) { return ToVertex2Color4(s, a); }


//ENUMS
#define ENUM_ARITHMETIC(T)													\
inline constexpr T operator+ (T   a, u64 b) { return T((u64)a  +      b); } \
inline constexpr T operator+ (u64 a, T   b) { return T(     a  + (u64)b); } \
inline constexpr T operator+ (T   a, T   b) { return T((u64)a  + (u64)b); } \
inline constexpr T operator- (T   a, u64 b) { return T((u64)a  -      b); } \
inline constexpr T operator- (u64 a, T   b) { return T(     a  - (u64)b); } \
inline constexpr T operator- (T   a, T   b) { return T((u64)a  - (u64)b); } \
inline constexpr T operator<<(T   a, u64 b) { return T((u64)a <<      b); } \
inline constexpr T operator<<(u64 a, T   b) { return T(     a << (u64)b); } \
inline constexpr T operator<<(T   a, T   b) { return T((u64)a << (u64)b); } \
inline constexpr T operator>>(T   a, u64 b) { return T((u64)a >>      b); } \
inline constexpr T operator>>(u64 a, T   b) { return T(     a >> (u64)b); } \
inline constexpr T operator>>(T   a, T   b) { return T((u64)a >> (u64)b); } \
inline constexpr T /*Pre*/ operator++(T& a) { a = a + 1; return a; } \
inline constexpr T /*Pre*/ operator--(T& a) { a = a - 1; return a; } \
inline constexpr T /*Post*/ operator++(T& a, /*weird*/ int) { const T prev = a; a = a + 1; return prev; } \
inline constexpr T /*Post*/ operator--(T& a, /*weird*/ int) { const T prev = a; a = a - 1; return prev; }

#define ENUM_COMPARE_INT(T)											   \
inline constexpr bool operator==(T a, u64 b)  { return (u64)a == b; }  \
inline constexpr bool operator==(u64 a, T b)  { return a == (u64)b; }  \
inline constexpr bool operator!=(T a, u64 b)  { return (u64)a != b; }  \
inline constexpr bool operator!=(u64 a, T b)  { return a != (u64)b; }  \
inline constexpr bool operator<(T a, u64 b)   { return (u64)a < b; }   \
inline constexpr bool operator<(u64 a, T b)   { return a < (u64)b; }   \
inline constexpr bool operator>(T a, u64 b)   { return (u64)a > b; }   \
inline constexpr bool operator>(u64 a, T b)   { return a > (u64)b; }   \
inline constexpr bool operator<=(T a, u64 b)  { return (u64)a <= b; }  \
inline constexpr bool operator<=(u64 a, T b)  { return a <= (u64)b; }  \
inline constexpr bool operator>=(T a, u64 b)  { return (u64)a >= b; }  \
inline constexpr bool operator>=(u64 a, T b)  { return a >= (u64)b; }  \
inline constexpr auto operator<=>(T a, u64 b) { return (u64)a <=> b; } \
inline constexpr auto operator<=>(u64 a, T b) { return a <=> (u64)b; }

#define ENUM_FLAGS(T)														   \
inline constexpr T operator~ (T a) { return (T)~(u64)a; }					   \
inline constexpr T operator| (T a, T b) { return (T)((u64)a | (u64)b); }	   \
inline constexpr T operator& (T a, T b) { return (T)((u64)a & (u64)b); }	   \
inline constexpr T operator^ (T a, T b) { return (T)((u64)a ^ (u64)b); }	   \
inline constexpr T& operator|= (T& a, T b) { return (T&)((u64&)a |= (u64)b); } \
inline constexpr T& operator&= (T& a, T b) { return (T&)((u64&)a &= (u64)b); } \
inline constexpr T& operator^= (T& a, T b) { return (T&)((u64&)a ^= (u64)b); } \
ENUM_COMPARE_INT(T)															   \
ENUM_ARITHMETIC(T)
