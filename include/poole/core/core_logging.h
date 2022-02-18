#pragma once

#include "core.h"

namespace Poole
{
	class EngineTime
	{
		friend class Engine;
	public:
		static u64 GetTickCount() { return s_TickCount; }
	private:
		static u64 s_TickCount;
		static u64 s_FrameNS;
		static f32 s_FrameMS;
		static f32 s_DeltaTime;
		static f32 s_FPS;

		static long long s_LaunchSinceEpochNS;
		static long long s_FirstTickSinceEpochNS;
		static f32 s_SecondsSinceLaunch;
		static f32 s_SecondsSinceFirstTick;

		static f32 s_AccDeltaTimeThisSecond;
		static u64 s_AccTicksThisSecond;
		static f32 s_AvgDeltaTime;
		static f32 s_AvgFPS;
	};

	size_t CharactersToRemoveFromPath();
	std::string_view ShortenFilename(const char* fullFileName);
}


#define INTERNAL_LOG_TICK_LITERALS "[{:3}] "
#define INTERNAL_LOG_TICK_PARAMS Poole::EngineTime::GetTickCount() % 1000

#define INTERNAL_LOG_LITERALS INTERNAL_LOG_TICK_LITERALS "[{}] {}:({},{}): "
#define INTERNAL_LOG_PARAMS   INTERNAL_LOG_TICK_PARAMS, \
							  ShortenFilename(std::source_location::current().file_name()).data(), \
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

