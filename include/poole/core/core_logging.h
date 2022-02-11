#pragma once

#include "core.h"

namespace Poole
{
	class EngineLogTime
	{
		friend class Engine;
	public:
		static u64 GetTickCount() { return m_TickCount; }
	private:
		static u64 m_TickCount;
	};
}

#define INTERNAL_LOG_TICK_LITERALS "[{}]"
#define INTERNAL_LOG_TICK_PARAMS Poole::EngineLogTime::GetTickCount() % 1000

#define INTERNAL_LOG_LITERALS INTERNAL_LOG_TICK_LITERALS "[{}] {}:({},{}):"
#define INTERNAL_LOG_PARAMS   INTERNAL_LOG_TICK_PARAMS, \
							  std::source_location::current().file_name(), \
							  std::source_location::current().function_name(), \
							  std::source_location::current().line(), \
							  std::source_location::current().column()

#define LOG(msg, ...) \
	std::cout << std::format(INTERNAL_LOG_TICK_LITERALS msg "\n", INTERNAL_LOG_TICK_PARAMS, __VA_ARGS__);

#define LOG_LINE(msg, ...) \
	std::cout << std::format("LOG: " INTERNAL_LOG_LITERALS msg "\n", INTERNAL_LOG_PARAMS, __VA_ARGS__);

#define LOG_WARNING(msg, ...) \
	std::cout << std::format("WARNING: " INTERNAL_LOG_LITERALS msg "\n", INTERNAL_LOG_PARAMS, __VA_ARGS__);

#define LOG_ERROR(msg, ...) \
	std::cerr << std::format("ERROR: " INTERNAL_LOG_LITERALS msg "\n", INTERNAL_LOG_PARAMS, __VA_ARGS__);

#define ASSERT(cond) assert(cond);

#define LOG_FATAL(msg, ...) /*TODO: Use C++23 std::basic_stacktrace*/ \
	{ \
		std::cerr << std::format("FATAL ERROR: " INTERNAL_LOG_LITERALS msg "\n", INTERNAL_LOG_PARAMS, __VA_ARGS__); \
		ASSERT(false); \
	}

#define ASSERT_MSG(cond, msg, ...)		\
	if (!cond)							\
	{									\
		LOG_FATAL(msg, __VA_ARGS__);	\
	}

#define HALT() ASSERT(false);

#define ENSURE_MSG(cond, msg, ...)										\
	[](bool inCond)->bool												\
	{																	\
		if (!inCond)													\
		{																\
			LOG_ERROR("Ensure Failed: " #cond ". " msg, __VA_ARGS__);	\
			return false;												\
		}																\
		return true;													\
	}(cond)

#define ENSURE(cond) ENSURE_MSG(cond, "")

