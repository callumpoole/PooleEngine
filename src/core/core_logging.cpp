#include "core/core_logging.h"

#include "core.h"
#include <filesystem>

namespace Poole
{
	u64 EngineTime::s_TickCount = 0;
	u64 EngineTime::s_FrameNS = 0;
	f32 EngineTime::s_FrameMS = 0.f;
	f32 EngineTime::s_DeltaTime = 0.f;
	f32 EngineTime::s_FPS = 0.f;

	long long EngineTime::s_LaunchSinceEpochNS = 0;
	long long EngineTime::s_FirstTickSinceEpochNS = 0;
	f32 EngineTime::s_SecondsSinceLaunch = 0.f;
	f32 EngineTime::s_SecondsSinceFirstTick = 0.f;

	f32 EngineTime::s_AccDeltaTimeThisSecond = 0.f;
	u64 EngineTime::s_AccTicksThisSecond = 0;
	f32 EngineTime::s_AvgDeltaTime = 0.f;
	f32 EngineTime::s_AvgFPS = 0.f;

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
}
