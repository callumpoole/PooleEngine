#include "core/core_logging.h"

#include "core.h"
#include <filesystem>
#include <chrono>
#include "engine.h"

namespace Poole
{
	u64 LoggingGetTickCount()
	{
		if (const Engine* e = Engine::Get())
		{
			return e->GetTimeData().GetTickCount();
		}
		return 0;
	}

	size_t CharactersToRemoveFromPath()
	{
		static i32 result = -1;
		if (result != -1)
		{
			return result;
		}

		const std::string_view fileName = std::source_location::current().file_name();
		const std::string& currentPath = std::filesystem::current_path().string();

		const size_t lessThan = std::min(fileName.size(), currentPath.size());
		for (size_t i = 0; i < lessThan; i++)
		{
			if (currentPath[i] != fileName[i])
			{
				result = std::max(i32(i) - 1, 0);
				return size_t(result);
			}
		}
		result = i32(currentPath.size());
		return size_t(result);
	}

	std::string_view ShortenFilename(const char* fullFileName)
	{
		const std::string_view fullFileView = fullFileName;

		const size_t ToRemove = CharactersToRemoveFromPath();
		return fullFileView.substr(ToRemove);
	}

	
#if DO_PROFILE_LOGGING_WITH_AVG
	/*static*/ u64 ScopedProfiler::m_TotalTicks = 0;
	/*static*/ u64 ScopedProfiler::m_TickCount = 0;
#endif

	ScopedProfiler::ScopedProfiler(const char* functionName)
		: m_TicksSinceEpoch(std::chrono::system_clock::now().time_since_epoch().count())
		, m_FunctionName(functionName)
	{		
	}
	ScopedProfiler::~ScopedProfiler()
	{
		const u64 newNow = std::chrono::system_clock::now().time_since_epoch().count();
		const u64 deltaTicks = newNow - m_TicksSinceEpoch;

#if DO_PROFILE_LOGGING_WITH_AVG
		m_TotalTicks += deltaTicks;
		m_TickCount++;

		LOG("FUNCTION PROFILER: {} -> {}ticks (average: {}ticks)", m_FunctionName, deltaTicks, m_TotalTicks / (float)m_TickCount);
#else
		LOG("FUNCTION PROFILER: {} -> {}ticks", m_FunctionName, deltaTicks);
#endif
	}
}
