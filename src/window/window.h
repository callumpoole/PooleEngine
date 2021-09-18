#pragma once

#include "core.h"
#include <string_view>

struct GLFWwindow;

namespace Poole
{
	class Window
	{
	public:
		[[nodiscard]]
		static GLFWwindow* Init(const char* windowName, glm::uvec2 size);
		static void SetupPostGlad(GLFWwindow* window);
		static void Tick();
		static void Close();
	private:
		static std::string_view m_windowName;
		static glm::uvec2 m_windowSize;
	};
}
