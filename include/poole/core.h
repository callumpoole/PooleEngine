#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <concepts>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <unordered_map>
#include <vector>

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

	#define NEWLINE		"\r\n"
#else
	#error Other Platforms not supported

	#define NEWLINE		"\n" //Keeping it a const char[] to make it compatible with the same use cases as windows one.
#endif

#include "core/core_types.h"
#include "core/core_logging.h"

static constexpr f32 SMALL_NUMBER = 1.e-6f;
static constexpr f32 KINDA_SMALL_NUMBER = 1.e-4f;
static constexpr f32 PI = 3.14159265358979323846f;

namespace Poole
{
	//UNTESTED
	template<typename T>
	void eraseAtSwap(T& container, i32 index)
	{
		if (container.size() > 1)
		{
			std::iter_swap(container.begin() + index, container.end() - 1);
			container.pop_back();
		}
		else
		{
			container.clear();
		}
	}
}


template<typename ... TemplateParam, typename Lambda, typename ... FunctionParam>
decltype(auto) InvokeTemplatedLambda(Lambda&& lambda, FunctionParam&& ... functionParam)
{
	return std::forward<Lambda>(lambda).template operator()<TemplateParam...>(std::forward<FunctionParam>(functionParam)...);
}

namespace Poole::Math
{
	template<typename T>
	static constexpr T Abs(T f) //std's Abs is not constexpr!!!
	{
		return (f < 0) ? -f : f;
	}

	static bool IsMultipleOf(f32 a, f32 b, f32 tolerance = KINDA_SMALL_NUMBER)
	{
		const f32 modt = std::fmod(a, b);
		return (Abs(modt) <= tolerance) || (Abs(b - modt) <= tolerance);
	}

	template<bool incZero = false>
	static constexpr bool IsPowerOfTwo(u64 n)
	{
		if constexpr (incZero)
		{
			return (n & (n - 1)) == 0;
		}
		else
		{
			return (n > 0 && ((n & (n - 1)) == 0));
		}
	}

	static constexpr bool IsNearlyZero(f32 f, f32 tolerance = SMALL_NUMBER)
	{
		return Abs(f) < tolerance;
	}

	static constexpr bool IsNearly(f32 f1, f32 f2, f32 tolerance = SMALL_NUMBER)
	{
		return Abs(f1 - f2) < tolerance;
	}

	static constexpr std::array<char,2> ToHex(const u8 num)
	{
		const u8 upper = (num & 0xf0) >> 4;
		const u8 lower = (num & 0x0f);

		const char upperChar = upper < 10 ? (char(upper) + '0') : (char(upper) - 10 + 'A');
		const char lowerChar = lower < 10 ? (char(lower) + '0') : (char(lower) - 10 + 'A');

		return { upperChar, lowerChar };
	}
	static constexpr std::array<char, 4> ToHex(const u16 num)
	{
		const u8 upper = u8((num & 0xff00) >> 8);
		const u8 lower = u8((num & 0x00ff));

		const std::array<char, 2> u = ToHex(upper);
		const std::array<char, 2> l = ToHex(lower);
		return { u[0], u[1], l[0], l[1] };
	}
	static constexpr std::array<char, 4> ToHex(const u8color2 num)
	{
		return ToHex(u16(u16(num[1]) | (u16(num[0]) << 8)));
	}
	static constexpr std::array<char, 6> ToHex(const u8color3 num)
	{
		const std::array<char, 2> r = ToHex(num[0]);
		const std::array<char, 2> g = ToHex(num[1]);
		const std::array<char, 2> b = ToHex(num[2]);
		return { r[0], r[1], g[0], g[1], b[0], b[1] };
	}
	static constexpr std::array<char, 8> ToHex(const u32 num)
	{
		const u16 upper = u16((num & 0xffff0000) >> 16);
		const u16 lower = u16((num & 0x0000ffff));

		const std::array<char, 4> u = ToHex(upper);
		const std::array<char, 4> l = ToHex(lower);
		return { u[0], u[1], u[2], u[3], l[0], l[1], l[2], l[3] };
	}
	static constexpr std::array<char, 8> ToHex(const u8color4 num)
	{
		return ToHex(u32(u32(num[3]) | (u32(num[2]) << 8) | (u32(num[1]) << 16) | (u32(num[0]) << 24)));
	}
	static constexpr std::string ToHexStr(const auto num)
	{
		auto hexArr = ToHex(num);

		std::ostringstream out;
		for (const char c : hexArr)
		{
			out << c;
		}
		return out.str();
	}
}

template<typename Prim>
static constexpr std::optional<Prim> ToPrimitive(std::string_view str)
{
	Prim p;
	auto res = std::from_chars(str.data(), str.data() + str.size(), p);
	if (res.ec == std::errc::invalid_argument)
	{
		return std::nullopt;
	}
	return p;
}

static constexpr std::optional<std::pair<std::string_view, std::string_view>> Split(std::string_view str, char delib)
{
	const size_t pos = str.find(delib);
	if (pos != std::string_view::npos)
	{
		return std::pair{str.substr(0, pos), str.substr(pos + 1)};
	}
	return {};
}
