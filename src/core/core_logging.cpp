#include "core/core_logging.h"

#include <filesystem>
#include <locale.h>

#include "core.h"
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
	/*static*/ std::unordered_map<std::string, ScopedProfiler::OverManyCallsData> ScopedProfiler::s_DataOverManyCalls;
#endif

	ScopedProfiler::ScopedProfiler(std::string&& identifier)
		: m_StartTimePoint(std::chrono::high_resolution_clock::now())
		, m_Identifier(std::move(identifier))
	{}
	ScopedProfiler::~ScopedProfiler()
	{
		const std::chrono::steady_clock::time_point finish = std::chrono::high_resolution_clock::now();
		const std::chrono::duration dur = finish - m_StartTimePoint;
		const long long ns = std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
		const float ms = ns / 1000.f;

#if DO_PROFILE_LOGGING_WITH_AVG
		ScopedProfiler::OverManyCallsData& OverCalls = s_DataOverManyCalls[m_Identifier];
		OverCalls.m_AccumulativeNS += ns;
		OverCalls.m_TickCount++;

		const float avgMS = (OverCalls.m_AccumulativeNS / (float)OverCalls.m_TickCount) / 1000.f;

		LOG("FuncProf: {} -> {:L}ms (average: {:L}ms)", m_Identifier, ms, avgMS);
#else
		LOG("FuncProf: {} -> {:L}ms", m_Identifier, ms);
#endif
	}
}
