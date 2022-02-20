#pragma once

#include "core.h"
#include "engine/engine_time.h"

namespace Poole {

	class Engine
	{
		friend class CommandArgs;
	public:
		static const Engine* Get() { return m_Instance; }

	   	Engine(std::vector<std::string_view>&& commandArgs, const char* windowName, uvec2 size);
		Engine(std::vector<std::string_view>&& commandArgs = {});
		virtual ~Engine() {}

		void Run();

		virtual void BeginApp() = 0;
		virtual void UpdateApp(float deltaTime) = 0;
		virtual void EndApp() = 0;

		const EngineTime& GetTimeData() const { return m_TimeData; }
	private:
		void HandleTimeParams(i64 PreviousFrameTimeSinceEpochNS);

		static Engine* m_Instance;

		struct
		{
			std::vector<std::string_view> m_CommandArgs;
			const char* m_WindowName;
			uvec2 m_Size;
		} m_RunData; //Used to pass data from ctr to Run();

		EngineTime m_TimeData;
	}; 

	Engine* CreateApplication(std::vector<std::string_view>&& commandArgs);
}