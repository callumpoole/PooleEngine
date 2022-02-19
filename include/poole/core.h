#pragma once

#include <algorithm>
#include <array>
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
#else
	#error Other Platforms not supported
#endif

#include "core/core_types.h"
#include "core/core_logging.h"

template<bool incZero = true>
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
