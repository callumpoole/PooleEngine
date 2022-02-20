#include "core/core_logging.h"

#include "core.h"
#include <filesystem>
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
}
