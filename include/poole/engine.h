#pragma once

#include "core.h"
#include <stdio.h>
namespace poole {

	class POOLE_API engine
	{
	public:
		engine() {}
	   	engine(const char* windowName, glm::uvec2 size);
		virtual ~engine() { }

		void run() { printf("Run()"); }
	}; 

	engine* create_application();
}