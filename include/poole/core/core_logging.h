#pragma once

#include "core.h"

#include <locale.h>

namespace Poole
{
	u64 LoggingGetTickCount();
	size_t CharactersToRemoveFromPath();
	std::string_view ShortenFilename(const char* fullFileName);
#define GetCurrentFileNameShortened() ShortenFilename(std::source_location::current().file_name())

#define DO_PROFILE_LOGGING 1
#define DO_PROFILE_LOGGING_WITH_AVG 1

	class ScopedProfiler {
	public:
		ScopedProfiler(std::string&& identifier);
		~ScopedProfiler();
	private:
		std::chrono::steady_clock::time_point m_StartTimePoint;
		std::string m_Identifier;
#if DO_PROFILE_LOGGING_WITH_AVG
		struct OverManyCallsData
		{
			long long m_AccumulativeNS = 0;
			u64 m_TickCount = 0;
		};
		static std::unordered_map<std::string, OverManyCallsData> s_DataOverManyCalls;
#endif
	};
}

#if DO_PROFILE_LOGGING
#	define SCOPED_PROFILER() auto __ScopedProf = Poole::ScopedProfiler(std::format("{}:{}:{} {}()",							\
																	   Poole::GetCurrentFileNameShortened().data(),			\
																	   std::source_location::current().line(),				\
																	   std::source_location::current().column(),			\
																	   std::source_location::current().function_name()));
#else
#	define SCOPED_PROFILER()
#endif

#define INTERNAL_LOG_TICK_LITERALS "[{:3}]"
#define INTERNAL_LOG_TICK_PARAMS Poole::LoggingGetTickCount() % 1000

#define INTERNAL_LOG_LITERALS INTERNAL_LOG_TICK_LITERALS "[{}]{}:({},{})"
#define INTERNAL_LOG_PARAMS   INTERNAL_LOG_TICK_PARAMS,							\
							  Poole::GetCurrentFileNameShortened().data(),		\
							  std::source_location::current().function_name(),	\
							  std::source_location::current().line(),			\
							  std::source_location::current().column()

#define LOG_LINE(msg, ...) \
	std::cout << std::format(INTERNAL_LOG_LITERALS "LOG: " msg "\n", INTERNAL_LOG_PARAMS, __VA_ARGS__);

#if 0 //If 1, will convert LOG to LOG_LINE to find those unlocated prints more easily.
	#define LOG(msg, ...) LOG_LINE(msg, __VA_ARGS__);
#else
	#define LOG(msg, ...) \
		std::cout << std::format(INTERNAL_LOG_TICK_LITERALS " " msg "\n", INTERNAL_LOG_TICK_PARAMS, __VA_ARGS__);
#endif

#define LOG_WARNING(msg, ...) \
	std::cout << std::format(INTERNAL_LOG_LITERALS "WARNING: " msg "\n", INTERNAL_LOG_PARAMS, __VA_ARGS__);

#define LOG_ERROR(msg, ...) \
	std::cerr << std::format(INTERNAL_LOG_LITERALS "ERROR: " msg "\n", INTERNAL_LOG_PARAMS, __VA_ARGS__);

#define LOG_NOT_IMPL() \
	std::cerr << std::format(INTERNAL_LOG_LITERALS "ERROR NOT IMPLEMENTED. \n", INTERNAL_LOG_PARAMS);


#define ASSERT(cond) { if (!std::is_constant_evaluated() && !(cond)) { __debugbreak(); } assert(cond); }

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

