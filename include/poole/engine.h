#pragma once

#include "core.h"

namespace Poole {

	class Engine
	{
		friend class CommandArgs;
	public:
		Engine(std::vector<std::string_view>&& commandArgs = {});
	   	Engine(std::vector<std::string_view>&& commandArgs, const char* windowName, uvec2 size);
		virtual ~Engine() {}

		void Run();

		virtual void BeginApp() = 0;
		virtual void UpdateApp(float deltaTime) = 0;
		virtual void EndApp() = 0;


	private:
		void HandleTimeParams(i64 PreviousFrameTimeSinceEpochNS);

		struct
		{
			std::vector<std::string_view> m_CommandArgs;
			const char* m_WindowName;
			uvec2 m_Size;
		} m_RunData; //Used to pass data from ctr to Run();
	}; 

	Engine* CreateApplication(std::vector<std::string_view>&& commandArgs);
}