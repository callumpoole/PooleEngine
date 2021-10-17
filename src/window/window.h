#pragma once

#include "core.h"
#include <functional>

struct GLFWwindow;

namespace Poole
{
	class Window
	{
	public:
		[[nodiscard]]
		static GLFWwindow* Init(const char* windowName, uvec2 size);
		static void SetupPostGlad(GLFWwindow* window);
		static void Tick();
		static void Close();

		static std::vector<std::function<void(int width, int height)>> m_OnResize;

		static std::string_view GetWindowName() { return m_windowName; }
		static uvec2 GetWindowSize() { return m_windowSize; }
	private:
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
		static std::string_view m_windowName;
		static uvec2 m_windowSize;
	};
}
