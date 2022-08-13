#pragma once

#include "core.h"

namespace Poole
{
	u64 LoggingGetTickCount();
	size_t CharactersToRemoveFromPath();
	std::string_view ShortenFilename(const char* fullFileName);

#define DO_PROFILE_LOGGING 0
#define DO_PROFILE_LOGGING_WITH_AVG 1

	class ScopedProfiler {
	public:
		ScopedProfiler(const char* functionName);
		~ScopedProfiler();
	private:
		u64 m_TicksSinceEpoch = 0;
		const char* m_FunctionName = nullptr;
#if DO_PROFILE_LOGGING_WITH_AVG
		static u64 m_TotalTicks;
		static u64 m_TickCount;
#endif
	};
}

#if DO_PROFILE_LOGGING
#	define SCOPED_PROFILER() auto __ScopedProf = ScopedProfiler(std::source_location::current().function_name());
#else
#	define SCOPED_PROFILER()
#endif


#define INTERNAL_LOG_TICK_LITERALS "[{:3}] "
#define INTERNAL_LOG_TICK_PARAMS LoggingGetTickCount() % 1000

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

