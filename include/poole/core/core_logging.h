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

#define LOG_NOT_IMPL() \
	std::cerr << std::format("ERROR: " INTERNAL_LOG_LITERALS "Hit Not Implemented. \n", INTERNAL_LOG_PARAMS);

#define ASSERT(cond) assert(cond);

#define HALT() ASSERT(false);

#define LOG_FATAL_INTERNAL(msg, ...) \
	std::cerr << std::format("FATAL ERROR: " INTERNAL_LOG_LITERALS msg "\n", INTERNAL_LOG_PARAMS, __VA_ARGS__);

#define LOG_FATAL(msg, ...) /*TODO: Use C++23 std::basic_stacktrace*/	\
	{																	\
		LOG_FATAL_INTERNAL(msg, __VA_ARGS__);							\
		HALT();															\
	}

#define ASSERT_MSG(cond, msg, ...)				\
	if (!bool(cond))							\
	{											\
		LOG_FATAL_INTERNAL(msg, __VA_ARGS__);	\
		ASSERT(cond);							\
	}

#define IF_ASSERT(ifCond, assertCond) ASSERT(!bool(ifCond) || bool(assertCond))
#define IF_ASSERT_MSG(ifCond, assertCond, msg, ...) ASSERT_MSG(!bool(ifCond) || bool(assertCond), msg, __VA_ARGS__)


#define ENSURE_MSG(cond, msg, ...)																				\
	[](bool inCond)->bool																						\
	{																											\
		if (!inCond)																							\
		{																										\
			std::cerr << std::format("ENSURE FAILED: " INTERNAL_LOG_LITERALS msg "\n", INTERNAL_LOG_PARAMS);	\
			return false;																						\
		}																										\
		return true;																							\
	}(cond)

#define ENSURE(cond) ENSURE_MSG(cond, "")

