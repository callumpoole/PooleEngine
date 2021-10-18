#pragma once

#include "core.h"

namespace Poole {

	class POOLE_API Engine
	{
	public:
		Engine();
	   	Engine(const char* windowName, uvec2 size);
		virtual ~Engine() {}

		void Run();

		virtual void BeginApp() = 0;
		virtual void UpdateApp(float deltaTime) = 0;
		virtual void EndApp() = 0;
	private:
		struct
		{
			const char* windowName;
			uvec2 size;
		} m_RunData; //Used to pass data from ctr to Run();
	}; 

	Engine* CreateApplication();
}