#pragma once

#include "Core.h"
#include <stdio.h>
namespace Poole {

	class Engine
	{
	public:
		Engine() {}
	   	Engine(const char* windowName, glm::uvec2 size);
		virtual ~Engine() { }

		void Run() { printf("Run()"); }
	}; 

	Engine* CreateApplication();
}